#include <iostream>
#include <fstream>
using namespace std;

#define ARRAY_SIZE 100

void euler();

void runge_kutta();

void heun();

void punto_medio();

double f1 (double x, double y1, double y2) {
    return 3*x + y2;
}

double f2 (double x, double y1, double y2) {
    return x*x - y1 - 1;
}


int main(void)
{

    int opcion = 0;
    do
    {
        cout << "Seleccione el metodo para resolver el sistema de EDOs:" << endl;
        cout << "1. Metodo de Euler" << endl;
        cout << "2. Metodo de Runge-Kutta de orden 4" << endl;
        cout << "3. Metodo de Heun" << endl;
        cout << "4. Metodo del Punto Medio" << endl;
        cin >> opcion;
        switch (opcion)
        {
        case 1:
            euler();
            break;

        case 2:
            runge_kutta();
            break;
        case 3:
            heun();
            break;
        case 4:
            punto_medio();
            break;
        default:
            cout << "Opcion no valida" << endl;
            break;
        }
    } while (opcion < 1 || opcion > 4);
    return 0;
}

void euler()
{
    double x0, xf, h, y1[ARRAY_SIZE], y2[ARRAY_SIZE], x[ARRAY_SIZE];
    int n = 0;
    int opcion = 0;
    cout << "Ingrese el valor inicial de x: ";
    cin >> x0;
    cout << "Ingrese el valor final de x: ";
    cin >> xf;
    cout << "Ingrese el valor inicial de y1: ";
    cin >> y1[0];
    cout << "Ingrese el valor inicial de y2: ";
    cin >> y2[0];
    cout << "1. Ingresar paso h" << endl;
    cout << "2. Ingresar numero de pasos n" << endl;
    cin >> opcion;
    if (opcion == 1)
    {
        cout << "Ingrese el valor del paso h: ";
        cin >> h;
        if (h <= 0)
        {
            cerr << "El paso h debe ser positivo.\n";
            return;
        }

        // Calcula n redondeando al entero más cercano
        n = static_cast<int>((xf - x0) / h + 0.5);
        if (n < 1)
        {
            cerr << "Error: n calculado < 1.\n";
            return;
        }
        if (n + 1 > ARRAY_SIZE)
        {
            cerr << "Error: demasiados pasos (" << n << "). Aumenta ARRAY_SIZE.\n";
            return;
        }

        // Ajusta h para que el último punto caiga justo en xf
        h = (xf - x0) / n;
    }
    else if (opcion == 2)
    {
        cout << "Ingrese el numero de pasos n: ";
        cin >> n;
        if (n < 1)
        {
            cerr << "Error: n debe ser >= 1.\n";
            return;
        }
        if (n + 1 > ARRAY_SIZE)
        {
            cerr << "Error: demasiados pasos (" << n << "). Aumenta ARRAY_SIZE.\n";
            return;
        }

        // Calcula h exacto para cubrir [x0, xf]
        h = (xf - x0) / n;
    }

    x[0] = x0;

    // Metodo de Euler
    for (int i = 0; i < n; i++)
    {
        x[i + 1] = x[i] + h;
        y1[i + 1] = y1[i] + h * f1(x[i], y1[i], y2[i]);
        y2[i + 1] = y2[i] + h * f2(x[i], y1[i], y2[i]); // Si mi sistema tiene mas ecuaciones, seguiria asi
    }
    // Resultados escritos en archivo
    ofstream archivo("resultadosEuler1.dat");
    for (int i = 0; i <= n; i++)
    {
        archivo << x[i] << " " << y1[i] << endl;
    }
    archivo.close();
    ofstream archivo2("resultadosEuler2.dat");
    for (int i = 0; i <= n; i++)
    {
        archivo2 << x[i] << " " << y2[i] << endl;
    }
    archivo2.close();
}

void runge_kutta()
{
    double x0, xf, h, y1[ARRAY_SIZE], y2[ARRAY_SIZE], x[ARRAY_SIZE];
    int n = 0;
    int opcion = 0;
    cout << "Ingrese el valor inicial de x: ";
    cin >> x0;
    cout << "Ingrese el valor final de x: ";
    cin >> xf;
    cout << "Ingrese el valor inicial de y1: ";
    cin >> y1[0];
    cout << "Ingrese el valor inicial de y2: ";
    cin >> y2[0];
    cout << "1. Ingresar paso h" << endl;
    cout << "2. Ingresar numero de pasos n" << endl;
    cin >> opcion;
    if (opcion == 1)
    {
        cout << "Ingrese el valor del paso h: ";
        cin >> h;
        if (h <= 0)
        {
            cerr << "El paso h debe ser positivo.\n";
            return;
        }

        // Calcula n redondeando al entero más cercano
        n = static_cast<int>((xf - x0) / h + 0.5);
        if (n < 1)
        {
            cerr << "Error: n calculado < 1.\n";
            return;
        }
        if (n + 1 > ARRAY_SIZE)
        {
            cerr << "Error: demasiados pasos (" << n << "). Aumenta ARRAY_SIZE.\n";
            return;
        }

        // Ajusta h para que el último punto caiga justo en xf
        h = (xf - x0) / n;
    }
    else if (opcion == 2)
    {
        cout << "Ingrese el numero de pasos n: ";
        cin >> n;
        if (n < 1)
        {
            cerr << "Error: n debe ser >= 1.\n";
            return;
        }
        if (n + 1 > ARRAY_SIZE)
        {
            cerr << "Error: demasiados pasos (" << n << "). Aumenta ARRAY_SIZE.\n";
            return;
        }

        // Calcula h exacto para cubrir [x0, xf]
        h = (xf - x0) / n;
    }

    x[0] = x0;

    // Metodo de Runge-Kutta de orden 4
    double k11, k12, k21, k22, k31, k32, k41, k42;
    for (int i = 0; i < n; i++)
    {
        x[i + 1] = x[i] + h;
        k11 = f1(x[i], y1[i], y2[i]);
        k12 = f2(x[i], y1[i], y2[i]);
        k21 = f1(x[i] + h / 2, y1[i] + h / 2 * k11, y2[i] + h / 2 * k12);
        k22 = f2(x[i] + h / 2, y1[i] + h / 2 * k11, y2[i] + h / 2 * k12);
        k31 = f1(x[i] + h / 2, y1[i] + h / 2 * k21, y2[i] + h / 2 * k22);
        k32 = f2(x[i] + h / 2, y1[i] + h / 2 * k21, y2[i] + h / 2 * k22);
        k41 = f1(x[i] + h, y1[i] + h * k31, y2[i] + h * k32);
        k42 = f2(x[i] + h, y1[i] + h * k31, y2[i] + h * k32);
        y1[i + 1] = y1[i] + (h / 6.0) * (k11 + 2 * k21 + 2 * k31 + k41);
        y2[i + 1] = y2[i] + (h / 6.0) * (k12 + 2 * k22 + 2 * k32 + k42);
    }

    // Resultados escritos en dos archivos
    ofstream archivo("resultadosRungeKutta1.dat");
    for (int i = 0; i <= n; i++)
    {
        archivo << x[i] << " " << y1[i] << endl;
    }
    archivo.close();

    ofstream archivo2("resultadosRungeKutta2.dat");
    for (int i = 0; i <= n; i++)
    {
        archivo2 << x[i] << " " << y2[i] << endl;
    }
    archivo2.close();
}

void heun()
{
    double x0, xf, h, y1[ARRAY_SIZE], y2[ARRAY_SIZE], x[ARRAY_SIZE];
    int n = 0;
    int opcion = 0;
    cout << "Ingrese el valor inicial de x: ";
    cin >> x0;
    cout << "Ingrese el valor final de x: ";
    cin >> xf;
    cout << "Ingrese el valor inicial de y1: ";
    cin >> y1[0];
    cout << "Ingrese el valor inicial de y2: ";
    cin >> y2[0];
    cout << "1. Ingresar paso h" << endl;
    cout << "2. Ingresar numero de pasos n" << endl;
    cin >> opcion;
    if (opcion == 1)
    {
        cout << "Ingrese el valor del paso h: ";
        cin >> h;
        if (h <= 0)
        {
            cerr << "El paso h debe ser positivo.\n";
            return;
        }

        // Calcula n redondeando al entero más cercano
        n = static_cast<int>((xf - x0) / h + 0.5);
        if (n < 1)
        {
            cerr << "Error: n calculado < 1.\n";
            return;
        }
        if (n + 1 > ARRAY_SIZE)
        {
            cerr << "Error: demasiados pasos (" << n << "). Aumenta ARRAY_SIZE.\n";
            return;
        }

        // Ajusta h para que el último punto caiga justo en xf
        h = (xf - x0) / n;
    }
    else if (opcion == 2)
    {
        cout << "Ingrese el numero de pasos n: ";
        cin >> n;
        if (n < 1)
        {
            cerr << "Error: n debe ser >= 1.\n";
            return;
        }
        if (n + 1 > ARRAY_SIZE)
        {
            cerr << "Error: demasiados pasos (" << n << "). Aumenta ARRAY_SIZE.\n";
            return;
        }

        // Calcula h exacto para cubrir [x0, xf]
        h = (xf - x0) / n;
    }

    x[0] = x0;

    // Metodo de Heun
    for (int i = 0; i < n; i++)
    {
        x[i + 1] = x[i] + h;

        double k11 = f1(x[i], y1[i], y2[i]);
        double k12 = f2(x[i], y1[i], y2[i]);

        double y1_tilde = y1[i] + h * k11;
        double y2_tilde = y2[i] + h * k12;

        double k21 = f1(x[i] + h, y1_tilde, y2_tilde);
        double k22 = f2(x[i] + h, y1_tilde, y2_tilde);

        y1[i + 1] = y1[i] + (h / 2.0) * (k11 + k21);
        y2[i + 1] = y2[i] + (h / 2.0) * (k12 + k22);
    }
    // Resultados escritos en archivos
    ofstream archivo("resultadosHeun1.dat");
    for (int i = 0; i <= n; i++)
    {
        archivo << x[i] << " " << y1[i] << endl;
    }
    archivo.close();

    ofstream archivo2("resultadosHeun2.dat");
    for (int i = 0; i <= n; i++)
    {
        archivo2 << x[i] << " " << y2[i] << endl;
    }
    archivo2.close();
}

void punto_medio()
{
    double x0, xf, h, y1[ARRAY_SIZE], y2[ARRAY_SIZE], x[ARRAY_SIZE];
    int n = 0;
    int opcion = 0;
    cout << "Ingrese el valor inicial de x: ";
    cin >> x0;
    cout << "Ingrese el valor final de x: ";
    cin >> xf;
    cout << "Ingrese el valor inicial de y1: ";
    cin >> y1[0];
    cout << "Ingrese el valor inicial de y2: ";
    cin >> y2[0];
    cout << "1. Ingresar paso h" << endl;
    cout << "2. Ingresar numero de pasos n" << endl;
    cin >> opcion;
    if (opcion == 1)
    {
        cout << "Ingrese el valor del paso h: ";
        cin >> h;
        if (h <= 0)
        {
            cerr << "El paso h debe ser positivo.\n";
            return;
        }

        // Calcula n redondeando al entero más cercano
        n = static_cast<int>((xf - x0) / h + 0.5);
        if (n < 1)
        {
            cerr << "Error: n calculado < 1.\n";
            return;
        }
        if (n + 1 > ARRAY_SIZE)
        {
            cerr << "Error: demasiados pasos (" << n << "). Aumenta ARRAY_SIZE.\n";
            return;
        }

        // Ajusta h para que el último punto caiga justo en xf
        h = (xf - x0) / n;
    }
    else if (opcion == 2)
    {
        cout << "Ingrese el numero de pasos n: ";
        cin >> n;
        if (n < 1)
        {
            cerr << "Error: n debe ser >= 1.\n";
            return;
        }
        if (n + 1 > ARRAY_SIZE)
        {
            cerr << "Error: demasiados pasos (" << n << "). Aumenta ARRAY_SIZE.\n";
            return;
        }

        // Calcula h exacto para cubrir [x0, xf]
        h = (xf - x0) / n;
    }

    x[0] = x0;

    // Metodo del Punto Medio
    for (int i = 0; i < n; i++)
    {
        x[i + 1] = x[i] + h;
        double k1 = f1(x[i], y1[i], y2[i]);
        double k2 = f2(x[i], y1[i], y2[i]);
        double y1_tilde = y1[i] + (h / 2) * k1;
        double y2_tilde = y2[i] + (h / 2) * k2;
        double k1_mid = f1(x[i] + h / 2, y1_tilde, y2_tilde);
        double k2_mid = f2(x[i] + h / 2, y1_tilde, y2_tilde);
        y1[i + 1] = y1[i] + h * k1_mid;
        y2[i + 1] = y2[i] + h * k2_mid; // Si mi sistema tiene mas ecuaciones, seguiria asi
    }
    // Resultados escritos en archivos
    ofstream archivo("resultadosPuntoMedio1.dat");
    for (int i = 0; i <= n; i++)
    {
        archivo << x[i] << " " << y1[i] << endl;
    }
    archivo.close();

    ofstream archivo2("resultadosPuntoMedio2.dat");
    for (int i = 0; i <= n; i++)
    {
        archivo2 << x[i] << " " << y2[i] << endl;
    }
    archivo2.close();
}