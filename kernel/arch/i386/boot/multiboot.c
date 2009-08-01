#include "kernel.h"
#include "multiboot.h"

#define MULTIBOOT_HEADER_FLAGS (MULTIBOOT_ALIGN_MODULES | MULTIBOOT_MEMORY_INFO)

__global
multiboot_header_t multiboot_header = {
    .magic = MULTIBOOT_HEADER_MAGIC,
    .flags = MULTIBOOT_HEADER_FLAGS,
    .checksum = -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
};

multiboot_info_t *multiboot_info;
