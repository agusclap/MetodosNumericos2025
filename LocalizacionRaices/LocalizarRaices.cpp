#include <iostream>
#include <cmath>

using namespace std;

void biseccion();
void falsaposicion(); 

int main (void) {
    int opcion;
    do {
        cout << "Menu de opciones:" << endl;
        cout << "1. Biseccion" << endl;
        cout << "2. Falsa posicion" << endl;
        cout << "3. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                biseccion();
                break;
            case 2:
                falsaposicion(); 
                break;
            case 3:
                cout << "Saliendo del programa." << endl;
                break;
            default:
                cout << "Opcion no valida, intente de nuevo." << endl;
        }
    } while (opcion != 3);
    return 0;
}

void biseccion() {
    double a, b, c, c_viejo, fa, fb, fc, error;
    int iter = 0, max_iter = 100;
    double tol = 1e-6;
    auto f = [](double x) { return x * x - 4; };
    cout << "Ingrese el intervalo [a, b]: ";
    cin >> a >> b;

    fa = f(a);
    fb = f(b);

    if (fa * fb > 0) {
        cerr << "El intervalo no es valido." << endl;
        return;
    }
    c_viejo = a;
    do {
        c = (a + b) / 2;
        fc = f(c);

        error = fabs(c - c_viejo);
        c_viejo = c;

        cout << "Iteracion " << iter + 1 << ": a = " << a << ", b = " << b << ", c = " << c << endl;

        if (error < tol ||fc == 0.0) {
            break;
        }

        if (fa * fc < 0) {
            b = c;
            fb = fc;
        } else {
            a = c;
            fa = fc;
        }
        
        iter++;
    } while (fabs(b - a) > tol && iter < max_iter);

    cout << "La raiz es: " << c << " y el error es: "<<error<<endl;
}

void falsaposicion() {
    double a, b, c, c_viejo, fa, fb, fc, error;
    int iter = 0, max_iter = 100;
    double tol = 1e-6;
    auto f = [](double x) { return x * x - 4; };
    cout << "Ingrese el intervalo [a, b]: ";
    cin >> a >> b;

    fa = f(a);
    fb = f(b);

    if (fa * fb > 0) {
        cerr << "El intervalo no es valido." << endl;
        return;
    }
    c_viejo = a;
    c = a; // Inicializar c para evitar uso de variable no inicializada
    do {
        c = (a * fb - b * fa) / (fb - fa);
        fc = f(c);
        if (fc == 0.0) {
            break; // Si encontramos la raiz exacta
        }

        if (fa* fc > 0){
            a = c; // Actualizar a si el signo es positivo
            fa = fc;
        } else {
            b = c; // Actualizar b si el signo es negativo
            fb = fc;
        }

        error = fabs(c - c_viejo);
        c_viejo = c;
        

        cout << "Iteracion " << iter + 1 << ": a = " << a << ", b = " << b << ", c = " << c << endl;

        if (error < tol || fc == 0.0) {
            break;
        }

        iter++;
    } while (fabs(b - a) > tol && iter < max_iter);

    cout << "La raiz es: " << c << " y el error es: "<<error<<endl;
}


