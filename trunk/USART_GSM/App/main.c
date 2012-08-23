/**********************************************************************
* $Id$		main.c					2012-07-02
*//**
* @file		main.c
* @brief	
* @MCU		STM32f104VC
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
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "usart_interrupt.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/************************************************************************
 * @brief 	main function	
 * @param[in]	None
 * @return		None
 ***********************************************************************/
int main(void)
{
	 uint16_t ReData;
	
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	
	
	/* enable peripheral clock for USART2 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
  /* GPIOA clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	//RCC_PCLK2Config(RCC_HCLK_Div16);
	//NVIC_PriorityGroupConfig();
  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	//GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
	/* GPIOA Configuration:  USART1  */
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

	
	
	
	/* USART1 configuration ------------------------------------------------------*/
  /* USART1 configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - Two Stop Bit
        - Odd parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART1, &USART_InitStructure);

  /* Enable the USART1 Receive interrupt: this interrupt is generated when the 
     USART1 receive data register is not empty */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	/* Enable the USART1 Transmit interrupt: this interrupt is generated when the 
     USART1 transmit data register is empty */  
  //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	
	/* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);
	
	USART_SendData(USART1,'A' );
	USART_SendData(USART1,'B' );
	while(1)
	{
		 //USART_SendData(USART1,'a');
		//ReData = USART_GetChar();
    //USART_SendData(USART1, ReData);
	}
	
	
}
