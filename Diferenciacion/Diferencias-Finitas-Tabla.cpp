#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <stdexcept>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // leer los datos del spline
    ifstream in("tabla_uniforme.dat");
    if (!in) {
        cerr << "No se pudo abrir tabla_uniforme.dat\n";
        return 1;
    }

    vector<double> x, f;
    double xi, fi;
    while (in >> xi >> fi) {
        x.push_back(xi);
        f.push_back(fi);
    }
    in.close();

    int n = x.size();
    if (n < 3) {
        cerr << "Se necesitan al menos 3 puntos para diferencias centradas.\n";
        return 1;
    }

    // h uniforme
    double h = x[1] - x[0];
    vector<double> df(n);

    // extremos (O(h))
    df[0]   = (f[1] - f[0]) / h;             // forward
    df[n-1] = (f[n-1] - f[n-2]) / h;         // backward

    // interior (O(h^2))
    for (int i = 1; i < n-1; ++i) {
        df[i] = (f[i+1] - f[i-1]) / (2*h);
    }

    ofstream out("diferencias_finitas.dat");
    if (!out) {
        cerr << "No se pudo crear diferencias_finitas.dat\n";
        return 1;
    }
    out << fixed << setprecision(8);
    out << "# x f'(x) f(x)\n";
    for (int i = 0; i < n; ++i) {
        out << setw(10) << x[i] << " " << setw(12) << df[i] << " " << setw(12) << f[i] << "\n";
    }
    out.close();

    cout << "OK: generé diferencias_finitas.dat con " << n << " puntos.\n";
    cout << "h = " << h << "\n";
    return 0;
}
