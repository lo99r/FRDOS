#include<MYOS>

inline void wait(uint32 ticks) {
    for (volatile uint32 i = 0; i < ticks; i++) {
        // 빈 루프: volatile로 최적화 방지
        asm volatile ("nop");
    }
}

inline void wait_ms(uint32 ms) {
    const uint32 cycles_per_ms = 100000; // CPU 속도에 맞춰 조정 필요
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

    // x86 키보드 컨트롤러를 통해 CPU 리셋
    void reboot() {
        console::print("Rebooting...\n");

        asm volatile ("cli"); // 인터럽트 끄기

        while (true) {
            uint8 status;
            do {
                status = inb(0x64);
            } while (status & 0x02); // 입력 버퍼가 비면

            outb(0x64, 0xFE); // CPU 재시작 명령
            asm volatile("hlt"); // 혹시 실행 안 되면 CPU halt
        }
    }

    // 단순히 무한 루프로 정지 (전원 끄기 ACPI 없으면)
    void shutdown() {
        console::print("Shutting down...\n");

        asm volatile ("cli"); // 인터럽트 끄기

        while (true) {
            asm volatile("hlt"); // CPU halt
        }
    }



}
