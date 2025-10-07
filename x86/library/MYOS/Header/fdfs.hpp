#pragma once
#include<MYOS>

////name

//namespace console {\
    static void print(const char* str);\
    static void printChar(char c);\
}
//

//#include "Header/fs.hpp"
#define MAX_TOKENS 8
#define MAX_TOKEN_LEN 32
namespace myos::fdfs {
    extern uint8_t* diskMem; // 임의의 RAM 주소
    bool cd(const char* name);
    extern char currentPath[128];
    const char* getCurrentPath();
    int tokenize(const char* str, char tokens[MAX_TOKENS][MAX_TOKEN_LEN]);

    constexpr uint32_t BLOCK_SIZE = 512;
    constexpr uint32_t MAX_BLOCKS = 65536;
    constexpr uint32_t ROOT_BLOCKS = 32;
    constexpr uint32_t END_BLOCK = 0xFFFFFFFF;

    extern uint32_t FAT[MAX_BLOCKS];
    extern uint8_t disk[MAX_BLOCKS][BLOCK_SIZE];

    struct FileEntry {
        char name[32];
        bool isDirectory;
        uint32_t firstBlock;
        uint32_t size;
    };

    extern FileEntry rootDir[BLOCK_SIZE / sizeof(FileEntry)];
    extern uint32_t rootDirCount;

    extern FileEntry* currentDir;
    extern uint32_t currentDirCount;

    // 문자열 길이
    uint32_t strLen(const char* s);
    //}

    // 문자열 비교
    bool strEquals(const char* a, const char* b);

    // 문자열 복사
    void strCopy(char* dest, const char* src);

    // 메모리 복사
    void memCopy(uint8_t* dest, const uint8_t* src, uint32_t size);

    void init();

    int allocateBlock();

    void freeBlocks(uint32_t startBlock);

    FileEntry* findFile(const char* name);

    bool mkdir(const char* name);

    bool rmdir(const char* name);

    bool writeFile(const char* name, const uint8_t* data, uint32_t size);

    bool cat(const char* name);

    void dir();
    bool move(const char* srcName, const char* destName);
    bool copy(const char* srcName, const char* destName);

    enum class FMode {
        READ,
        WRITE,
        APPEND
    };

    struct FILE {
        FileEntry* entry;
        uint32_t pos;
        FMode mode;
    };

    // 열린 파일 테이블 (최대 16개)
    extern FILE* fileTable[16];

    // 파일 열기
    FILE* fopen(const char* name, const char* modeStr);

    // 파일 닫기
    int fclose(FILE* f);

    // 파일 읽기
    size_t fread(void* buf, size_t size, size_t count, FILE* f);

    // 파일 쓰기
    size_t fwrite(const void* buf, size_t size, size_t count, FILE* f);

    // 파일 위치 이동
    int fseek(FILE* f, long offset, int whence);

} // namespace fdfs
