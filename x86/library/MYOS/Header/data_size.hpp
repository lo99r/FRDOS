#pragma once
#include "defines.hpp"

constexpr uint64 operator"" _B(unsigned long long value) {
    return value;
}

constexpr uint64 operator"" _KB(unsigned long long value) {
    return value * 1024ULL;
}

constexpr uint64 operator"" _MB(unsigned long long value) {
    return value * 1024ULL * 1024;
}

constexpr uint64 operator"" _GB(unsigned long long value) {
    return value * 1024ULL * 1024 * 1024;
}

constexpr uint64 operator"" _TB(unsigned long long value) {
    return value * 1024ULL * 1024 * 1024 * 1024;
}

constexpr uint64 operator"" _PB(unsigned long long value) {
    return value * 1024ULL * 1024 * 1024 * 1024 * 1024;
}