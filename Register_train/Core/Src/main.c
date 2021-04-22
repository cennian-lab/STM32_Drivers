/*
 * main.c
 *
 *  Created on: Apr 4, 2021
 *      Author: cennian
 */
#include "main.h"
#include "rcc.h"
#include "stdio.h"
#include "uart.h"

/*
 * System Clock Frequency
 */
#define SysClockFreq	72000000UL

int main(void)
{
	RCC_HSEInit();
    /* Loop forever */
	RCC_SysTickInit(SysClockFreq);
	//UART Configuration
	UART_3_GPIO_Config();
	UART_3_Config();

	printf("This is a test program\n");
	while(1)
	{
		//Main Loop
		printf("Hello World");
		RCC_msDelay(1000);
	}
}
