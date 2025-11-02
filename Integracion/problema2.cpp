#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <stdexcept>
using namespace std;

// ---- utilidades de tabla ----
struct Tabla {
    vector<double> x;      // nodos
    vector<double> y;      // valores a integrar (p.ej. f'(x))
    vector<double> fcol;   // opcional: f(x) si existe 3ra columna
    bool uniforme = false;
    double h = 0.0;
};

Tabla leer_tabla(const string& fname, int ycol = 2 /*1->2da col, 2->3ra col*/) {
    // ycol: si el archivo tiene 3 columnas: 1 = usar columna 2 (f'), 2 = usar columna 3 (f)
    ifstream in(fname);
    if (!in) throw runtime_error("No se pudo abrir " + fname);

    Tabla T;
    string line;
    while (getline(in, line)) {
        if (line.empty() || line[0]=='#') continue;
        istringstream ss(line);
        double a,b,c;
        // intentamos 3 columnas
        if (ss >> a >> b >> c) {
            T.x.push_back(a);
            if (ycol == 1) T.y.push_back(b); else T.y.push_back(c);
            T.fcol.push_back(c); // guardo f(x) por si luego quiero comparar
        } else {
            // intentamos 2 columnas
            ss.clear(); ss.str(line);
            if (ss >> a >> b) {
                T.x.push_back(a);
                T.y.push_back(b);
            }
        }
    }
    if (T.x.size() < 2) throw runtime_error("Tabla con menos de 2 puntos");

    // chequear uniformidad
    T.uniforme = true;
    T.h = T.x[1]-T.x[0];
    for (size_t i=1;i+1<T.x.size();++i) {
        double hi = T.x[i+1]-T.x[i];
        if (fabs(hi - T.h) > 1e-10) { T.uniforme=false; break; }
    }
    return T;
}

// ---- integradores sobre tabla ----
double trapecio_tabla(const vector<double>& x, const vector<double>& y) {
    int n = (int)x.size();
    double acc = 0.0;
    for (int i=0;i<n-1;i++) acc += 0.5*(x[i+1]-x[i])*(y[i]+y[i+1]);
    return acc;
}

double simpson_tabla_uniforme(const vector<double>& x, const vector<double>& y) {
    int n = (int)x.size();                   // puntos
    int m = n - 1;                           // subintervalos
    if (m % 2 != 0) throw runtime_error("Simpson requiere #subintervalos par");
    double h = x[1]-x[0];
    double s = y.front() + y.back();
    for (int i=1;i<m;i++) s += (i%2? 4.0:2.0)*y[i];
    return (h/3.0)*s;
}

int main(){
    //ios::sync_with_stdio(false);
    //cin.tie(nullptr);
    cout<<fixed<<setprecision(8);

    cout << "=== Integrar df/dx desde archivo ===\n"
            "1) Trapecio con tabla (x, y)\n"
            "2) Simpson 1/3 con tabla uniforme (x, y)\n"
            "3) Integrar df/dx desde diferencias_finitas.dat (x, f'(x), f(x)) y comparar contra f(2)-f(1)\n"
            "Opcion: ";
    int op; 
    if(!(cin>>op)) return 0;

    try{
        if (op==1){
            string fn; cout<<"Archivo (x y): "; cin>>fn;
            auto T = leer_tabla(fn, 1); // da igual 1 u 2 si hay 2 columnas
            double I = trapecio_tabla(T.x, T.y);
            cout << "Trapecio = " << I << "\n";
        } else if (op==2){
            string fn; cout<<"Archivo (x y uniforme, #subint par): "; cin>>fn;
            auto T = leer_tabla(fn, 1);
            if(!T.uniforme) { cout<<"No es uniforme. Uso trapecio: "; 
                              cout<< trapecio_tabla(T.x,T.y) <<"\n"; return 0; }
            int m = (int)T.x.size()-1;
            if (m%2!=0){ cout<<"Subintervalos impares. Uso trapecio: ";
                         cout<< trapecio_tabla(T.x,T.y) <<"\n"; return 0; }
            double I = simpson_tabla_uniforme(T.x, T.y);
            cout << "Simpson = " << I << "\n";
        } else if (op==3){
            // Lee tu archivo generado por diferencias_finitas:
            // "# x f'(x) f(x)"  -> usar ycol=1 para tomar la 2da columna (f')
            string fn = "diferencias_finitas.dat";
            auto T = leer_tabla(fn, /*ycol=*/1);

            // Integrar df/dx
            double Itrap = trapecio_tabla(T.x, T.y);
            double Isimp;
            if (T.uniforme && ((int)T.x.size()-1)%2==0)
                Isimp = simpson_tabla_uniforme(T.x,T.y);
            else
                throw runtime_error("La tabla debe ser uniforme con #subint par para Simpson directo.");

            cout << "h = " << T.h << "  puntos = " << T.x.size() << "\n";
            cout << "Trapecio(df/dx) = " << Itrap << "\n";
            cout << "Simpson (df/dx) = " << Isimp << "\n";

            // Si vino 3ra columna (f), comparo contra f(b)-f(a)
            if (!T.fcol.empty() && T.fcol.size()==T.x.size()){
                double exacto = T.fcol.back() - T.fcol.front(); // TFC
                auto err = [&](double I){ return fabs(I - exacto); };
                auto errp = [&](double I){ return 100.0*err(I)/max(1e-12,fabs(exacto)); };

                cout << "Exacto por TFC (f(b)-f(a)) = " << exacto << "\n";
                cout << "  Error Trapecio : " << err(Itrap) << "  ("<< errp(Itrap) <<" %)\n";
                cout << "  Error Simpson  : " << err(Isimp) << "  ("<< errp(Isimp) <<" %)\n";
            } else {
                cout << "(No hay 3ra columna f(x); no comparo contra f(b)-f(a)).\n";
            }
        } else {
            cout<<"Opcion invalida.\n";
        }
    } catch(const exception& e){
        cerr << "Error: " << e.what() << "\n";
    }
    return 0;
}
