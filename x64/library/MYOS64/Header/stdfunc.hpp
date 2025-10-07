#pragma once
#include "defines.hpp"

void memcpy(uint8 * source, uint8 * copoied, uint32 size);
char* num_to_str(uint32 num);
void memset(uint8* start, uint8 val, uint32 size);
extern "C" void _outp(uint16 port, uint32 data);
extern "C" uint8 _inp(uint16 port);
extern "C" void _cli();
extern "C" void _sti();
extern "C" void gdt_load(uint32 gdt_reg);
uint64 align_size(uint64 size, uint64 aligner); // aligner 는 2의 n 제곱이어야만 합니다!         aligner should be 2 ^ n !!

int64 max(int64 a, int64 b);
int64 min(int64 a, int64 b);