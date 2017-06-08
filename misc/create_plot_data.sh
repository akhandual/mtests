cdir=`pwd`
load_and_trace() {
	cd /home/khandual/linux/
	make clean
	make -s -j $2 vmlinux &
	sleep 10
	cd /home/khandual/test/migration/
	./ftrace_mm $1
	cd $cdir
	cat /sys/kernel/debug/tracing/trace > output_$1_$2
	wait	
}

process_log() {
	count=1
	while [ $count -le 128 ]
	do
		echo Processing file output_$1_$count
		cat output_$1_$count | awk '{print $2 }' > plot_data_$1_$count.txt
		count=`expr $count \\* 2`
	done
	mkdir plot_data_$1
	mv plot_data_$1_* plot_data_$1/
	tar -cvf plot_data_$1.tar.gz plot_data_$1
	rm -rf plot_data_$1
	rm -rf output_*
	
}

test() {
	count=1
	while [ $count -le 128 ]
	do
		echo Tracing for $1 with $count threads 
		load_and_trace $1 $count
		count=`expr $count \\* 2`
	done
}

test handle_mm_fault
process_log handle_mm_fault

test do_page_fault
process_log do_page_fault
