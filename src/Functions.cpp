#include "include.hpp"

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

/// @brief Intervallo di funzioni visualizzato.
double T = M_PI * 4;

/// @brief Precisione del render (delta tra i punti).
double Precision = .01;

/// @brief Imposta l'intervallo della funzione visualizzato nel canvas.
/// @param v Intervallo da impostare.
exported void setT(double v) {
    PageCanvas::Clear();
    T = v;
}

/// @brief Imposta la precisione (DELTA) del render.
/// @param v Precisione impostata.
exported void setPrecision(double v) {
    Precision = v;
}

namespace Functions
{
    /// @brief Funzione seno, implementata usando la successione di Taylor.
    /// @param x Un angolo qualunque.
    /// @return Il seno dell'angolo.
    double SineWave(double x) {
        return sin(x);
    }

    /// @brief Funzione onda quadra [IDEALE]. Restituisce solo valori corrispondenti a -1 e 1.
    /// @param x Valore in input.
    /// @return Valore dell'onda quadra in output.
    double IdealSquareWave(double x) {
        return floor(SineWave(x)) == 0? .78 : -.78;
    }

    /// @brief Array che raccoglie alcune armoniche dell'onda quadra.
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

    /// @brief Funzione onda quadra calcolata usando la successione di Fourier (con tanti termini quanto vale SquareHarmonicsNumber).
    /// @param x Valore dell'ascissa.
    /// @return Valore dell'ordinata.
    double SquareFourier(double x) {
        int coeff = 1;
        double n = 0;
        for (int i = 0; i < SquareHarmonicsNumber; i++)
        {
            n += SineWave(coeff * x) / coeff;

            // Incremento il coefficiente di 2 perchè la funzione ha solo armoniche dispari.
            coeff += 2;
        }
        return n;
    }

    /// @brief Numero di armoniche usate nella funzione SawToothFourier.
    int SawToothHarmonicsNumber = 3;

    /// @brief Funzione onda a dente di sega calcolata usando la successione di Fourier (con tanti termini quanto vale SawToothHarmonicsNumber).
    /// @param x Valore dell'ascissa.
    /// @return Valore dell'ordinata.
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

    /// @brief Numero di armoniche usate nella funzione TriangleFourier.
    int TriangleHarmonicsNumber = 3;

    /// @brief Funzione onda a triangolo calcolata usando la successione di Fourier (con tanti termini quanto vale TriangleHarmonicsNumber).
    /// @param x Valore dell'ascissa.
    /// @return Valore dell'ordinata.
    double TriangleFourier(double x) {
        int coeff = 1;
        double n = 0;
        for (int i = 0; i < TriangleHarmonicsNumber; i++)
        {
            n += cos(coeff * x) / pow(coeff, 2);
            coeff += 2;
        }
        return n*.5;
    }

    /// @brief Array di funzioni visualizzate nel render.
    struct t_fnelm {
        bool Display;
        void(*DrawPt)(bool, bool);
    } All[] = {
        true, DRAW_FN_POINT(Functions::SineWave, (PageCanvas::Width-6), (PageCanvas::Height-6), T, 0, Precision, 6, "black"),
        true, DRAW_FN_POINT(Functions::IdealSquareWave, (PageCanvas::Width-6), (PageCanvas::Height-6), T, 0, Precision, 4, "red"),
        true, DRAW_FN_POINT(Functions::SquareHarmonics[3], (PageCanvas::Width-6), (PageCanvas::Height-6), T, 0, Precision, 6, "green"),
        true, DRAW_FN_POINT(Functions::SquareHarmonics[5], (PageCanvas::Width-6), (PageCanvas::Height-6), T, 0, Precision, 6, "violet"),
        true, DRAW_FN_POINT(Functions::SquareHarmonics[7], (PageCanvas::Width-6), (PageCanvas::Height-6), T, 0, Precision, 6, "yellowgreen"),
        true, DRAW_FN_POINT(Functions::SquareFourier, (PageCanvas::Width-6), (PageCanvas::Height-6), T, 0, Precision, 10, "blue"),
        false, DRAW_FN_POINT(Functions::SawToothFourier, (PageCanvas::Width-6), (PageCanvas::Height-6), T, 0, Precision, 10, "orange"),
        false, DRAW_FN_POINT(Functions::TriangleFourier, (PageCanvas::Width-6), (PageCanvas::Height-6), T, 0, Precision, 10, "darkviolet"),
    };

    /// @brief Imposta una funzione come visualizzabile o meno.
    /// @param index L'indice della funzione nell'array Functions::All.
    /// @param v Un valore booleano che indice se la funzione deve essere visualizzata o meno.
    exported void DisplayFunction(int index, bool v) {
        All[index].Display = v;
    }

    /// @brief Imposta il numero di armoniche della funzione SquareFourier.
    /// @param n Numero di armoniche.
    exported void setSquareHarmonicsNumber(int n) {
        SquareHarmonicsNumber = n;
    }

    /// @brief Imposta il numero di armoniche della funzione SawToothFourier.
    /// @param n Numero di armoniche.
    exported void setSawToothHarmonicsNumber(int n) {
        SawToothHarmonicsNumber = n;
    }

    /// @brief Imposta il numero di armoniche della funzione TriangleFourier.
    /// @param n Numero di armoniche.
    exported void setTriangleHarmonicsNumber(int n) {
        TriangleHarmonicsNumber = n;
    }

    /// @brief Numero di punti disegnati a ogni render.
    int PointsPerRender = 100;

    /// @brief Imposta il numero di punti disegnati a ogni render.
    /// @param n Numero di punti.
    exported void setPointsPerRender(int n) {
        PointsPerRender = n;
    }

    /// @brief Esegue il render di tanti punti quanto vale PointsPerRender. Calcola inoltre tempo di rendering e frame rate.
    void Render(void*) {
        double start = microtime();

        for (int i = 0; i < PointsPerRender; i++)
         for(int j = 0; j < sizeof(All) / sizeof(All[0]); j++) 
                All[j].DrawPt(All[j].Display, j == 0);

        double frameTime = microtime() - start;

        $("#frameTime").innerText = (string)round(frameTime);
        $("#frameRate").innerText = (string)round(1000 / frameTime);
    }

    int RenderProcessHandler;

    /// @brief Avvia il render.
    void StartRender() {
        RenderProcessHandler = setInterval(Render, 0);
    }

    /// @brief Ferma il render.
    void StopRender() {
        clearInterval(RenderProcessHandler);
    }

    exported void ToggleRender() {
        static bool rendering = false;
        rendering = !rendering;

        HTMLElement btn = $("#btnToggle");

        if(rendering)  {
            StopRender();
            btn.className = "btn btn-success";
            btn.innerHTML = "<i class='fa fa-play'></i>";
        }
        else {
            StartRender();
            btn.className = "btn btn-danger";
            btn.innerHTML = "<i class='fa fa-stop'></i>";
        }
    }
} // namespace Functions
