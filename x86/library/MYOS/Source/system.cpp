#include<MYOS>

//

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
