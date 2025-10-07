#include <MYOS>

namespace myos {

	const char scancode_to_ascii[128] = {
		0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b', // 0x00 - 0x0F
	   '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',    // 0x10 - 0x1D
		0,  'a','s','d','f','g','h','j','k','l',';','\'','`',        // 0x1E - 0x2B
		0,  '\\','z','x','c','v','b','n','m',',','.','/', 0,         // 0x2C - 0x38
		'*', 0,  ' ', 0,  0,  0,  0,  0,  0,  0,                      // 0x39 - 0x42
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,                        // 0x43 - 0x4C
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,                        // 0x4D - 0x56
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,                        // 0x57 - 0x60
		0,  0,  0,  0,  0,  0,  0,  0                                 // 0x61 - 0x68
		// 나머지 0x69~0x7F 도 대부분 확장 키 / F키로 0으로 유지
	};

	void Keyboard::enableInterrupt() {
		sendPort<uint8>(0x21, readPort<uint8>(0x21) & 0xfd);
		sendPort<uint8>(0x64, 0x20);

		uint8 readed = readPort<uint8>(0x60);

		readed |= 1;

		sendPort<uint8>(0x64, 0x60);
		sendPort<uint8>(0x60, readed);
		asm("sti");
	}

	void Keyboard::disableInterrupt() {
		sendPort<uint8>(0x64, 0x20);

		uint8 readed = readPort<uint8>(0x60);
		readed |= ~1;

		sendPort<uint8>(0x64, 0x60);
		sendPort<uint8>(0x60, readed);
	}

	Key Keyboard::readKey() {
		Key key = Key(readPort<uint8>(0x60));
		return key;
	}

	Key::Key(Scancode _sc) : __scancode(_sc) {}

	Key::Key(uint8 _sc) : Key((Scancode)_sc) {}

	Key::Key() : Key((Scancode)0) {}

	char Key::chr() {
		return scancode_to_ascii[(uint8)__scancode & 0b01111111];
	}
	Scancode Key::scancode() {
		return (Scancode)((uint8)__scancode & 0b01111111);
	}

	bool Key::isKeyDown() {
		return ((uint8)__scancode & 0b10000000) == 0;
	}

	bool Key::isKeyUp() {
		return ((uint8)__scancode & 0b10000000) == 1;
	}
}