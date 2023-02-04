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

    void ClearXAt(double x, int size) {
        canvas.DrawRectangle(x, 0, size, canvas.Height, "#FFF");
    }

    void Clear() {
        canvas.Clear();
    }

    #undef canvas
} // namespace PageCanvas
