// tabla_fx.cpp
#include <bits/stdc++.h>
using namespace std;

// >>>>>> CAMBIÁ SOLO ESTO SEGÚN EL ENUNCIADO <<<<<<
double f(double x) {
    // ejemplo del parcial:
    // f(x) = exp(sqrt(1+x)) * ln(1 + 2 x^3)
    return exp(sqrt(1.0 + x)) * log(1.0 + 2.0*x*x);
}
// >>>>>> FIN SECCIÓN EDITABLE <<<<<<

int main() {

    double a, b; 
    int n;                // cantidad de subintervalos (n >= 1) => n+1 puntos
    cout << "a b n: ";
    if (!(cin >> a >> b >> n) || n < 1 || b <= a) {
        cerr << "Entrada invalida. Requerido: n>=1 y b>a.\n";
        return 1;
    }

    const double h = (b - a) / n;

    // salida a archivo y a pantalla
    ofstream out("tabla_fx.dat");
    if (!out) { cerr << "No pude abrir tabla_fx.dat\n"; return 1; }
    cout << fixed << setprecision(12);
    out  << fixed << setprecision(12);

    cout << "# i            x_i              f(x_i)\n";
    for (int i = 0; i <= n; ++i) {
        double xi = a + i*h;
        double fi = f(xi);

        cout << setw(2) << i << "  " 
             << setw(14) << xi << "  " 
             << setw(14) << fi << "\n";

        out  << xi << " " << fi << "\n";   // 2 columnas: x  f(x)
    }
    out.close();
    return 0;
}
