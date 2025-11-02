#include <iostream>
#include <vector>
#include <cmath>
#include <cstdio>    // fopen, fscanf, fclose
#include <algorithm> // max
#include <iomanip>   // setprecision
#include <stdexcept>
using namespace std;

// ---------------------------------------------------------
// Función a integrar (podés cambiarla)
double f(double x) { return x * x; }
// ---------------------------------------------------------

// ---------------------------------------------------------
// LECTURA DE TABLA CON fopen/fscanf/fclose
// Formato esperado del archivo: una pareja "x y" por línea.
// Ejemplo:
// 0 0
// 1 1
// 2 4
// 3 9
// ---------------------------------------------------------
bool leer_tabla_archivo(const char* filename, vector<double>& x, vector<double>& y) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        cerr << "No se pudo abrir el archivo: " << filename << "\n";
        return false;
    }
    x.clear(); y.clear();

    double xi, yi;
    while (fscanf(fp, "%lf %lf", &xi, &yi) == 2) {
        x.push_back(xi);
        y.push_back(yi);
    }

    fclose(fp);

    if (x.size() < 2) {
        cerr << "Se requieren al menos 2 puntos (x,y).\n";
        return false;
    }
    // Validar orden estrictamente creciente en x
    for (size_t i = 1; i < x.size(); ++i) {
        if (!(x[i] > x[i-1])) {
            cerr << "Error: x debe estar en orden estrictamente creciente.\n";
            return false;
        }
    }
    return true;
}

// ---------------------------------------------------------
// Chequeo de paso "casi uniforme"
// ---------------------------------------------------------
bool es_uniforme(const vector<double>& x, double tol_rel = 1e-6) {
    if (x.size() < 3) return true;
    double h0 = x[1] - x[0];
    if (h0 <= 0) return false;
    for (size_t i = 1; i + 1 < x.size(); ++i) {
        double hi = x[i+1] - x[i];
        double rel = fabs(hi - h0) / max(1.0, fabs(h0));
        if (rel > tol_rel) return false;
    }
    return true;
}

// ---------------------------------------------------------
// SPLINE CÚBICO NATURAL (1D)
// Construye segundas derivadas m[i] y evalúa con búsqueda binaria.
// ---------------------------------------------------------
struct SplineCubico {
    vector<double> x, y, m; // m = y'' en nodos

    bool build(const vector<double>& X, const vector<double>& Y) {
        int n = (int)X.size();
        if (n < 2 || (int)Y.size() != n) return false;
        x = X; y = Y; m.assign(n, 0.0);
        if (n == 2) { m[0] = m[1] = 0.0; return true; }

        vector<double> h(n - 1);
        for (int i = 0; i < n - 1; ++i) h[i] = x[i+1] - x[i];

        vector<double> a(n), b(n), c(n), d(n);
        // Condiciones naturales: m[0] = m[n-1] = 0
        a[0] = 0; b[0] = 1; c[0] = 0; d[0] = 0;
        for (int i = 1; i < n - 1; ++i) {
            a[i] = h[i-1];
            b[i] = 2 * (h[i-1] + h[i]);
            c[i] = h[i];
            d[i] = 6 * ((y[i+1] - y[i]) / h[i] - (y[i] - y[i-1]) / h[i-1]);
        }
        a[n-1] = 0; b[n-1] = 1; c[n-1] = 0; d[n-1] = 0;

        // Resolver tridiagonal (Thomas)
        for (int i = 1; i < n; ++i) {
            double w = a[i] / b[i-1];
            b[i] -= w * c[i-1];
            d[i] -= w * d[i-1];
        }
        m[n-1] = d[n-1] / b[n-1];
        for (int i = n - 2; i >= 0; --i) {
            m[i] = (d[i] - c[i] * m[i+1]) / b[i];
        }
        return true;
    }

    double eval(double xq) const {
        int n = (int)x.size();
        if (n == 0) return NAN;
        if (xq <= x.front()) return y.front();
        if (xq >= x.back())  return y.back();

        int lo = 0, hi = n - 1;
        while (hi - lo > 1) {
            int mid = (lo + hi) / 2;
            if (x[mid] <= xq) lo = mid; else hi = mid;
        }
        double h = x[lo+1] - x[lo];
        double A = (x[lo+1] - xq) / h;
        double B = (xq - x[lo]) / h;
        double S = A * y[lo] + B * y[lo+1]
                 + ((A*A*A - A) * m[lo] + (B*B*B - B) * m[lo+1]) * (h*h) / 6.0;
        return S;
    }
};

// ---------------------------------------------------------
// INTEGRACIÓN
// ---------------------------------------------------------

// Trapecio (función), n subintervalos
double trapecio_func(double a, double b, int n) {
    if (n < 1) throw runtime_error("n >= 1");
    double h = (b - a) / n;
    double suma = f(a) + f(b);
    for (int i = 1; i <= n - 1; ++i) {
        double x = a + i * h;
        suma += 2.0 * f(x);
    }
    return (h / 2.0) * suma;
}

// Simpson (función), n par
double simpson_func(double a, double b, int n) {
    if (n < 2 || (n % 2) != 0) throw runtime_error("n debe ser par y >= 2");
    double h = (b - a) / n;
    double suma = f(a) + f(b);
    for (int i = 1; i <= n - 1; ++i) {
        double x = a + i * h;
        if (i % 2 == 1) suma += 4.0 * f(x);
        else            suma += 2.0 * f(x);
    }
    return (h / 3.0) * suma;
}

// Trapecio (tabla no uniforme)
double trapecio_tabla_no_uniforme(const vector<double>& x, const vector<double>& y) {
    int n = (int)x.size();
    if (n < 2 || (int)y.size() != n) throw runtime_error("tabla invalida");
    double acc = 0.0;
    for (int i = 0; i < n - 1; ++i) {
        double h = x[i+1] - x[i];
        acc += 0.5 * h * (y[i] + y[i+1]);
    }
    return acc;
}

// Simpson directo (tabla uniforme): requiere #subintervalos par
double simpson_tabla_uniforme(const vector<double>& x, const vector<double>& y) {
    int n = (int)x.size();
    if (n < 3 || (int)y.size() != n) throw runtime_error("tabla invalida");
    int m = n - 1; // subintervalos
    if (m % 2 != 0) throw runtime_error("Subintervalos impares: Simpson requiere pares.");
    double h = x[1] - x[0];
    double suma = y.front() + y.back();
    for (int i = 1; i <= m - 1; ++i) {
        if (i % 2 == 1) suma += 4.0 * y[i];
        else            suma += 2.0 * y[i];
    }
    return (h / 3.0) * suma;
}

// Simpson vía spline (tabla NO uniforme): muestrea spline en malla uniforme fina
double simpson_via_spline(const vector<double>& x, const vector<double>& y, int N_uniform = 200) {
    if ((N_uniform % 2) != 0) N_uniform++; // asegurar par
    SplineCubico sp;
    if (!sp.build(x, y)) throw runtime_error("No se pudo construir el spline.");
    double a = x.front(), b = x.back();
    double h = (b - a) / N_uniform;

    double suma = sp.eval(a) + sp.eval(b);
    for (int i = 1; i <= N_uniform - 1; ++i) {
        double xi = a + i * h;
        if (i % 2 == 1) suma += 4.0 * sp.eval(xi);
        else            suma += 2.0 * sp.eval(xi);
    }
    return (h / 3.0) * suma;
}

// ---------------------------------------------------------
// MENU
// ---------------------------------------------------------
void menu() {
    cout << "\n=== Integracion Numerica ===\n"
         << "1) Trapecio (funcion f(x))\n"
         << "2) Simpson (funcion f(x))\n"
         << "3) Trapecio (tabla no uniforme, lee archivo)\n"
         << "4) Simpson (tabla: uniforme directo / no uniforme via spline; lee archivo)\n"
         << "0) Salir\n"
         << "Opcion: ";
}

int main() {


    cout << fixed << setprecision(10);

    while (true) {
        menu();
        int op;
        if (!(cin >> op)) return 0;
        try {
            if (op == 0) break;

            if (op == 1) {
                double a, b; int n;
                cout << "Ingrese a b n: ";
                cin >> a >> b >> n;
                double I = trapecio_func(a, b, n);
                cout << "Trapecio (funcion) = " << I << "\n";
            }
            else if (op == 2) {
                double a, b; int n;
                cout << "Ingrese a b n (n par): ";
                cin >> a >> b >> n;
                double I = simpson_func(a, b, n);
                cout << "Simpson (funcion) = " << I << "\n";
            }
            else if (op == 3) {
                string fname;
                cout << "Archivo de tabla (x y por linea): ";
                cin >> fname;
                vector<double> x, y;
                if (!leer_tabla_archivo(fname.c_str(), x, y)) continue;
                double I = trapecio_tabla_no_uniforme(x, y);
                cout << "Trapecio (tabla no uniforme) = " << I << "\n";
            }
            else if (op == 4) {
                string fname;
                cout << "Archivo de tabla (x y por linea): ";
                cin >> fname;
                vector<double> x, y;
                if (!leer_tabla_archivo(fname.c_str(), x, y)) continue;

                if (es_uniforme(x)) {
                    int m = (int)x.size() - 1;
                    if (m % 2 == 0) {
                        double I = simpson_tabla_uniforme(x, y);
                        cout << "Simpson (tabla uniforme) = " << I << "\n";
                    } else {
                        cout << "Aviso: subintervalos impares; aplico spline + Simpson en malla uniforme.\n";
                        double I = simpson_via_spline(x, y, 200);
                        cout << "Simpson (via spline) = " << I << "\n";
                    }
                } else {
                    cout << "Tabla no uniforme detectada; aplico spline + Simpson en malla uniforme.\n";
                    double I = simpson_via_spline(x, y, 200);
                    cout << "Simpson (via spline) = " << I << "\n";
                }
            }
            else {
                cout << "Opcion invalida.\n";
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << "\n";
        }
    }
    return 0;
}
