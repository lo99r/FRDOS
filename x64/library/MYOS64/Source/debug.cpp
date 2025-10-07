#include <MYOS64>

void dbg_break() {
	dd(0xcc);
	dd(0);
	dd(0);
	dd(0);
}