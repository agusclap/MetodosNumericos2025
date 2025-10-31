#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>
using namespace std;

// EDO: y' = f(x,y)
double f(double x, double y) {
    return (2*x + 1) * sqrt(y); // ejemplo
}

struct Params {
    double x0, xf, y0, h;
    int n; // cantidad de pasos
};

// Lee x0, xf, y0 y (h o n). Ajusta n y h para que sean consistentes.
bool leer_params(Params& p) {
    cout << "Ingrese el valor inicial x0: ";
    if (!(cin >> p.x0)) return false;
    cout << "Ingrese el valor final xf: ";
    if (!(cin >> p.xf)) return false;
    if (p.xf <= p.x0) { cerr << "Se requiere xf > x0.\n"; return false; }
    cout << "Ingrese el valor inicial y0: ";
    if (!(cin >> p.y0)) return false;

    cout << "Ingrese h o n\n1. Ingresar h (paso)\n2. Ingresar n (numero de pasos)\n";
    int choice; cin >> choice;
    if (choice == 1) {
        cout << "Ingrese h: "; cin >> p.h;
        if (p.h <= 0) { cerr << "h debe ser > 0.\n"; return false; }
        p.n = static_cast<int>((p.xf - p.x0) / p.h + 0.5); // redondeo
        if (p.n < 1) { cerr << "n calculado < 1.\n"; return false; }
        p.h = (p.xf - p.x0) / p.n; // re-ajusto h para caer EXACTO en xf
    } else if (choice == 2) {
        cout << "Ingrese n: "; cin >> p.n;
        if (p.n < 1) { cerr << "n debe ser >= 1.\n"; return false; }
        p.h = (p.xf - p.x0) / p.n;
    } else {
        cerr << "Opcion no valida.\n"; return false;
    }
    return true;
}

void guardar(const string& fname, const vector<double>& x, const vector<double>& y) {
    ofstream out(fname);
    if (!out) { cerr << "No se pudo abrir " << fname << "\n"; return; }
    out << fixed << setprecision(12);
    for (size_t i = 0; i < x.size(); ++i) out << x[i] << ' ' << y[i] << '\n';
    cout << "Resultados escritos en " << fname << "\n";
}

void euler_method() {
    Params p;
    if (!leer_params(p)) return;

    vector<double> x(p.n + 1), y(p.n + 1);
    x[0] = p.x0; y[0] = p.y0;

    for (int i = 1; i <= p.n; ++i) {
        x[i] = p.x0 + i * p.h;
        y[i] = y[i-1] + p.h * f(x[i-1], y[i-1]);
    }
    guardar("euler_method.dat", x, y);
}

void heun_method() { // RK2 predictor-corrector (orden 2)
    Params p;
    if (!leer_params(p)) return;

    vector<double> x(p.n + 1), y(p.n + 1);
    x[0] = p.x0; y[0] = p.y0;

    for (int i = 1; i <= p.n; ++i) {
        x[i] = p.x0 + i * p.h;

        double k1 = f(x[i-1], y[i-1]);                 // pendiente en el inicio
        double y_tilde = y[i-1] + p.h * k1;            // predictor (Euler)
        double k2 = f(x[i-1] + p.h, y_tilde);          // pendiente al final
        y[i] = y[i-1] + (p.h / 2.0) * (k1 + k2);       // corrector promedio
    }
    guardar("heun_method.dat", x, y);
}

void runge_kutta_method() { // RK4 clásico (orden 4)
    Params p;
    if (!leer_params(p)) return;

    vector<double> x(p.n + 1), y(p.n + 1);
    x[0] = p.x0; y[0] = p.y0;

    for (int i = 1; i <= p.n; ++i) {
        x[i] = p.x0 + i * p.h;

        double k1 = f(x[i-1], y[i-1]);
        double k2 = f(x[i-1] + p.h/2.0, y[i-1] + (p.h/2.0)*k1);
        double k3 = f(x[i-1] + p.h/2.0, y[i-1] + (p.h/2.0)*k2);
        double k4 = f(x[i-1] + p.h,       y[i-1] +  p.h      *k3);

        y[i] = y[i-1] + (p.h/6.0) * (k1 + 2*k2 + 2*k3 + k4);
    }
    guardar("runge_kutta_method.dat", x, y);
}

void punto_medio_method() {
    Params p;
    if (!leer_params(p)) return;

    vector<double> x(p.n + 1), y(p.n + 1);
    x[0] = p.x0; y[0] = p.y0;

    for (int i = 1; i <= p.n; ++i) {
        x[i] = p.x0 + i * p.h;

        double k1 = f(x[i-1], y[i-1]);
        double k2 = f(x[i-1] + p.h / 2.0, y[i-1] + (p.h / 2.0) * k1);

        y[i] = y[i-1] + p.h * k2;
    }

    guardar("punto_medio_method.dat", x, y);
}


int main() {
    //ios::sync_with_stdio(false);
    //cin.tie(nullptr);

    cout << "Seleccione el metodo de resolucion de EDO:\n";
    cout << "1. Metodo de Euler\n";
    cout << "2. Metodo de Heun\n";
    cout << "3. Metodo de Runge-Kutta (RK4)\n";
    cout << "4. Punto medio" << endl;
    cout << "Ingrese su opcion: ";
    int opcion = 0; cin >> opcion;

    switch (opcion) {
        case 1: cout << "Ha seleccionado el metodo de Euler.\n"; euler_method(); break;
        case 2: cout << "Ha seleccionado el metodo de Heun.\n"; heun_method(); break;
        case 3: cout << "Ha seleccionado el metodo de Runge-Kutta (RK4).\n"; runge_kutta_method(); break;
        case 4: cout << "Ha seleccionado el metodo del punto medio.\n"; punto_medio_method(); break;
        default: cerr << "Opcion no valida.\n"; return 1;
    }
    return 0;
}
