#pragma once
//#include<cstddfs>//}//s>
//]
#include <MYOS>
#ifdef __INTELLISENSE__
// IDE용 size_t 가짜 정의
typedef unsigned int size_t;
#endif
/*namespace myos {
    void* operator new(unsigned int size) {
        return memory::malloc(size); // 커널에서 구현한 malloc 사용
    }

    void operator delete(void* ptr, unsigned int size) noexcept {
        memory::free(ptr); // 커널에서 구현한 free 사용
    }

    void* operator new[](unsigned int size) {
        return operator new(size);
    }

    void operator delete[](void* ptr) noexcept {
        operator delete(ptr, 0);
    }
}* /
// size_t 직접 정의 (보통 unsigned int 또는 unsigned long 사용)
using size_t = unsigned int;
void* operator new(size_t size) {
    return memory::malloc(size); // 커널에서 구현한 malloc 사용
}

size_t operator delete(void* ptr, size_t size) noexcept {
    memory::free(ptr); // 커널에서 구현한 free 사용
}

void* operator new[](size_t size) {
    return operator new(size);
}

size_t operator delete[](void* ptr) noexcept {
    operator delete(ptr, 0);
}
*/
// size_t 직접 정의 (보통 unsigned int 또는 unsigned long 사용)
//,using size_t = unsigned int;
//typesef ;
#ifndef __cplusplus
#define __cplusplus 1
#endif

#ifndef _SIZE_T_DEFINED
typedef unsigned int size_t;
#define _SIZE_T_DEFINED
#endif
//using namespace std;
//1
namespace stdt{
    typedef unsigned long long size_t;
}
//typedef unsigned long long size_t;
//using namespace myos::memory;
// malloc/free는 커널에서 직접 구현한 함수 사용
namespace myos::memory{
    extern uint8_t* heap_start; // 1MB 이후부터 시작
    extern uint8_t* heap_end; // 2MB까지
    extern uint8_t* current;
    //.exe..
    extern void* mallocc(size_t size);

    extern void freec(void* ptr);
    extern void reset();
}
//#define _SIZE_T_DEFINED
#ifndef _SIZE_T_DEFINED
    using size_t = unsigned long long;
#define _SIZE_T_DEFINED
#endif

    //\
    typedef unsigned int size_t;
// 전역 new/delete
    inline void* operator new(unsigned long long size) noexcept {
        return myos::memory::mallocc(size);
    }

    inline void operator delete(void* ptr) noexcept {
        myos::memory::freec(ptr);
    }

    // 크기 정보를 받는 delete
    inline void operator delete(void* ptr, unsigned int) noexcept {
        myos::memory::freec(ptr);
    }

    inline void* operator new[](unsigned long long size) noexcept {
        return operator new(size);
    }

    inline void operator delete[](void* ptr) noexcept {
        operator delete(ptr);
    }

    inline void operator delete[](void* ptr, unsigned int size) noexcept {
        operator delete(ptr, size);
    }