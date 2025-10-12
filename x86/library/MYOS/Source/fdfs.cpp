//#include"Header/fdfs.hpp"
#include <MYOS> // console::print 사용
//#incl

using namespace myos;

//\
namespace myos::console {\
    static void print(const char* str);
    //static void printChar(char c);\
}
//\
a

#define fdfs_readBlock readBlock//ata_read_sector
#define fdfs_writeBlock writeBlock//ata_write_sector

namespace myos::fdfs {
    char currentPath[128] = "/";
    FileEntry rootDir[BLOCK_SIZE / sizeof(FileEntry)];
    uint32_t rootDirCount = 0;

    uint8_t* diskMem = reinterpret_cast<uint8_t*>(0x10000000); // 임의의 RAM 주소
    //myos::fdfs::disk = diskMem;

    FileEntry* currentDir = rootDir;
    uint32_t currentDirCount = 0;
    uint32_t FAT[MAX_BLOCKS];
    uint8_t disk[BLOCK_SIZE];
    
    // 디스크에서 1섹터(512B) 읽기
    void ata_read_sector(uint32_t lba, uint8_t* buffer) {
        // 드라이브 선택 (LBA mode)
        outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xE0 | ((lba >> 24) & 0x0F));
        outb(ATA_PRIMARY_IO + ATA_REG_SECCOUNT0, 1);
        outb(ATA_PRIMARY_IO + ATA_REG_LBA0, (uint8_t)lba);
        outb(ATA_PRIMARY_IO + ATA_REG_LBA1, (uint8_t)(lba >> 8));
        outb(ATA_PRIMARY_IO + ATA_REG_LBA2, (uint8_t)(lba >> 16));
        outb(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_READ_PIO);

        // 대기 (BSY 해제 + DRQ 설정)
        bool ata_wait_ready(uint32_t timeout_ms) {
            uint32_t t = 0;
            while (inb(ATA_PRIMARY_IO + ATA_REG_STATUS) & ATA_SR_BSY) {
                if (t++ > timeout_ms * 1000) return false; // 타임아웃
            }
            t = 0;
            while (!(inb(ATA_PRIMARY_IO + ATA_REG_STATUS) & ATA_SR_DRQ)) {
                if (t++ > timeout_ms * 1000) return false;
            }
            return true;
        }

        // 512바이트 읽기
        for (int i = 0; i < 256; i++) {
            uint16_t data = inw(ATA_PRIMARY_IO + ATA_REG_DATA);
            buffer[i * 2] = (uint8_t)data;
            buffer[i * 2 + 1] = (uint8_t)(data >> 8);
        }
    }

    // 디스크에 1섹터(512B) 쓰기
    void ata_write_sector(uint32_t lba, const uint8_t* buffer) {
        outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xE0 | ((lba >> 24) & 0x0F));
        outb(ATA_PRIMARY_IO + ATA_REG_SECCOUNT0, 1);
        outb(ATA_PRIMARY_IO + ATA_REG_LBA0, (uint8_t)lba);
        outb(ATA_PRIMARY_IO + ATA_REG_LBA1, (uint8_t)(lba >> 8));
        outb(ATA_PRIMARY_IO + ATA_REG_LBA2, (uint8_t)(lba >> 16));
        outb(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_WRITE_PIO);

        while (inb(ATA_PRIMARY_IO + ATA_REG_STATUS) & ATA_SR_BSY);
        while (!(inb(ATA_PRIMARY_IO + ATA_REG_STATUS) & ATA_SR_DRQ));

        for (int i = 0; i < 256; i++) {
            uint16_t data = buffer[i * 2] | (buffer[i * 2 + 1] << 8);
            outw(ATA_PRIMARY_IO + ATA_REG_DATA, data);
        }

        outb(ATA_PRIMARY_IO + ATA_REG_COMMAND, 0xE7); // flush
    }

    // 현재 디렉터리 변경
    bool cd(const char* name) {
        if (strEquals(name, "/")) {
            currentDir = rootDir;
            currentDirCount = rootDirCount;
            return true;
        }

        FileEntry* f = nullptr;
        for (uint32_t i = 0; i < currentDirCount; i++) {
            if (strEquals(currentDir[i].name, name) && currentDir[i].isDirectory) {
                f = &currentDir[i];
                break;
            }
        }

        if (!f) return false;

        // 하위 디렉터리 이동: currentDir 포인터와 count 변경
        currentDir = &(*f);        // 현재는 단일 배열 기반, 필요 시 구현 조정
        currentDirCount = 0;        // 하위 디렉터리 카운트 초기화
        return true;
    }
    //
    const char* getCurrentPath() {
        return currentPath;
    }
    int tokenize(const char* str, char tokens[MAX_TOKENS][MAX_TOKEN_LEN]) {
        int tokenCount = 0;
        int i = 0, j = 0;

        while (*str) {
            if (*str == ' ' || *str == '\t') {
                if (j > 0) { // 토큰 끝
                    tokens[tokenCount][j] = '\0';
                    tokenCount++;
                    j = 0;
                    if (tokenCount >= MAX_TOKENS) break;
                }
            }
            else {
                tokens[tokenCount][j++] = *str;
                if (j >= MAX_TOKEN_LEN - 1) j = MAX_TOKEN_LEN - 2; // 오버플로 방지
            }
            str++;
        }

        if (j > 0 && tokenCount < MAX_TOKENS) {
            tokens[tokenCount][j] = '\0';
            tokenCount++;
        }

        return tokenCount;
    }

    //

    //

    //

    //

    // 문자열 길이
    uint32_t strLen(const char* s) {
        uint32_t len = 0;
        while (s[len] != '\0') len++;
        return len;
    }

    // 문자열 비교
    bool strEquals(const char* a, const char* b) {
        uint32_t i = 0;
        while (a[i] && b[i]) {
            if (a[i] != b[i]) return false;
            i++;
        }
        return a[i] == b[i];
    }

    // 문자열 복사
    void strCopy(char* dest, const char* src) {
        while (*src) {
            *dest++ = *src++;
        }
        *dest = 0;
    }

    // 메모리 복사
    void memCopy(uint8_t* dest, const uint8_t* src, uint32_t size) {
        for (uint32_t i = 0; i < size; i++) dest[i] = src[i];
    }

    void init() {
        for (uint32_t i = 0; i < MAX_BLOCKS; i++) FAT[i] = 0;
        for (uint32_t i = 0; i < ROOT_BLOCKS; i++) {
            for (uint32_t j = 0; j < BLOCK_SIZE; j++) {
                fdfs_readBlock(i, disk);
                disk[j] = 0;
            }
        }
        rootDirCount = 0;
        currentDir = rootDir;
        currentDirCount = 0;
    }

    int allocateBlock() {
        for (uint32_t i = ROOT_BLOCKS; i < MAX_BLOCKS; i++) {
            if (FAT[i] == 0) {
                FAT[i] = END_BLOCK;
                return i;
            }
        }
        return -1;
    }

    void freeBlocks(uint32_t startBlock) {
        uint32_t b = startBlock;
        while (b != END_BLOCK) {
            uint32_t next = FAT[b];
            FAT[b] = 0;
            b = next;
        }
    }

    FileEntry* findFile(const char* name) {
        for (uint32_t i = 0; i < currentDirCount; i++) {
            if (strEquals(currentDir[i].name, name)) return &currentDir[i];
        }
        return nullptr;
    }

    bool writeFile(const char* name, const uint8_t* data, uint32_t size) {
        if (findFile(name)) return false;
        if (currentDirCount >= BLOCK_SIZE / sizeof(FileEntry)) return false;

        FileEntry& f = currentDir[currentDirCount++];
        strCopy(f.name, name);
        f.isDirectory = false;
        f.size = size;

        uint32_t neededBlocks = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
        int prevBlock = -1;

        for (uint32_t i = 0; i < neededBlocks; i++) {
            int b = allocateBlock();
            if (b == -1) return false;

            if (prevBlock != -1) FAT[prevBlock] = b;
            else f.firstBlock = b;
            prevBlock = b;

            uint32_t offset = i * BLOCK_SIZE;
            uint32_t copySize = (size - offset > BLOCK_SIZE) ? BLOCK_SIZE : (size - offset);
            // 버퍼 채우기
            for (uint32_t j = 0; j < copySize; j++)
                disk[j] = data[offset + j];

            // 남은 부분 0으로 채움 (섹터 크기보다 작을 때)
            for (uint32_t j = copySize; j < BLOCK_SIZE; j++)
                disk[j] = 0;

            // 실제 디스크에 기록
            ata_write_sector(b, disk);
        }

        return true;
    }

    bool cat(const char* name) {
        FileEntry* f = findFile(name);
        if (!f || f->isDirectory) return false;

        uint32_t b = f->firstBlock;
        uint32_t remaining = f->size;

        while (b != END_BLOCK && remaining > 0) {
            uint32_t chunk = (remaining > BLOCK_SIZE) ? BLOCK_SIZE : remaining;
            for (uint32_t i = 0; i < chunk; i++){
                fdfs_readBlock(b, disk);
                console::print(disk[i]); // console::printChar 사용
            }
            remaining -= chunk;
            b = FAT[b];
        }

        console::print("\n");
        return true;
    }

    void dir() {
        for (uint32_t i = 0; i < currentDirCount; i++) {
            console::print(currentDir[i].name);
            if (currentDir[i].isDirectory) console::print("/");
            console::print("\n");
        }
    }

    bool mkdir(const char* name) {
        if (findFile(name)) return false;
        if (currentDirCount >= BLOCK_SIZE / sizeof(FileEntry)) return false;

        FileEntry& f = currentDir[currentDirCount++];
        strCopy(f.name, name);
        f.isDirectory = true;
        f.firstBlock = allocateBlock();
        f.size = 0;
        return true;
    }

    bool rmdir(const char* name) {
        for (uint32_t i = 0; i < currentDirCount; i++) {
            if (strEquals(currentDir[i].name, name) && currentDir[i].isDirectory) {
                freeBlocks(currentDir[i].firstBlock);
                for (uint32_t j = i; j < currentDirCount - 1; j++)
                    currentDir[j] = currentDir[j + 1];
                currentDirCount--;
                return true;
            }
        }
        return false;
    }

    bool move(const char* srcName, const char* destName) {
        FileEntry* f = findFile(srcName);
        if (!f) return false;

        strCopy(f->name, destName); // 같은 디렉토리 내 이름 변경
        return true;
    }

    bool copy(const char* srcName, const char* destName) {
        FileEntry* src = findFile(srcName);
        if (!src) return false;

        uint8_t buffer[BLOCK_SIZE * 16]; // 최대 16 블록 임시
        uint32_t copied = 0;

        uint32_t b = src->firstBlock;
        while (b != END_BLOCK && copied < src->size) {
            uint32_t chunk = (src->size - copied > BLOCK_SIZE) ? BLOCK_SIZE : (src->size - copied);
            for (uint32_t i = 0; i < chunk; i++) {
                fdfs_readBlock(b, disk);
                buffer[copied + i] = disk[i];
            }
            copied += chunk;
            b = FAT[b];
        }

        return writeFile(destName, buffer, src->size);
    }

    // 열린 파일 테이블 (최대 16개)
    FILE* fileTable[16] = { nullptr };

    // 파일 열기
    FILE* fopen(const char* name, const char* modeStr) {
        FMode mode;
        if (modeStr[0] == 'r') mode = FMode::READ;
        else if (modeStr[0] == 'w') mode = FMode::WRITE;
        else if (modeStr[0] == 'a') mode = FMode::APPEND;
        else return nullptr;

        FileEntry* f = findFile(name);
        if (!f) {
            if (mode == FMode::READ) return nullptr; // 읽기 모드인데 파일 없음
            // 쓰기/추가 모드라면 새로 생성
            if (!writeFile(name, nullptr, 0)) return nullptr;
            f = findFile(name);
        }

        // 빈 자리 찾기
        for (int i = 0; i < 16; i++) {
            if (!fileTable[i]) {
                fileTable[i] = new FILE{ f, (mode == FMode::APPEND ? f->size : 0), mode };
                return fileTable[i];
            }
        }
        return nullptr; // 파일 테이블 가득참
    }

    // 파일 닫기
    int fclose(FILE* f) {
        for (int i = 0; i < 16; i++) {
            if (fileTable[i] == f) {
                delete fileTable[i];
                fileTable[i] = nullptr;
                return 0;
            }
        }
        return -1;
    }

    // 파일 읽기
    size_t fread(void* buf, size_t size, size_t count, FILE* f) {
        if (!f || f->mode != FMode::READ) return 0;
        uint8_t* out = (uint8_t*)buf;
        uint32_t total = size * count;
        uint32_t remaining = f->entry->size - f->pos;
        if (total > remaining) total = remaining;

        uint32_t copied = 0;
        uint32_t block = f->entry->firstBlock;
        uint32_t posInBlock = f->pos % BLOCK_SIZE;
        uint32_t skipBlocks = f->pos / BLOCK_SIZE;
        for (uint32_t i = 0; i < skipBlocks; i++) block = FAT[block];

        while (copied < total && block != END_BLOCK) {
            uint32_t chunk = (total - copied > BLOCK_SIZE - posInBlock) ? BLOCK_SIZE - posInBlock : total - copied;
            fdfs_readBlock(block, disk);
            memCopy(out + copied, disk + posInBlock, chunk);
            copied += chunk;
            posInBlock = 0;
            block = FAT[block];
        }

        f->pos += copied;
        return copied / size; // 읽은 아이템 수
    }

    // 파일 쓰기
    size_t fwrite(const void* buf, size_t size, size_t count, FILE* f) {
        if (!f || (f->mode != FMode::WRITE && f->mode != FMode::APPEND)) return 0;
        const uint8_t* in = (const uint8_t*)buf;
        uint32_t total = size * count;

        // 기존 파일 지우고 새로 쓰기
        if (f->mode == FMode::WRITE) {
            freeBlocks(f->entry->firstBlock);
            f->entry->size = 0;
            f->entry->firstBlock = END_BLOCK;
            f->pos = 0;
        }

        uint32_t prevBlock = END_BLOCK;
        uint32_t posInBlock = f->pos % BLOCK_SIZE;
        uint32_t block = f->entry->firstBlock;

        if (block == END_BLOCK && total > 0) {
            block = allocateBlock();
            f->entry->firstBlock = block;
        }

        uint32_t written = 0;
        while (written < total) {
            if (block == END_BLOCK) {
                block = allocateBlock();
                FAT[prevBlock] = block;
            }
            uint32_t chunk = (total - written > BLOCK_SIZE - posInBlock) ? BLOCK_SIZE - posInBlock : total - written;
            fdfs_readBlock(block, disk);
            memCopy(disk + posInBlock, in + written, chunk);
            written += chunk;
            posInBlock = 0;
            prevBlock = block;
            block = FAT[block];
        }

        f->pos += written;
        f->entry->size = (f->pos > f->entry->size) ? f->pos : f->entry->size;
        return written / size;
    }

    // 파일 위치 이동
    int fseek(FILE* f, long offset, int whence) {
        if (!f) return -1;
        uint32_t newPos = 0;
        if (whence == 0) newPos = offset; // SEEK_SET
        else if (whence == 1) newPos = f->pos + offset; // SEEK_CUR
        else if (whence == 2) newPos = f->entry->size + offset; // SEEK_END
        else return -1;

        if (newPos > f->entry->size) return -1;
        f->pos = newPos;
        return 0;
    }

    // FAT 블록 번호를 실제 디스크 LBA로 변환
    uint32_t blockToLBA(uint32_t block) {
        return ROOT_BLOCKS + block; // ROOT_BLOCKS 이후부터 데이터 영역
    }

    // 실제 섹터 읽기
    void readBlock(uint32_t blockNum, uint8_t* buffer) {
        uint32_t lba = blockToLBA(blockNum);
        ata_read_sector(lba, buffer);
    }

    // 실제 섹터 쓰기
    void writeBlock(uint32_t blockNum, const uint8_t* buffer) {
        uint32_t lba = blockToLBA(blockNum);
        ata_write_sector(lba, buffer);
    }

    bool writeFileFixed(const char* name, const uint8_t* data, uint32_t size) {
        if (findFile(name)) return false;
        if (currentDirCount >= BLOCK_SIZE / sizeof(FileEntry)) return false;

        FileEntry& f = currentDir[currentDirCount++];
        strCopy(f.name, name);
        f.isDirectory = false;
        f.size = size;

        uint32_t neededBlocks = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
        int prevBlock = -1;

        for (uint32_t i = 0; i < neededBlocks; i++) {
            int b = allocateBlock();
            if (b == -1) return false;

            if (prevBlock != -1) FAT[prevBlock] = b;
            else f.firstBlock = b;
            prevBlock = b;

            uint32_t offset = i * BLOCK_SIZE;
            uint32_t copySize = (size - offset > BLOCK_SIZE) ? BLOCK_SIZE : (size - offset);

            for (uint32_t j = 0; j < copySize; j++)
                disk[j] = data[offset + j];

            for (uint32_t j = copySize; j < BLOCK_SIZE; j++)
                disk[j] = 0;

            writeBlock(b, disk); // 수정: block → 실제 LBA
        }

        return true;
    }

    bool catFixed(const char* name) {
        FileEntry* f = findFile(name);
        if (!f || f->isDirectory) return false;

        uint32_t b = f->firstBlock;
        uint32_t remaining = f->size;

        while (b != END_BLOCK && remaining > 0) {
            uint32_t chunk = (remaining > BLOCK_SIZE) ? BLOCK_SIZE : remaining;
            readBlock(b, disk); // 수정: block → 실제 LBA
            for (uint32_t i = 0; i < chunk; i++)
                console::print(disk[i]); // console::printChar 사용
            remaining -= chunk;
            b = FAT[b];
        }

        console::print("\n");
        return true;
    }

} // namespace fdfs
