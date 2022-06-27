import matplotlib.pyplot as plt
from numpy import loadtxt, arange
from sys import argv, stderr

if len(argv) != 2:
    print(f'Select as second argument csv file', file=stderr)
    exit(1)

# Load data
data = loadtxt(argv[1])
plt.plot(data)
plt.xticks(arange(0, len(data)+1, 20.0))

# Add labels and render
plt.ylabel('Watty')
plt.xlabel('Čas')
plt.title("Energeticky zakódovaná binárna správa")
plt.grid()
plt.show()
