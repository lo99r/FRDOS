#include<MYOS>
namespace myos::pci {

    PCI_Device enumerateDevice(uint8 bus, uint8 device, uint8 function) {
        PCI_Device dev{};
        uint32 data = readConfig(bus, device, function, 0x00);
        dev.vendor_id = data & 0xFFFF;
        dev.device_id = (data >> 16) & 0xFFFF;
        // 클래스 코드
        data = readConfig(bus, device, function, 0x08);
        dev.class_code = (data >> 24) & 0xFF;
        dev.subclass = (data >> 16) & 0xFF;
        dev.prog_if = (data >> 8) & 0xFF;
        dev.bus = bus;
        dev.device = device;
        dev.function = function;
        return dev;
    }

    void enumerateAllDevices() {
        for (uint8 bus = 0; bus < 256; bus++) {
            for (uint8 dev = 0; dev < 32; dev++) {
                for (uint8 func = 0; func < 8; func++) {
                    PCI_Device d = enumerateDevice(bus, dev, func);
                    if (d.vendor_id != 0xFFFF) {
                        // Device found
                    }
                }
            }
        }
    }

    PCI_Device* findUSBController() {
        /*for (uint8_t bus = 0; bus < 256; bus++) {
            for (uint8_t dev = 0; dev < 32; dev++) {
                for (uint8_t func = 0; func < 8; func++) {
                    PCI_Device* device = enumerateDevice(bus, dev, func);
                    if (device->vendor_id != 0xFFFF && device->class_code == 0x0C && device->subclass == 0x03) {
                        return device; // USB controller found
                    }
                }
            }
        }
        return PCI_Device{};
        for (uint8_t bus = 0; bus < 256; bus++) {
            for (uint8_t dev = 0; dev < 32; dev++) {
                for (uint8_t func = 0; func < 8; func++) {
                    PCI_Device device = enumerateDevice(bus, dev, func);
                    if (device.vendor_id != 0xFFFF && device.class_code == 0x0C && device.subclass == 0x03) {
                        return device; // USB controller found
                    }
                }
            }
        }
        return PCI_Device{ .vendor_id = 0xFFFF }; // 발견 못하면 vendor_id=0xFFFF로 반환*/
        static PCI_Device device;
        for (uint8_t bus = 0; bus < 256; bus++)
            for (uint8_t dev = 0; dev < 32; dev++)
                for (uint8_t func = 0; func < 8; func++) {
                    PCI_Device d = enumerateDevice(bus, dev, func);
                    if (d.vendor_id != 0xFFFF && d.class_code == 0x0C && d.subclass == 0x03) {
                        device = d;    // 값 복사
                        return &device; // 주소 반환
                    }
                }
        return nullptr; // 없으면
    }

}


