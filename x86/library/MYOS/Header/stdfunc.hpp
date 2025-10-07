#pragma once
#include "defines.hpp"

namespace myos {

	void memoryCopy(uint8* source, uint8* copied, uint32 size);
	void intToStr(uint64 num, char* str);
	uint64 alignSize(uint64 size, uint64 aligner); // aligner �� 2�� n �����̾�߸� �մϴ�!         aligner should be 2 ^ n !!
	char shiftChar(char chr);
	int64 max(int64 a, int64 b);
	int64 min(int64 a, int64 b);

}