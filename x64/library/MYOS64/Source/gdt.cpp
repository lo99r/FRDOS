#include <MYOS64>

namespace gdt{
    struct __attribute__((packed)) gdtr { uint16 s; uint32 a; };
    gdtr gdtreg;
    uint32 gdt_arr[1024] = { 0, };

    const gdt_table default_data_set = { 0, 0xFFFFFFFF, Permission::KERNEL, true, true, false, type::CODE_DATA_SEG,  dir::GROW_UP,  accesser::SAME_OR_HIGHER, 32 };
    const gdt_table default_code_set = { 0, 0xFFFFFFFF, Permission::KERNEL, true, false, true, type::CODE_DATA_SEG,  dir::GROW_UP,  accesser::SAME_OR_HIGHER, 32 };

    void load_gdt(gdt_table* address, uint32 table_count) {
        uint16 size = (table_count) * 8 - 1;
        for (int idx = 1; idx < table_count; idx++){
            uint32 &field1 = gdt_arr[idx * 2];
            uint32 &field2 = gdt_arr[idx * 2 + 1];
            field1 = 0;
            field2 = 0;
            int g_flag = 0;
            if (address[idx].limit_address > 0xFFFFF) {
                g_flag = 1;
                address[idx].limit_address >>= 12;
            }

            field1 |= address[idx].limit_address & 0xFFFF;
            field1 |= (address[idx].base_address & 0xFFFF) << 16;
            field2 |= (address[idx].base_address & 0xFF0000) >> 16;
            uint8 access_field = 0;
            
            if (address[idx].excutable) {
                access_field |= 0b1000;
                access_field |= (int)(address[idx].readable) << 1;
                access_field |= (int)(address[idx].conforming) << 2;
            }
            else{
                access_field |= (int)(address[idx].writable) << 1;
                access_field |= (int)(address[idx].direction) << 2;
            }

            access_field |= (int)(address[idx].seg_type) << 4;
            access_field |= (int)(address[idx].previlege_level) << 5;
            access_field |= 1 << 7;

            field2 |= access_field << 8;
            field2 |= address[idx].limit_address & 0xf << 16;
            switch(address[idx].bits) {
                case 16:
                    break;
                case 32:
                    field2 |= 1 << 22;
                    break;
                case 64:
                    field2 |= 1 << 21;
                    break;
                default:
                    field2 |= 1 << 22;
            }
            field2 |= g_flag << 23;

            field2 |= address[idx].base_address & 0xFF000000;
            
        }
        gdtreg.s = size;
        gdtreg.a = (uint32)gdt_arr;
        
        asm volatile("\n.att_syntax prefix\n""mov %0, %%rax"::"r"(&gdtreg):"rax");
        asmbly("lgdt [rax]");
    }
}