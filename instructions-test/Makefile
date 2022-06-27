phony:
	gcc main.c ../src/measurement.c instruction_test.c -o test_instructions

run:
	@echo "Average instruction consumption"
	@sudo taskset -c 1 ./test_instructions nop 2> nop.csv
	@sudo taskset -c 1 ./test_instructions rdtsc 2> rdtsc.csv
	@sudo taskset -c 1 ./test_instructions clflush 2> clflush.csv
	@sudo taskset -c 1 ./test_instructions fscale 2> fscale.csv
	@sudo taskset -c 1 ./test_instructions mov 2> mov.csv

plot:
	python3 plot.py nop.csv rdtsc.csv clflush.csv fscale.csv mov.csv
