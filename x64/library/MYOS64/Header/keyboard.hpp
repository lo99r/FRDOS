#include "defines.hpp"

const char scancode_to_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b', // 0x00 - 0x0F
   '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',    // 0x10 - 0x1D
    0,  'a','s','d','f','g','h','j','k','l',';','\'','`',        // 0x1E - 0x2B
    0,  '\\','z','x','c','v','b','n','m',',','.','/', 0,         // 0x2C - 0x38
    '*', 0,  ' ', 0,  0,  0,  0,  0,  0,  0,                      // 0x39 - 0x42
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,                        // 0x43 - 0x4C
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,                        // 0x4D - 0x56
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,                        // 0x57 - 0x60
    0,  0,  0,  0,  0,  0,  0,  0                                 // 0x61 - 0x68
    // 나머지 0x69~0x7F 도 대부분 확장 키 / F키로 0으로 유지
};

enum class Scancode : uint8 {

    // 숫자 키
    KEY_1 = 0x02,
    KEY_2 = 0x03,
    KEY_3 = 0x04,
    KEY_4 = 0x05,
    KEY_5 = 0x06,
    KEY_6 = 0x07,
    KEY_7 = 0x08,
    KEY_8 = 0x09,
    KEY_9 = 0x0A,
    KEY_0 = 0x0B,

    // 알파벳 키
    KEY_Q = 0x10,
    KEY_W = 0x11,
    KEY_E = 0x12,
    KEY_R = 0x13,
    KEY_T = 0x14,
    KEY_Y = 0x15,
    KEY_U = 0x16,
    KEY_I = 0x17,
    KEY_O = 0x18,
    KEY_P = 0x19,
    KEY_A = 0x1E,
    KEY_S = 0x1F,
    KEY_D = 0x20,
    KEY_F = 0x21,
    KEY_G = 0x22,
    KEY_H = 0x23,
    KEY_J = 0x24,
    KEY_K = 0x25,
    KEY_L = 0x26,
    KEY_Z = 0x2C,
    KEY_X = 0x2D,
    KEY_C = 0x2E,
    KEY_V = 0x2F,
    KEY_B = 0x30,
    KEY_N = 0x31,
    KEY_M = 0x32,

    // 특수 키
    ENTER = 0x1C,
    ESCAPE = 0x01,
    BACKSPACE = 0x0E,
    TAB = 0x0F,
    SPACE = 0x39,
    CAPSLOCK = 0x3A,
    LEFT_SHIFT = 0x2A,
    RIGHT_SHIFT = 0x36,
    LEFT_CTRL = 0x1D,
    LEFT_ALT = 0x38,

    // 기능 키
    F1 = 0x3B,
    F2 = 0x3C,
    F3 = 0x3D,
    F4 = 0x3E,
    F5 = 0x3F,
    F6 = 0x40,
    F7 = 0x41,
    F8 = 0x42,
    F9 = 0x43,
    F10 = 0x44,
    F11 = 0x57,
    F12 = 0x58,

    // 방향키 (확장 스캔코드 필요)
    ARROW_UP = 0x48,  // E0 48
    ARROW_DOWN = 0x50,  // E0 50
    ARROW_LEFT = 0x4B,  // E0 4B
    ARROW_RIGHT = 0x4D,  // E0 4D

    // 기타
    INSERT = 0x52,  // E0 52
    DELETE = 0x53,  // E0 53
    HOME = 0x47,  // E0 47
    END = 0x4F,  // E0 4F
    PAGE_UP = 0x49,  // E0 49
    PAGE_DOWN = 0x51,  // E0 51
    NUMLOCK = 0x45,
    SCROLLLOCK = 0x46,

    UNKNOWN = 0x00
};

class Key {
private:
    Scancode scancode;
public:
    char chr();
    Scancode scode();

    // 키가 눌렸다면 True를, 키가 떼졌다면 False 를 반환합니다.
    bool Pressed();

    Key(Scancode _scancode);
    Key(uint8 _scancode);    
    Key();
};

class Keyboard {
private:


public:
    void Enable_Interrupt();
    void Disable_Interrupt();
    Key Readkey();
};