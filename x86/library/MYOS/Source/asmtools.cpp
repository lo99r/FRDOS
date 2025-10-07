#include <MYOS>

namespace myos {

    inline uint64_t readMSR(uint32_t msr) {
        uint32_t low, high;
        asm volatile (
            "rdmsr"
            : "=a"(low), "=d"(high)
            : "c"(msr)
            );
        return ((uint64_t)high << 32) | low;
    }

    inline void writeMSR(uint32_t msr, uint64_t value) {
        uint32_t low = (uint32_t)(value & 0xFFFFFFFF);
        uint32_t high = (uint32_t)(value >> 32);
        asm volatile (
            "wrmsr"
            :
        : "c"(msr), "a"(low), "d"(high)
            );
    }

}