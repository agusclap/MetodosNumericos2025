#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <stdexcept>
using namespace std;

double f(double x) {
    return x*x; // ejemplo
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    double a, b;
    int n;
    cout << "Ingrese a y b: ";
    cin >> a >> b;
    cout << "Ingrese n (cantidad de subintervalos): ";
    cin >> n;

    if (n < 2) {
        cerr << "n debe ser un entero >= 2.\n";
        return 1;
    }
    if (b <= a) {
        cerr << "Se requiere b > a.\n";
        return 1;
    }

    double h = (b - a) / n;
    vector<double> x(n), df(n);

    // nodos
    for (int i = 0; i < n; ++i) x[i] = a + i*h;

    // extremos (O(h))
    df[0]     = (f(x[0] + h)   - f(x[0]))   / h; // forward
    df[n-1]   = (f(x[n-1])     - f(x[n-1]-h))/ h; // backward

    // interior (O(h^2))
    for (int i = 1; i < n-1; ++i) {
        df[i] = (f(x[i] + h) - f(x[i] - h)) / (2*h);
    }

    ofstream out("diferencias_finitas.dat");
    if (!out) {
        cerr << "No se pudo abrir diferencias_finitas.dat para escribir.\n";
        return 1;
    }
    out << fixed << setprecision(12);
    for (int i = 0; i < n; ++i) {
        out << x[i] << ' ' << df[i] << ' ' << f(x[i]) << '\n';
    }
    out.close();
    cout << "Resultados escritos en diferencias_finitas.dat\n";
    return 0;
}
