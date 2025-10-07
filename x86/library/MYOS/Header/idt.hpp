#pragma once
#include "asmtools.hpp"

#pragma region "IDT vector"

const int IDT_zero_division = 0;  // 0으로 나누기 오류
const int IDT_debug_exception = 1;  // Debug Exception
const int IDT_nmi_interrupt = 2;  // Non-Maskable Interrupt
const int IDT_breakpoint = 3;  // Breakpoint
const int IDT_overflow = 4;  // Overflow
const int IDT_bound_range_exceeded = 5;  // BOUND Range Exceeded
const int IDT_invalid_opcode = 6;  // Invalid Opcode
const int IDT_device_not_available = 7;  // Device Not Available (No Math Coprocessor)
const int IDT_double_fault = 8;  // Double Fault
const int IDT_coprocessor_segment_overrun = 9;  // Coprocessor Segment Overrun (reserved, legacy)
const int IDT_invalid_tss = 10; // Invalid TSS
const int IDT_segment_not_present = 11; // Segment Not Present
const int IDT_stack_segment_fault = 12; // Stack-Segment Fault
const int IDT_general_protection = 13; // General Protection Fault
const int IDT_page_fault = 14; // Page Fault
const int IDT_reserved_15 = 15; // Reserved
const int IDT_floating_point_error = 16; // x87 Floating-Point Exception
const int IDT_alignment_check = 17; // Alignment Check
const int IDT_machine_check = 18; // Machine Check
const int IDT_simd_floating_point = 19; // SIMD Floating-Point Exception
const int IDT_virtualization = 20; // Virtualization Exception
const int IDT_control_protection = 21; // Control Protection Exception (CET-related)

const int IDT_hypervisor_injection = 29; // Hypervisor Injection Exception
const int IDT_vmm_communication = 30; // VMM Communication Exception
const int IDT_security_exception = 31; // Security Exception

#pragma endregion

#define start_handler()\
		asm_intel( \
			"pushf\n" \
			"cli\n" \
			"pushad\n" \
		) \

#define end_handler() \
		myos::sendPort<uint8>(0x20, 0x20); \
		asm_intel( \
			"popad\n" \
			"popf\n" \
			"iretd\n" \
		) \

namespace myos {

	void emptyFunction();

	enum class IDT_Type {
		INTERRUPT = 0xE,
		TRAP = 0xF,
	};

	class InterruptHandler {

	public:
		uint16 code_segment;
		IDT_Type table_type;
		Permission privilege_level;
		uint32 tmp[2];
		int error_code;

		uint32* legacy_idt;
		void* legacy_handler;
		void* handler;

		void update() {
			int idx1 = 0;
			int idx2 = 1;
			legacy_idt[idx1] = legacy_idt[idx2] = 0;
			legacy_idt[idx1] |= (uint32)legacy_handler & 0xFFFF;
			legacy_idt[idx1] |= (uint32)(code_segment) << 16;
			legacy_idt[idx2] |= (uint32)(table_type) << 8;
			legacy_idt[idx2] |= (uint32)(privilege_level) << 13;
			legacy_idt[idx2] |= 1 << 15;
			legacy_idt[idx2] |= (uint32)legacy_handler & 0xFFFF0000;
		}

		InterruptHandler::InterruptHandler(void (*Handler)() = emptyFunction, IDT_Type Table_type = IDT_Type::INTERRUPT, Permission Privilege_level = Permission::KERNEL, uint16 Code_segment = 0x08) {
			handler = Handler;
			code_segment = Code_segment;
			table_type = Table_type;
			privilege_level = Privilege_level;
			legacy_idt = tmp;

			update();
		}

		InterruptHandler operator=(InterruptHandler source) {
			handler = source.handler;
			code_segment = source.code_segment;
			table_type = source.table_type;
			privilege_level = source.privilege_level;

			update();

			return *this;
		}
	};

	class IDT {

	private:

		struct __attribute__((packed)) idtr {
			uint16 size;
			uint32 addr;
		};

		idtr idtreg;
		InterruptHandler IDT_arr[256];
		uint32 legacy_idt[512];

	public:

		InterruptHandler& operator[](uint32 index) {
			if (index >= 256) return *(InterruptHandler*)0;
			return IDT_arr[index];
		}

		void setHandler(int IDT_vector, void (*handler)());
		void initialize();
		void load();
		IDT();
	};
}