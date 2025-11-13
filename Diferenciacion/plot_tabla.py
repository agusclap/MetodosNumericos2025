import sys
import os
import numpy as np
import matplotlib.pyplot as plt

def main():
    # 1) Tomar nombre de archivo
    if len(sys.argv) < 2:
        print("Uso: python plot_tabla.py archivo.dat")
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
        # Si hay una sola fila, np.loadtxt devuelve 1D
        data = data.reshape(1, -1)

    n_filas, n_cols = data.shape

    if n_cols < 2:
        print("Se necesitan al menos 2 columnas (x y ...)")
        return

    x = data[:, 0]

    # 3) Graficar
    plt.figure()
    # Si solo hay 2 columnas: se grafica x vs y
    if n_cols == 2:
        y = data[:, 1]
        plt.plot(x, y, label="columna 2")
    else:
        # Si hay mas columnas: x vs cada y
        for col in range(1, n_cols):
            y = data[:, col]
            plt.plot(x, y, label=f"columna {col+1}")

    plt.xlabel("x")
    plt.ylabel("valor")
    base = os.path.splitext(os.path.basename(fname))[0]
    plt.title(f"Grafico de '{base}'")
    plt.grid(True)
    if n_cols >= 2:
        plt.legend()

    plt.tight_layout()

    # 4) Guardar PNG con el mismo nombre que el archivo
    out_png = base + ".png"
    plt.savefig(out_png, dpi=200)
    print(f"Grafico guardado como: {out_png}")

    # Mostrar en pantalla
    plt.show()

if __name__ == "__main__":
    main()
