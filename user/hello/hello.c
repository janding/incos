int main()
{
	short *v = (short*)0xb8000;
	while(1)
		v[80] = 0x4141;
}
