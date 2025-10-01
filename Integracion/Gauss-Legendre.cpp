#include <iostream>
using namespace std;

double f (double x) {
    return x*x;
}

int main (void) {
    double a, b, I = 0.0;
    int n;
    cout << "Ingresar los limites de integracion (a,b): ";
    cin >> a >> b;
    
    // Calculo de I
    cout<<"Ingresar el numero de puntos (n): ";
    cin>>n; 
    switch(n) {
        case 2:
        {
            double c0 = 1.0;
            double c1 = 1.0;
            double x0 = -0.5773502691896257;
            double x1 = -x0;
            I = (b-a)/2 * (c0*f(((b-a)*x0 + (b+a))/2) + c1*f(((b-a)*x1 + (b+a))/2));
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
            I = (b-a)/2 * (c0*f(((b-a)*x0 + (b+a))/2) + c1*f(((b-a)*x1 + (b+a))/2) + c2*f(((b-a)*x2 + (b+a))/2));
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
            I = (b-a)/2 * (c0*f(((b-a)*x0 + (b+a))/2) + c1*f(((b-a)*x1 + (b+a))/2) + c2*f(((b-a)*x2 + (b+a))/2) + c3*f(((b-a)*x3 + (b+a))/2));
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
            I = (b-a)/2 * (c0*f(((b-a)*x0 + (b+a))/2) + c1*f(((b-a)*x1 + (b+a))/2) + c2*f(((b-a)*x2 + (b+a))/2) + c3*f(((b-a)*x3 + (b+a))/2) + c4*f(((b-a)*x4 + (b+a))/2));
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
            I = (b-a)/2 * (c0*f(((b-a)*x0 + (b+a))/2) + c1*f(((b-a)*x1 + (b+a))/2) + c2*f(((b-a)*x2 + (b+a))/2) + c3*f(((b-a)*x3 + (b+a))/2) + c4*f(((b-a)*x4 + (b+a))/2) + c5*f(((b-a)*x5 + (b+a))/2));
            break;
        }
        default:
            cout << "n debe estar entre 2 y 6" << endl;
            return 0;
    }
    cout << "El valor de la integral es: " << I << endl;
    return 0;
}