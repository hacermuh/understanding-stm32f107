/**********************************************************************
* $Id$		main.c					2012-07-02
*//**
* @file		main.c
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
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "usart_interrupt.h"
//#include <string.h>


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void USART_Config(void);
//void SEND_S(char *ch);
void USART_Puts(unsigned char * data);
/* Private functions ---------------------------------------------------------*/



/************************************************************************
 * @brief 	main function	
 * @param[in]	None
 * @return		None
 ***********************************************************************/
int main(void)
{
	 uint16_t ReData;
	
	USART_Config();
	
	//USART_SendData(USART1,'A' );
	//USART_SendData(USART1,'B' );
	//SEND_S("MAIPHUOCSANG");
	USART_Puts("VBEB Co.,Ltd\n\r");
	USART_Puts("Phuoc Sang\n\r");
	while(1)
	{
		 //USART_SendData(USART1,'a');
		ReData = USART_GetChar();
    USART_SendData(USART1, ReData);
		USART_Puts("\n\r");
		
	}
	
	
}

void USART_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	
	
	/* enable peripheral clock for USART2 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
  /* GPIOA clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	
	//RCC_PCLK2Config(RCC_HCLK_Div16);
	
	
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	//GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
	/* GPIOA Configuration:  USART1  */
 
	/* Configure PA10 for USART Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure PA9 for USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
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
	USART_ClearFlag(USART1, USART_FLAG_TC);
}


void USART_Puts(unsigned char *data)
{
	int i=0;
	for (i = 0; data[i] != '\0'; i++)
	{
		  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
			USART_SendData( USART1, data[i]);
	}
}