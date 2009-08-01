#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <types.h>

#define MULTIBOOT_HEADER_MAGIC			0x1badb002
#define MULTIBOOT_BOOTLOADER_MAGIC		0x2badb002

// These flags specify required features of the boot loader.
enum multiboot_header_flags {
	MULTIBOOT_ALIGN_MODULES		= (1 << 0),
	MULTIBOOT_MEMORY_INFO		= (1 << 1),
	MULTIBOOT_VIDEO_MODES		= (1 << 2),
	MULTIBOOT_ADDRESS_FIELDS	= (1 << 16),
};

// These flags indicate validity of fields in multiboot_header_t.
enum multiboot_info_flags {
	MULTIBOOT_MEM				= (1 << 0),
	MULTIBOOT_BOOT_DEVICE		= (1 << 1),
	MULTIBOOT_CMDLINE			= (1 << 2),
	MULTIBOOT_MODULES			= (1 << 3),
	MULTIBOOT_AOUT				= (1 << 4),
	MULTIBOOT_ELF				= (1 << 5),
	MULTIBOOT_MMAP				= (1 << 6),
	MULTIBOOT_DRIVES			= (1 << 7),
	MULTIBOOT_CONFIG_TABLE		= (1 << 8),
	MULTIBOOT_BOOT_LOADER_NAME	= (1 << 9),
	MULTIBOOT_APM_TABLE			= (1 << 10),
	MULTIBOOT_VBE_TABLE			= (1 << 11),
};

typedef struct multiboot_header {
	uint32_t magic;
	uint32_t flags;
	uint32_t checksum;
	uint32_t header_addr;
	uint32_t load_addr;
	uint32_t load_end_addr;
	uint32_t bss_end_addr;
	uint32_t entry_addr;
} multiboot_header_t;

typedef struct multiboot_info {
	uint32_t flags;
	uint32_t mem_lower;
	uint32_t mem_upper;
	uint32_t boot_device;
	uint32_t cmdline;
	uint32_t mods_count;
	uint32_t mods_addr;
	uint32_t elf0;
	uint32_t elf1;
	uint32_t elf2;
	uint32_t elf3;
	uint32_t mmap_length;
	uint32_t mmap_addr;
	uint32_t drives_length;
	uint32_t drives_addr;
} multiboot_info_t;

typedef struct multiboot_mmap {
	uint32_t size;
	uint64_t base_addr;
	uint64_t length;
	uint32_t type;
} multiboot_mmap_t;

typedef struct multiboot_module {
    uint32_t mod_start;
    uint32_t mod_end;
    uint32_t string;
    uint32_t reserved;
} multiboot_module_t;

extern multiboot_info_t *multiboot_info;

#endif /* ndef MULTIBOOT_H */
