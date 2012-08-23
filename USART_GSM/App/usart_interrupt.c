/**********************************************************************
* $Id$		usart_interrupt.c					
*//**
* @file		usart_interrupt.c
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
#include "usart_interrupt.h"
#include "stm32f10x_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TxBufferSize   (countof(TxBuffer) - 1)
#define RxBufferSize   0x20
/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))
#define rxBufferSize 8
/* Private variables ---------------------------------------------------------*/
uint8_t TxBuffer[] = "\n\rUSART Hyperterminal Interrupts Example: USART-Hyperterminal\
 communication using Interrupt\n\r";
uint8_t RxBuffer[RxBufferSize];
uint8_t NbrOfDataToTransfer = TxBufferSize;
uint8_t NbrOfDataToRead = RxBufferSize;
uint8_t TxCounter = 0; 
uint16_t RxCounter = 0; 

uint8_t rxBufferOverflow;
uint16_t rxBuffer[rxBufferSize];
uint16_t rxWriteIndex, rxReadIndex, rxCounter;
uint16_t Data;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            STM32F107 Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles USART1 global interrupt request.
  * @param  None
  * @retval None
  */


#if 0
void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    RxBuffer[RxCounter++] = (USART_ReceiveData(USART1) & 0x7F);

    if(RxCounter == NbrOfDataToRead)
    {
      /* Disable the EVAL_COM1 Receive interrupt */
      USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
    }
  }



}
#endif
void USART1_IRQHandler(void) //USART1_IRQHandler
{

  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // Wait for Char
  {
    Data = (USART_ReceiveData(USART1)  & 0x7F ); // Collect Char
    rxBuffer[rxWriteIndex] = Data;
    if (++rxWriteIndex == rxBufferSize) rxWriteIndex = 0;
   if (++rxCounter == rxBufferSize)
      {
      rxCounter = 0;
      rxBufferOverflow = 1;
			
      }
    USART_ClearITPendingBit(USART1,USART_IT_RXNE);
  }
    
     
}


uint16_t USART_GetChar(void)
{
  uint16_t data;
  while (rxCounter==0);
  data = rxBuffer[rxReadIndex];
  if (++rxReadIndex == rxBufferSize) rxReadIndex=0;
  --rxCounter;
  return data;
}


