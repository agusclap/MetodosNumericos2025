#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

namespace {

double f(double x) {
    const double sqrt_term = std::sqrt(1.0 + x);
    return std::exp(sqrt_term) * std::log(1.0 + 2.0 * x * x * x);
}

double fprime(double x) {
    const double sqrt_term = std::sqrt(1.0 + x);
    const double exp_term  = std::exp(sqrt_term);
    const double log_term  = std::log(1.0 + 2.0 * x * x * x);
    const double denom_log = 1.0 + 2.0 * x * x * x;
    const double sqrt_der  = 1.0 / (2.0 * sqrt_term);
    const double log_der   = (6.0 * x * x) / denom_log;
    return exp_term * (log_term * sqrt_der + log_der);
}

} // namespace

int main() {
    constexpr double a = 0.0;
    constexpr double b = 1.0;
    constexpr int    subintervals = 6; // intervalo dividido en 6 sub-intervalos

    const double h = (b - a) / static_cast<double>(subintervals);
    const std::size_t n = subintervals + 1;

    std::vector<double> xs(n), fx(n), dfx(n), dfx_exact(n);
    for (std::size_t i = 0; i < n; ++i) {
        xs[i] = a + h * static_cast<double>(i);
        fx[i] = f(xs[i]);
        dfx_exact[i] = fprime(xs[i]);
    }

    // Derivadas aproximadas (orden O(h^2) para extremos y centrada interior)
    dfx[0] = (-3.0 * fx[0] + 4.0 * fx[1] - fx[2]) / (2.0 * h);
    for (std::size_t i = 1; i + 1 < n; ++i) {
        dfx[i] = (fx[i + 1] - fx[i - 1]) / (2.0 * h);
    }
    dfx[n - 1] = (3.0 * fx[n - 1] - 4.0 * fx[n - 2] + fx[n - 3]) / (2.0 * h);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Tabla de valores (x, f(x), f'(x) aprox, f'(x) exacta, error)\n";
    std::cout << "----------------------------------------------------------------\n";
    std::cout << "    x\t      f(x)\t   f'(x) aprox\t   f'(x) exacta\t  |error|\n";
    for (std::size_t i = 0; i < n; ++i) {
        std::cout << std::setw(7) << xs[i] << '\t'
                  << std::setw(12) << fx[i] << '\t'
                  << std::setw(12) << dfx[i] << '\t'
                  << std::setw(12) << dfx_exact[i] << '\t'
                  << std::setw(11) << std::fabs(dfx[i] - dfx_exact[i]) << '\n';
    }
    std::cout << '\n';

    // Simpson 1/3 usando las derivadas aproximadas
    double simpson_sum = dfx[0] + dfx[n - 1];
    for (std::size_t i = 1; i + 1 < n; ++i) {
        const double weight = (i % 2 == 1) ? 4.0 : 2.0;
        simpson_sum += weight * dfx[i];
    }
    const double integral_simpson = (h / 3.0) * simpson_sum;

    const double exact_difference = f(b) - f(a);
    const double abs_error_simpson = std::fabs(integral_simpson - exact_difference);

    std::cout << "Integral de f'(x) en [0,1] via Simpson 1/3: "
              << std::setprecision(9) << integral_simpson << '\n';
    std::cout << "Valor exacto f(1) - f(0):                 "
              << exact_difference << '\n';
    std::cout << "Error absoluto (Simpson):                 "
              << abs_error_simpson << "\n\n";

    // Cuadratura de Gauss-Legendre de 4 puntos para f'(x)
    static const double nodes[] = {
        -0.8611363115940525752239465,
        -0.3399810435848562648026658,
         0.3399810435848562648026658,
         0.8611363115940525752239465
    };
    static const double weights[] = {
        0.3478548451374538573730639,
        0.6521451548625461426269361,
        0.6521451548625461426269361,
        0.3478548451374538573730639
    };

    const double half_length = (b - a) / 2.0;
    const double midpoint    = (a + b) / 2.0;
    double integral_gauss = 0.0;
    for (int i = 0; i < 4; ++i) {
        const double xi = midpoint + half_length * nodes[i];
        integral_gauss += weights[i] * fprime(xi);
    }
    integral_gauss *= half_length;

    const double abs_error_gauss = std::fabs(integral_gauss - exact_difference);

    std::cout << std::setprecision(9);
    std::cout << "Integral de f'(x) via Gauss-Legendre (4 pts): "
              << integral_gauss << '\n';
    std::cout << "Error absoluto (Gauss 4 pts):               "
              << abs_error_gauss << '\n';

    return 0;
}
