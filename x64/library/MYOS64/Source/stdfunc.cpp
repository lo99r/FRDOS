#include <MYOS64>

void memcpy(uint8* source, uint8* copied, uint32 size) {
	for (int i = 0; i < size; i++) copied[i] = source[i];
}

char str[100];
char str2[100];

char* num_to_str(uint32 num) {
	for (int i = 0; i < 100; i++) str[i] = 0;
	if (num == 0) {
		str[0] = '0';
		return str;
	}
	int i = 0;
	while (num != 0) {
		str[i] = (num % 10) + '0';
		num /= 10;
		i++;
	}
	i--;
	for (int j = 0; j < 100; j++) str2[j] = 0;
	int length = i;
	for (; i >= 0; i--) str2[length - i] = str[i];
	return str2;
}

uint64 align_size(uint64 size, uint64 aligner) {
	return (size + aligner - 1) & ~(aligner - 1);
}

int64 max(int64 a, int64 b) {
	return a < b ? b : a;
}

int64 min(int64 a, int64 b) {
	return a < b ? a : b;
}
