#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// ============================================================
// INTERPOLACIÓN SPLINE CÚBICA
// Genera tabla equiespaciada desde datos no uniformes
// ============================================================

typedef struct {
    double x;
    double y;
} Punto;

// Resolver sistema tridiagonal (algoritmo de Thomas)
void resolver_tridiagonal(int n, double *a, double *b, double *c, double *d, double *x) {
    double *c_temp = (double *)malloc(n * sizeof(double));
    double *d_temp = (double *)malloc(n * sizeof(double));
    
    c_temp[0] = c[0] / b[0];
    d_temp[0] = d[0] / b[0];
    
    for (int i = 1; i < n; i++) {
        double m = 1.0 / (b[i] - a[i] * c_temp[i-1]);
        c_temp[i] = c[i] * m;
        d_temp[i] = (d[i] - a[i] * d_temp[i-1]) * m;
    }
    
    x[n-1] = d_temp[n-1];
    for (int i = n - 2; i >= 0; i--) {
        x[i] = d_temp[i] - c_temp[i] * x[i+1];
    }
    
    free(c_temp);
    free(d_temp);
}

// Calcular coeficientes del spline cúbico
void calcular_spline(int n, Punto *datos, double *M) {
    // M[i] son las segundas derivadas en cada punto
    // Sistema: M[i-1]*h[i-1] + 2*M[i]*(h[i-1]+h[i]) + M[i+1]*h[i] = 6*(delta[i] - delta[i-1])
    
    double *h = (double *)malloc((n-1) * sizeof(double));
    double *delta = (double *)malloc((n-1) * sizeof(double));
    
    // Calcular h[i] = x[i+1] - x[i] y delta[i] = (y[i+1] - y[i]) / h[i]
    for (int i = 0; i < n - 1; i++) {
        h[i] = datos[i+1].x - datos[i].x;
        delta[i] = (datos[i+1].y - datos[i].y) / h[i];
    }
    
    // Sistema tridiagonal para M[1] a M[n-2]
    int m = n - 2;  // Ecuaciones internas
    double *a = (double *)malloc(m * sizeof(double));
    double *b = (double *)malloc(m * sizeof(double));
    double *c = (double *)malloc(m * sizeof(double));
    double *d = (double *)malloc(m * sizeof(double));
    
    for (int i = 0; i < m; i++) {
        if (i > 0) a[i] = h[i];
        b[i] = 2.0 * (h[i] + h[i+1]);
        if (i < m-1) c[i] = h[i+1];
        d[i] = 6.0 * (delta[i+1] - delta[i]);
    }
    
    // Condiciones naturales: M[0] = M[n-1] = 0
    M[0] = 0.0;
    M[n-1] = 0.0;
    
    double *M_interno = (double *)malloc(m * sizeof(double));
    resolver_tridiagonal(m, a, b, c, d, M_interno);
    
    for (int i = 0; i < m; i++) {
        M[i+1] = M_interno[i];
    }
    
    free(h);
    free(delta);
    free(a);
    free(b);
    free(c);
    free(d);
    free(M_interno);
}

// Evaluar spline en un punto x
double evaluar_spline(double x, int n, Punto *datos, double *M) {
    // Encontrar intervalo [x[i], x[i+1]] que contiene x
    int i = 0;
    for (i = 0; i < n - 1; i++) {
        if (x >= datos[i].x && x <= datos[i+1].x) {
            break;
        }
    }
    
    if (i >= n - 1) i = n - 2;  // x está fuera, usar último intervalo
    
    double h = datos[i+1].x - datos[i].x;
    double t = x - datos[i].x;
    
    // Fórmula del spline cúbico
    double A = (datos[i+1].x - x) / h;
    double B = (x - datos[i].x) / h;
    double C = (A*A*A - A) * h*h / 6.0;
    double D = (B*B*B - B) * h*h / 6.0;
    
    return A * datos[i].y + B * datos[i+1].y + C * M[i] + D * M[i+1];
}

int main() {
    char archivo_entrada[100], archivo_salida[100];
    int n_original, n_subintervalos;
    
    printf("=== INTERPOLACIÓN SPLINE CÚBICA ===\n\n");
    printf("Archivo de entrada (datos originales): ");
    scanf("%s", archivo_entrada);
    
    // Leer datos originales
    FILE *fp = fopen(archivo_entrada, "r");
    if (!fp) {
        printf("Error: No se puede abrir %s\n", archivo_entrada);
        return 1;
    }
    
    n_original = 0;
    double temp1, temp2;
    while (fscanf(fp, "%lf %lf", &temp1, &temp2) == 2) {
        n_original++;
    }
    rewind(fp);
    
    Punto *datos = (Punto *)malloc(n_original * sizeof(Punto));
    for (int i = 0; i < n_original; i++) {
        fscanf(fp, "%lf %lf", &datos[i].x, &datos[i].y);
    }
    fclose(fp);
    
    printf("\nDatos originales (%d puntos):\n", n_original);
    printf("%-4s %-12s %-12s\n", "i", "x", "f(x)");
    printf("-------------------------------------\n");
    for (int i = 0; i < n_original; i++) {
        printf("%-4d %-12.6f %-12.6f\n", i, datos[i].x, datos[i].y);
    }
    
    printf("\n¿Cuántos subintervalos para tabla equiespaciada? ");
    scanf("%d", &n_subintervalos);
    
    printf("Archivo de salida (tabla equiespaciada): ");
    scanf("%s", archivo_salida);
    
    // Calcular coeficientes del spline
    double *M = (double *)malloc(n_original * sizeof(double));
    calcular_spline(n_original, datos, M);
    
    printf("\nSegundas derivadas (M):\n");
    for (int i = 0; i < n_original; i++) {
        printf("M[%d] = %.6f\n", i, M[i]);
    }
    
    // Generar tabla equiespaciada
    double x_min = datos[0].x;
    double x_max = datos[n_original - 1].x;
    double h = (x_max - x_min) / n_subintervalos;
    int n_nuevos = n_subintervalos + 1;
    
    printf("\n=== TABLA EQUIESPACIADA ===\n");
    printf("Rango: [%.6f, %.6f]\n", x_min, x_max);
    printf("h = %.6f\n", h);
    printf("Puntos: %d\n\n", n_nuevos);
    
    FILE *fout = fopen(archivo_salida, "w");
    
    printf("%-4s %-12s %-12s\n", "i", "x", "f(x) interp");
    printf("-------------------------------------\n");
    
    for (int i = 0; i < n_nuevos; i++) {
        double x = x_min + i * h;
        double y = evaluar_spline(x, n_original, datos, M);
        
        printf("%-4d %-12.6f %-12.6f\n", i, x, y);
        fprintf(fout, "%.6f  %.6f\n", x, y);
    }
    
    fclose(fout);
    
    printf("\n✓ Tabla guardada en: %s\n", archivo_salida);
    printf("✓ Lista para usar con trapecio_tabla.c o simpson_tabla.c\n");
    
    free(datos);
    free(M);
    return 0;
}
