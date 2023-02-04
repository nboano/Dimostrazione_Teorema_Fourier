#include "include.hpp"

namespace PageCanvas
{
    #define canvas HTMLCanvasElement($("canvas"))

    /// @brief Larghezza del canvas (px)
    int Width;

    /// @brief Altezza del canvas (px)
    int Height;

    void Init() {
        Width = (int)canvas.Width;
        Height = (int)canvas.Height;
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
