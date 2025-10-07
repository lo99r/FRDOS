#pragma once
//#include"Header/"
#include <MYOS>

namespace myos::fs {

	constexpr int MAX_TOKENS = 8;
	constexpr int MAX_TOKEN_LEN = 32;

	// ���� �������� ���ڿ� ������
	int tokenize(const char* str, char tokens[MAX_TOKENS][MAX_TOKEN_LEN]);

	/*struct Dir {
		char name[32];
		Dir* parent;       // ���� ���丮
		Dir* subdirs[16];  // ���� ���丮 ������
		int subdirCount = 0;
		File* files[32];   // ���� ������
		int fileCount = 0;
	};*/

	

	//struct Disk { /* ... */ };

	struct Disk {
		char name[16];
		uint8 type;   // 0:floppy, 1:HDD, 2:SSD, 3:USB
		uint64 size;
		bool mounted;
		void* device;
	};

	struct File {
		char name[32];
		bool isDirectory;
		uint64 size;
		uint8* data;       // ���� ����
		File* parent;
		File* children;    // ���͸��̸� �ڽ� ���
		File* next;        // ���� ���
	};
	extern File* rootDir;
	extern File* currentDir;

	extern File* root;
	// ���� ����
	extern Disk* rootDisk;
	extern Disk* currentDisk;
	extern char currentPath[128];

	// ���ڿ� �Լ�
	int strLen(const char* str);
	void strCopy(char* dest, const char* src);
	void strCat(char* dest, const char* src);
	//
	bool strEquals(const char* a, const char* b);

	// ��ũ/��� ����
	void mountRoot(Disk* disk);
	void changeDirectory(const char* path);
	const char* getCurrentPath();

	// ���丮/���� ���
	File* findFile(File* dir, const char* name);
	bool mkdir(const char* name);
	bool rmdir(const char* name);
	bool copy(const char* srcName, const char* destName);
	bool move(const char* srcName, const char* destName);
	bool cat(const char* name);

	void listDirectory();

} // namespace myos::fs
