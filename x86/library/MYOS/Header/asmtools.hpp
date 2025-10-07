#pragma once
#include "defines.hpp"

#define void_naked \
void __attribute__((naked))

#define asm_intel(command) \
    __asm__ __volatile__ (".intel_syntax noprefix\n");\
    __asm__ __volatile__ (command);\
    __asm__ __volatile__ ( "\n.att_syntax prefix\n")

#define asm_att(command) \
    __asm__ __volatile__("\n.att_syntax prefix\n");\
    __asm__ __volatile__ (command)

#define stack_push(value) \
    asm_intel("push " #value)

#define stack_pop(value) \
    asm_intel("pop " #value)

#define popf()\
    asm_intel("popf")

#define pushf()\
    asm_intel("pushf")

#define halt() \
    asm_intel("hlt")


#define dd(value) \
    asm_intel(".long " #value)

#define mov(reg, value) \
    asm_intel("mov " #reg "," #value)

#define set_cs(value) \
    asm_intel("jmp " #value ":after_cs_set" #value); \
    asm_intel("after_cs_set" #value ":")

#define Set_x64_Flag(value) \
asm_intel( \
    "mov ecx, 0xC0000080\n" \
    "rdmsr\n" \
);\
if (value) asm_intel("or eax, 1 << 8\n"); \
else asm_intel("and eax, ~(1 << 8)\n"); \
asm_intel("wrmsr\n") \

#define declare_reg(reg) \
struct Register_##reg { \
    operator int() const { \
        int v; \
        asm_att("mov %%" #reg ", %0" : "=r"(v)); \
        return v; \
    } \
    Register_##reg& operator=(int v) { \
        asm_att("mov %0, %%" #reg :: "r"(v)); \
        return *this; \
    } \
} reg\

#define declare_reg16(reg) \
struct Register_##reg { \
    inline operator uint16() const { \
        uint16 v = 99; \
        asm_att("mov %%" #reg ", %0" : "=r"(v)); \
        return v; \
    } \
    inline Register_##reg& operator=(uint16 v) { \
        asm_att("mov %0, %%" #reg :: "r"(v)); \
        return *this; \
    } \
} reg\

namespace myos {

    //void set_cs(gdt::permission required_previlege, bool is_long_mode, int gdt_index);

    struct Registers {

        declare_reg(esp);
        declare_reg(ebp);

        declare_reg16(ds);
        declare_reg16(es);
        declare_reg16(ss);
        declare_reg16(fs);
        declare_reg16(gs);

        declare_reg(cr0);
        declare_reg(cr2);
        declare_reg(cr3);
        declare_reg(cr4);
    };

    enum class Permission { // 지정한 영역에 접근할 수 있는 존재를 지정합니다. 이걸 처음 보신다면, 일단 당신은 "신" 입니다.
        KERNEL, // 신
        SUPERADMIN, // 고급 관리자
        ADMIN, // 일반 관리자
        USER // 일반 사용자
    };

    template<typename T>
    static T readPort(uint16_t port) {
        T value;
        if constexpr (sizeof(T) == 1) {
            asm volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
        }
        else if constexpr (sizeof(T) == 2) {
            asm volatile ("inw %1, %0" : "=a"(value) : "Nd"(port));
        }
        else if constexpr (sizeof(T) == 4) {
            asm volatile ("inl %1, %0" : "=a"(value) : "Nd"(port));
        }
        else {
            static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4,
                "readPort<T>: unsupported data size");
        }
        return value;
    }

    template<typename T>
    static void sendPort(uint16_t port, T data) {
        if constexpr (sizeof(T) == 1) {
            asm volatile ("outb %0, %1" : : "a"(data), "Nd"(port));
        }
        else if constexpr (sizeof(T) == 2) {
            asm volatile ("outw %0, %1" : : "a"(data), "Nd"(port));
        }
        else if constexpr (sizeof(T) == 4) {
            asm volatile ("outl %0, %1" : : "a"(data), "Nd"(port));
        }
        else {
            static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4,
                "sendPort<T>: unsupported data size");
        }
    }

    uint64_t readMSR(uint32_t msr);
    void writeMSR(uint32_t msr, uint64_t value);

}

#undef declare_reg
#undef declare_reg16