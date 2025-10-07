#pragma once
#include "defines.hpp"

namespace myos::console{
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
    void clear(color back_color, color text_color, char text = ' ');
    void clear(int console_color, char text = ' ');
    void clear();
    void print(char* str, int y, int x);
    void print(char chr, int y, int x);
    void print(char* str);
    void print(char chr);
    void printInt(uint64 num, int y, int x);
    void erase();
}