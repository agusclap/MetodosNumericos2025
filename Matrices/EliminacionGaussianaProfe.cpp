#include <stdio.h>
#include <math.h>
#include <iostream>
using namespace std;

int main() {
    FILE *fp = fopen("datos.dat","r");
    if (!fp) { puts("No se puede abrir el archivo"); return 1; }

    // Contar filas
    int filas = 0;
    int columnas = 0;
    int c;
    while ((c = fgetc(fp)) != EOF) if (c == '\n') filas++;
    if (filas <= 0) { puts("Archivo vacío"); fclose(fp); return 1; }

    // Reiniciar lectura
    fclose(fp);
    fp = fopen("datos.dat","r");
    if (!fp) { puts("No se puede abrir el archivo"); return 1; }

    // Cargar matriz aumentada: m[filas][columnas]
    // Leemos por fila hasta '\n'
    // ¡OJO! aquí usamos un ancho máximo de 100 columnas
    double m[filas][100];
    for (int i = 0; i < filas; ++i) {
        int j = 0;
        do {
            if (fscanf(fp, "%lf", &(m[i][j])) != 1) { puts("Formato inválido"); fclose(fp); return 1; }
            j++;
        } while ((c = fgetc(fp)) != '\n' && c != EOF);
        if (i == 0) columnas = j;          // fijamos columnas con la primera fila
        else if (j != columnas) { puts("Filas con distinta cantidad de columnas"); fclose(fp); return 1; }
    }
    fclose(fp);

    printf("numero de filas = %d\n", filas);
    printf("numero de columnas = %d\n", columnas);

    int n = filas;                 // sistema de n ecuaciones
    if (columnas != n + 1) {       // esperamos matriz aumentada [A|b]
        puts("Se esperaba matriz aumentada [A|b]: columnas = filas + 1");
        return 1;
    }

    const double EPS    = 1e-12;   // singularidad
    const double THRESH = 1e-2;    // umbral de pivoteo “profe”

    // ---------- Eliminación con pivoteo parcial condicional ----------
    for (int i = 0; i < n - 1; ++i) {

        // Pivoteo parcial SOLO si |a_ii| < THRESH
        int p = i;
        if (fabs(m[i][i]) < THRESH) {
            for (int l = i + 1; l < n; ++l)
                if (fabs(m[l][i]) > fabs(m[p][i])) p = l;

            if (p != i) {
                // swap de filas completas (A y b) → columnas = n+1
                for (int col = i; col <= n; ++col) {
                    double aux = m[i][col];
                    m[i][col] = m[p][col];
                    m[p][col] = aux;
                }
            }
        }

        if (fabs(m[i][i]) < EPS) {
            fprintf(stderr, "Sistema singular o casi singular en etapa %d (pivote ~ 0)\n", i);
            return 1;
        }

        // Eliminación hacia abajo
        for (int j = i + 1; j < n; ++j) {
            double factor = m[j][i] / m[i][i];
            if (fabs(factor) < EPS) continue;
            for (int k = i; k <= n; ++k) {           // <= n porque incluye la columna de b
                m[j][k] -= factor * m[i][k];
            }
        }
    }

    if (fabs(m[n-1][n-1]) < EPS) {
        fprintf(stderr, "Sistema singular en última etapa (pivote ~ 0)\n");
        return 1;
    }

    // ---------- Retrosustitución ----------
    double x[100]; // n <= 100 implícitamente por como leemos
    for (int i = n - 1; i >= 0; --i) {
        double sum = m[i][n];               // término independiente (columna b)
        for (int j = i + 1; j < n; ++j) {
            sum -= m[i][j] * x[j];
        }
        if (fabs(m[i][i]) < EPS) {
            fprintf(stderr, "Division por ~0 en retrosustitucion (fila %d)\n", i);
            return 1;
        }
        x[i] = sum / m[i][i];
    }

    // ---------- Mostrar solución ----------
    for (int i = 0; i < n; ++i) {
        printf("x[%d] = %.10f\n", i, x[i]);
    }

    return 0;
}
