#include <MYOS64>

void Keyboard::Enable_Interrupt() {
	outb(0x21, inb(0x21) & 0xfd);
	outb(0x64, 0x20);
	uint8 readed = inb(0x60);
	readed |= 1;
	outb(0x64, 0x60);
	outb(0x60, readed);
	asm("sti");
}

void Keyboard::Disable_Interrupt() {
	outb(0x64, 0x20);
	uint8 readed = inb(0x60);
	readed |= ~1;
	outb(0x64, 0x60);
	outb(0x60, readed);
}

Key Keyboard::Readkey() {
	Key key = Key(inb(0x60));
	return key;
}

Key::Key(Scancode _scancode) {
	scancode = _scancode;
}

Key::Key(uint8 _scancode) {
	scancode = (Scancode)_scancode;
}

Key::Key() {
	scancode = (Scancode)0;
}

char Key::chr() {
	return scancode_to_ascii[(uint8)scancode & 0b01111111];
}
Scancode Key::scode() {
	return (Scancode)((uint8)scancode & 0b01111111);
}

bool Key::Pressed() {
	return ((uint8)scancode & 0b10000000) == 0;
}