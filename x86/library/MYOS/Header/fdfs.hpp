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
    extern uint8_t* diskMem; // ������ RAM �ּ�
    bool cd(const char* name);
    extern char currentPath[128];
    const char* getCurrentPath();
    int tokenize(const char* str, char tokens[MAX_TOKENS][MAX_TOKEN_LEN]);

    constexpr uint32_t BLOCK_SIZE = 512;
    constexpr uint32_t MAX_BLOCKS = 65536;
    constexpr uint32_t ROOT_BLOCKS = 32;
    constexpr uint32_t END_BLOCK = 0xFFFFFFFF;

    extern uint32_t FAT[MAX_BLOCKS];
    extern uint8_t disk[BLOCK_SIZE];

#define ATA_PRIMARY_IO  0x1F0
#define ATA_PRIMARY_CTRL 0x3F6

#define ATA_REG_DATA      0x00
#define ATA_REG_ERROR     0x01
#define ATA_REG_SECCOUNT0 0x02
#define ATA_REG_LBA0      0x03
#define ATA_REG_LBA1      0x04
#define ATA_REG_LBA2      0x05
#define ATA_REG_HDDEVSEL  0x06
#define ATA_REG_COMMAND   0x07
#define ATA_REG_STATUS    0x07

#define ATA_CMD_READ_PIO  0x20
#define ATA_CMD_WRITE_PIO 0x30

#define ATA_SR_BSY  0x80
#define ATA_SR_DRDY 0x40
#define ATA_SR_DRQ  0x08

    // ��ũ���� 1����(512B) �б�
    void ata_read_sector(uint32_t lba, uint8_t* buffer);

    // ��ũ�� 1����(512B) ����
    void ata_write_sector(uint32_t lba, const uint8_t* buffer);

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

    // ���ڿ� ����
    uint32_t strLen(const char* s);
    //}

    // ���ڿ� ��
    bool strEquals(const char* a, const char* b);

    // ���ڿ� ����
    void strCopy(char* dest, const char* src);

    // �޸� ����
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

    // ���� ���� ���̺� (�ִ� 16��)
    extern FILE* fileTable[16];

    // ���� ����
    FILE* fopen(const char* name, const char* modeStr);

    // ���� �ݱ�
    int fclose(FILE* f);

    // ���� �б�
    size_t fread(void* buf, size_t size, size_t count, FILE* f);

    // ���� ����
    size_t fwrite(const void* buf, size_t size, size_t count, FILE* f);

    // ���� ��ġ �̵�
    int fseek(FILE* f, long offset, int whence);

} // namespace fdfs
