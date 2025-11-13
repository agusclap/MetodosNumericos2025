import sys
import os
import numpy as np
import matplotlib.pyplot as plt

def main():
    # 1) Tomar nombre de archivo
    if len(sys.argv) < 2:
        print("Uso: python plot_num_vs_exacta.py archivo.dat")
        return

    fname = sys.argv[1]

    if not os.path.exists(fname):
        print(f"Error: no se encontro el archivo '{fname}'")
        return

    # 2) Cargar datos (ignora lineas que empiezan con '#')
    try:
        data = np.loadtxt(fname, comments="#")
    except Exception as e:
        print("No se pudo leer el archivo:", e)
        return

    # Asegurar que es 2D
    if data.ndim == 1:
        data = data.reshape(1, -1)

    n_filas, n_cols = data.shape

    if n_cols < 2:
        print("Se necesitan al menos 2 columnas.")
        return

    x  = data[:, 0]
    yn = data[:, 1]                # y numerica SIEMPRE col 2

    # Si hay al menos 3 columnas, usamos la 3ª como exacta
    if n_cols >= 3:
        ye = data[:, 2]
        usar_exacta = True
    else:
        usar_exacta = False

    # 3) Graficar
    plt.figure()
    plt.plot(x, yn, label="Numerica", linewidth=2)

    if usar_exacta:
        plt.plot(x, ye, "--", label="Exacta", linewidth=2)

    plt.xlabel("x")
    plt.ylabel("y")
    base = os.path.splitext(os.path.basename(fname))[0]
    plt.title(f"{base} - Numerica vs Exacta")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    # 4) Guardar PNG
    out_png = base + "_num_vs_exacta.png"
    plt.savefig(out_png, dpi=200)
    print(f"Grafico guardado como: {out_png}")

    plt.show()

if __name__ == "__main__":
    main()
