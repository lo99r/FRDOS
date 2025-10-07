#include <MYOS>

#define IDT_HANDLER(idx) \
void_naked IDT_handler_##idx() { \
	start_handler(); \
	((void (*)())((*current_idt)[idx].handler))(); \
\
	if constexpr(idx < 32 && IDT_error_code[idx]) { asm_intel("add esp, 4"); } \
\
	end_handler(); \
}

constexpr bool IDT_error_code[32] = {
	false,false,false,false,false,false,false,false,  // 0~7
	true, false,true,true,true,true,true,false,     // 8~15
	false,false,false,false,false,false,false,false, // 16~23
	false,false,false,false,false,false,false,false  // 24~31
};

namespace myos {

	void* handler_funcs[256];
	IDT* current_idt;

#include "idt_funcs.inc"

	void IDT::load() {
		current_idt = this;
		asm_att("mov %0, %%eax"::"r"(&idtreg) : "eax");
		asm_intel("lidt [eax]");
	}

	void emptyFunction() {}

	void IDT::initialize() {

#include "idt_funcs2.inc"

		for (int i = 0; i < 256; i++) {
			IDT_arr[i].legacy_idt = &(legacy_idt[i * 2]);
			IDT_arr[i] = InterruptHandler();
		}

		idtreg.addr = (uint32)legacy_idt;
		idtreg.size = 512 * 4 - 1;
	}

	void IDT::setHandler(int IDT_vector, void (*handler)()) {
		(*this)[IDT_vector].handler = handler;
		(*this)[IDT_vector].update();
	}

	IDT::IDT() {
		for (int i = 0; i < 256; i++) {
			IDT_arr[i].legacy_idt = &(legacy_idt[i * 2]);
			IDT_arr[i] = InterruptHandler();
		}
		idtreg.addr = (uint32)legacy_idt;
		idtreg.size = 512 * 4 - 1;
	}
}