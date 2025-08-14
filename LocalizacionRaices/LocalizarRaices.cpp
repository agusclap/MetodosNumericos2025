#include <iostream>
#include <cmath>

using namespace std;

void biseccion();
void falsaposicion(); 
void puntofijo();
void newtonRaphson();
void secante();

int main (void) {
    int opcion;
    do {
        cout << "Menu de opciones:" << endl;
        cout << "1. Biseccion [Metodo Cerrado]" << endl;
        cout << "2. Falsa posicion [Metodo Cerrado]" << endl;
        cout << "3. Punto fijo [Metodo Abierto]" << endl;
        cout << "4. Newton-Raphson [Metodo Abierto]" << endl;
        cout<< "5. Secante [Metodo Abierto]" << endl;
        cout<< "6. Salir" << endl;
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
                puntofijo();
                break;
            case 4:
                newtonRaphson();
                break;
            case 5:
                secante();
                break;
            case 6:
                cout << "Saliendo del programa." << endl;
                break;
            default:
                cout << "Opcion no valida, intente de nuevo." << endl;
        }
    } while (opcion != 6);
    return 0;
}

void biseccion() {
    double a, b, c, c_viejo, fa, fb, fc, error;
    int iter = 0, max_iter = 100;
    double tol = 1e-6;
    auto f = [](double x) { return x * x - 9 * x + 2; };
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
    auto f = [](double x) { return x * x - 9 * x + 2; };
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



void puntofijo(){
    double x0, x1, error;
    int iter = 0, max_iter = 100;
    double tol = 1e-6; 
    auto g = [] (double x) {return sqrt((x+5)/2);};  
    cout << "Ingrese el valor inicial x0: ";
    cin >> x0;

    do {
        if(abs((g(x0 + 0.001) - g(x0))/ 0.001) >= 1){
            printf("El metodo no converge");
            exit(1);
        }
        x1 = g(x0);
        error = fabs(x1 - x0);
        cout << "Iteracion " << iter + 1 << ": x0 = " << x0 << ", x1 = " << x1 << endl;

        x0 = x1;
        iter++;
    } while (iter < max_iter && error > tol);

    cout << "La raiz es: " << x1 << ", el error es: " << error << " y la cantidad total de iteraciones es: " << iter << endl;
}


void newtonRaphson(){
    double x0, x1, error;
    int iter = 0, max_iter = 100;
    double tol = 1e-6;
    auto f = [](double x) { return x * x - 9 * x + 2; };
    auto df = [](double x) { return 2 * x - 9; };
    cout << "Ingrese el valor inicial x0: ";
    cin >> x0;

    do {
        if(df(x0) < 1e-6) {
            cerr << "La derivada es pequeña. No se puede continuar." << endl;
            return;
        }
        x1 = x0 - f(x0) / df(x0);
        error = fabs(x1 - x0);
        cout << "Iteracion " << iter + 1 << ": x0 = " << x0 << ", x1 = " << x1 << endl;

        x0 = x1;
        iter++;
    } while (iter < max_iter && error > tol);

    cout << "La raiz es: " << x1 << ", el error es: " << error << " y la cantidad total de iteraciones es: " << iter << endl;
}

void secante() {
    double x0, x1, x2, error;
    int iter = 0, max_iter = 100;
    double tol = 1e-6;
    auto f = [](double x) { return x * x - 9 * x + 2; };
    cout << "Ingrese los valores iniciales x0 y x1: ";
    cin >> x0 >> x1;

    do {
        if(f(x1) - f(x0) < 1e-6) {
            cerr << "Denominador pequenio. No se puede continuar." << endl;
            return;
        }
        x2 = x1 - f(x1) * (x1 - x0) / (f(x1) - f(x0));
        error = fabs(x2 - x1);
        cout << "Iteracion " << iter + 1 << ": x0 = " << x0 << ", x1 = " << x1 << ", x2 = " << x2 << endl;

        x0 = x1;
        x1 = x2;
        iter++;
    } while (iter < max_iter && error > tol);

    cout << "La raiz es: " << x2 << ", el error es: " << error << " y la cantidad total de iteraciones es: " << iter << endl;

}