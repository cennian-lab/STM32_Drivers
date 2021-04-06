/*
 * main.c
 *
 *  Created on: Apr 4, 2021
 *      Author: cennian
 */
#include "main.h"
#include "rcc.h"

/*
 * System Clock Frequency
 */
#define SysClockFreq	72000000UL

int main(void)
{
	RCC_HSEInit();
    /* Loop forever */
	RCC_SysTickInit(SysClockFreq);
	while(1)
	{

	}
}
