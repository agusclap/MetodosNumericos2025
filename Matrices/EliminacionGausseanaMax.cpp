#include <stdio.h>
#include <math.h>

int main() {
    FILE *fp = fopen("datos.dat","r");
    if (!fp) { puts("No se puede abrir el archivo"); return 1; }

    // Contar filas (n)
    int filas = 0, c;
    while ((c = fgetc(fp)) != EOF) if (c == '\n') filas++;
    if (filas <= 0) { puts("Archivo vacío"); fclose(fp); return 1; }

    // Reiniciar lectura
    fclose(fp);
    fp = fopen("datos.dat","r");
    if (!fp) { puts("No se puede abrir el archivo"); return 1; }

    // Cargar matriz aumentada m[filas][columnas]
    // Nota: límite 100 columnas como en el ejemplo de la profe
    int columnas = 0;
    double m[filas][100];

    for (int i = 0; i < filas; ++i) {
        int j = 0;
        do {
            if (fscanf(fp, "%lf", &(m[i][j])) != 1) {
                puts("Formato inválido en datos.dat");
                fclose(fp);
                return 1;
            }
            j++;
        } while ((c = fgetc(fp)) != '\n' && c != EOF);

        if (i == 0) columnas = j;
        else if (j != columnas) {
            puts("Filas con distinta cantidad de columnas");
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);

    int n = filas;                         // n ecuaciones
    if (columnas != n + 1) {
        puts("Se esperaba matriz aumentada [A|b]: columnas = filas + 1");
        return 1;
    }

    const double EPS = 1e-12;

    // -------- Eliminación Gauss con pivoteo parcial (siempre máximo) --------
    for (int i = 0; i < n - 1; ++i) {
        // 1) Elegir fila p con mayor |m[p][i]| para p ∈ [i..n-1]
        int p = i;
        for (int l = i + 1; l < n; ++l)
            if (fabs(m[l][i]) > fabs(m[p][i])) p = l;

        // 2) Swap de filas completas (incluye columna b en índice n)
        if (p != i) {
            for (int col = 0; col <= n; ++col) {
                double aux = m[i][col];
                m[i][col] = m[p][col];
                m[p][col] = aux;
            }
        }

        // 3) Chequear pivote
        if (fabs(m[i][i]) < EPS) {
            printf("Sistema singular o casi singular en etapa %d (pivote ~ 0)\n", i);
            return 1;
        }

        // 4) Eliminación hacia abajo
        for (int j = i + 1; j < n; ++j) {
            double factor = m[j][i] / m[i][i];
            if (fabs(factor) < EPS) continue;
            for (int k = i; k <= n; ++k) { // <= n porque incluye la columna de b
                m[j][k] -= factor * m[i][k];
            }
        }
    }

    if (fabs(m[n-1][n-1]) < EPS) {
        puts("Sistema singular en última etapa (pivote ~ 0)");
        return 1;
    }

    // -------- Retrosustitución --------
    double x[100]; // suficiente para n<=100
    for (int i = n - 1; i >= 0; --i) {
        double sum = m[i][n]; // término independiente
        for (int j = i + 1; j < n; ++j) sum -= m[i][j] * x[j];
        if (fabs(m[i][i]) < EPS) {
            printf("Division por ~0 en retrosustitucion (fila %d)\n", i);
            return 1;
        }
        x[i] = sum / m[i][i];
    }

    // -------- Mostrar solución --------
    for (int i = 0; i < n; ++i) printf("x[%d] = %.10f\n", i, x[i]);

    return 0;
}
