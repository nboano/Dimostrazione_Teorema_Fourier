#include "include.hpp"

namespace PageCanvas
{
    /// @brief Canvas dove vengono disegnate le funzioni.
    #define canvas HTMLCanvasElement($("canvas"))

    /// @brief Larghezza del canvas (px)
    int Width;

    /// @brief Altezza del canvas (px)
    int Height;

    void SetResolution(int w, int h) {
        Width = w;
        Height = h;
        canvas.setAttribute("width", (string)w);
        canvas.setAttribute("height", (string)h);
    }

    void Init() {
        ScreenInformations si = Enviroment::Screen;

        SetResolution(si.Width, si.Height);
        canvas.Init();
    }

    void DrawPoint(double x, double y, int size, const char* color) {
        canvas.DrawRectangle(x, y, size, size, color);
    }

    void DrawLine(double sx, double sy, double ex, double ey, int size, const char* color) {
        canvas.DrawLine(sx, sy, ex, ey, size, color);
    }

    void Clear() {
        canvas.Clear();
    }

    #undef canvas
} // namespace PageCanvas
