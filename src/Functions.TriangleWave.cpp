#include "include.hpp"

namespace Functions::TriangleWave
{
    const double COEFF = .8;

    double SingleHarmonic(double x, int harmonicn) {
        return (cos(harmonicn * x) / pow(harmonicn, 2)) * COEFF;
    }

    /// @brief Funzione onda a triangolo calcolata usando la successione di Fourier.
    /// @param x Valore dell'ascissa.
    /// @return Valore dell'ordinata.
    double Series(double x, int harmonicsn) {
        int coeff = 1;
        double n = 0;
        for (int i = 0; i < harmonicsn; i++)
        {
            n += SingleHarmonic(x, coeff);
            coeff += 2;
        }
        return n;
    }
} // namespace Functions::TriangleWave
