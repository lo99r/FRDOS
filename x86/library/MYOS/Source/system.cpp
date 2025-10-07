#include<MYOS>

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

inline void outb(uint16 port, uint8 val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

namespace myos::system {

    // x86 Ű���� ��Ʈ�ѷ��� ���� CPU ����
    void reboot() {
        console::print("Rebooting...\n");

        asm volatile ("cli"); // ���ͷ�Ʈ ����

        while (true) {
            uint8 status;
            do {
                status = inb(0x64);
            } while (status & 0x02); // �Է� ���۰� ���

            outb(0x64, 0xFE); // CPU ����� ���
            asm volatile("hlt"); // Ȥ�� ���� �� �Ǹ� CPU halt
        }
    }

    // �ܼ��� ���� ������ ���� (���� ���� ACPI ������)
    void shutdown() {
        console::print("Shutting down...\n");

        asm volatile ("cli"); // ���ͷ�Ʈ ����

        while (true) {
            asm volatile("hlt"); // CPU halt
        }
    }



}
