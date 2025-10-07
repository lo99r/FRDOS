#include <MYOS64>

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1



namespace PIC {

    int IDT_offset1;
    int IDT_offset2;

    void Initialize(int offset1 = 32, int offset2 = 40) {
        IDT_offset1 = offset1;
        IDT_offset2 = offset2;

        unsigned char a1, a2;

        a1 = inb(PIC1_DATA); // 현재 마스크값 저장
        a2 = inb(PIC2_DATA);

        // 초기화 시작
        outb(PIC1_COMMAND, 0x11);
        outb(PIC2_COMMAND, 0x11);

        // 인터럽트 벡터 오프셋 재설정
        outb(PIC1_DATA, offset1);
        outb(PIC2_DATA, offset2);

        // PIC1에 PIC2 존재 알림
        outb(PIC1_DATA, 4);
        // PIC2는 슬레이브 ID 2
        outb(PIC2_DATA, 2);

        // 모드 설정
        outb(PIC1_DATA, 0x01);
        outb(PIC2_DATA, 0x01);

        // 마스크 복원
        outb(PIC1_DATA, a1);
        outb(PIC2_DATA, a2);
    }
};