#include<MYOS>
#include"Header/porter.hpp"

namespace myos::memory {

    // 간단한 힙 영역 (1MB)
    static uint8_t* heap_start = reinterpret_cast<uint8_t*>(0x0100000); // 1MB 이후부터 시작
    static uint8_t* heap_end = reinterpret_cast<uint8_t*>(0x0200000); // 2MB까지
    static uint8_t* current = heap_start;

    void* mallocc(size_t size) {
        // 16바이트 정렬
        size = (size + 15) & ~15;

        if (current + size > heap_end) {
            // 힙 메모리 부족
            return nullptr;
        }

        void* block = current;
        current += size;
        return block;
    }

    void freec(void* ptr) {
        // 이 간단한 버전에서는 free는 아무 일도 안 합니다.
        // (실제 해제 로직을 구현하려면 메모리 블록 메타데이터가 필요)
        (void)ptr;
    }

    void reset() {
        current = heap_start;
    }
}