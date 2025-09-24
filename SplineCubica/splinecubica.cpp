#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

int N = 6;

int main(void)
{
    int count = 0;
    double x[N], y[N], A[4 * N][4 * N] = {0}, b[4 * N] = {0};
    const double EPS = 1e-12;   // singularidad
    const double THRESH = 1e-2; // umbral de pivoteo “profe”
    FILE *fp = fopen("datos.dat", "r");
    if (!fp)
    {
        cerr << "Error al abrir datos.dat\n";
        return 1;
    }
    for (int i = 0; i < N; ++i)
    {
        if (fscanf(fp, "%lf %lf", &x[i], &y[i]) != 2)
        {
            cerr << "Error de formato en datos.dat (fila " << i << ")\n";
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);

    // ----- ARMADO CORRECTO DEL SISTEMA PARA SPLINE CÚBICA NATURAL -----
    // Mantenemos tus variables: x[], y[], A[][], b[], N.

    const int M = 4 * (N - 1); // cantidad de incógnitas (4 por tramo)
    int r = 0;                 // fila actual del sistema

    auto colBase = [&](int k)
    { return 4 * k; }; // columna base del tramo k

    // 1) Interpolación en extremos de cada tramo: S_k(x_k)=y_k y S_k(x_{k+1})=y_{k+1}
    for (int k = 0; k < N - 1; ++k)
    {
        int c = colBase(k);
        double xk = x[k];
        double xk1 = x[k + 1];

        // S_k(x_k) = y_k
        A[r][c + 0] = pow(xk, 3);
        A[r][c + 1] = pow(xk, 2);
        A[r][c + 2] = xk;
        A[r][c + 3] = 1.0;
        b[r] = y[k];
        r++;

        // S_k(x_{k+1}) = y_{k+1}
        A[r][c + 0] = pow(xk1, 3);
        A[r][c + 1] = pow(xk1, 2);
        A[r][c + 2] = xk1;
        A[r][c + 3] = 1.0;
        b[r] = y[k + 1];
        r++;
    }

    // 2) Continuidad de 1ª derivada en nodos internos: S'_{k-1}(x_k) = S'_k(x_k)
    for (int k = 1; k <= N - 2; ++k)
    {
        int cL = colBase(k - 1);
        int cR = colBase(k);
        double xc = x[k];

        // S'_{k-1}(xc) - S'_k(xc) = 0
        A[r][cL + 0] = 3 * xc * xc;
        A[r][cL + 1] = 2 * xc;
        A[r][cL + 2] = 1.0;
        A[r][cR + 0] -= 3 * xc * xc;
        A[r][cR + 1] -= 2 * xc;
        A[r][cR + 2] -= 1.0;
        b[r] = 0.0;
        r++;
    }

    // 3) Continuidad de 2ª derivada en nodos internos: S''_{k-1}(x_k) = S''_k(x_k)
    for (int k = 1; k <= N - 2; ++k)
    {
        int cL = colBase(k - 1);
        int cR = colBase(k);
        double xc = x[k];

        // S''_{k-1}(xc) - S''_k(xc) = 0
        A[r][cL + 0] = 6 * xc;
        A[r][cL + 1] = 2.0;
        A[r][cR + 0] -= 6 * xc;
        A[r][cR + 1] -= 2.0;
        b[r] = 0.0;
        r++;
    }

    // 4) Condiciones naturales: S''_0(x_0)=0 y S''_{N-2}(x_{N-1})=0
    {
        // S''_0(x_0) = 6*a_0*x0 + 2*b_0 = 0
        int c0 = colBase(0);
        A[r][c0 + 0] = 6 * x[0];
        A[r][c0 + 1] = 2.0;
        b[r] = 0.0;
        r++;

        // S''_{N-2}(x_{N-1}) = 6*a_{N-2}*x_{N-1} + 2*b_{N-2} = 0
        int cLast = colBase(N - 2);
        A[r][cLast + 0] = 6 * x[N - 1];
        A[r][cLast + 1] = 2.0;
        b[r] = 0.0;
        r++;
    }

    // (opcional) chequeo rápido:
     if (r != M) { fprintf(stderr, "filas cargadas = %d, esperado = %d\n", r, M); return 1; }

    // Llamar a Gauss con pivoteo


    // --- Eliminación Gauss con pivoteo parcial (A * sol = b), trabajando con M ---
    for (int i = 0; i < M - 1; ++i)
    {
        // Elegir pivote (parcial)
        int p = i;
        for (int rr = i + 1; rr < M; ++rr)
            if (fabs(A[rr][i]) > fabs(A[p][i]))
                p = rr;

        if (fabs(A[p][i]) < 1e-12)
        {
            fprintf(stderr, "Sistema singular/casi singular en etapa %d\n", i);
            return 1;
        }
        if (p != i)
        {
            // swap filas en A
            for (int c = 0; c < M; ++c)
                std::swap(A[i][c], A[p][c]);
            // swap en b
            std::swap(b[i], b[p]);
        }

        // Eliminación hacia abajo
        for (int rr = i + 1; rr < M; ++rr)
        {
            double f = A[rr][i] / A[i][i];
            if (fabs(f) < 1e-18)
                continue;
            for (int c = i; c < M; ++c)
                A[rr][c] -= f * A[i][c];
            b[rr] -= f * b[i];
        }
    }

    if (fabs(A[M - 1][M - 1]) < 1e-12)
    {
        fprintf(stderr, "Sistema singular en última etapa\n");
        return 1;
    }

    // --- Retrosustitución ---
    double sol[M];
    for (int i = M - 1; i >= 0; --i)
    {
        double acc = b[i];
        for (int j = i + 1; j < M; ++j)
            acc -= A[i][j] * sol[j];
        sol[i] = acc / A[i][i];
    }

    // Mostrar solución (coeficientes por tramo)
    for (int i = 0; i < M; ++i)
        printf("coef[%d] = %.10f\n", i, sol[i]);

    // Evaluar spline en un xs:
    double xs;
    cout << "Ingresar xs: ";
    if (cin >> xs)
    {
        // localizar tramo k tal que x[k] <= xs <= x[k+1]
        int k = -1;
        for (int i = 0; i < N - 1; ++i)
        {
            if (xs >= x[i] && xs <= x[i + 1])
            {
                k = i;
                break;
            }
        }
        if (k == -1)
        {
            // fuera de los nodos: elegí extrapolar con el extremo más cercano
            k = (xs < x[0]) ? 0 : (N - 2);
        }
        int c = 4 * k;
        double a = sol[c + 0], b2 = sol[c + 1], c2 = sol[c + 2], d2 = sol[c + 3];
        double ys = a * pow(xs, 3) + b2 * pow(xs, 2) + c2 * xs + d2;
        cout << "S(" << xs << ") = " << ys << "\n";
    }

    return 0;
}