#include "include.hpp"

namespace Functions::SawtoothWave
{
    const double COEFF = .5;

    double SingleHarmonic(double x, int harmonicn) {
        return pow(-1, harmonicn) * (sin(harmonicn * x) / harmonicn) * COEFF;
    }

    /// @brief Funzione onda a dente di sega calcolata usando la successione di Fourier.
    /// @param x Valore dell'ascissa.
    /// @return Valore dell'ordinata.
    double Series(double x, int harmonicsn) {
        int coeff = 1;
        double n = 0;
        for (int i = 0; i < harmonicsn; i++)
        {
            n += SingleHarmonic(x, coeff);
            coeff++;
        }
        return n;
    }
} // namespace Functions::SawtoothWave