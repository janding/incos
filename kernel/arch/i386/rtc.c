#include "arch/cmos.h"

void
rtc_init()
{
	//cmos_write(0x0B, cmos_read(0x0B) | 0x40);
	//cmos_write(0x0A, (cmos_read(0x0A) & 0xF0) | 0x0F);
}

void
rtc_irq_handler()
{
	//kprintf("RTC [%02x]", cmos_read(0x0C));
}
