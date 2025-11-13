#include <stdio.h>
#include <math.h>

double f(double x) {
    return exp(sqrt(1.0 + x)) * log(1.0 + 2.0*x*x);
}

int main() {

    double a, b, h, x;
    int n;

    FILE *out = fopen("tabla_y_derivadas.txt", "w");
    if (out == NULL) {
        printf("❌ Error creando archivo tabla_y_derivadas.txt\n");
        return -1;
    }

    printf("Ingrese el valor inicial (a): ");
    scanf("%lf", &a);
    printf("Ingrese el valor final (b): ");
    scanf("%lf", &b);
    printf("Ingrese la cantidad de subintervalos (n): ");
    scanf("%d", &n);

    h = (b - a) / n;

    double fx[n+1];
    double xvals[n+1];
    double derivative[n+1];

    // Generar tabla f(x)
    for (int i = 0; i <= n; i++) {
        xvals[i] = a + h * i;
        fx[i] = f(xvals[i]);
    }

    // EXTREMO IZQUIERDO (O(h²))
    derivative[0] = (-3*fx[0] + 4*fx[1] - fx[2]) / (2*h);

    // PUNTOS INTERIORES (centrada O(h²))
    for (int i = 1; i < n; i++) {
        derivative[i] = (fx[i+1] - fx[i-1]) / (2*h);
    }

    // EXTREMO DERECHO (O(h²))
    derivative[n] = (3*fx[n] - 4*fx[n-1] + fx[n-2]) / (2*h);

    fprintf(out, "# x\tf(x)\tf'(x)\n");

    for (int i = 0; i <= n; i++) {
        fprintf(out, "%lf\t%lf\t%lf\n", xvals[i], fx[i], derivative[i]);
    }

    fclose(out);

    printf("✅ Archivo generado: tabla_y_derivadas.txt\n");
    return 0;
}
