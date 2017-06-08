#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/time.h>

#define ALLOC_SIZE 128 * 0x10000UL

#define CTRACER		"/sys/kernel/debug/tracing/current_tracer"
#define FUNCTION	"/sys/kernel/debug/tracing/set_ftrace_filter"
#define PROCESS		"/sys/kernel/debug/tracing/set_ftrace_pid"
#define ENABLED		"/sys/kernel/debug/tracing/tracing_on"

int fs_ctracer;
int fd_function;
int fd_process;
int fd_enabled;

static void open_trace_files(void)
{
	fs_ctracer = open(CTRACER, O_RDWR);
	if (fs_ctracer == -1)
		perror("open() failed for CTRACER");

	fd_process = open(PROCESS, O_RDWR);
	if (fd_process == -1)
		perror("open() failed for PROCESS");

	fd_enabled = open(ENABLED, O_RDWR);
	if (fd_enabled == -1)
		perror("open() failed for ENABLED");
}


static void close_trace_files(void)
{
	close(fs_ctracer);
	close(fd_process);
	close(fd_enabled);
}

static void disable_tracing(void)
{
	int ret;

	ret = write(fd_enabled, "0", 1);
	if (ret == -1)
		perror("write() failed for ENABLED");
}

static void enable_tracing(void)
{
	int ret;

	ret = write(fd_enabled, "1", 1);
	if (ret == -1)
		perror("write() failed for ENABLED");
}

static void remove_ctracer(void)
{
	int ret;

	ret = write(fs_ctracer, "nop", 3);
	if (ret == -1)
		perror("write() failedf or CTRACER");
}

static void enable_function_graph(void)
{
	int ret;

	ret = write(fs_ctracer, "function_graph", 14);
	if (ret == -1)
		perror("write() failed CTRACER");
}

static void enable_pid_filter(void)
{
	int ret;
	char buf[10];

	sprintf(buf, "%d", getpid());
	ret = write(fd_process, buf, sizeof(buf));
	if (ret == -1)
		perror("write() failed");
}

static void clear_filter(void)
{
	int fd_function = open(FUNCTION, O_TRUNC|O_WRONLY);

	close(fd_function);	
}

static void add_filter(char *str)
{
	int ret;

	fd_function = open(FUNCTION, O_WRONLY);
	if (fd_function == -1)
		perror("open() failed for FUNCTION");

	ret = write(fd_function, str, strlen(str));
	if (ret == -1)
		perror("write() failed for FUNCTION");

	close(fd_function);
}

int main(int argc, char *argv[])
{
	unsigned long mask;
	char *ptr, buf[10];
	int ret, i;

	open_trace_files();	
	disable_tracing();
	remove_ctracer();
	enable_function_graph();
	enable_pid_filter();
	clear_filter();

	ptr = mmap(NULL, ALLOC_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (ptr == MAP_FAILED) {
		perror("map() failed");
		return -1;
	}

	add_filter(argv[1]);
	enable_tracing();
	memset(ptr, 0, ALLOC_SIZE);
	disable_tracing();

	close_trace_files();
	munmap(ptr, ALLOC_SIZE);
	return 0;
}
