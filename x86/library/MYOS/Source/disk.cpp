#include<MYOS>
namespace myos::disk {

    bool detectHDD() {
        uint8 status = myos::readPort<uint8>(0x1F7); // status port
        return (status != 0xFF);
    }

    void readSector(uint32 lba, uint16* buffer) {
        // ATA read sector sequence using ports
    }



}

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

    //*/