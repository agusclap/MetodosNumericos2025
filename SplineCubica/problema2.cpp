#include <bits/stdc++.h>
using namespace std;

// -------------------- ESTRUCTURA DE SPLINE --------------------
struct Spline {
    vector<double> x, y;
    vector<double> a,b,c,d;   // coef locales (a,b,c,d por tramo)

    bool build(const vector<double>& X, const vector<double>& Y) {
        x = X; y = Y;
        int n = (int)x.size();
        if (n < 2) return false;

        // --- Verificar orden creciente ---
        for (int i=1;i<n;i++) {
            if (!(x[i] > x[i-1])) return false;
        }

        // --- Paso 1: diferencias h ---
        vector<double> h(n-1);
        for (int i=0;i<n-1;i++) h[i] = x[i+1] - x[i];

        // --- Paso 2: sistema tridiagonal para segundas derivadas ---
        vector<double> m(n, 0.0);
        if (n > 2) {
            vector<double> A(n-2), B(n-2), C(n-2), rhs(n-2);
            for (int i=1;i<=n-2;i++) {
                A[i-1]  = h[i-1];
                B[i-1]  = 2.0*(h[i-1]+h[i]);
                C[i-1]  = h[i];
                rhs[i-1]= 6.0*((y[i+1]-y[i])/h[i] - (y[i]-y[i-1])/h[i-1]);
            }
            // Método de Thomas
            for (int i=1;i<n-2;i++) {
                double w = A[i]/B[i-1];
                B[i] -= w*C[i-1];
                rhs[i] -= w*rhs[i-1];
            }
            m[n-2] = rhs[n-3]/B[n-3];
            for (int i=n-4;i>=0;i--) {
                m[i+1] = (rhs[i]-C[i]*m[i+2])/B[i];
            }
            m[0]=0.0; m[n-1]=0.0;
        }

        // --- Paso 3: calcular coeficientes ---
        int mtr = n-1;
        a.resize(mtr); b.resize(mtr); c.resize(mtr); d.resize(mtr);
        for (int i=0;i<mtr;i++) {
            a[i] = y[i];
            c[i] = m[i]/2.0;
            d[i] = (m[i+1]-m[i])/(6.0*h[i]);
            b[i] = (y[i+1]-y[i])/h[i] - (h[i]/6.0)*(2.0*m[i] + m[i+1]);
        }
        return true;
    }

    // --- Evaluar spline ---
    double eval(double xq) const {
        int n = (int)x.size();
        int i;
        if (xq <= x.front()) i = 0;
        else if (xq >= x.back()) i = n-2;
        else {
            i = int(upper_bound(x.begin(), x.end(), xq) - x.begin()) - 1;
        }
        double t = xq - x[i];
        return a[i] + b[i]*t + c[i]*t*t + d[i]*t*t*t;
    }
};

// -------------------- MAIN --------------------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // --- Leer y ordenar datos ---
    vector<pair<double,double>> P;
    {
        ifstream f("C:/Users/Agustin/Desktop/Metodos2025/datos.dat"); // ruta absoluta recomendada
        if (!f) {
            cerr << "No pude abrir datos.dat\n";
            return 1;
        }
        double xx, yy;
        while (f >> xx >> yy) P.emplace_back(xx,yy);
    }
    if (P.size() < 2) {
        cerr << "Datos insuficientes\n";
        return 1;
    }

    sort(P.begin(), P.end());
    vector<double> X, Y;
    for (auto &p : P) {
        if (X.empty() || fabs(p.first - X.back()) > 1e-12) {
            X.push_back(p.first);
            Y.push_back(p.second);
        }
    }

    cerr << "Leidos " << X.size() << " puntos:\n";
    for (size_t i=0;i<X.size();++i)
        cerr << "  x=" << X[i] << "  y=" << Y[i] << "\n";

    // --- Construir spline ---
    Spline sp;
    if (!sp.build(X,Y)) {
        cerr << "Error construyendo spline (x no estrictamente creciente?)\n";
        return 1;
    }

    // --- Generar tabla uniforme [1,2] con 10 subintervalos ---
    const int NU = 10;
    const double aU = 1.0, bU = 2.0, hU = (bU - aU)/NU;

    ofstream out("C:/Users/Agustin/Desktop/Metodos2025/tabla_uniforme.dat");
    out.setf(std::ios::fixed); out << setprecision(6);

    for (int i=0;i<=NU;i++) {
        double xi = aU + i*hU;
        double fi = sp.eval(xi);
        out << xi << " " << fi << "\n";
    }

    cout << "OK: escribí tabla_uniforme.dat (11 puntos, h=0.1)\n";
    return 0;
}
