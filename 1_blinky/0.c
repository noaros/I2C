volatile int *vintp(int i) {
	return (volatile int*)i;
}
void mystart() {
	*vintp(0x20000000) = 0xaacc;
	*vintp(0x40023830) |= 2;//RCC_AHB1ENR enable GPIOB
	*vintp(0x40020400) |= 0x4000;//GPIOB_MODER set GPIOB pin 7 to output mode
again:
	*vintp(0x40020418) = 128;//GPIOB_BSRR pin 7 ON
	for (int i=0; i<999999; i++) {}
	*vintp(0x40020418) = 128<<16;//GPIOB_BSRR pin 7 OFF
	for (int i=0; i<999999; i++) {}
	goto again;
}

