#include <bits/stdc++.h>
using namespace std;

double f(double x) {
    return exp(-sqrt(1.0 + x)) * log(1.0 + 2.0*x*x);
}

// derivada analitica de f (segun f definida arriba)
double f_analitica(double x) {
    double u = exp(-sqrt(1.0 + x));
    double v = log(1.0 + 2.0*x*x);
    double du = -u * (1.0 / (2.0 * sqrt(1.0 + x)));        // u' = -e^{-sqrt(1+x)} * 1/(2 sqrt(1+x))
    double dv = (4.0 * x) / (1.0 + 2.0*x*x);             // v' = 4x/(1+2x^2)
    return du * v + u * dv;
}

int main() {
  

    double a, b;
    int n;
    cout << "a b n: ";
    if (!(cin >> a >> b >> n) || n < 1 || b <= a) {
        cerr << "Entrada invalida. Requerido: n>=1 y b>a.\n";
        return 1;
    }

    double h = (b - a) / n;

    // genera tabla_fx.dat (x f(x))
    ofstream out_fx("tabla_fx.dat");
    if (!out_fx) { cerr << "No pude abrir tabla_fx.dat\n"; return 1; }
    out_fx << fixed << setprecision(12);
    for (int i = 0; i <= n; ++i) {
        double xi = a + i*h;
        double fi = f(xi);
        out_fx << xi << " " << fi << "\n";
    }
    out_fx.close();

    // lee la misma tabla y calcula derivadas numericas y analiticas
    vector<double> x(n+1), fx(n+1);
    {
        ifstream in("tabla_fx.dat");
        if (!in) { cerr << "No pude abrir tabla_fx.dat para leer\n"; return 1; }
        for (int i = 0; i <= n; ++i) {
            in >> x[i] >> fx[i];
        }
    }

    vector<double> df_num(n+1), df_ana(n+1);

    // extremos: usar formula de 3 puntos (error O(h^2))
    // forward de 3 puntos en x0: f'(x0) ≈ (-3 f0 + 4 f1 - f2) / (2h)
    df_num[0] = (-3.0*fx[0] + 4.0*fx[1] - fx[2]) / (2.0*h);
    // backward de 3 puntos en xn: f'(xn) ≈ (3 fn - 4 f_{n-1} + f_{n-2}) / (2h)
    df_num[n] = (3.0*fx[n] - 4.0*fx[n-1] + fx[n-2]) / (2.0*h);

    // interiores: diferencias centradas (error O(h^2))
    for (int i = 1; i <= n-1; ++i) {
        df_num[i] = (fx[i+1] - fx[i-1]) / (2.0*h);
    }

    // derivada analitica
    for (int i = 0; i <= n; ++i) df_ana[i] = f_analitica(x[i]);

    // escribir resultados
    ofstream out("tabla_derivadas.dat");
    if (!out) { cerr << "No pude crear tabla_derivadas.dat\n"; return 1; }
    out << fixed << setprecision(8);
    out << "# x   f(x)   f'(num)   f'(analitica)   diff(num-analit)\n";
    for (int i = 0; i <= n; ++i) {
        out << setw(10) << x[i] << " "
            << setw(12) << fx[i] << " "
            << setw(12) << df_num[i] << " "
            << setw(14) << df_ana[i] << " "
            << setw(14) << (df_num[i] - df_ana[i]) << "\n";
    }
    out.close();

    cout << "Hecho: tabla_fx.dat y tabla_derivadas.dat generadas. h = " << h << "\n";
    return 0;
}
