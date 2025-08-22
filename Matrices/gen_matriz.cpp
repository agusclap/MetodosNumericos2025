#include <stdio.h>

int main() {
    const int n = 100;
    for (int i = 0; i < n; ++i) {
        // columnas 0..n-1 = A ; columna n = b
        for (int j = 0; j < n; ++j) {
            double val = 0.0;
            if (j == i) val = 2.0;
            else if (j == i-1 || j == i+1) val = 1.0;
            printf("%.10f ", val);
        }
        double bi = (i == 0 || i == n-1) ? 4.5 : 6.0;
        printf("%.10f\n", bi);
    }
    return 0;
}
