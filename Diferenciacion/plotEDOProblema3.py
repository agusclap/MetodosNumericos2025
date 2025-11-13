import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("metodo_dos_pasos.dat", comments="#")

x  = data[:,0]
yn = data[:,1]
ye = data[:,2]

plt.plot(x, yn, label="Numerica (dos pasos)")
plt.plot(x, ye, label="Exacta", linestyle="--")

plt.xlabel("x")
plt.ylabel("y")
plt.title("Problema 3 - Metodo de dos pasos vs exacta")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("solucion_problema3.png", dpi=200)
plt.show()
