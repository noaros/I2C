volatile int *vintp(int i) {
	return (volatile int*)i;
}
void mystart() {
	*vintp(0x20000000) = 0xaacc;
	*vintp(0x40023830) |= 2;//RCC_AHB1ENR enable GPIOB
	*vintp(0x40020400) |= 0x4000;//GPIOB_MODER set GPIOB pin 7 to output mode
	*vintp(0xE000E014) = 15999999;//SYST_RVR
	*vintp(0xE000E018) = 0;//SYST_CVR
	*vintp(0xE000E010) = 5;//SYST_CSR enable, processor clock
	*vintp(0x40023844) |= 1<<14;//RCC_APB2ENR bit 14
again:
	*vintp(0x40020418) = 128;//GPIOB_BSRR pin 7 ON
	while (!(*vintp(0xE000E010) & 1<<16)) {}//SYST_CSR wait for count flag
	*vintp(0x40020418) = 128<<16;//GPIOB_BSRR pin 7 OFF
	while (!(*vintp(0xE000E010) & 1<<16)) {}
	goto again;
}
