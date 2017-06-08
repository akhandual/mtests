all: ftrace_mm ppc_max_mapsize

ftrace_mm: ftrace_mm.c
	cc -o ftrace_mm ftrace_mm.c

ppc_max_mapsize: ppc_max_mapsize.c
	cc -o ppc_max_mapsize ppc_max_mapsize.c

clean: 
	rm ftrace_mm ppc_max_mapsize
