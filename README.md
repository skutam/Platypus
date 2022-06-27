# Platypus re-creation project

It is a copy of [Platypus](https://platypusattack.com/) side channel attack. We focus on analysing different instruction for covert channel communication.

## Author

**Matúš Škuta (xskuta04)**

- Email: [skuta.matus@gmail.com](mailto:skuta.matus@gmail.com)

## Installation

Before starting please create environment with command
```bash
python3 -m venv env
```
activate the environment with command
```bash
source env/bin/activate
```
and install the libraries in `requirement.txt` with command
```bash
pip install -r requirements.txt
```

### Instruction-test

When we want to test the different instructions simply go into folder `instructions-test` type in `make` to build the project and then `make run` to generate the data. This may take a while. After the process finishes we can show the data with command `make plot`. Manually we can run the script with command
```bash
sudo taskset -c 1 ./test_instructions INSTRUCTION 2> out.csv
```
where `INSTRUCTION` is one of the following strings `nop, rdtsc, clflush, fscale, mov`.

### Covert-channel

When we want to run the covert channel simply go into folder `covert-channel` type in `make` to build the project and then `make run` to generate data for each instruction. This may take a while. After the process finishs we can show the with command `make plot` that will show the result for each instruction. Manually we can run the script with command
```bash
sudo taskset -c 1 ./covert_channel INSTRUCTION BTS 2> out.csv
```
where `INSTRUCTION` is one of the following strings `nop, rdtsc, clflush, fscale, mov` and `BTS` is bits per second that are in range `<1, 20>`.
