#include "include.hpp"

namespace Functions::SquareWave
{
    double SingleHarmonic(double x, int harmonicn) {
        if(harmonicn % 2 == 0) return 0;
        return sin(harmonicn * x) / harmonicn;
    }

    /// @brief Funzione onda quadra calcolata usando la successione di Fourier.
    /// @param x Valore dell'ascissa.
    /// @return Valore dell'ordinata.
    double Series(double x, int harmonicsn) {
        int coeff = 1;
        double n = 0;
        for (int i = 0; i < harmonicsn; i++)
        {
            n += SingleHarmonic(x, coeff);

            // Incremento il coefficiente di 2 perchè la funzione ha solo armoniche dispari.
            coeff += 2;
        }
        return n;
    }
} // namespace Functions::SquareWave
