#include <stdio.h>
#include <sys/stat.h>

int _close(int fd) {}
int _lseek(int fd, int ptr, int dir) {}
int _read(int fd, char *ptr, int len) {}
int _fstat(int fd, struct stat *st) {}
int _isatty(int fd) {}

void *_sbrk(int incr) {
  extern char _end;
  static unsigned char *heap = NULL;
  unsigned char *prev_heap;
  if (heap == NULL) heap = (unsigned char *) &_end;
  prev_heap = heap;
  heap += incr;
  return prev_heap;
}

volatile int *vintp(int i) {
	return (volatile int*)i;
}

int _write(int file, char *ptr, int len) {
	while (len--) {
		*vintp(0x40004804) = *ptr++;//USART_DR
		while ((*vintp(0x40004800) & 1<<7)==0) {}//USART_SR wait for TXE
	}
}

void mystart() {
	extern long _sbss, _ebss, _sdata, _edata, _sidata;
	for (long *dst = &_sbss; dst < &_ebss; dst++) *dst = 0;
  	for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

	*vintp(0x20000000) = 0xaacc;
	*vintp(0x40023830) |= 2;//RCC_AHB1ENR enable GPIOB
	*vintp(0x40020400) |= 0x4000;//GPIOB_MODER set GPIOB pin 7 to output mode
	*vintp(0xE000E014) = 15999999;//SYST_RVR
	*vintp(0xE000E018) = 0;//SYST_CVR
	*vintp(0xE000E010) = 5;//SYST_CSR enable, processor clock
	*vintp(0x40023844) |= 1<<14;//RCC_APB2ENR bit 14

	*vintp(0x40023840) |= 1<<18;//RCC_APB1ENR bit 18 enable UART3
	*vintp(0x40023830) |= 8;//RCC_AHB1ENR enable GPIOD.
	*vintp(0x40020c00) |= 1 << 17;//GPIOD_MODER set GPIOD pin 8 to af mode; moder pin 17
	*vintp(0x40020c00) |= 1 << 19;//GPIOD_MODER set GPIOD pin 9 to af mode; moder pin 19
	*vintp(0x40020c24) = 7 << 4 | 7;//GPIOD_AFRH set GPIOD8 and GPIOD9 to be af7 uart3
	*vintp(0x40004808) = 16000000 / 115200;
	*vintp(0x4000480c) = 1<<3 | 1<<13; //CR1 enable UART, send

again:
	printf("Tick %d\r\n", *vintp(0xE000E018));
	*vintp(0x40020418) = 128;//GPIOB_BSRR pin 7 ON
	while (!(*vintp(0xE000E010) & 1<<16)) {}//SYST_CSR wait for count flag
	*vintp(0x40020418) = 128<<16;//GPIOB_BSRR pin 7 OFF
	while (!(*vintp(0xE000E010) & 1<<16)) {}
	goto again;
}

