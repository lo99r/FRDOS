#pragma once
#include <MYOS>

namespace myos::pci {

    struct PCI_Device {
        uint8 bus;
        uint8 device;
        uint8 function;
        uint16 vendor_id;
        uint16 device_id;
        uint8 class_code;
        uint8 subclass;
        uint8 prog_if;
    };

    // PCI Config ÀÐ±â
    inline uint32 readConfig(uint8 bus, uint8 device, uint8 function, uint8 offset) {
        uint32 address = (1 << 31)            // enable bit
            | (bus << 16)
            | (device << 11)
            | (function << 8)
            | (offset & 0xFC);
        myos::sendPort<uint32>(0xCF8, address);
        return myos::readPort<uint32>(0xCFC);
    }

    // PCI Å½»ö
    PCI_Device* findUSBController();
    PCI_Device enumerateDevice(uint8 bus, uint8 device, uint8 function);

    void enumerateAllDevices();

}

