#pragma once
#include <MYOS> // ��Ʈ �����, console, �⺻ �ڷ���

inline void wait(uint32 ticks) {
    for (volatile uint32 i = 0; i < ticks; i++) {
        // �� ����: volatile�� ����ȭ ����
        asm volatile ("nop");
    }
}

inline void wait_ms(uint32 ms) {
    const uint32 cycles_per_ms = 100000; // CPU �ӵ��� ���� ���� �ʿ�
    for (uint32 i = 0; i < ms * cycles_per_ms; i++) {
        asm volatile ("nop");
    }
}

inline uint8 inb(uint16 port) {
    uint8 ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

inline uint16_t inw(uint16_t port) {
    uint16 ret;
    asm volatile ("inw %1, %0" : "=a"(ret) : "ND"(port))
        ;
    return ret;
}

inline void outb(uint16 port, uint8 val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}


inline void outw(uint16 port, uint16 val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

namespace myos::system {

    void reboot();
    void shutdown();

}
