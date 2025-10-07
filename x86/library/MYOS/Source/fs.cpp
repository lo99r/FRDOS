/*#include<MYOS>//\

/*namespace myos::fs {

    struct Disk {
        char name[16];       // ��ũ �̸�, ��: "root"
        uint8 type;          // 0:floppy,1:HDD,2:SSD,3:USB
        uint64 size;         // ��ũ ũ��
        bool mounted;        // ����Ʈ ����
        void* device;        // PCI_Device* �� ��ũ ����̹� ����
    };

    Disk* rootDisk = nullptr;
    Disk* currentDisk = nullptr;
    char currentPath[128] = "/"; // ���� ���͸�

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

    // ���� �������� ���ڿ� ������
    int tokenize(const char* str, char tokens[MAX_TOKENS][MAX_TOKEN_LEN]) {
        int tokenCount = 0;
        int i = 0, j = 0;

        while (*str) {
            if (*str == ' ' || *str == '\t') {
                if (j > 0) { // ��ū ��
                    tokens[tokenCount][j] = '\0';
                    tokenCount++;
                    j = 0;
                    if (tokenCount >= MAX_TOKENS) break;
                }
            }
            else {
                tokens[tokenCount][j++] = *str;
                if (j >= MAX_TOKEN_LEN - 1) j = MAX_TOKEN_LEN - 2; // �����÷� ����
            }
            str++;
        }

        if (j > 0 && tokenCount < MAX_TOKENS) {
            tokens[tokenCount][j] = '\0';
            tokenCount++;
        }

        return tokenCount;
    }

    // ��Ʈ ���丮
    File rootDirStorage;
    File* rootDir = &rootDirStorage;

    // ���� ���丮
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
        while (*dest) dest++; // dest ������ �̵�
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

    // ��Ʈ ��ũ ����Ʈ
    void mountRoot(Disk* disk) {
        rootDisk = disk;
        currentDisk = disk;
        strCopy(currentPath, "/");
        console::print("Root disk mounted: ");
        console::print(disk->name);
        console::print("\n");
    }

    // ���͸� �̵�
    void changeDirectory(const char* path) {
        if (path[0] == '/') {
            strCopy(currentPath, path); // ���� ���
        }
        else {
            strCat(currentPath, path); // ��� ���
        }
        console::print("Current dir: ");
        console::print(currentPath);
        console::print("\n");
    }

    // ���� ���͸� Ȯ��
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

    // �ܼ� ����: move = copy + rmdir/delete
    bool move(const char* srcName, const char* destName) {
        File* f = findFile(currentDir, srcName);
        if (!f) return false;
        strCopy(f->name, destName);
        return true;
    }

    // �ܼ� copy: �޸� ����
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
            console::print(f->name);  // �̸� ���
            if (f->isDirectory) {
                console::print("/"); // ���͸��� �ڿ� / ����
            }
            console::print("\n");
            f = f->next;
        }
    }

} // namespace myos::fs

//#include<MYOS>*/