#include <iostream>
#include <cmath>
using namespace std;

int MAXC = 100;

int main()
{
    double w = 1.1; // valor w para relajación
    // OJO: declaramos n cuando ya sabemos cuántas filas hay
    double suma = 0, tol = 1e-11, errorviejo = 1000, error;
    int iter = 0, max_iter = 100;

    FILE *fp = fopen("datos.dat", "r");
    if (!fp)
    {
        puts("No se puede abrir el archivo");
        return 1;
    }

    // Contar filas
    int filas = 0;
    int columnas = 0;
    int c;
    while ((c = fgetc(fp)) != EOF)
        if (c == '\n')
            filas++;
    if (filas <= 0)
    {
        puts("Archivo vacío");
        fclose(fp);
        return 1;
    }
    // Reiniciar lectura
    fclose(fp);
    fp = fopen("datos.dat", "r");
    if (!fp)
    {
        puts("No se puede abrir el archivo");
        return 1;
    }

    // Cargar matriz aumentada: m[filas][columnas]
    double m[filas][MAXC];
    for (int i = 0; i < filas; ++i)
    {
        int j = 0;
        do
        {
            if (fscanf(fp, "%lf", &(m[i][j])) != 1)
            {
                puts("Formato inválido");
                fclose(fp);
                return 1;
            }
            j++;
        } while ((c = fgetc(fp)) != '\n' && c != EOF);
        if (i == 0)
            columnas = j;
        else if (j != columnas)
        {
            puts("Filas con distinta cantidad de columnas");
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);

    printf("numero de filas = %d\n", filas);
    printf("numero de columnas = %d\n", columnas);

    int n = filas; // sistema de n ecuaciones
    if (columnas != n + 1)
    { // esperamos matriz aumentada [A|b]
        puts("Se esperaba matriz aumentada [A|b]: columnas = filas + 1");
        return 1;
    }

    // Declaraciones que dependen de n (mantenemos mismos nombres)
    double x[n], A[n][n], b[n], xv[n], xn[n];

    // Pasar m -> A y b
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
            A[i][j] = m[i][j];
        b[i] = m[i][n];
    }

    // Chequeo de diagonal dominante (índices 0-based y con fabs)
    for (int i = 0; i < n; i++)
    {
        suma = 0.0;
        for (int j = 0; j < n; j++)
        {
            if (j != i)
                suma += fabs(A[i][j]);
        }
        if (fabs(A[i][i]) <= suma)
            cout << "La matriz no es diagonalmente dominante en la fila " << i << endl;
    }
    int p = 0;
    const double Z = 1e-14; // umbral de "casi cero"
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (fabs(A[i][j]) > Z)
                p = max(p, abs(j - i));
    cout << "Semi-ancho de banda p = " << p << "\n"; // en este problema: 2

    // Inicialización de xv
    for (int i = 0; i < n; i++)
    {
        xv[i] = 0;
    }

    // Gauss-Seidel
    double res = 1e300;
    
    do
    {
        iter = iter + 1;

        for (int i = 0; i < n; i++)
        {
            suma = 0.0; // reset por fila
            // límites de la banda para la fila i
            int j0 = max(0, i - p);
            int j1 = min(n - 1, i + p);

            // usa xn[j] si j<i, xv[j] si j>i, pero SOLO dentro de la banda
            for (int j = j0; j < i; ++j)
                suma += A[i][j] * xn[j];
            for (int j = i + 1; j <= j1; ++j)
                suma += A[i][j] * xv[j];

            // usa xn[j] para j < i (ya actualizado en esta iteración)
            /*for (int j = 0; j < i; j++)
            {
                suma = suma + A[i][j] * xn[j];
            }
            // usa xv[j] para j > i (todavía de la iteración anterior)
            for (int j = i + 1; j < n; j++)
            {
                suma = suma + A[i][j] * xv[j];
            }*/

            if (A[i][i] == 0.0)
            {
                cout << "Pivote cero en A[" << i << "," << i << "]" << endl;
                return 0;
            }
            double nuevo = (b[i] - suma) / A[i][i]; //Relajacion
            xn[i] = (1.0 - w) * xv[i] + w * nuevo ; //Relajacion

            //xn[i] = (b[i] - suma) / A[i][i]; // normal
        }

        error = 0.0;
        for (int i = 0; i < n; i++)
        {
            error = error + pow(xn[i] - xv[i], 2);
        }
        error = sqrt(error);

        res = 0.0;
        for (int i = 0; i < n; ++i)
        {
            double ax = 0.0;
            for (int j = 0; j < n; ++j)
                ax += A[i][j] * xn[j];
            double ri = fabs(ax - b[i]);
            if (ri > res)
                res = ri;
        }

        // --- criterio de paro por residuo (Problema 3) ---
        if (res < tol)
            break;
    
        if (error > errorviejo)
        {
            cout << "Advertencia: el error aumento (puede no converger)" << endl;
            // si querés cortar acá, podés hacer 'return 0;'
        }

        errorviejo = error;
        for (int i = 0; i < n; i++)
        {
            xv[i] = xn[i];
        }

    } while (/*error > tol*/res>tol && iter < max_iter);

    cout << "El metodo " << (error <= tol ? "converge" : "se detuvo por max_iter")
         << " en " << iter << " iteraciones." << endl;
    cout << "El error es " << error << endl;
    cout << "Resultado:" << endl;
    for (int x_i = 0; x_i < n; x_i++)
    {
        cout << "x[" << x_i << "] = " << xn[x_i] << endl;
    }
    cout << "El metodo " << (res <= tol ? "converge" : "se detuvo por max_iter")
     << " en " << iter << " iteraciones." << endl;

    // Verificación: residuo ||A*x - b||_inf
    /*double res = 0.0;
    for (int i = 0; i < n; ++i)
    {
        double ax = 0.0;
        for (int j = 0; j < n; ++j)
            ax += A[i][j] * xn[j];
        double ri = fabs(ax - b[i]);
        if (ri > res)
            res = ri;
    }*/
    cout << "Residuo ||A*x - b||_inf = " << res << endl;

    return 0;
}