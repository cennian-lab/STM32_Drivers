/*
 * rcc.h
 *
 *  Created on: Apr 4, 2021
 *      Author: cennian
 */

#ifndef INC_RCC_H_
#define INC_RCC_H_

#include "main.h"
/*
 * This function as all the clock related configurations
 */
void RCC_HSEInit(void);
/*
 * System Timer Setup
 */
void RCC_SysTickInit(uint32_t);
/*
 * Increment ms ticks
 */
void RCC_Increment_msTicks(void);
/*
 * Get ms ticks
 */
uint32_t RCC_Get_msTicks(void);
/*
 * ms delay functions
 */
void RCC_msDelay(uint32_t);

#endif /* INC_RCC_H_ */
