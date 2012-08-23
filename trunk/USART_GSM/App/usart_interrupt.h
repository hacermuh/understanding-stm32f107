/**********************************************************************
* $Id$		usart_interrupt.h					
*//**
* @file		usart_interrupt.h
* @brief	
* @MCU		STM32f107VC
* @version	1.0
* @date		22. Aug. 2012
* @Company   VBEB Corp,.Ltd
* @Website    http://www.vbeb.vn
* @author	Sang Mai - IT Training Assistance - sang.mai@vbeb.vn 
*
* @References:
*
* All rights reserved.
*
***********************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_INTERRUPT_H
#define __USART_INTERRUPT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void USART1_IRQHandler(void);
uint16_t USART_GetChar(void);

#endif
