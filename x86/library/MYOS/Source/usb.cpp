#include<MYOS>

using namespace myos::pci;

namespace myos::usb {

    void initHostController() {
        PCI_Device* usbController = findUSBController();
        if (!usbController) return;
        // MMIO base 주소 획득 후 초기화
    }

    bool detectMassStorage() {
        // USB device descriptor 확인
        return true;
    }

}
