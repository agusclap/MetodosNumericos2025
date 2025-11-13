#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>
using namespace std;

// EDO: y' = f(x,y)
double f(double x, double y) {
    return (4*x*y) / (1 + x*x);
}
    
// Solución exacta para y' = (1+x) sqrt(y), y(0)=1
double y_exact(double x) {
    double s = 1.0 + x*x; // 1 + x/2 + x^2/4
    return s * s;                            // ( ... )^2
}

// Parámetros del método
struct Params {
    double x0, xf, y0, h;
    int n;
};

// Leer x0, xf, y0 y h o n
bool leer_params(Params& p) {
    cout << "Ingrese el valor inicial x0: ";
    if (!(cin >> p.x0)) return false;

    cout << "Ingrese el valor final xf: ";
    if (!(cin >> p.xf)) return false;
    if (p.xf <= p.x0) {
        cerr << "Se requiere xf > x0.\n";
        return false;
    }

    cout << "Ingrese el valor inicial y0: ";
    if (!(cin >> p.y0)) return false;

    cout << "Ingrese h o n\n1. Ingresar h\n2. Ingresar n\n";
    int choice;
    cin >> choice;

    if (choice == 1) {
        cout << "Ingrese h: ";
        cin >> p.h;
        if (p.h <= 0) { cerr << "h debe ser > 0.\n"; return false; }

        p.n = static_cast<int>((p.xf - p.x0) / p.h + 0.5);
        if (p.n < 1) { cerr << "n < 1.\n"; return false; }

        p.h = (p.xf - p.x0) / p.n;

    } else if (choice == 2) {
        cout << "Ingrese n: ";
        cin >> p.n;
        if (p.n < 1) { cerr << "n debe ser >= 1.\n"; return false; }
        p.h = (p.xf - p.x0) / p.n;

    } else {
        cerr << "Opcion no valida.\n";
        return false;
    }

    return true;
}

// -------------------------------------------------------------
// GUARDA LA TABLA CON ERRORES
// -------------------------------------------------------------
void guardar(const string& fname, const vector<double>& x, const vector<double>& y) {
    ofstream out(fname);
    if (!out) {
        cerr << "No se pudo abrir " << fname << "\n";
        return;
    }

    out << fixed << setprecision(12);
    out << "# x    y_num    y_exact    error_abs    error_rel_pct\n";

    // Nota: esta función la usan TODOS los métodos.
    for (size_t i = 0; i < x.size(); ++i) {

        // ============================================================
        // OPCIONAL: SOLO IMPRIMIR CADA 0.2 EN x (por ejemplo, para el
        // enunciado que pide x = 0, 0.2, 0.4, ..., 1 con h = 0.01).
        //
        // Para usarlo:
        //  1) asegurate de que el paso h sea uniforme (como acá).
        //  2) DESCOMENTÁ el bloque siguiente.
        //
        // Explicación:
        //  - h ~ x[1] - x[0]
        //  - k = 0.2 / h  → con h = 0.01 ⇒ k = 20
        //  - imprimimos solo cuando i es múltiplo de k
        //    (y siempre el último punto).
        // ----------------------------------------------------
        /*
        if (x.size() > 1) {
            double h_aprox = x[1] - x[0];
            double paso_salida = 0.2;              // queremos cada 0.2
            int    k = (int)lround(paso_salida / h_aprox); // ~20 si h=0.01

            // si NO es múltiplo de k y NO es el último punto, lo saltamos
            if ((int)i % k != 0 && i != x.size() - 1) {
                continue;
            }
        }
        */
        // ============================================================
         // ============================================================
    // OPCIONAL 2: SOLO IMPRIMIR LOS ULTIMOS 5 PUNTOS
    // Usar esto para el inciso c) (RK4).
    // Descomentá este bloque cuando quieras esa tabla.
    /*
    if (x.size() > 5 && i < x.size() - 5) {
        // si no estoy en los ultimos 5 indices, lo salto
        continue;
    }
    */
    // ============================================================
        double ye = y_exact(x[i]);
        double e_abs = fabs(ye - y[i]);
        double e_rel = (e_abs / fabs(ye)) * 100.0;

        out << x[i] << "  "
            << y[i] << "  "
            << ye << "  "
            << e_abs << "  "
            << e_rel << "\n";
    }

    cout << "Resultados escritos en: " << fname << endl;
}

// -------------------------------------------------------------
// MÉTODO DE EULER
// -------------------------------------------------------------
void euler_method() {
    Params p;
    if (!leer_params(p)) return;

    vector<double> x(p.n + 1), y(p.n + 1);
    x[0] = p.x0;
    y[0] = p.y0;

    for (int i = 1; i <= p.n; ++i) {
        x[i] = p.x0 + i * p.h;
        y[i] = y[i - 1] + p.h * f(x[i - 1], y[i - 1]);
    }

    guardar("euler_method.dat", x, y);
}

// -------------------------------------------------------------
// MÉTODO DE HEUN (RK2)
// -------------------------------------------------------------
void heun_method() {
    Params p;
    if (!leer_params(p)) return;

    vector<double> x(p.n + 1), y(p.n + 1);
    x[0] = p.x0;
    y[0] = p.y0;

    for (int i = 1; i <= p.n; ++i) {
        x[i] = p.x0 + i * p.h;

        double k1 = f(x[i - 1], y[i - 1]);
        double y_tilde = y[i - 1] + p.h * k1;
        double k2 = f(x[i - 1] + p.h, y_tilde);

        y[i] = y[i - 1] + (p.h / 2.0) * (k1 + k2);
    }

    guardar("heun_method.dat", x, y);
}

// -------------------------------------------------------------
// MÉTODO RUNGE–KUTTA DE ORDEN 4
// -------------------------------------------------------------
void runge_kutta_method() {
    Params p;
    if (!leer_params(p)) return;

    vector<double> x(p.n + 1), y(p.n + 1);
    x[0] = p.x0;
    y[0] = p.y0;

    for (int i = 1; i <= p.n; ++i) {
        x[i] = p.x0 + i * p.h;

        double k1 = f(x[i - 1], y[i - 1]);
        double k2 = f(x[i - 1] + p.h / 2.0, y[i - 1] + (p.h / 2.0) * k1);
        double k3 = f(x[i - 1] + p.h / 2.0, y[i - 1] + (p.h / 2.0) * k2);
        double k4 = f(x[i - 1] + p.h, y[i - 1] + p.h * k3);

        y[i] = y[i - 1] + (p.h / 6.0) * (k1 + 2*k2 + 2*k3 + k4);
    }

    guardar("runge_kutta_method.dat", x, y);
}

// -------------------------------------------------------------
// MÉTODO DEL PUNTO MEDIO
// -------------------------------------------------------------
void punto_medio_method() {
    Params p;
    if (!leer_params(p)) return;

    vector<double> x(p.n + 1), y(p.n + 1);
    x[0] = p.x0;
    y[0] = p.y0;

    for (int i = 1; i <= p.n; ++i) {
        x[i] = p.x0 + i * p.h;

        double k1 = f(x[i - 1], y[i - 1]);
        double k2 = f(x[i - 1] + p.h / 2.0, y[i - 1] + (p.h / 2.0) * k1);

        y[i] = y[i - 1] + p.h * k2;
    }

    guardar("punto_medio_method.dat", x, y);
}
void metodo_dos_pasos() {
    Params p;
    if (!leer_params(p)) return;

    vector<double> x(p.n + 1), y(p.n + 1);
    x[0] = p.x0;
    y[0] = p.y0;

    // PASO i = 0 ➜ y1 usando Euler (obligatorio por el método)
    x[1] = x[0] + p.h;
    y[1] = y[0] + p.h * f(x[0], y[0]);

    // PASO i >= 1 ➜ fórmula del enunciado
    // yi+1 = yi + h ( 2 f(xi, yi) – f(xi-1, yi-1) )
    for (int i = 1; i < p.n; i++) {

        x[i+1] = x[i] + p.h;

        double fi   = f(x[i],   y[i]);
        double fim1 = f(x[i-1], y[i-1]);

        y[i+1] = y[i] + p.h * (2.0 * fi - fim1);
    }

    guardar("metodo_dos_pasos.dat", x, y);
}

/*PSUEDOCODIGO DE EL METODO DE ARRIBA
    Datos:
    EDO: y' = f(x, y)
    Intervalo: [x0, xf]
    Condición inicial: y(x0) = y0
    Paso: h

1. Leer x0, xf, y0, h
2. Calcular n = (xf - x0) / h          (n = cantidad de pasos)
3. Reservar vectores x[0..n], y[0..n]

4. Asignar:
       x[0] = x0
       y[0] = y0

5. Calcular y1 con método de Euler (i = 0):
       x[1] = x[0] + h
       y[1] = y[0] + h * f(x[0], y[0])

6. Para i = 1 hasta n-1 hacer:
       x[i+1] = x[i] + h

       fi   = f(x[i],   y[i])
       fim1 = f(x[i-1], y[i-1])

       y[i+1] = y[i] + h * (2*fi - fim1)

7. Imprimir o guardar la tabla de (x[i], y[i]) para i = 0..n

*/
// -------------------------------------------------------------
// MENÚ PRINCIPAL
// -------------------------------------------------------------
int main() {
    cout << "Seleccione el metodo de resolucion de EDO:\n";
    cout << "1. Euler\n";
    cout << "2. Heun\n";
    cout << "3. Runge-Kutta (RK4)\n";
    cout << "4. Punto Medio\n";
    cout << "5. Metodo de dos pasos (del enunciado)\n";
    cout << "Ingrese su opcion: ";

    int opcion;
    cin >> opcion;

    switch (opcion) {
        case 1: cout << "Metodo de Euler seleccionado.\n"; euler_method(); break;
        case 2: cout << "Metodo de Heun seleccionado.\n"; heun_method(); break;
        case 3: cout << "Metodo de Runge-Kutta (RK4) seleccionado.\n"; runge_kutta_method(); break;
        case 4: cout << "Metodo de Punto Medio seleccionado.\n"; punto_medio_method(); break;
        case 5: cout << "Metodo de dos pasos seleccionado.\n"; metodo_dos_pasos(); break;
        default: cerr << "Opcion no valida.\n"; return 1;
    }

    return 0;
}



