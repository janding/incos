#include "arch/bios.h"
#include "arch/pio.h"

#define TAB_WIDTH	8
#define ROWS		25
#define COLS		80

static int attribute = 0x07;
static short *vidmem = (short*)0xb8000;

static unsigned int cursor;

int bochs_putc(char c)
{
	outb(0xE9, c);
	return c;
}

void cons_init()
{
	if (bios_data_area[0x50] < COLS && bios_data_area[0x51] < ROWS)
		cursor = bios_data_area[0x50] + COLS * bios_data_area[0x51];
	else
		cursor = 0;
}

void cons_unload()
{
	bios_data_area[0x50] = cursor % COLS;
	bios_data_area[0x51] = cursor / COLS;
}

static void cons_set_hw_cursor()
{
    outb(0x3d4, 15);
    outb(0x3d5, cursor);
    outb(0x3d4, 14);
    outb(0x3d5, cursor >> 8);
}

static void cons_scroll_up()
{
	int i;
	
	for(i = 0; i < COLS * (ROWS - 1); i++)
	{
		vidmem[i] = vidmem[i + COLS];
	}

	for(i = 0; i < COLS; i++)
	{
		vidmem[COLS * (ROWS - 1) + i] = attribute;
	}

	cursor -= COLS;
}

static void cons_putc(char c)
{
	bochs_putc(c);
	switch (c) {
		case '\t':
			do { vidmem[cursor++] = ' ' | (attribute << 8); } while (cursor % TAB_WIDTH != 0);
			break;
		case '\b':
			vidmem[cursor--] = ' ' | (attribute << 8);
			break;
		case '\r':
			cursor -= cursor % COLS;
			break;
		case '\n':
			do { vidmem[cursor++] = ' ' | (attribute << 8); } while (cursor % COLS != 0);
			break;
		default:
			vidmem[cursor++] = c | (attribute << 8);
			break;
	}

	if (cursor > (ROWS - 1) * COLS) {
		cons_scroll_up();
	}
}

int cons_puts(char *p)
{	
	const char *d = p;

	while (*p)
		cons_putc(*p++);

	cons_set_hw_cursor();

	return p - d;
}
