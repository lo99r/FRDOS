#include<MYOS>
#include"Header/porter.hpp"

namespace myos::memory {

    // ������ �� ���� (1MB)
    static uint8_t* heap_start = reinterpret_cast<uint8_t*>(0x0100000); // 1MB ���ĺ��� ����
    static uint8_t* heap_end = reinterpret_cast<uint8_t*>(0x0200000); // 2MB����
    static uint8_t* current = heap_start;

    void* mallocc(size_t size) {
        // 16����Ʈ ����
        size = (size + 15) & ~15;

        if (current + size > heap_end) {
            // �� �޸� ����
            return nullptr;
        }

        void* block = current;
        current += size;
        return block;
    }

    void freec(void* ptr) {
        // �� ������ ���������� free�� �ƹ� �ϵ� �� �մϴ�.
        // (���� ���� ������ �����Ϸ��� �޸� ��� ��Ÿ�����Ͱ� �ʿ�)
        (void)ptr;
    }

    void reset() {
        current = heap_start;
    }
}