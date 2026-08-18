
void mystart() {
	*(volatile int *)0x20000000 = 0xaabb;
again:
	goto again;
}

