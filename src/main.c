#include "stm32f10x.h"


void clock_init(void) {
	/* Activate HSE. */
	RCC->CR |= RCC_CR_HSEON;

	/* Wait until HSE is stable. */
	while (!(RCC->CR & RCC_CR_HSERDY));

	/* PLL setup. */
	RCC->CFGR |= RCC_CFGR_PLLMULL9 | RCC_CFGR_PLLXTPRE_HSE | RCC_CFGR_PLLSRC_HSE;

	/* Activate PLL. */
	RCC->CR   |= RCC_CR_PLLON;

	/* Wait until PLL is stable. */
	while (!(RCC->CR & RCC_CR_PLLRDY));

	/* AHB, APB1 and APB2 prescaler setup. */
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_PPRE2_DIV1;

	/* FLASH wait states setup. */
	FLASH->ACR |= FLASH_ACR_LATENCY_2;

	/* Set PLL as system clock */
	RCC->CFGR |= RCC_CFGR_SW_PLL;

	/* Wait until PLL is the system clock. */
	while (!(RCC->CFGR & RCC_CFGR_SWS_PLL ));
}

void UARTinit(void) {

	uint32_t BRR=0x00000271;

	/*GPIOA, GPIOB, USART1 on APB2 bus clock enable. */
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_USART1EN | RCC_APB2ENR_AFIOEN;

	/*Ensure no remap, keep PA9,PA10. */
	AFIO->MAPR &= ~AFIO_MAPR_USART1_REMAP;

	/*UART Enable. */
	USART1->CR1 |= 0x00002000;

	/*Use 1 stop bit. */
	USART1->CR2 &= ~0x00003000;

	/*Set BaudRate=115.2Ksymb/sec @72MHz CLK, 1 Symb=1 Bit. */
	USART1->BRR = BRR;

	/*Set word length to 8 bits. */
	USART1->CR1 &= ~0x00001000;

	/*Configure GPIOA output mode for UART. */
	GPIOA->CRH = 0x444444B4;

}


void delay(uint32_t cnt) {
	while(cnt) {
		cnt--;
	}
}


int main(void) {

	/*Initialize clock main infrastructure*/
	clock_init();

	/*Initialize UART peripheral*/
	UARTinit();

	//uint32_t crcheck;

	/*AHB CRC clock enable. */
	//RCC->AHBENR |= RCC_AHBENR_CRCEN;

	/*Initial CRC reset. */
	//CRC->CR |= 0x00000001;

	/*Load 0xxxxx. */
	//CRC->DR = 0x12345678;

	/*Wait min 4 AHB clock cycles. */
	//delay(0xFFFFF);

	/*Read computed CRC code. */
	//crcheck = CRC->DR;



	GPIOA->CRL =(0x03 << (3 * 4)) | (0x03 << (2 * 4)) | (0x03 << (1 * 4)) | (0x03 << (0 * 4));

	GPIOB->CRL = (0x08 << (1 * 4)) | (0x08 << (0 * 4));

	GPIOB->ODR |= GPIO_ODR_ODR0 | GPIO_ODR_ODR1;

	/*Set TE RE (transmission and receive enable) bits. */
	//USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;

	//char rec = 'h';
	//char c = 'a';

	/*Wait until TXE is stable. */
	/*i.e. loop until bit 7 is set. */
//	while ((USART1->SR & USART_SR_TXE) != 1);

	/*Load DR which begins transmission automatically*/
	//USART1->DR = 'X';

	/* Wait for transfer complete. */
	//while (!(USART1->SR & USART_SR_TC));

	/*Load DR which begins transmission automatically*/
	//USART1->DR = 'X';

	/* Wait for transfer complete. */
	//while (!(USART1->SR & USART_SR_TC));

	while(1) {

		delay(0xFFFFF);

		GPIOA->BSRR |= GPIO_BSRR_BS0 | GPIO_BSRR_BS1 | GPIO_BSRR_BR2 | GPIO_BSRR_BR3;

		delay(0xFFFFF);

		GPIOA->BSRR |= GPIO_BSRR_BR0 | GPIO_BSRR_BR1 | GPIO_BSRR_BS2 | GPIO_BSRR_BS3;

		delay(0xFFFFF);

	}


}

void function() {

		int a = 0;

		clock_init();

		int start = 0;

	    int stop;

		int finish;

		/* Activate GPIOC clock. */
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;

		/* Configure PC6 and PC7 as push-pull outputs.
		 *
		 * For pin x, the values are:
		 * MODEx	= 11 (Output mode, max speed 50 MHz)
		 * CNFx		= 00 (General purpose output push-pull)
		 *
		 * Using hex notation:
		 * 0x03 << (x * 4)
		 */
		GPIOA->CRL =(0x03 << (3 * 4)) | (0x03 << (2 * 4)) | (0x03 << (1 * 4)) | (0x03 << (0 * 4));

		GPIOB->CRL = (0x08 << (1 * 4)) | (0x08 << (0 * 4));

		GPIOB->ODR |= GPIO_ODR_ODR0 | GPIO_ODR_ODR1;

		while(1) {
			a++;

			if (a == 100) {
				a = 0;
			}

			if(((GPIOB->IDR & GPIO_IDR_IDR0) == 0) && start == 0)
				{stop = 0; start = 1; finish = 0;}
			else {start = 0;stop = 1;finish = 0;}

			if (finish == 1) continue;

			if((GPIOB->IDR & GPIO_IDR_IDR1) == 0) {
				finish = 1; stop = 0; start = 0;
			    GPIOA->BSRR |= GPIO_BSRR_BR0 | GPIO_BSRR_BR1 | GPIO_BSRR_BR2 | GPIO_BSRR_BR3;
			    continue;
			}
			/* Turn on LED1, turn off LED2. */
			GPIOA->BSRR |= GPIO_BSRR_BR0 | GPIO_BSRR_BS1 | GPIO_BSRR_BS2 | GPIO_BSRR_BS3;


			/* Delay. */
			delay(0xFFFFF);

			if((GPIOB->IDR & GPIO_IDR_IDR1) == 0) {
				finish = 1; stop = 0; start = 0;
			    GPIOA->BSRR |= GPIO_BSRR_BR0 | GPIO_BSRR_BR1 | GPIO_BSRR_BR2 | GPIO_BSRR_BR3;
			    continue;
			}

			/* Turn off LED1, turn on LED2. */

			GPIOA->BSRR |= GPIO_BSRR_BR1 | GPIO_BSRR_BS0 | GPIO_BSRR_BS2 | GPIO_BSRR_BS3;

			/* Delay. */
			delay(0xFFFFF);

			if((GPIOB->IDR & GPIO_IDR_IDR1) == 0) {
				finish = 1; stop = 0; start = 0;
			    GPIOA->BSRR |= GPIO_BSRR_BR0 | GPIO_BSRR_BR1 | GPIO_BSRR_BR2 | GPIO_BSRR_BR3;
			    continue;
			}

			/*Turn on LED2, off others. */
			GPIOA->BSRR |= GPIO_BSRR_BR2 | GPIO_BSRR_BS1 | GPIO_BSRR_BS0 | GPIO_BSRR_BS3;

			delay(0xFFFFF);

			if((GPIOB->IDR & GPIO_IDR_IDR1) == 0) {
				finish = 1; stop = 0; start = 0;
			    GPIOA->BSRR |= GPIO_BSRR_BR0 | GPIO_BSRR_BR1 | GPIO_BSRR_BR2 | GPIO_BSRR_BR3;
			    continue;
			}

			/*Turn on LED3, off others. */
			GPIOA->BSRR |= GPIO_BSRR_BR3 | GPIO_BSRR_BS1 | GPIO_BSRR_BS2 | GPIO_BSRR_BS0;

			delay(0xFFFFF);
		}
}


