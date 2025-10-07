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
        char name[16];       // ��ũ �̸�, ��: "root"
        uint8 type;          // 0:floppy,1:HDD,2:SSD,3:USB
        uint64 size;         // ��ũ ũ��
        bool mounted;        // ����Ʈ ����
        void* device;        // PCI_Device* �� ��ũ ����̹� ����
    };

    Disk* rootDisk = nullptr;
    Disk* currentDisk = nullptr;
    char currentPath[128] = "/"; // ���� ���͸�

    //*/