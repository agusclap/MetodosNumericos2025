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
    ifstream in("tabla_fx.dat");
    if (!in) {
        cerr << "No se pudo abrir tabla_fx.dat\n";
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
    // Se necesitan al menos 3 puntos para las fórmulas de 3 puntos (O(h²))
    if (n < 3) {
        cerr << "Se necesitan al menos 3 puntos para usar diferencias O(h^2).\n";
        return 1;
    }

    // h uniforme
    double h = x[1] - x[0];
    vector<double> df(n);

    // ** Extremo izquierdo (i=0): Diferencia Forward de 3 puntos (O(h²)) **
    // f'(x_0) ≈ (-3*f(x_0) + 4*f(x_1) - f(x_2)) / (2*h)
    df[0] = (-3.0 * f[0] + 4.0 * f[1] - f[2]) / (2.0 * h);

    // ** Interior (1 <= i < n-1): Diferencia Central de 3 puntos (O(h²)) **
    // f'(x_i) ≈ (f(x_{i+1}) - f(x_{i-1})) / (2*h)
    for (int i = 1; i < n - 1; ++i) {
        df[i] = (f[i + 1] - f[i - 1]) / (2.0 * h);
    }

    // ** Extremo derecho (i=n-1): Diferencia Backward de 3 puntos (O(h²)) **
    // f'(x_{n-1}) ≈ (3*f(x_{n-1}) - 4*f(x_{n-2}) + f(x_{n-3})) / (2*h)
    df[n - 1] = (3.0 * f[n - 1] - 4.0 * f[n - 2] + f[n - 3]) / (2.0 * h);

    // -----------------------------------------------------------------

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
    cout << "Fórmulas usadas: O(h^2) para todos los puntos.\n";
    cout << "h = " << h << "\n";
    return 0;
}