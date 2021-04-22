/*
 * uart.c
 *
 *  Created on: 05-Apr-2021
 *      Author: Ihtesham
 */

#include "uart.h"
#include "main.h"
/*
 * Configuring USART 3 related GPIOs
 */
void UART_3_GPIO_Config(void)
{
	//Tx PB10, Rx PB11
	//Enable Port A and it's clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	//Set AF Mode for Tx
	GPIOB->CRH &= ~(GPIO_CRH_MODE10_Msk | GPIO_CRH_CNF10_Msk); //clear PB10
	//Set Tx as output max 10MHz
	GPIOB->CRH |= GPIO_CRH_MODE10_Msk | GPIO_CRH_CNF10_0;
	//Rx as Floating Input
	GPIOB->CRH &= ~(GPIO_CRH_MODE11_Msk | GPIO_CRH_CNF11_Msk); //clear PB11
	GPIOB->CRH |= GPIO_CRH_CNF11_0; //MODE is already 0
	//PB10, PB11 Mapping to UART
	AFIO->MAPR &= ~AFIO_MAPR_USART3_REMAP_Msk;
	//Enable Alternate Function
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
}

/*
 * Configurations for USART 3
 */
void UART_3_Config(void)
{
	//Enable UART3 Clock
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	//Enable Transmit
	USART3->CR1 |= USART_CR1_TE;
	//No Parity
	//Parity Control Disabled
	USART3->CR1 |= USART_CR1_PCE_Msk;
	//Word length 8 bits
	USART3->CR1 &= ~USART_CR1_M_Msk;
	//Stop bits 1
	USART3->CR2 &= ~USART_CR2_STOP_Msk;
	//Disable Hardware Flow Control
	USART3->CR3 &= ~(USART_CR3_CTSE_Msk | USART_CR3_RTSE_Msk);
	//Set baudrate to 115200
	USART3->BRR = 0;
	USART3->BRR = (0x13<<4 | 0x5<<0);
	//Clear and Enable Flags
	USART3->CR2 &= ~(USART_CR2_LINEN_Msk | USART_CR2_CLKEN_Msk);
	USART3->CR3 &= ~(USART_CR3_SCEN_Msk
			| USART_CR3_IREN_Msk | USART_CR3_HDSEL_Msk);
	//Enable UART
	USART3->CR1 |= USART_CR1_UE_Msk;
}

/*
 * Transmit function for USART 3
 */
bool UART_3_Transmit(uint32_t* data, uint8_t len, uint32_t timeout)
{
	uint8_t dataIndex = 0;
	uint32_t startTick = RCC_Get_msTicks();

	while(dataIndex > len)
	{
		//If the transmission buffer is empty
		if(USART3->SR & USART_SR_TXE)
		{
			//Transmit data
			USART3->DR = data[dataIndex];
			dataIndex++;
		}
		else if ((RCC_Get_msTicks() - startTick) >= timeout )
			return false;

	}
	//While the transmission is not complete
	while(USART3->SR = USART_SR_TC)
	{
		if((RCC_Get_msTicks() - startTick) >= timeout )
			return false;
	}
	return true;
}






