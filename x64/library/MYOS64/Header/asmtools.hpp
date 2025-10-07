#pragma once
#include "defines.hpp"

#define pushaq \
    asm volatile ( \
        "push %rbp\n\t" \
        "mov %rsp, %rbp\n\t" \
        "pushfq\n\t" \
        "push %rax\n\t" \
        "push %rbx\n\t" \
        "push %rcx\n\t" \
        "push %rdx\n\t" \
        "push %rsi\n\t" \
        "push %rdi\n\t" \
        "push %r8\n\t" \
        "push %r9\n\t" \
        "push %r10\n\t" \
        "push %r11\n\t" \
        "push %r12\n\t" \
        "push %r13\n\t" \
        "push %r14\n\t" \
        "push %r15\n\t" \
        "push %gs\n\t" \
        "push %fs\n\t" \
        "mov %es, %ax\n\t" \
        "push %rax\n\t" \
        "mov %ds, %ax\n\t" \
        "push %rax\n\t" \
    )
    
#define popaq \
    asm volatile ( \
        "pop %rax\n\t" \
        "mov %ax, %ds\n\t" \
        "pop %rax\n\t" \
        "mov %ax, %es\n\t" \
        "pop %fs\n\t" \
        "pop %gs\n\t" \
        "pop %r15\n\t" \
        "pop %r14\n\t" \
        "pop %r13\n\t" \
        "pop %r12\n\t" \
        "pop %r11\n\t" \
        "pop %r10\n\t" \
        "pop %r9\n\t" \
        "pop %r8\n\t" \
        "pop %rdi\n\t" \
        "pop %rsi\n\t" \
        "pop %rdx\n\t" \
        "pop %rcx\n\t" \
        "pop %rbx\n\t" \
        "pop %rax\n\t" \
        "popfq\n\t" \
        "pop %rbp\n\t" \
    )

#define code \
void __attribute__((naked))

#define Generate(pointer_var, generator, size) \
    int pointer_var[(size) / 4] = generator
 

#define asmbly(command) \
    __asm__ __volatile__ (".intel_syntax noprefix\n" command "\n.att_syntax prefix\n")\

#define asm_att(command) \
    __asm__ __volatile__("\n.att_syntax prefix\n");\
    __asm__ __volatile__ (command)

#define stack_push(value) \
    asmbly("push " #value)

#define stack_pop(value) \
    asmbly("pop " #value)

#define popf()\
    asmbly("popf")

#define pushf()\
    asmbly("pushf")

#define halt() \
    asmbly("hlt")


#define dd(value) \
    asmbly(".long " #value)

#define mov(reg, value) \
    asmbly("mov " #reg "," #value)

#define set_cs(value) \
    asmbly("jmp " #value ":after_cs_set" #value); \
    asmbly("after_cs_set" #value ":")

#define Set_x64_Flag(value) \
asmbly( \
    "mov ecx, 0xC0000080\n" \
    "rdmsr\n" \
);\
if (value) asmbly("or eax, 1 << 8\n"); \
else asmbly("and eax, ~(1 << 8)\n"); \
asmbly("wrmsr\n") \

#define declare_reg(reg) \
struct Register_##reg { \
    operator int() const { \
        int64 v; \
        asm_att("mov %%" #reg ", %0" : "=r"(v)); \
        return v; \
    } \
    Register_##reg& operator=(int64 v) { \
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

//void set_cs(gdt::permission required_previlege, bool is_long_mode, int gdt_index);

struct Registers {

    declare_reg(rsp);
    declare_reg(rbp);

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

void outb(uint16 port, uint8 data);
void outw(uint16 port, uint16 data);

uint8 inb(uint16 port);
uint16 inw(uint16 port);
uint64_t rdmsr(uint32_t msr);
void wrmsr(uint32_t msr, uint64_t value);