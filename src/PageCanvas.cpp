#include "D:\kinderc\kinderc.hpp"

namespace PageCanvas
{
    //static HTMLCanvasElement canvas = $("canvas");
    #define canvas HTMLCanvasElement($("canvas"))

    void Init() {
        canvas.Init();
    }

    void DrawPoint(double x, double y, int size, const char* color, int len) {
        canvas.DrawRectangle(x, y, size, size, color);
    }

    void Clear() {
        canvas.Clear();
    }
} // namespace PageCanvas
