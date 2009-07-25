extern int dprintf(const char *, ...);

int main()
{
	dprintf("hello from %08x", main);	
	for(;;);
}
