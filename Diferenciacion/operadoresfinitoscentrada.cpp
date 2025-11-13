#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

int main() {
    string filename = "datos.dat";  // nombre del archivo
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << filename << endl;
        return 1;
    }

    vector<double> x, y;
    double xv, yv;

    // Leer archivo
    while (file >> xv >> yv) {
        x.push_back(xv);
        y.push_back(yv);
    }
    file.close();

    int n = x.size();
    if (n < 2) {
        cerr << "Error: se necesitan al menos dos puntos." << endl;
        return 1;
    }

    vector<double> dydx(n);

    // Extremo izquierdo → forward
    dydx[0] = (y[1] - y[0]) / (x[1] - x[0]);

    // Centrales
    for (int i = 1; i < n - 1; i++) {
        dydx[i] = (y[i+1] - y[i-1]) / (x[i+1] - x[i-1]);
    }

    // Extremo derecho → backward
    dydx[n-1] = (y[n-1] - y[n-2]) / (x[n-1] - x[n-2]);

    // Mostrar tabla
    cout << fixed << setprecision(6);
    cout << "x\tf(x)\tf'(x)\n";
    for (int i = 0; i < n; i++) {
        cout << x[i] << "\t" << y[i] << "\t" << dydx[i] << "\n";
    }

    return 0;
}
