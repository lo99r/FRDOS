#include<MYOS>

//

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
