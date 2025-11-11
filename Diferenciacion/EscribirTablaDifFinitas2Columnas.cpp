#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cmath>
using namespace std;

int main() {
    ifstream in("diferencias_finitas.dat");
    if (!in) {
        cerr << "No se pudo abrir diferencias_finitas.dat\n";
        return 1;
    }

    ofstream out("razon.txt");
    if (!out) {
        cerr << "No se pudo crear razon.txt\n";
        return 1;
    }

    string header;
    getline(in, header); // salta la línea "# x f'(x) f(x)"
    double x, fprima, f;
    out << fixed << setprecision(8);
    while (in >> x >> fprima >> f) {
        if (fabs(f) < 1e-12) continue; // evitar dividir por cero
        double razon = fprima / f;
        out << setw(10) << x << " " << setw(12) << razon << "\n";
    }

    cout << "Archivo razon.txt generado correctamente ✅\n";
    return 0;
}
