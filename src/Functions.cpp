#include "include.hpp"

#include "Functions.TriangleWave.cpp"
#include "Functions.SquareWave.cpp"
#include "Functions.SawtoothWave.cpp"

#define HM_PRECISION (Functions::Precision)
#define HM_MAXN (1 << 7)

struct WaveInfo {
    double* prevx;
    double* prevy;
    double* x;

    void Reset() {
        *x = 0;
        *prevx = 0;
        *prevy = PageCanvas::Height/2;
    }
};

struct HarmonicsGroupInfo {
    double* prevx_arr;
    double* prevy_arr;
    double* x;

    void Reset() {
        *x = 0;
        for (int i = 0; i < HM_MAXN; i++)
        {
            prevx_arr[i] = 0;
            prevy_arr[i] = PageCanvas::Height/2;
        }
    }
};


#define DRAW_FN(f, W, H, image, phase, delta, linewidth, fillStyle, onlypoints) \
[](double p = 0, const char* color = fillStyle) {\
    static double x = phase;\
    double y = f(x, p);\
    double ex = (W / (image)) * x;\
    double ey = (H/2 +  y* H/2);\
    x += delta;\
    static double last_x = ex;\
    static double last_y = ey;\
    if(ex >= W) {\
        x = phase;\
        last_x = 0;\
        return;\
    }\
    PageCanvas::DrawLine(last_x, last_y, ex, ey, linewidth, color);\
    last_x = ex; last_y = ey;\
    CurrentWave = {&last_x,&last_y,&x};\
}\

#define DRAW_HARMONIC(f, W, H, image, phase, delta, linewidth, fillStyle, onlypoints) \
[](int p = 0, const char* color = fillStyle) {\
    static double x = phase;\
    double y = f(x, p);\
    double ex = (W / (image)) * x;\
    double ey = (H/2 +  y* H/2);\
    x += delta;\
    static double last_x[HM_MAXN];\
    static double last_y[HM_MAXN];\
    if(ex >= W) {\
        x = phase;\
        for(int i = 0; i < HM_MAXN; i++) {last_x[i] = 0; last_y[i] = 0;}\
        return;\
    }\
    if(!last_y[p]) last_y[p] = H/2;\
    PageCanvas::DrawLine(last_x[p], last_y[p], ex, ey, linewidth, color);\
    last_x[p] = ex; last_y[p] = ey;\
    CurrentHarmonics =  {last_x,last_y,&x};\
}\

namespace Functions
{
    WaveInfo CurrentWave;
    HarmonicsGroupInfo CurrentHarmonics;

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
    Function Selected;

    /// @brief Intervallo di funzioni visualizzato.
    double T = M_PI * 4;

    /// @brief Precisione del render (delta tra i punti).
    double Precision = .01;

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
        CurrentWave.Reset();
        CurrentHarmonics.Reset();
        PageCanvas::Clear();
        FirstDrawnHarmonic = hn;
    }

    exported void SetLastDrawnHarmonic(int hn) {
        CurrentWave.Reset();
        CurrentHarmonics.Reset();
        PageCanvas::Clear();
        LastDrawnHarmonic = hn;
    }

    exported void SetHarmonicsNumber(int hn) {
        CurrentWave.Reset();
        CurrentHarmonics.Reset();
        PageCanvas::Clear();
        HarmonicsNumber = hn;
    }

    exported void SetT(double v) {
        CurrentWave.Reset();
        CurrentHarmonics.Reset();
        PageCanvas::Clear();
        T = v;
    }

    exported void SetPrecision(double v) {
        CurrentWave.Reset();
        CurrentHarmonics.Reset();
        PageCanvas::Clear();
        Precision = v;
    }

    exported void SelectFunction(Function sf) {
        CurrentWave.Reset();
        CurrentHarmonics.Reset();
        PageCanvas::Clear();
        Selected = sf;
    }

    exported void ToggleHarmonics(bool v) {
        CurrentWave.Reset();
        CurrentHarmonics.Reset();
        PageCanvas::Clear();
        ViewHarmonics = v;
    }

    // QUADRA

    auto SquareFourierSeries = DRAW_FN(Functions::SquareWave::Series, (PageCanvas::Width-4), (PageCanvas::Height-4), T, 0, Precision, 4, "blue", false);

    auto SquareFourierHarmonic = DRAW_HARMONIC(Functions::SquareWave::SingleHarmonic, (PageCanvas::Width-3), (PageCanvas::Height-3), T, 0, HM_PRECISION, 3, "", true);

    // DENTE DI SEGA

    auto SawToothFourierSeries = DRAW_FN(Functions::SawtoothWave::Series, (PageCanvas::Width-4), (PageCanvas::Height-4), T, 0, Precision, 4, "orange", false);

    auto SawToothFourierHarmonic = DRAW_HARMONIC(Functions::SawtoothWave::SingleHarmonic, (PageCanvas::Width-4), (PageCanvas::Height-4), T, 0, HM_PRECISION, 3, "", true);

    // TRIANGOLO
    
    auto TriangleFourierSeries = DRAW_FN(Functions::TriangleWave::Series, (PageCanvas::Width-4), (PageCanvas::Height-4), T, 0, Precision, 4, "darkviolet", false);

    auto TriangleFourierHarmonic = DRAW_HARMONIC(Functions::TriangleWave::SingleHarmonic, (PageCanvas::Width-4), (PageCanvas::Height-4), T, 0, HM_PRECISION, 3, "", true);

    /// @brief Esegue il render di tanti punti quanto vale PointsPerRender. Calcola inoltre tempo di rendering e frame rate.
    void Render(void*) {
        double start = microtime();

        for (int i = 0; i < PointsPerRender; i++)
        {
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
