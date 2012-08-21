/************************************************
*
*
************************************************/
#include "stm32f10x.h"
#include "stm32f10x_conf.h"

GPIO_InitTypeDef  GPIO_InitStructure;

void Delay(__IO uint32_t nCount);

int main(void)
{
	/*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
        system_stm32f4xx.c file
     */

  /* GPIOD Periph clock enable */
  //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

  /* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  while (1)
  {
    /* PD12 to be toggled */
    GPIO_SetBits(GPIOD, GPIO_Pin_2);
    
    /* Insert delay */
    Delay(0x3FFFFF);
    
    /* PD13 to be toggled */
    GPIO_SetBits(GPIOD, GPIO_Pin_3);
    
    /* Insert delay */
    Delay(0x3FFFFF);
  
    /* PD14 to be toggled */
    GPIO_SetBits(GPIOD, GPIO_Pin_4);
    
    /* Insert delay */
    Delay(0x3FFFFF);
    
    /* PD15 to be toggled */
    GPIO_SetBits(GPIOD, GPIO_Pin_7);
    
    /* Insert delay */
    Delay(0x7FFFFF);
    
    GPIO_ResetBits(GPIOD, GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7);
    
    /* Insert delay */
    Delay(0xFFFFFF);
	}
	return 0;
}

void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}
