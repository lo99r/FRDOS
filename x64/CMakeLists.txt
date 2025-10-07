cmake_minimum_required(VERSION 3.8)

file(GLOB_RECURSE SOURCES_x64 CONFIGURE_DEPENDS

"${CMAKE_SOURCE_DIR}/x64/*.cpp"

)

file(GLOB ISO_DIR CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/iso")


add_library(OBJECTS_x64 OBJECT ${SOURCES_x64})
target_compile_options(OBJECTS_x64 PRIVATE -nostdlib -m64 -fpermissive -fno-common -fno-exceptions -fno-rtti)


file(GLOB HEADER_DIRS CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/x64/library/*"
)

foreach(dir ${HEADER_DIRS})
    target_include_directories(OBJECTS_x64 PRIVATE ${dir})
endforeach()



add_custom_target(kernel_x64 ALL
    COMMAND ld -n -T "${CMAKE_SOURCE_DIR}/x64/linker_x64.ld" -melf_x86_64 -o "${ISO_DIR}/kernel_x64.elf"

    $<TARGET_OBJECTS:OBJECTS_x64>

    DEPENDS OBJECTS_x64
    COMMAND_EXPAND_LISTS
)

add_custom_target(kernel_x64_bin ALL
    COMMAND objcopy -O binary "${ISO_DIR}/kernel_x64.elf" "${ISO_DIR}/kernel_x64.bin"

    DEPENDS kernel_x64
)
#target_compile_options(kernel_x64 PRIVATE -fno-exceptions -fno-rtti)