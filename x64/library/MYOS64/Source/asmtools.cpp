#include <MYOS64>

struct far_jmp{
    uint32 addr;
    uint16 cs_value;
} jmp_set;

// MSR �������Ϳ��� 64��Ʈ �� �б�
uint64_t rdmsr(uint32_t msr) {
    uint32_t low, high;
    asm volatile (
        "rdmsr"
        : "=a"(low), "=d"(high)
        : "c"(msr)
        );
    return ((uint64_t)high << 32) | low;
}

// MSR �������Ϳ� 64��Ʈ �� ����
void wrmsr(uint32_t msr, uint64_t value) {
    uint32_t low = (uint32_t)(value & 0xFFFFFFFF);
    uint32_t high = (uint32_t)(value >> 32);
    asm volatile (
        "wrmsr"
        :
    : "c"(msr), "a"(low), "d"(high)
        );
}

void outb(uint16 port, uint8 data){
    asm volatile (
        "outb %0, %1"
        :
        : "a"(data), "Nd"(port)
    );
}

void outw(uint16 port, uint16 data){
    asm volatile (
        "outw %0, %1"
        :
        : "a"(data), "Nd"(port)
    );
}

uint8 inb(uint16 port) {
    uint8 value;
    asm volatile ("inb %1, %0"
        : "=a"(value)
        : "Nd"(port));
    return value;
}

// 16��Ʈ ��Ʈ �Է�
uint16 inw(uint16 port) {
    uint16 value;
    asm volatile ("inw %1, %0"
        : "=a"(value)
        : "Nd"(port));
    return value;
}
