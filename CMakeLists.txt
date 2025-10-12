cmake_minimum_required(VERSION 3.8)

add_subdirectory(x64)
add_subdirectory(x86)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")



add_custom_target(ISO_BUILD ALL
    COMMAND rm -f "${ISO_DIR}/kernel_x64.bin" "${ISO_DIR}/kernel_x64.elf"
    COMMAND grub-mkrescue -o ${CMAKE_SOURCE_DIR}/out/OS.iso "${CMAKE_SOURCE_DIR}/iso"

    DEPENDS kernel_x86_bin
    DEPENDS kernel_x64_bin
)

add_custom_target(RUN ALL
    COMMAND qemu-system-x86_64 -drive file=${CMAKE_SOURCE_DIR}/out/OS.iso,format=raw,media=cdrom -drive file=${CMAKE_SOURCE_DIR}/out/disk.img,format=raw,if=ide,index=1,media=disk -D log.txt -d int -m 100M -audiodev pa,id=snd0 -machine pcspk-audiodev=snd0
    DEPENDS ISO_BUILD
    COMMENT "QEMU is Running..."
)

#virtio, ide