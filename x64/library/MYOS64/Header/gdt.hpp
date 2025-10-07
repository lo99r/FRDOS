#pragma once
#include "defines.hpp"

namespace gdt {

    extern uint32 gdt_arr[1024];

    enum class dir{ // 처음보신다면 GROW_UP으로 설정하세요.
        GROW_UP,
        GROW_DOWN
    };

    enum class accesser{ // 영역에 접근하는 존재들에 대한 세부사항입니다.
        SAME_OR_HIGHER, // 위계질서가 뚜렷합니다. 지정된 permission 보다 더 낮은 단계의 존재들은 접근할 수 없습니다.
        ALL // 모두가 접근할 수 있습니다.
    };

    enum class type{ // 처음 보신다면 CODE_DATA_SEG 로 설정하세요.
        SYSTEM_SEG, 
        CODE_DATA_SEG
    };

    struct gdt_table{
        uint32 base_address;
        uint32 limit_address;

        Permission previlege_level;

        bool readable;
        bool writable;
        bool excutable;
        type seg_type;
        dir direction;
        accesser conforming;

        int bits; // 16, 32, 64 중 하나로 설정하세요. 그렇지 않으면 그냥 32비트로 수정됩니다.
        

    };

    void load_gdt(gdt_table* address, uint32 table_count);

    extern const gdt_table default_data_set;
    extern const gdt_table default_code_set;

}