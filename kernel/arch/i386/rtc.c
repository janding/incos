#include "arch/cmos.h"

static int now;

void
rtc_init()
{
	now = 0;

	cmos_write(0x0B, cmos_read(0x0B) | 0x40);
	cmos_write(0x0A, (cmos_read(0x0A) & 0xF0) | 0x0F);
}

void
rtc_irq_handler()
{
	now += 500;
	cmos_read(0x0C);
}

int
rtc_get_time()
{
	return now;
}
