#pragma once
#include <MYOS>


// Master PIC (IRQ 0~7)
#define IDT_timer         PIC::IDT_offset1 + 0   // IRQ0
#define IDT_keyboard      PIC::IDT_offset1 + 1   // IRQ1
#define IDT_cascade       PIC::IDT_offset1 + 2   // IRQ2 - Cascade to slave PIC
#define IDT_com2          PIC::IDT_offset1 + 3   // IRQ3
#define IDT_com1          PIC::IDT_offset1 + 4   // IRQ4
#define IDT_lpt2          PIC::IDT_offset1 + 5   // IRQ5
#define IDT_floppy        PIC::IDT_offset1 + 6   // IRQ6
#define IDT_lpt1          PIC::IDT_offset1 + 7   // IRQ7

// Slave PIC (IRQ 8~15)
#define IDT_cmos_rtc      PIC::IDT_offset2 + 0   // IRQ8
#define IDT_periph1       PIC::IDT_offset2 + 1   // IRQ9
#define IDT_periph2       PIC::IDT_offset2 + 2   // IRQ10
#define IDT_periph3       PIC::IDT_offset2 + 3   // IRQ11
#define IDT_mouse         PIC::IDT_offset2 + 4   // IRQ12
#define IDT_fpu           PIC::IDT_offset2 + 5   // IRQ13
#define IDT_primary_ata   PIC::IDT_offset2 + 6   // IRQ14
#define IDT_secondary_ata PIC::IDT_offset2 + 7   // IRQ15

namespace myos::PIC {

	extern int IDT_offset1;
	extern int IDT_offset2;

	void initialize(int offset1 = 32, int offset2 = 40);
}