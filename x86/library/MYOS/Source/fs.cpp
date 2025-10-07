/*#include<MYOS>//\

/*namespace myos::fs {

    struct Disk {
        char name[16];       // 디스크 이름, 예: "root"
        uint8 type;          // 0:floppy,1:HDD,2:SSD,3:USB
        uint64 size;         // 디스크 크기
        bool mounted;        // 마운트 여부
        void* device;        // PCI_Device* 나 디스크 드라이버 연결
    };

    Disk* rootDisk = nullptr;
    Disk* currentDisk = nullptr;
    char currentPath[128] = "/"; // 현재 디렉터리

    void mountRoot(Disk* disk) {
        rootDisk = disk;
        currentDisk = disk;
        strCopy(currentPath, "/");
        console::print("Root disk mounted: ");
        console::print(disk->name);
        console::print("\n");
        return;
    }
}* /
using namespace myos::pci;
namespace myos::fs {

    //constexpr int MAX_TOKENS;
    //constexpr int MAX_TOKEN_LEN;

    // 공백 기준으로 문자열 나누기
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

    // 루트 디렉토리
    File rootDirStorage;
    File* rootDir = &rootDirStorage;

    // 현재 디렉토리
    File* currentDir = &rootDirStorage;

    inline int strLen(const char* str) {
        int len = 0;
        while (str[len] != '\0') len++;
        return len;
    }

    inline void strCopy(char* dest, const char* src) {
        while (*src) {
            *dest = *src;
            dest++;
            src++;
        }
        *dest = '\0';
    }

    inline void strCat(char* dest, const char* src) {
        while (*dest) dest++; // dest 끝으로 이동
        while (*src) {
            *dest = *src;
            dest++;
            src++;
        }
        *dest = '\0';
    }

    bool strEquals(const char* a, const char* b) {
        while (*a && *b) {
            if (*a != *b) return false;
            a++; b++;
        }
        return *a == *b;
    }

    extern File* root;
    extern File* currentDir;

    extern Disk* rootDisk = nullptr;
    extern Disk* currentDisk = nullptr;
    extern char currentPath[128] = "/";

    // 루트 디스크 마운트
    void mountRoot(Disk* disk) {
        rootDisk = disk;
        currentDisk = disk;
        strCopy(currentPath, "/");
        console::print("Root disk mounted: ");
        console::print(disk->name);
        console::print("\n");
    }

    // 디렉터리 이동
    void changeDirectory(const char* path) {
        if (path[0] == '/') {
            strCopy(currentPath, path); // 절대 경로
        }
        else {
            strCat(currentPath, path); // 상대 경로
        }
        console::print("Current dir: ");
        console::print(currentPath);
        console::print("\n");
    }

    // 현재 디렉터리 확인
    const char* getCurrentPath() {
        return currentPath;
    }

    File* findFile(File* dir, const char* name) {
        File* f = dir->children;
        while (f) {
            if (strEquals(f->name, name)) return f;
            f = f->next;
        }
        return nullptr;
    }

    bool mkdir(const char* name) {
        if (!currentDir || findFile(currentDir, name)) return false;

        File* f = new File{};
        strCopy(f->name, name);
        f->isDirectory = true;
        f->parent = currentDir;
        f->next = currentDir->children;
        currentDir->children = f;
        return true;
    }

    bool rmdir(const char* name) {
        if (!currentDir) return false;
        File* prev = nullptr;
        File* f = currentDir->children;
        while (f) {
            if (strEquals(f->name, name) && f->isDirectory) {
                if (prev) prev->next = f->next;
                else currentDir->children = f->next;
                delete f;
                return true;
            }
            prev = f;
            f = f->next;
        }
        return false;
    }

    bool cat(const char* name) {
        File* f = findFile(currentDir, name);
        if (!f || f->isDirectory) return false;

        for (uint64 i = 0; i < f->size; i++) {
            console::print(f->data[i]);
        }
        return true;
    }

    // 단순 구현: move = copy + rmdir/delete
    bool move(const char* srcName, const char* destName) {
        File* f = findFile(currentDir, srcName);
        if (!f) return false;
        strCopy(f->name, destName);
        return true;
    }

    // 단순 copy: 메모리 복사
    bool copy(const char* srcName, const char* destName) {
        File* f = findFile(currentDir, srcName);
        if (!f) return false;

        File* c = new File{};
        strCopy(c->name, destName);
        c->isDirectory = f->isDirectory;
        c->size = f->size;
        if (f->data) {
            c->data = new uint8[f->size];
            for (uint64 i = 0; i < f->size; i++) c->data[i] = f->data[i];
        }
        c->parent = currentDir;
        c->next = currentDir->children;
        currentDir->children = c;
        return true;
    }

    void listDirectory() {
        if (!currentDir) return;

        File* f = currentDir->children;
        while (f) {
            console::print(f->name);  // 이름 출력
            if (f->isDirectory) {
                console::print("/"); // 디렉터리면 뒤에 / 붙임
            }
            console::print("\n");
            f = f->next;
        }
    }

} // namespace myos::fs

//#include<MYOS>*/