#include<MYOS>

using namespace myos::pci;

namespace myos::usb {

    void initHostController() {
        PCI_Device* usbController = findUSBController();
        if (!usbController) return;
        // MMIO base �ּ� ȹ�� �� �ʱ�ȭ
    }

    bool detectMassStorage() {
        // USB device descriptor Ȯ��
        return true;
    }

}
