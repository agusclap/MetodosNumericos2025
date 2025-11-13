#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>
using namespace std;

// y' = f(x,y) = 4xy / (1 + x^2)
double f(double x, double y) {
    return (4.0 * x * y) / (1.0 + x * x);
}

// Solución exacta: y(x) = (1 + x^2)^2
double y_exact(double x) {
    double t = 1.0 + x * x;
    return t * t;
}

int main() {
    double x0, xf, h, y0;
    int n;

    cout << fixed << setprecision(6);

    cout << "Problema: y' = 4xy/(1+x^2),  y(0)=1,  en [0,1]\n";
    cout << "Ingrese x0, xf (ej: 0 1): ";
    cin  >> x0 >> xf;

    cout << "Ingrese y0 (para el problema: 1): ";
    cin  >> y0;

    cout << "Ingrese h (ej: 0.1 o 0.01): ";
    cin  >> h;

    if (h <= 0) {
        cerr << "h debe ser > 0\n";
        return 1;
    }

    // cantidad de pasos
    n = (int) ((xf - x0)/h + 0.5);
    h = (xf - x0)/n;         // ajusto para caer justo en xf

    vector<double> x(n+1), y(n+1);

    x[0] = x0;
    y[0] = y0;

    // Paso i = 0 -> calculo y1 con Euler
    x[1] = x[0] + h;
    y[1] = y[0] + h * f(x[0], y[0]);

    // Pasos i >= 1 -> método de dos pasos: y_{i+1} = y_i + h(2 f_i - f_{i-1})
    for (int i = 1; i < n; ++i) {
        x[i+1] = x[i] + h;

        double fi   = f(x[i],   y[i]);
        double fim1 = f(x[i-1], y[i-1]);

        y[i+1] = y[i] + h * (2.0 * fi - fim1);
    }

    // Guardar tabla con errores
    ofstream out("metodo_dos_pasos.dat");
    if (!out) {
        cerr << "No se pudo abrir archivo de salida\n";
        return 1;
    }

    out << fixed << setprecision(10);
    out << "# x    y_num    y_exact    error_abs    error_rel_pct\n";

    for (int i = 0; i <= n; ++i) {

        double ye    = y_exact(x[i]);
        double e_abs = fabs(ye - y[i]);
        double e_rel = e_abs / fabs(ye) * 100.0;

        out << x[i] << "  "
            << y[i] << "  "
            << ye   << "  "
            << e_abs << "  "
            << e_rel << "\n";
    }

    out.close();
    cout << "Tabla escrita en metodo_dos_pasos.dat\n";

    return 0;
}


void metodo_dos_pasos() {
    Params p;
    if (!leer_params(p)) return;

    vector<double> x(p.n + 1), y(p.n + 1);
    x[0] = p.x0;
    y[0] = p.y0;

    // PASO i = 0 ➜ y1 usando Euler (obligatorio por el método)
    x[1] = x[0] + p.h;
    y[1] = y[0] + p.h * f(x[0], y[0]);

    // PASO i >= 1 ➜ fórmula del enunciado
    // yi+1 = yi + h ( 2 f(xi, yi) – f(xi-1, yi-1) )
    for (int i = 1; i < p.n; i++) {

        x[i+1] = x[i] + p.h;

        double fi   = f(x[i],   y[i]);
        double fim1 = f(x[i-1], y[i-1]);

        y[i+1] = y[i] + p.h * (2.0 * fi - fim1);
    }

    guardar("metodo_dos_pasos.dat", x, y);
}
