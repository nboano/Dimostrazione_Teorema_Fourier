#include "include.h"

namespace PageCanvas
{
    #define canvas HTMLCanvasElement($("canvas"))

    void Init() {
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
