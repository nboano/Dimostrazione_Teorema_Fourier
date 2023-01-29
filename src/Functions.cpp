#include "D:\kinderc\kinderc.hpp"
#include "PageCanvas.cpp"

/// @brief Crea una lambda che scrive un punto della funzione ogni qualvolta viene richiamata.
/// @param f La funzione.
/// @param W La larghezza del render.
/// @param H L'altezza del render.
/// @param image L'insieme immagine rappresentato della funzione.
/// @param delta Il delta iniziale della funzione.
/// @param linewidth Spessore della linea della funzione in pixel.
/// @param fillStyle Colore della linea (CSS).
#define DRAW_FN_POINT(f, W, H, image, phase, delta, linewidth, fillStyle) \
[](bool dw, bool clear) {\
    static double x = phase;\
    double y = f(x);\
    double ex = (W / (image)) * x;\
    double ey = (H/2 +  y* H/2);\
    x += delta;\
    if(ex >= W) {\
        x = phase;\
        return;\
    }\
    if(clear) PageCanvas::ClearXAt(ex, linewidth);\
    if(dw) PageCanvas::DrawPoint(ex,ey,linewidth,fillStyle);\
}\

/// @brief Risoluzione orizzontale del render.
int W = 1280;

/// @brief Risoluzione verticale del render.
int H = 720;

/// @brief Intervallo di funzioni visualizzato.
double T = M_PI * 4;

/// @brief Precisione del render (delta tra i punti).
double Precision = .01;

exported void setT(double v) {
    PageCanvas::Clear();
    T = v;
}
exported void setPrecision(double v) {
    Precision = v;
}

namespace Functions
{
    double SineWave(double x) {
        return sin(x);
    }
    double SquareWave(double x) {
        return floor(SineWave(x)) == 0? .78 : -.78;
    }
    double(*SquareHarmonics[])(double) = {
        [](double) {return .0;},

        /// PRIMA ARMONICA (SENO)
        SineWave,

        /// SECONDA ARMONICA (SEMPRE 0)
        [](double){return .0;},

        /// TERZA ARMONICA
        [](double x) {return SineWave(3*x) / 3;},

        /// QUARTA ARMONICA (SEMPRE 0)
        [](double){return .0;},

        /// QUINTA ARMONICA
        [](double x) {return SineWave(5*x) / 5;},

        /// SESTA ARMONICA (SEMPRE 0)
        [](double){return .0;},

        /// SETTIMA ARMONICA
        [](double x ){return SineWave(7*x) / 7;},
    }; 

    /// @brief Numero di armoniche usate nella funzione SquareFourier.
    int SquareHarmonicsNumber = 3;

    double SquareFourier(double x) {
        int coeff = 1;
        double n = 0;
        for (int i = 0; i < SquareHarmonicsNumber; i++)
        {
            n += SineWave(coeff * x) / coeff;
            coeff += 2;
        }
        return n;
    }

    /// @brief Numero di armoniche usate nella funzione SawToothFourier.
    int SawToothHarmonicsNumber = 3;

    double SawToothFourier(double x) {
        int coeff = 1;
        double n = 0;
        for (int i = 0; i < SawToothHarmonicsNumber; i++)
        {
            n += pow(-1, coeff) * (sin(coeff * x) / coeff);
            coeff++;
        }
        
        return n * .5;
    }

    /// @brief Array di funzioni visualizzate nel render.
    struct {
        bool Display;
        void(*DrawPt)(bool, bool);
    } All[] = {
        true, DRAW_FN_POINT(Functions::SineWave, (W-6), (H-6), T, 0, Precision, 6, "black"),
        true, DRAW_FN_POINT(Functions::SquareWave, (W-6), (H-6), T, 0, Precision, 4, "red"),
        true, DRAW_FN_POINT(Functions::SquareHarmonics[3], (W-6), (H-6), T, 0, Precision, 6, "green"),
        true, DRAW_FN_POINT(Functions::SquareHarmonics[5], (W-6), (H-6), T, 0, Precision, 6, "violet"),
        true, DRAW_FN_POINT(Functions::SquareHarmonics[7], (W-6), (H-6), T, 0, Precision, 6, "yellowgreen"),
        true, DRAW_FN_POINT(Functions::SquareFourier, (W-6), (H-6), T, 0, Precision, 10, "blue"),
        false, DRAW_FN_POINT(Functions::SawToothFourier, (W-6), (H-6), T, 0, Precision, 10, "orange"),
    };

    exported void DisplayFunction(int index, bool v) {
        All[index].Display = v;
    }

    exported void setSquareHarmonicsNumber(int n) {
        SquareHarmonicsNumber = n;
    }

    exported void setSawToothHarmonicsNumber(int n) {
        SawToothHarmonicsNumber = n;
    }

    int Interval = 0;

    exported void setint(int n) {
        Interval = n;
    }

    void Render(void*) {
        double start = microtime();

        for (int i = 0; i < 100; i++)
         for(int j = 0; j < sizeof(All) / sizeof(All[0]); j++) 
                All[j].DrawPt(All[j].Display, j == 0);

        double frameTime = microtime() - start;

        $("#frameTime").innerText = (string)round(frameTime);
        $("#frameRate").innerText = (string)round(1000 / frameTime);

        setTimeout(Render, Interval);
    }

    void StartRender() {
        setTimeout(Render, Interval);
    }
} // namespace Functions
