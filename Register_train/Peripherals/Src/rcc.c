/*
 * rcc.c
 *
 *  Created on: Apr 4, 2021
 *      Author: cennian
 */

#include "rcc.h"

//millisecond ticks
static __IO uint32_t msTicks;

void RCC_HSEInit(void)
{
	/*
	 * HSE = 8MHz
	 * PLLMUL = 9
	 * USB_Prescaler = 1
	 * AHB_Prescaler = 1
	 * APB1_Prescaler = 2
	 * APB2_Prescaler = 1
	 * Cortex_Prescaler = 1
	 * ADC_Prescaler = 2
	 * SysClk = 72MHz
	 * PLCK1= 36MHz
	 * PLCK2= 72MHz
	 * ADC = 36MHz
	 * MCO Source Sysclk
	 */

	/*
	 * Configuring PLL
	 */
	RCC->CFGR &= ~RCC_CFGR_PLLMULL;
	RCC->CFGR |= RCC_CFGR_PLLMULL9;

	/*
	 * Configuring USB Clock
	 */
	RCC->CFGR &= ~RCC_CFGR_USBPRE;
	RCC->CFGR |= RCC_CFGR_USBPRE; // (1<<22)

	/*
	 * HSE Configuration
	 */
	RCC->CR |= RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY));
	//Selecting HSE as PLL source
	RCC->CFGR |= RCC_CFGR_PLLSRC;

	/*
	 * Enable PLL
	 */
	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY));

	/*
	 * Configuring Flash
	 */
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	FLASH->ACR |= FLASH_ACR_LATENCY_1;
	//Enabling Prefetch buffer
	FLASH->ACR |= FLASH_ACR_PRFTBE;

	/*
	 * Configuring Sysclk
	 */
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL; // Selecting PLL as Sysclk

	while(!(RCC->CFGR & RCC_CFGR_SWS_PLL));


	/*
	 * Configuring AHB, APB Clocks
	 */
	RCC->CFGR &= ~RCC_CFGR_HPRE; //0 mean DIV1 AHB presecaler

	RCC->CFGR &= ~RCC_CFGR_PPRE1;
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; //APB1 or PCLK1 prescaler 36MHz

	RCC->CFGR &= ~RCC_CFGR_PPRE2; //APB2 or PCKL2 prescaler 72MHz

	/*
	 * Configuring ADC Clock
	 */
	RCC->CFGR &= ~RCC_CFGR_ADCPRE;
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV2; //ADC Prescaler clock = 36MHz
	/*
	 * MCO Source
	 */
	RCC->CFGR |= RCC_CFGR_MCOSEL_SYSCLK;

}

/*
 * System Timer Setup
 */
void RCC_SysTickInit(uint32_t freq)
{
	uint32_t reload  = freq/1000;
	// Reset Control Register
	SysTick->CTRL = 0; //Disables the timer
	//Set the reload value
	SysTick->LOAD = reload-1;
	//Priority set Systick interrupt NVIC
	NVIC_SetPriority(SysTick_IRQn, 0);
	//Reset Systick Val to 0 (not reload value)
	SysTick->VAL = 0;
	//Set a clock source
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk; //Masking it because we want AHB clock
	//Enable Tick interrupt
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	//Enable Systick
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

/*
 * Increment ms ticks
 */
void RCC_Increment_msTicks(void)
{
	msTicks++;
}
/*
 * Get ms ticks
 */
uint32_t RCC_Get_msTicks(void)
{
	return msTicks;
}
/*
 * ms delay functions
 */
void RCC_msDelay(uint32_t ms)
{
	uint32_t startTicks = RCC_Get_msTicks();
	while(RCC_Get_msTicks() - startTicks < ms);
}

void SysTick_Handler(void) {
	NVIC_ClearPendingIRQ(SysTick_IRQn);
	RCC_Increment_msTicks();
}




