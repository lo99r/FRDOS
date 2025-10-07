#include <MYOS>

namespace myos {

    const char shiftedCharTable[256] = {
        '\0','\x01','\x02','\x03','\x04','\x05','\x06','\x07',
        '\x08','\x09','\x0A','\x0B','\x0C','\x0D','\x0E','\x0F',
        '\x10','\x11','\x12','\x13','\x14','\x15','\x16','\x17',
        '\x18','\x19','\x1A','\x1B','\x1C','\x1D','\x1E','\x1F',

        ' ', '!', '\"', '#', '$', '%', '&', '\'',
        '(', ')', '*', '+', '<', '_', '>', '?',

        ')', '!', '@', '#', '$', '%', '^', '&', '*', '(',

        ':', ':', '<', '+', '>', '?', '@',

        'A','B','C','D','E','F','G','H','I','J',
        'K','L','M','N','O','P','Q','R','S','T',
        'U','V','W','X','Y','Z',

        '{', '|', '}', '~', '\x7F', '~',

        'A','B','C','D','E','F','G','H','I','J',
        'K','L','M','N','O','P','Q','R','S','T',
        'U','V','W','X','Y','Z',

        '{', '|', '}', '~', '\x7F',


        '\x80','\x81','\x82','\x83','\x84','\x85','\x86','\x87',
        '\x88','\x89','\x8A','\x8B','\x8C','\x8D','\x8E','\x8F',
        '\x90','\x91','\x92','\x93','\x94','\x95','\x96','\x97',
        '\x98','\x99','\x9A','\x9B','\x9C','\x9D','\x9E','\x9F',
        '\xA0','\xA1','\xA2','\xA3','\xA4','\xA5','\xA6','\xA7',
        '\xA8','\xA9','\xAA','\xAB','\xAC','\xAD','\xAE','\xAF',
        '\xB0','\xB1','\xB2','\xB3','\xB4','\xB5','\xB6','\xB7',
        '\xB8','\xB9','\xBA','\xBB','\xBC','\xBD','\xBE','\xBF',
        '\xC0','\xC1','\xC2','\xC3','\xC4','\xC5','\xC6','\xC7',
        '\xC8','\xC9','\xCA','\xCB','\xCC','\xCD','\xCE','\xCF',
        '\xD0','\xD1','\xD2','\xD3','\xD4','\xD5','\xD6','\xD7',
        '\xD8','\xD9','\xDA','\xDB','\xDC','\xDD','\xDE','\xDF',
        '\xE0','\xE1','\xE2','\xE3','\xE4','\xE5','\xE6','\xE7',
        '\xE8','\xE9','\xEA','\xEB','\xEC','\xED','\xEE','\xEF',
        '\xF0','\xF1','\xF2','\xF3','\xF4','\xF5','\xF6','\xF7',
        '\xF8','\xF9','\xFA','\xFB','\xFC','\xFD','\xFE','\xFF'
    };


	void memoryCopy(uint8* source, uint8* copied, uint32 size) {
		for (int i = 0; i < size; i++) copied[i] = source[i];
	}

	void intToStr(uint64 num, char* str) {
		for (int i = 0; str[i] != '\0' && i <= 25; i++) str[i] = 0;

		if (num == 0) {
			str[0] = '0';
			return;
		}
		int i = -1;

		uint64 num2 = num;
		for (; num2 != 0; i++) num2 /= 10;

		while (num != 0) {
			str[i] = (num % 10) + '0';
			num /= 10;
			i--;
		}
	}

	uint64 alignSize(uint64 size, uint64 aligner) {
		return (size + aligner - 1) & ~(aligner - 1);
	}

	int64 max(int64 a, int64 b) {
		return a < b ? b : a;
	}

	int64 min(int64 a, int64 b) {
		return a < b ? a : b;
	}

	char shiftChar(char chr) {
        return shiftedCharTable[chr];
	}

}