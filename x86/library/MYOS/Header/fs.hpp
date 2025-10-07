#pragma once
//#include"Header/"
#include <MYOS>

namespace myos::fs {

	constexpr int MAX_TOKENS = 8;
	constexpr int MAX_TOKEN_LEN = 32;

	// 공백 기준으로 문자열 나누기
	int tokenize(const char* str, char tokens[MAX_TOKENS][MAX_TOKEN_LEN]);

	/*struct Dir {
		char name[32];
		Dir* parent;       // 상위 디렉토리
		Dir* subdirs[16];  // 하위 디렉토리 포인터
		int subdirCount = 0;
		File* files[32];   // 파일 포인터
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
		uint8* data;       // 파일 내용
		File* parent;
		File* children;    // 디렉터리이면 자식 목록
		File* next;        // 형제 노드
	};
	extern File* rootDir;
	extern File* currentDir;

	extern File* root;
	// 전역 변수
	extern Disk* rootDisk;
	extern Disk* currentDisk;
	extern char currentPath[128];

	// 문자열 함수
	int strLen(const char* str);
	void strCopy(char* dest, const char* src);
	void strCat(char* dest, const char* src);
	//
	bool strEquals(const char* a, const char* b);

	// 디스크/경로 관련
	void mountRoot(Disk* disk);
	void changeDirectory(const char* path);
	const char* getCurrentPath();

	// 디렉토리/파일 명령
	File* findFile(File* dir, const char* name);
	bool mkdir(const char* name);
	bool rmdir(const char* name);
	bool copy(const char* srcName, const char* destName);
	bool move(const char* srcName, const char* destName);
	bool cat(const char* name);

	void listDirectory();

} // namespace myos::fs
