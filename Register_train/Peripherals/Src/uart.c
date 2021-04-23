/*
 * uart.c
 *
 *  Created on: 05-Apr-2021
 *      Author: Ihtesham
 */

#include "uart.h"
#include "main.h"
/*
 * Configuring USART 1 related GPIOs
 */
void UART_1_GPIO_Config(void)
{
	//Tx PA9, Rx PA10
	//Enable Port A and it's clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	//Set AF Mode for Tx
	GPIOA->CRH &= ~(GPIO_CRH_MODE9_Msk | GPIO_CRH_CNF9_Msk); //clear PA9
	//Set Tx as output max 50MHz
	GPIOA->CRH |= GPIO_CRH_MODE9_Msk | GPIO_CRH_CNF9_1;		//If using APB1 peripheral use 10 MHz
	//Rx as Floating Input
	GPIOA->CRH &= ~(GPIO_CRH_MODE10_Msk | GPIO_CRH_CNF10_Msk); //clear PA10
	GPIOA->CRH |= GPIO_CRH_CNF10_0; //MODE is already 0
	//PB10, PB11 Mapping to UART
	AFIO->MAPR &= ~AFIO_MAPR_USART1_REMAP_Msk;
	//Enable Alternate Function
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
}

/*
 * Configurations for USART 1
 */
void UART_1_Config(void)
{
	//to setup a variable for uartDiv
	uint32_t uartDiv = SysClockFreq / 9600;

	//Enable UART1 Clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	//Enable Transmit
	USART1->CR1 |= USART_CR1_TE;
	//No Parity
	//Parity Control Disabled
	USART1->CR1 |= USART_CR1_PCE_Msk;
	//Word length 8 bits
	USART1->CR1 &= ~USART_CR1_M_Msk;
	//Stop bits 1
	USART1->CR2 &= ~USART_CR2_STOP_Msk;
	//Disable Hardware Flow Control
	USART1->CR3 &= ~(USART_CR3_CTSE_Msk | USART_CR3_RTSE_Msk);
	//Set baudrate to 115200
	USART1->BRR = (((uartDiv / 16) << USART_BRR_DIV_Mantissa_Pos)
					|(uartDiv % 16) << USART_BRR_DIV_Fraction_Pos);
	//Clear and Enable Flags
	USART1->CR2 &= ~(USART_CR2_LINEN_Msk | USART_CR2_CLKEN_Msk);
	USART1->CR3 &= ~(USART_CR3_SCEN_Msk
			| USART_CR3_IREN_Msk | USART_CR3_HDSEL_Msk);
	//Enable UART
	USART1->CR1 |= USART_CR1_UE;
}

/*
 * Transmit function for USART 1
 */
bool UART_1_Transmit(uint32_t* data, uint8_t len, uint32_t timeout)
{
	uint8_t dataIndex = 0;
	uint32_t startTick = RCC_Get_msTicks();

	while(dataIndex > len)
	{
		//If the transmission buffer is empty
		if(USART1->SR & USART_SR_TXE)
		{
			//Transmit data
			USART1->DR = data[dataIndex];
			dataIndex++;
		}
		else if ((RCC_Get_msTicks() - startTick) >= timeout )
			return false;

	}
	//While the transmission is not complete
	while( (USART1->SR = USART_SR_TC) )
	{
		if((RCC_Get_msTicks() - startTick) >= timeout )
			return false;
	}
	return true;
}






