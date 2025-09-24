#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <filesystem>

using namespace std;


int main() {
    /*const int n = 100;
    for (int i = 0; i < n; ++i) {
        // columnas 0..n-1 = A ; columna n = b
        for (int j = 0; j < n; ++j) {
            double val = 0.0;
            if (j == i) val = 2.0;
            else if (j == i-1 || j == i+1) val = 1.0;
            printf("%.10f ", val);
        }
        double bi = (i == 0 || i == n-1) ? 4.5 : 6.0;
        printf("%.10f\n", bi);
    }*/

   const int n = 50;

    std::ofstream fout("datos.dat", std::ios::out | std::ios::trunc);
    if (!fout.is_open()) {
        std::cerr << "No pude abrir 'datos.dat' para escritura.\n";
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int val = 0;
            if (i == j) val = 2;                 // diagonal
            else if (std::abs(i - j) == 1) val = -2; // 1ª sub/super
            else if (std::abs(i - j) == 2) val = 1;  // 2ª sub/super
            fout << val << ' ';
        }
        fout << 5 << '\n'; // término independiente
    }
    fout.close();

    std::cout << "OK: escrito " << n << "x" << n << " (+b) en: "
              << (std::filesystem::current_path() / "datos.dat") << '\n';
    return 0;

    return 0;
}
