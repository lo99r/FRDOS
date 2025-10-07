#pragma once
#include "asmtools.hpp"
#include "defines.hpp"

#define MULTIBOOT_HEADER_MAGIC    0x1BADB002
#define MULTIBOOT_HEADER_FLAGS    0x10007

#define MULTIBOOT_HEADER(Header_address, Kernel_load_address, Start_function, GUI_enable) \
    { MULTIBOOT_HEADER_MAGIC, MULTIBOOT_HEADER_FLAGS, \
      -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS), \
      (Header_address), (Kernel_load_address), (0), (0), (Start_function), !(GUI_enable), 1280, 720, 32, }



#define DEFAULT_BOOT(Kernel_Load, Kernel_Stack, GUI_enable) \
\
void_naked start(); \
void start2(); \
\
int GRUB_Header_Array[0x30]__attribute__((section(".multiboot"))) = MULTIBOOT_HEADER((int)GRUB_Header_Array, Kernel_Load, (int)&start, GUI_enable);\
\
void Main(uint32, GrubBootInfo&);\
\
uint32 magic_number;\
uint32 boot_info_addr;\
GrubBootInfo* boot_info;\
\
void gdt_on();\
\
void_naked start() {\
\
    asm volatile("movl %%eax, %0\n\t"\
    "movl %%ebx, %1\n\t"\
    : "=r"(magic_number), "=r"(boot_info_addr)\
    :\
    : "eax", "ebx");\
    __asm__ __volatile__(\
        "movl %0, %%esp\n"\
        :\
    : "i"(Kernel_Stack)\
        );\
\
    stack_push(0);\
    popf();\
\
    gdt_on();\
    boot_info = (GrubBootInfo*)boot_info_addr;\
    Main(magic_number, *boot_info);\
    halt();\
\
}\
\
myos::gdt::gdt_table GDT[10]; \
\
\
void gdt_on() {\
    myos::Registers regs;\
\
    GDT[1] = myos::gdt::default_code_set;\
    GDT[2] = myos::gdt::default_data_set;\
\
    GDT[3] = myos::gdt::default_code_set;\
    GDT[3].bits = 64;\
\
    GDT[4] = myos::gdt::default_data_set;\
    GDT[4].bits = 64;\
\
    myos::gdt::load_gdt(GDT, 5);\
\
\
    regs.ds = regs.es = regs.fs = regs.gs = regs.ss = 0x10;\
    set_cs(0x8);\
} 

#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

// 메모리 맵 항목
typedef struct multiboot_memory_map {
    uint32_t size;
    uint64_t addr;
    uint64_t len;
#define MULTIBOOT_MEMORY_AVAILABLE              1
#define MULTIBOOT_MEMORY_RESERVED               2
    uint32_t type;
} __attribute__((packed)) multiboot_memory_map_t;

// 모듈 정보
typedef struct {
    uint32_t mod_start;
    uint32_t mod_end;
    uint32_t string;
    uint32_t reserved;
} __attribute__((packed)) GrubModule;


typedef struct {
    char     signature[4];      // 'VESA'로 시작해야 함
    uint16_t version;
    uint32_t oem_string_ptr;
    uint32_t capabilities;
    uint32_t video_modes_ptr;
    uint16_t total_memory;      // 64KB 단위
    // ... 기타 생략 (OEM-specific, optional fields)
} __attribute__((packed)) VbeInfo;


typedef struct {
    uint16_t attributes;
    uint8_t  winA, winB;
    uint16_t granularity;
    uint16_t winsize;
    uint16_t segmentA, segmentB;
    uint32_t realFctPtr;
    uint16_t pitch;

    uint16_t x_resolution;
    uint16_t y_resolution;
    uint8_t  x_char_size;
    uint8_t  y_char_size;
    uint8_t  planes;
    uint8_t  bpp;
    uint8_t  banks;
    uint8_t  memory_model;
    uint8_t  bank_size;
    uint8_t  image_pages;
    uint8_t  reserved0;

    uint8_t  red_mask_size;
    uint8_t  red_field_position;
    uint8_t  green_mask_size;
    uint8_t  green_field_position;
    uint8_t  blue_mask_size;
    uint8_t  blue_field_position;
    uint8_t  reserved_mask_size;
    uint8_t  reserved_field_position;
    uint8_t  direct_color_attributes;

    uint32_t framebuffer;
    uint32_t offscreen_mem_offset;
    uint16_t offscreen_mem_size;

    uint8_t  reserved[206];
} __attribute__((packed, aligned(1))) VbeModeInfo;


// 메인 정보 구조체
typedef struct GrubBootInfo {
    uint32_t flags;

    // flags[0] set
    uint32_t mem_lower;
    uint32_t mem_upper;

    // flags[1] set
    uint32_t boot_device;

    // flags[2] set
    uint32_t cmdline;

    // flags[3] set
    uint32_t mods_count;
    GrubModule* modules;

    // flags[4] or flags[5] set
    uint32_t syms[4];  // a.out or ELF section header

    // flags[6] set
    uint32_t mmap_length;
    uint32_t mmap_addr;

    // flags[7] set
    uint32_t drives_length;
    uint32_t drives_addr;

    // flags[8] set
    uint32_t config_table;

    // flags[9] set
    uint32_t boot_loader_name;

    // flags[10] set
    uint32_t apm_table;

    // flags[11] set
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;
} __attribute__((packed, aligned(1))) GrubBootInfo;
