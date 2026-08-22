#include <stdio.h>
#include <sys/stat.h>
#include "stm32f429xx.h"

void _init(void) {}
int _close(int fd) {}
int _lseek(int fd, int ptr, int dir) {}
int _read(int fd, char *ptr, int len) {}
int _fstat(int fd, struct stat *st) {}
int _isatty(int fd) {}

void SystemInit(void) {
	SysTick_Config(16000000);
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
}

void *_sbrk(int incr) {
  extern char _end;
  static unsigned char *heap = NULL;
  unsigned char *prev_heap;
  if (heap == NULL) heap = (unsigned char *) &_end;
  prev_heap = heap;
  heap += incr;
  return prev_heap;
}

int _write(int file, char *ptr, int len) {
	while (len--) {
		USART3->DR = *ptr++;
		while (!(USART3->SR & USART_SR_TXE)) {}
	}
}

void main() {
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= GPIO_MODER_MODER8_1;
	GPIOD->MODER |= GPIO_MODER_MODER9_1;
	GPIOD->AFR[1] |= (7 << GPIO_AFRH_AFSEL8_Pos) | (7 << GPIO_AFRH_AFSEL9_Pos);
	USART3->BRR = 16000000 / 115200;
	USART3->CR1 |= (USART_CR1_UE | USART_CR1_TE);

	// PB8, PB9 => SCL, SDA
	#define SCL (8)
	#define SDA (9)

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

	// GPIOB->MODER != GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1;
	GPIOB->MODER |= (2 << (SCL * 2)) | (2 << (SDA * 2));// set PB8, 9 to AF mode		
	// GPIOD->AFR[1] |= (4 << GPIO_AFRH_AFSEL8_Pos) | (4 << GPIO_AFRH_AFSEL9_Pos);		
	GPIOB->AFR[1] |= 4 | (4 << 4);// set to AF4 (I2C1); high reg so we use pins 0 1 not 8 9
	GPIOB->OTYPER |= (1 << SCL) | (1 << SDA);// set output mode to open drain
	GPIOB->OSPEEDR |= (3 << SCL*2) | (3 << SDA*2);// set very high speed

	I2C1->CR2 |= 0x10;// set periph clock freq to 16 MHz
	I2C1->CCR = 0x50;// tricky! SCL 100kHz
	I2C1->TRISE = 0x10;// also tricky, rise time 1000ns
	I2C1->CR1 |= I2C_CR1_PE;// enable I2C1


again:
	*(volatile int *)0x20000000 += 1;//why isn't this causing problems?
	I2C1->CR1 |= I2C_CR1_START | I2C_CR1_ACK;
	while (!(I2C1->SR1 & I2C_SR1_SB));// wait on start cond

	I2C1->DR = 0b11010000;//adr + 0-write-bit
	while (!(I2C1->SR1 & I2C_SR1_ADDR));
	uint16_t reg = I2C1->SR1 | I2C1->SR2;// clear
	while (!(I2C1->SR1 & I2C_SR1_TXE));
	I2C1->DR = 0;// address
	while (!(I2C1->SR1 & I2C_SR1_BTF));
	reg = I2C1->SR1 | I2C1->SR2;	
	I2C1->CR1 |= I2C_CR1_START;
	while (!(I2C1->SR1 & I2C_SR1_SB));
	I2C1->DR = 0b11010001;// read
	while (!(I2C1->SR1 & I2C_SR1_ADDR));
	reg = I2C1->SR1 | I2C1->SR2;
	char data[3];
	*(volatile int *)(SRAM_BASE+4) = 0;
	for (int i = 0; i < 3; i++)	{
		*(volatile int *)(SRAM_BASE+4) += 1;
		if (i + 1 == 3) {
			I2C1->CR1 &= ~I2C_CR1_ACK;
			I2C1->CR1 |= I2C_CR1_STOP;
		}
		while (!(I2C1->SR1 & I2C_SR1_RXNE));
		data[i] = I2C1->DR;
	}
	int s = (((data[0]) >> 4) * 10) + ((data[0]) & 0x0F);
	int min = (((data[1]) >> 4) * 10) + ((data[1]) & 0x0F);
	int hr = (((data[2]) >> 4) * 10) + ((data[2]) & 0x0F);

	printf("Tick %d Data H %d M %d S %d\r\n", SysTick->VAL, hr, min, s);
	while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) {}
	goto again;
}

