#include <MYOS>

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1



namespace myos::PIC {

    int IDT_offset1;
    int IDT_offset2;

    void initialize(int offset1 = 32, int offset2 = 40) {
        IDT_offset1 = offset1;
        IDT_offset2 = offset2;

        unsigned char a1, a2;

        a1 = readPort<uint8>(PIC1_DATA); // ���� ����ũ�� ����
        a2 = readPort<uint8>(PIC2_DATA);

        // �ʱ�ȭ ����
        sendPort<uint8>(PIC1_COMMAND, 0x11);
        sendPort<uint8>(PIC2_COMMAND, 0x11);

        // ���ͷ�Ʈ ���� ������ �缳��
        sendPort<uint8>(PIC1_DATA, offset1);
        sendPort<uint8>(PIC2_DATA, offset2);

        // PIC1�� PIC2 ���� �˸�
        sendPort<uint8>(PIC1_DATA, 4);
        // PIC2�� �����̺� ID 2
        sendPort<uint8>(PIC2_DATA, 2);

        // ��� ����
        sendPort<uint8>(PIC1_DATA, 0x01);
        sendPort<uint8>(PIC2_DATA, 0x01);

        // ����ũ ����
        sendPort<uint8>(PIC1_DATA, a1);
        sendPort<uint8>(PIC2_DATA, a2);
    }
};