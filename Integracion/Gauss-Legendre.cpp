#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
using namespace std;

// --------- FUNCION ANALITICA ORIGINAL ----------
// EDO: y' = f(x,y)  (acá solo integrás f(x), no y')
double f (double x) {
    return exp(sqrt(1.0 + x)) * ((log(1.0 + 2.0*x*x))/(2*sqrt(1+x)) + (4*x)/(1+2*x*x)); // ejemplo
}

// --------- SOPORTE PARA TABLA ----------
vector<double> Xtab, Ytab;
bool usar_tabla = false;

// Lee archivo de tabla: cada línea "x  f(x)"
bool leer_tabla(const string& filename) {
    ifstream in(filename);
    if (!in) {
        cerr << "No se pudo abrir el archivo de tabla: " << filename << endl;
        return false;
    }
    double xt, yt;
    Xtab.clear();
    Ytab.clear();
    while (in >> xt >> yt) {
        Xtab.push_back(xt);
        Ytab.push_back(yt);
    }
    if (Xtab.size() < 2) {
        cerr << "La tabla debe tener al menos 2 puntos.\n";
        return false;
    }
    return true;
}

// Interpolación lineal sobre la tabla
double f_tabla(double x) {
    int n = (int)Xtab.size();

    // Fuera de rango: clamp (usar extremo más cercano)
    if (x <= Xtab[0]) return Ytab[0];
    if (x >= Xtab[n-1]) return Ytab[n-1];

    // Buscar intervalo [Xi, Xi+1] que contiene a x
    for (int i = 0; i < n - 1; ++i) {
        if (x >= Xtab[i] && x <= Xtab[i+1]) {
            double t = (x - Xtab[i]) / (Xtab[i+1] - Xtab[i]);
            return Ytab[i]*(1.0 - t) + Ytab[i+1]*t;
        }
    }
    // No debería llegar acá si la tabla está ordenada
    return 0.0;
}

// Evaluador genérico: usa f() o f_tabla() según la bandera
double eval_f(double x) {
    if (usar_tabla) return f_tabla(x);
    else            return f(x);
}

int main (void) {
    double a, b, I = 0.0;
    int n;

    cout << "Ingresar los limites de integracion (a,b): ";
    cin >> a >> b;

    // Elegir modo de evaluacion
    cout << "Seleccione modo de integracion:\n";
    cout << "1) Funcion analitica f(x)\n";
    cout << "2) Leer f(x) desde tabla (archivo)\n";
    int modo;
    cin >> modo;

    if (modo == 2) {
        usar_tabla = true;
        string nombre;
        cout << "Ingrese el nombre del archivo de tabla (x f(x)): ";
        cin >> nombre;
        if (!leer_tabla(nombre)) {
            return 1; // error leyendo tabla
        }
    } else {
        usar_tabla = false;
    }

    // Calculo de I por Gauss-Legendre
    cout << "Ingresar el numero de puntos (n): ";
    cin >> n;

    switch(n) {
        case 2:
        {
            double c0 = 1.0;
            double c1 = 1.0;
            double x0 = -0.5773502691896257;
            double x1 = -x0;

            double x0m = ((b-a)*x0 + (b+a))/2.0;
            double x1m = ((b-a)*x1 + (b+a))/2.0;

            I = (b-a)/2 * (c0*eval_f(x0m) + c1*eval_f(x1m));
            break;
        }
        case 3:
        {
            double c0 = 0.5555555555555556;
            double c1 = 0.8888888888888888;
            double c2 = c0;
            double x0 = -0.7745966692414834;
            double x1 = 0.0;
            double x2 = -x0;

            double x0m = ((b-a)*x0 + (b+a))/2.0;
            double x1m = ((b-a)*x1 + (b+a))/2.0;
            double x2m = ((b-a)*x2 + (b+a))/2.0;

            I = (b-a)/2 * (c0*eval_f(x0m) + c1*eval_f(x1m) + c2*eval_f(x2m));
            break;
        }
        case 4:
        {
            double c0 = 0.3478548451374538;
            double c1 = 0.6521451548625461;
            double c2 = c1;
            double c3 = c0;
            double x0 = -0.8611363115940526;
            double x1 = -0.3399810435848563;
            double x2 = -x1;
            double x3 = -x0;

            double x0m = ((b-a)*x0 + (b+a))/2.0;
            double x1m = ((b-a)*x1 + (b+a))/2.0;
            double x2m = ((b-a)*x2 + (b+a))/2.0;
            double x3m = ((b-a)*x3 + (b+a))/2.0;

            I = (b-a)/2 * (c0*eval_f(x0m) + c1*eval_f(x1m)
                         + c2*eval_f(x2m) + c3*eval_f(x3m));
            break;
        }
        case 5:
        {
            double c0 = 0.2369268850561891;
            double c1 = 0.4786286704993665;
            double c2 = 0.5688888888888889;
            double c3 = c1;
            double c4 = c0;
            double x0 = -0.9061798459386640;
            double x1 = -0.5384693101056831;
            double x2 = 0.0;
            double x3 = -x1;
            double x4 = -x0;

            double x0m = ((b-a)*x0 + (b+a))/2.0;
            double x1m = ((b-a)*x1 + (b+a))/2.0;
            double x2m = ((b-a)*x2 + (b+a))/2.0;
            double x3m = ((b-a)*x3 + (b+a))/2.0;
            double x4m = ((b-a)*x4 + (b+a))/2.0;

            I = (b-a)/2 * (c0*eval_f(x0m) + c1*eval_f(x1m)
                         + c2*eval_f(x2m) + c3*eval_f(x3m)
                         + c4*eval_f(x4m));
            break;
        }
        case 6:
        {
            double c0 = 0.1713244923791704;
            double c1 = 0.3607615730481386;
            double c2 = 0.4679139345726910;
            double c3 = c2;
            double c4 = c1;
            double c5 = c0;
            double x0 = -0.9324695142031521;
            double x1 = -0.6612093864662645;
            double x2 = -0.2386191860831969;
            double x3 = -x2;
            double x4 = -x1;
            double x5 = -x0;

            double x0m = ((b-a)*x0 + (b+a))/2.0;
            double x1m = ((b-a)*x1 + (b+a))/2.0;
            double x2m = ((b-a)*x2 + (b+a))/2.0;
            double x3m = ((b-a)*x3 + (b+a))/2.0;
            double x4m = ((b-a)*x4 + (b+a))/2.0;
            double x5m = ((b-a)*x5 + (b+a))/2.0;

            I = (b-a)/2 * (c0*eval_f(x0m) + c1*eval_f(x1m)
                         + c2*eval_f(x2m) + c3*eval_f(x3m)
                         + c4*eval_f(x4m) + c5*eval_f(x5m));
            break;
        }
        default:
            cout << "n debe estar entre 2 y 6" << endl;
            return 0;
    }

    cout << "El valor de la integral es: " << I << endl;
    return 0;
}
