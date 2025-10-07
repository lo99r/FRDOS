#include <MYOS64>

#define IDT_HANDLER(idx) \
code IDT_handler_##idx() { \
	start_handler(); \
	((void (*)())((*current_idt)[idx].handler))(); \
    if (idx < 32) if(IDT_error_code[idx]) asmbly("add esp, 4"); \
	end_handler(); \
}

bool IDT_error_code[32] = {
	false,false,false,false,false,false,false,false,  // 0~7
	true, false,true,true,true,true,true,false,     // 8~15
	false,false,false,false,false,false,false,false, // 16~23
	false,false,false,false,false,false,false,false  // 24~31
};

void* handler_funcs[256];
IDT* current_idt;

//#include "idt_funcs.inc"

void IDT::Load() {
	current_idt = this;
	asm_att("mov %0, %%rax"::"r"(&idtreg) : "rax");
	asmbly("lidt [rax]");
}

void default_handler() {

}

void IDT::Initialize() {

//#include "idt_funcs2.inc"

	for (int i = 0; i < 256; i++) {
		IDT_arr[i].legacy_idt = &(legacy_idt[i * 2]);
		IDT_arr[i] = InterruptHandler();
	}

	idtreg.addr = (uint32)legacy_idt;
	idtreg.size = 512 * 4 - 1;
}

IDT::IDT() {
	for (int i = 0; i < 256; i++) {
		IDT_arr[i].legacy_idt = &(legacy_idt[i * 2]);
		IDT_arr[i] = InterruptHandler();
	}
	idtreg.addr = (uint32)legacy_idt;
	idtreg.size = 512 * 4 - 1;
}

