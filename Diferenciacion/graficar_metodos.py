import numpy as np
import matplotlib.pyplot as plt

# Cargar los archivos
x_e, y_e = np.loadtxt("euler_method.dat", unpack=True)
x_h, y_h = np.loadtxt("heun_method.dat", unpack=True)
x_r, y_r = np.loadtxt("runge_kutta_method.dat", unpack=True)

# Solución analítica (del apunte)
x_exact = np.linspace(x_e[0], x_e[-1], 200)
y_exact = 0.25 * ((x_exact**2 + x_exact + 2)**2)

# Graficar
plt.plot(x_exact, y_exact, 'k-', label="Analítica")
plt.plot(x_e, y_e, 'r--o', label="Euler")
plt.plot(x_h, y_h, 'b--s', label="Heun (RK2)")
plt.plot(x_r, y_r, 'g--^', label="RK4")

plt.xlabel("x")
plt.ylabel("y")
plt.title("Comparación de métodos numéricos para y' = (2x+1)*√y")
plt.legend()
plt.grid(True)
plt.show()
