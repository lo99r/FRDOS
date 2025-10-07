#pragma once
#include "defines.hpp"

namespace Console{
    enum color{
        black,
        blue,
        green,
        cyan,
        red,
        purple,
        yellow,
        white,
        gray,
        light_blue,
        light_green,
        light_cyan,
        light_red,
        light_purple,
        light_yellow,
        light,
    };
    void Clear(color back_color, color text_color, char text = ' ');
    void Clear(int console_color, char text = ' ');
    void Clear();
    void Print(char* str, int y, int x);
    void Print(char chr, int y, int x);
    void Printnum(uint32 num, int y, int x);
}