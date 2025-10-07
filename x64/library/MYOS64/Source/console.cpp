#include <MYOS64>

uint8* video = (uint8*)0xb8000;

namespace Console{

    void Clear(int console_color, char text = ' ') {
        video = (uint8*)0xb8000;
        for (int i = 0; i <= 25 * 80 * 2; i += 2) video[i] = text;
        for (int i = 1; i <= 25 * 80 * 2; i += 2) video[i] = console_color;
    }

    void Clear(color back_color, color text_color, char text = ' ') {
        Clear((back_color << 4) | text_color, text);
    }

    void Clear() {
        Clear(0x07);
    }

    void Print(char* str, int y, int x) {
        for (int i = 0; str[i] != 0; i++) video[(i + (y * 80 + x)) * 2] = str[i];
    }

    void Print(char chr, int y, int x) {
        video[(y * 80 + x) * 2] = chr;
    }

    void Printnum(uint64 number, int y, int x) {
        Print(num_to_str(number), y, x);
    }

}