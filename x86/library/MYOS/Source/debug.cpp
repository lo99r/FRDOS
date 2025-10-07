#include <MYOS>

namespace myos {

	void debugBreak() {
		dd(0xcc);
		dd(0);
		dd(0);
		dd(0);
	}
}