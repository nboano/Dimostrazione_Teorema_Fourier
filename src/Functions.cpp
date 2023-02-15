#include "include.hpp"

#include "Functions.TriangleWave.cpp"
#include "Functions.SquareWave.cpp"
#include "Functions.SawtoothWave.cpp"

/// @brief Crea una lambda che scrive un punto della funzione ogni qualvolta viene richiamata.
/// @param f La funzione.
/// @param W La larghezza del render.
/// @param H L'altezza del render.
/// @param image L'insieme immagine rappresentato della funzione.
/// @param delta Il delta iniziale della funzione.
/// @param linewidth Spessore della linea della funzione in pixel.
/// @param fillStyle Colore della linea (CSS).
#define DRAW_FN_POINT(f, W, H, image, phase, delta, linewidth, fillStyle) \
[](double p = 0, const char* color = fillStyle) {\
    static double x = phase;\
    double y = f(x, p);\
    double ex = (W / (image)) * x;\
    double ey = (H/2 +  y* H/2);\
    x += delta;\
    if(ex >= W) {\
        x = phase;\
        return;\
    }\
    PageCanvas::DrawPoint(ex,ey,linewidth,color);\
}\

namespace Functions
{
    const char* HARMONICS_COLORS[] = {
        "red",
        "grey",
        "green",
        "yellow",
        "purple",
        "blue",
        "orange",
        "darkgreen",
        "cyan"
    };

    enum Function {
        SQUARE          =           0,
        SAWTOOTH        =           1,
        TRIANGLE        =           2
    };

    /// @brief Funzione selezionata.
    Function Selected = SQUARE;

    /// @brief Intervallo di funzioni visualizzato.
    double T = M_PI * 4;

    /// @brief Precisione del render (delta tra i punti).
    double Precision = .0005;

    /// @brief Numero di armoniche calcolate.
    unsigned int HarmonicsNumber = 30;

    /// @brief Prima armonica visualizzata.
    unsigned int FirstDrawnHarmonic = 5;
    
    /// @brief Ultima armonica visualizzata.
    unsigned int LastDrawnHarmonic = 10;

    /// @brief Numero di punti disegnati a ogni render.
    int PointsPerRender = 500;

    /// @brief Booleana che indica se visualizzare o no le armoniche.
    bool ViewHarmonics = true;

    exported void SetFirstDrawnHarmonic(int hn) {
        PageCanvas::Clear();
        FirstDrawnHarmonic = hn;
    }

    exported void SetLastDrawnHarmonic(int hn) {
        PageCanvas::Clear();
        LastDrawnHarmonic = hn;
    }

    exported void SetHarmonicsNumber(int hn) {
        PageCanvas::Clear();
        HarmonicsNumber = hn;
    }

    exported void SetT(double v) {
        PageCanvas::Clear();
        T = v;
    }

    exported void SetPrecision(double v) {
        PageCanvas::Clear();
        Precision = v;
    }

    exported void SelectFunction(Function sf) {
        PageCanvas::Clear();
        Selected = sf;
    }

    exported void ToggleHarmonics(bool v) {
        PageCanvas::Clear();
        ViewHarmonics = v;
    }

    // QUADRA

    auto SquareFourierSeries = DRAW_FN_POINT(Functions::SquareWave::Series, (PageCanvas::Width-6), (PageCanvas::Height-6), T, 0, Precision, 6, "blue");

    auto SquareFourierHarmonic = DRAW_FN_POINT(Functions::SquareWave::SingleHarmonic, (PageCanvas::Width-4), (PageCanvas::Height-4), T, 0, Precision, 4, "");

    // DENTE DI SEGA

    auto SawToothFourierSeries = DRAW_FN_POINT(Functions::SawtoothWave::Series, (PageCanvas::Width-6), (PageCanvas::Height-6), T, 0, Precision, 6, "orange");

    auto SawToothFourierHarmonic = DRAW_FN_POINT(Functions::SawtoothWave::SingleHarmonic, (PageCanvas::Width-4), (PageCanvas::Height-4), T, 0, Precision, 4, "");

    // TRIANGOLO
    
    auto TriangleFourierSeries = DRAW_FN_POINT(Functions::TriangleWave::Series, (PageCanvas::Width-6), (PageCanvas::Height-6), T, 0, Precision, 6, "darkviolet");

    auto TriangleFourierHarmonic = DRAW_FN_POINT(Functions::TriangleWave::SingleHarmonic, (PageCanvas::Width-4), (PageCanvas::Height-4), T, 0, Precision, 4, "");

    /// @brief Esegue il render di tanti punti quanto vale PointsPerRender. Calcola inoltre tempo di rendering e frame rate.
    void Render(void*) {
        double start = microtime();

        for (int i = 0; i < PointsPerRender; i++) {
            switch (Selected)
            {
                case SQUARE:
                    SquareFourierSeries(HarmonicsNumber);
                    break;
                case TRIANGLE:
                    TriangleFourierSeries(HarmonicsNumber);
                    break;
                case SAWTOOTH:
                    SawToothFourierSeries(HarmonicsNumber);
                    break;
            }
            if(ViewHarmonics && FirstDrawnHarmonic && LastDrawnHarmonic)
                for (int i = FirstDrawnHarmonic; i < LastDrawnHarmonic; i++)
                    switch (Selected)
                    {
                        case SQUARE:
                            SquareFourierHarmonic(i, HARMONICS_COLORS[i % (sizeof(HARMONICS_COLORS) / sizeof(HARMONICS_COLORS[0]))]);
                            break;
                        case TRIANGLE:
                            TriangleFourierHarmonic(i, HARMONICS_COLORS[i % (sizeof(HARMONICS_COLORS) / sizeof(HARMONICS_COLORS[0]))]);
                            break;
                        case SAWTOOTH:
                            SawToothFourierHarmonic(i, HARMONICS_COLORS[i % (sizeof(HARMONICS_COLORS) / sizeof(HARMONICS_COLORS[0]))]);
                            break;
                    }
        }

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
