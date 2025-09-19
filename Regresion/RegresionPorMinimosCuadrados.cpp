#include <bits/stdc++.h>
using namespace std;

// === Parámetros a setear ===
const int N = 7;   // cantidad de puntos (filas en datos.dat)
const int P = 2;   // grado del polinomio a ajustar (0=const, 1=lineal, 2=cuadrático, ...)

int main() {
    // ----- Leer datos (x,y) -----
    double x[N], y[N];
    FILE* fp = fopen("datos.dat", "r");
    if (!fp) { cerr << "Error al abrir datos.dat\n"; return 1; }
    for (int i = 0; i < N; ++i) {
        if (fscanf(fp, "%lf %lf", &x[i], &y[i]) != 2) {
            cerr << "Error de formato en datos.dat (fila " << i << ")\n";
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    if (N < P+1) { cerr << "No hay datos suficientes para grado " << P << "\n"; return 1; }

    const int M = P + 1;                   // cantidad de coeficientes a0..aP
    long double A[M][M];                   // matriz normal
    long double b[M];                      // vector normal

    // =========================
    // Do l = 0..p   (recorre filas)  -> arma b[l] y A[l][m]
    // =========================
    for (int l = 0; l <= P; ++l) {
        long double suma_xy = 0.0L;        // suma_xy = Σ y_i * x_i^l
        for (int i = 0; i < N; ++i)
            suma_xy += (long double)y[i] * pow((long double)x[i], l);
        b[l] = suma_xy;                    // b[l] = suma_xy

        // Do m = 0..p   (recorre columnas)
        for (int m = 0; m <= P; ++m) {
            long double suma_x = 0.0L;     // suma_x = Σ x_i^(l+m)
            for (int i = 0; i < N; ++i)
                suma_x += pow((long double)x[i], l + m);
            A[l][m] = suma_x;              // A[l,m] = suma_x
        }
    }

    // =========================
    // Llamar a Gauss(A, b, a) [con pivoteo]  -> resuelve A*a = b
    // =========================
    // Eliminación con pivoteo parcial
    for (int k = 0; k < M-1; ++k) {
        int piv = k;
        for (int r = k+1; r < M; ++r)
            if (fabsl(A[r][k]) > fabsl(A[piv][k])) piv = r;
        if (fabsl(A[piv][k]) < 1e-18L) { cerr << "Sistema singular (pivote ~ 0)\n"; return 1; }
        if (piv != k) {
            for (int j = 0; j < M; ++j) swap(A[piv][j], A[k][j]);
            swap(b[piv], b[k]);
        }
        for (int i = k+1; i < M; ++i) {
            long double f = A[i][k] / A[k][k];
            if (fabsl(f) < 1e-30L) continue;
            for (int j = k; j < M; ++j) A[i][j] -= f * A[k][j];
            b[i] -= f * b[k];
        }
    }
    if (fabsl(A[M-1][M-1]) < 1e-18L) { cerr << "Sistema singular (última etapa)\n"; return 1; }

    // Retrosustitución
    long double a[M];                       // coeficientes a0..aP
    for (int i = M-1; i >= 0; --i) {
        long double acc = b[i];
        for (int j = i+1; j < M; ++j) acc -= A[i][j] * a[j];
        a[i] = acc / A[i][i];
    }

    // =========================
    // Mostrar coeficientes (para armar P(x) = a0 + a1 x + ... + aP x^P)
    // =========================
    for (int i = 0; i < M; ++i)
        cout << "a[" << i << "] = " << (double)a[i] << "\n";

    // =========================
    // Calcular St, Sr, r  (bondad de ajuste)
    // =========================
    long double ybar = 0.0L;
    for (int i = 0; i < N; ++i) ybar += (long double)y[i];
    ybar /= (long double)N;

    auto evalP = [&](long double xv) {
        long double xp = 1.0L, sum = 0.0L;
        for (int j = 0; j < M; ++j) { sum += a[j]*xp; xp *= xv; } // Horner simple
        return sum;
    };

    long double St = 0.0L, Sr = 0.0L;
    for (int i = 0; i < N; ++i)
    {
        long double yi = y[i];
        long double yhat = evalP(x[i]);
        St += pow(yi - ybar, 2); // (y_i - ȳ)^2
        Sr += pow(yi - yhat, 2); // (y_i - ŷ_i)^2
    }

    // --- Métricas ---
    double R2 = (St == 0.0) ? 1.0 : 1.0 - Sr / St;
    double r = sqrt(R2);        // coef. de correlación (en valor absoluto)
    double ECM = Sr / N;        // error cuadrático medio
    double ECMc = Sr / (N - M); // ECM corregido
    double RMSE = sqrt(ECMc);   // raíz del ECMc

    if (a[1] < 0)
        r = -r; // solo lo cambia de signo si la pendiente es negativa

    cout << "St   = " << St << "\n";
    cout << "Sr   = " << Sr << "\n";
    cout << "R^2  = " << R2 << "   r = " << r << "\n";
    cout << "ECM  = " << ECM << "   ECMc = " << ECMc
         << "   RMSE = " << RMSE << "\n";

    cout << "\nFuncion ajustada: P(x) = ";
    for (int j = 0; j <= P; j++)
    {
        if (j > 0)
        {
            if (a[j] >= 0)
                cout << " + ";
            else
                cout << " - ";
        }
        cout << fabs(a[j]);
        if (j >= 1)
            cout << "*x";
        if (j > 1)
            cout << "^" << j;
    }
    cout << endl;

    cout << "Ingresar x para predecir (ENTER para omitir): ";
    double xs;
    if (cin >> xs) {
        long double yhat = evalP(xs);
        cout << "P(" << xs << ") = " << (double)yhat << "\n";
    }
    return 0;
}
