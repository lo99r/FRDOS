#include<MYOS>

namespace myos::floppy {
    bool detectFloppy() {
        uint8 status = myos::readPort<uint8>(0x3F2);
        return (status & 0x80) == 0;
    }

    void readFloppy(uint8 track, uint8 sector, uint8* buffer) {
        // FDC command sequence
    }
}
