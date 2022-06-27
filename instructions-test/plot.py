import matplotlib.pyplot as plt
from sys import argv, stderr
from numpy import loadtxt, histogram

if len(argv) < 2:
    print(f'Use atleast one CSV file as argument', file=stderr)
    exit(1)

for csv_file in argv[1:]:
    data = loadtxt(csv_file, delimiter=';')
    y, x = histogram(data, bins=50)
    plt.plot(x[1:], y, label=csv_file)

plt.ylabel("Frekvencia")
plt.xlabel("Joul")
plt.title("Porovnanie inštrukcií")
plt.legend()
plt.grid()
plt.show()