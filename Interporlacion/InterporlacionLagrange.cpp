#include <iostream>
#include <cstdio>
using namespace std;

const int n = 3; // número de puntos

int main() {
    double x[n+1], y[n+1], xs, suma = 0, prod;

    FILE *fp = fopen("datos.dat", "r");
    if (!fp) { cerr << "Error al abrir el archivo" << endl; return 1; }

    for (int i = 0; i < n; i++) {
        if (fscanf(fp, "%lf %lf", &x[i], &y[i]) != 2) {
            cerr << "Error de formato en el archivo" << endl;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);

    // check x duplicados
    for (int i = 0; i < n; ++i)
        for (int j = i+1; j < n; ++j)
            if (x[i] == x[j]) { cerr << "x repetidos: no se puede interpolar" << endl; return 1; }

    cout << "Ingrese x (sombrerito)" << endl;
    cin >> xs;

    // si xs coincide exactamente con un nodo
    for (int i = 0; i < n; ++i)
        if (xs == x[i]) { 
            cout.setf(ios::fixed); cout.precision(10);
            cout << "El valor interpolado en x = " << xs << " es: " << y[i] << endl; 
            return 0; 
        }

    for (int k = 0; k < n; k++) {
        prod = 1.0;
        for (int i = 0; i < n; i++) {
            if (i != k) {
                double denom = (x[k] - x[i]);
                if (denom == 0.0) { cerr << "Division por cero en Lagrange" << endl; return 1; }
                prod *= (xs - x[i]) / denom;
            }
        }
        suma += y[k] * prod;
    }

    cout.setf(ios::fixed); cout.precision(10);
    cout << "El valor interpolado en x = " << xs << " es: " << suma << endl;
    return 0;
}
