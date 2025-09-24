#include <iostream>
using namespace std;

int main (void) {

    const int n = 100;
    double M[n][n] = {0};
    double b[n];

    for (int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            if(i==0){
                M[i][1] = 1;
            }
            if(i==j){
                M[i][j] = 2;
            }
            if(i>0 && i<n-1){
                if(j==i-1 || j==i+1){
                    M[i][j] = 1;
                }
            }
            if(i==n-1){
                M[i][n-2] = 1;
            }
        }
        if(i==0 || i==n-1){
            b[i] = 4.5;
        } else {
            b[i] = 6;
        }
    }
    //Escribir en archivo
    FILE *fp = fopen("datos.dat", "w");
    if (fp == NULL) {
        cerr << "No se pudo abrir el archivo para escribir.\n";
        return 1;
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            fprintf(fp, "%.10f ", M[i][j]);
        }
        fprintf(fp, "%.10f\n", b[i]);
    }
    fclose(fp);
    cout << "Matriz escrita en 'datos.dat'\n";
    return 0;
}