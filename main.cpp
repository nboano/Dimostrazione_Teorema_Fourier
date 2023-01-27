#include "D:/kinderc/kinderc.hpp"
#include "src/Functions.cpp"

int main() {
    PageCanvas::Init();

    setInterval([](void*) {
       Functions::Render();
    }, 0);
}