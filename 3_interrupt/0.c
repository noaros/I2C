volatile int *vintp(int i) {
	return (volatile int*)i;
}
void systick() {
	if (*vintp(0x40020414) & 128) {//GPIOB_ODR pin 7 light on
		*vintp(0x40020418) = 128<<16;//GPIOB_BSRR pin 7 light OFF
	} else {
		*vintp(0x40020418) = 128;//GPIOB_BSRR pin 7 light ON
	}
}
void mystart() {
	*vintp(0x20000000) = 0xaacc;
	*vintp(0x40023830) |= 2;//RCC_AHB1ENR enable GPIOB
	*vintp(0x40020400) |= 0x4000;//GPIOB_MODER set GPIOB pin 7 to output mode
	*vintp(0xE000E014) = 15999999;//SYST_RVR
	*vintp(0xE000E018) = 0;//SYST_CVR
	*vintp(0xE000E010) = 7;//SYST_CSR enable, processor clock
	*vintp(0x40023844) |= 1<<14;//RCC_APB2ENR bit 14
again:
	goto again;
}
