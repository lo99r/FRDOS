cmake_minimum_required(VERSION 3.8)

file(GLOB_RECURSE SOURCES_x86 CONFIGURE_DEPENDS

"${CMAKE_SOURCE_DIR}/x86/*.cpp"

)

file(GLOB ISO_DIR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/iso")

#target_compile_options(kernel_x86 PRIVATE -fno-exceptions -fno-rtti)

add_library(OBJECTS_x86 OBJECT ${SOURCES_x86} "library/MYOS/Source/system.cpp" "library/MYOS/Source/pci.cpp" "library/MYOS/Source/disk.cpp" "library/MYOS/Source/floppy.cpp" "library/MYOS/Source/usb.cpp" "library/MYOS/Source/fs.cpp" "library/MYOS/Source/porter.cpp" "library/MYOS/Source/fdfs.cpp")
target_compile_options(OBJECTS_x86 PRIVATE -nostdlib -m32 -fpermissive -fno-common -fno-exceptions -fno-rtti)


file(GLOB HEADER_DIRS CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/x86/library/*"
)

foreach(dir ${HEADER_DIRS})
    target_include_directories(OBJECTS_x86 PRIVATE ${dir})
endforeach()

add_custom_target(kernel_x86 ALL
    COMMAND ld -n -T "${CMAKE_SOURCE_DIR}/x86/linker_x86.ld" -melf_i386 -o "${ISO_DIR}/kernel_x86.elf"

    $<TARGET_OBJECTS:OBJECTS_x86>

    DEPENDS OBJECTS_x86
    COMMAND_EXPAND_LISTS

)

add_custom_target(kernel_x86_bin ALL
    COMMAND objcopy -O binary "${ISO_DIR}/kernel_x86.elf" "${ISO_DIR}/kernel_x86.bin"

    DEPENDS kernel_x86
)