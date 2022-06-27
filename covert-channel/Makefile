phony:
	gcc main.c ../src/measurement.c covert_channel.c timer.c -o covert_channel -lrt

run:
	@sudo taskset -c 1 ./covert_channel nop 10 2> nop.csv
	@sudo taskset -c 1 ./covert_channel rdtsc 10 2> rdtsc.csv
	@sudo taskset -c 1 ./covert_channel clflush 10 2> clflush.csv
	@sudo taskset -c 1 ./covert_channel fscale 10 2> fscale.csv
	@sudo taskset -c 1 ./covert_channel mov 10 2> mov.csv

plot:
	python3 plot.py nop.csv
	python3 plot.py rdtsc.csv
	python3 plot.py clflush.csv
	python3 plot.py fscale.csv
	python3 plot.py mov.csv
