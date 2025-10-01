#include <iostream>
#include <cstdio>
#include <cmath>
using namespace std;

const int n = 4; // número de puntos

double f(double x) {
    return 4*pow(x,3) - 3*pow(x,2) + 2;
}

int main (){
    double x[n], y[n], xs, suma = 0, prod;
    double A[n][n], b[n];

    FILE *fp = fopen("datosFuncion.dat", "r");
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

    // Matriz de Vandermonde (grado n-1 con n puntos)
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            A[i][j] = pow(x[i], j);
        }
        b[i] = y[i];
    }

    // --- Eliminación con PIVOT PARCIAL ---
    for (int k = 0; k < n-1; k++) {
        // buscar pivote máximo en columna k (fila k..n-1)
        int p = k;
        double maxabs = fabs(A[k][k]);
        for (int i = k+1; i < n; i++) {
            double val = fabs(A[i][k]);
            if (val > maxabs) { maxabs = val; p = i; }
        }
        // swap filas si hace falta
        if (p != k) {
            for (int j = 0; j < n; j++) std::swap(A[k][j], A[p][j]);
            std::swap(b[k], b[p]);
        }
        if (fabs(A[k][k]) < 1e-15) { cerr << "Pivote casi nulo en k="<<k<<endl; return 1; }

        // eliminación hacia abajo
        for (int i = k+1; i < n; i++) {
            double factor = A[i][k] / A[k][k];
            for (int j = k; j < n; j++) {
                A[i][j] = A[i][j] - factor * A[k][j];
            }
            b[i] = b[i] - factor * b[k];
        }
    }

    // --- Sustitución hacia atrás: resuelve A * a = b ---
    for (int i = n-1; i >= 0; i--) {
        double acc = 0.0;
        for (int j = i+1; j < n; j++) acc += A[i][j] * b[j];
        if (fabs(A[i][i]) < 1e-15) { cerr << "Pivote nulo en diagonal" << endl; return 1; }
        b[i] = (b[i] - acc) / A[i][i];   // ahora b[i] guarda el coeficiente a_i
    }

    // Mostrar coeficientes a_i
    for(int i=0; i<n; i++){
        cout << "Coeficiente a[" << i << "] = " << b[i] << endl;
    }

    // Evaluar el polinomio en xs
    cout <<  "Ingresa el valor a interpolar" << endl;
    cin >> xs;

    suma = 0.0;
    for(int i=0; i<n; i++){
        suma += b[i] * pow(xs, i);   // <-- corregido: sin 'suma + suma'
    }
    cout << "El valor interpolado en x = " << xs << " es: " << suma << endl;
   /* double fx = f(xs);              // valor real de la función
    double error = fabs(fx - suma); // error absoluto


    cout << "P(" << xs << ") = " << suma << endl;
    cout << "f(" << xs << ") = " << fx << endl;
    cout << "Error = " << error << endl;*/
    return 0;
}
