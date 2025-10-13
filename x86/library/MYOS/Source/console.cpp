#include <MYOS>

uint8* video = (uint8*)0xb8000;
int cur_y = 0;
int cur_x = 0;

namespace myos::console {

    void clear(int console_color, char text = ' ') {
        video = (uint8*)0xb8000;
        for (int i = 0; i <= 25 * 80 * 2; i += 2) video[i] = text;
        for (int i = 1; i <= 25 * 80 * 2; i += 2) video[i] = console_color;
    }

    void clear(color back_color, color text_color, char text = ' ') {
        clear((back_color << 4) | text_color, text);
    }

    void clear() {
        clear(0x07);
        cur_x = 0;
        cur_y = 0;
    }

    void print(char* str) {
        for (int i = 0; str[i] != 0; i++) {
            print(str[i]);
        }
    }

    inline void erase() {
        if (cur_x > 0) cur_x--;
        print(' ', cur_y, cur_x);
    }
    uint16_t posst = 0;
    void print(char chr) {
        if (chr == '\n') {
            cur_y++;
            cur_x = 0;
            // 화면 끝까지 갔으면 스크롤
            if (cur_y >= 25) {
                // 한 줄 위로 밀기
                for (int i = 0; i < (24 * 80 * 2); i++)
                    video[i] = video[i + 160]; // 한 줄(160바이트) 위로 복사
                // 마지막 줄 비우기
                for (int i = 24 * 80 * 2; i < 25 * 80 * 2; i += 2) {
                    video[i] = ' ';
                    video[i + 1] = 0x07;
                }
                cur_y = 24;
            }
        }
        else if (chr == '\b') {
            erase();
        }
        else {
            video[(cur_y * 80 + cur_x) * 2] = chr;
            video[(cur_y * 80 + cur_x) * 2 + 1] = 0x07; // 기본 흰색 글씨
            cur_x++;
            // 한 줄 끝났을 때
            if (cur_x >= 80) {
                cur_x = 0;
                cur_y++;
                // 화면 끝까지 갔으면 스크롤
                if (cur_y >= 25) {
                    for (int i = 0; i < (24 * 80 * 2); i++)
                        video[i] = video[i + 160];
                    for (int i = 24 * 80 * 2; i < 25 * 80 * 2; i += 2) {
                        video[i] = ' ';
                        video[i + 1] = 0x07;
                    }
                    cur_y = 24;
                }
            }
        }
        posst = (cur_y * 80) + cur_x;
        outb(0x3D4, 0x0F);
        outb(0x3D5, posst & 0xFF);
        outb(0x3D4, 0x0E);
        outb(0x3D5, (posst >> 8) & 0xFF);
    }


    void print(char* str, int y, int x) {
        for (int i = 0; str[i] != 0; i++) video[(i + (y * 80 + x)) * 2] = str[i];
    }

    void print(char chr, int y, int x) {
        video[(y * 80 + x) * 2] = chr;
    }

    void printInt(uint64 number, int y, int x) {
        char str[25] = { 0, };
        intToStr(number, str);
        print(str, y, x);
    }

}
