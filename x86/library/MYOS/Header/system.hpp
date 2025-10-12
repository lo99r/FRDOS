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

inline uint16 inw(uint16 port) {
    uint16 ret;
    asm volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port))
        ;
    return ret;
}

inline void outb(uint16 port, uint8 val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}


inline void outw(uint16 port, uint16 val) {
    asm volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

inline uint32 inl(uint16 port) {
    uint32 ret;
    asm volatile("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

inline void outl(uint16 port, uint32 val) {
    asm volatile("outl %0, %1" : : "a"(val), "Nd"(port));
}

static inline void io_wait() {
    asm volatile("outb %%al, $0x80" : : "a"(0));
}

namespace myos::system {

    void reboot();
    void shutdown();

}
