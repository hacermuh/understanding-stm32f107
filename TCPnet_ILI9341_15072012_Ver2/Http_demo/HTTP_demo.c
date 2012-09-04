/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    HTTP_DEMO.C
 *      Purpose: HTTP Server demo example
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2009 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <RTL.h>
#include <Net_Config.h>
#include <stm32f10x_cl.h>
#include "GLCD.h"
#include "logo_RGB565.h"
#include <string.h>

BOOL LEDrun;
BOOL LCDupdate;
BOOL tick;
U32  dhcp_tout;
U8 tcp_soc;
U8 soc_state;
BOOL wait_ack;
U8   lcd_text[2][16+1] = {" ",                /* Buffer for LCD text         */
                          "Waiting for DHCP"};

//char *data_buf = "HELLO";
char *data1_buf = "Hello The World!!";
static U8 rem_IP[4] = {192,168,1,18};
extern LOCALM localm[];                       /* Local Machine Settings      */
#define MY_IP localm[NETIF_ETH].IpAdr
#define DHCP_TOUT   50                        /* DHCP timeout 5 seconds      */
#define msgLength 5
extern uint16_t ASCII_Table[];

static void init_io (void);
static void init_display (void);
 void send_data (U8 *data_buf, U8 data_length); 
BOOL send_datalog ();
U16 tcp_callback (U8 soc, U8 event, U8 *ptr, U16 par);
/*--------------------------- init ------------------------------------------*/

static void init () {
  /* Add System initialisation code here */ 

  init_io ();
  init_display ();
  init_TcpNet ();

  /* Setup and enable the SysTick timer for 100ms. */
  SysTick->LOAD = (SystemFrequency / 10) - 1;
  SysTick->CTRL = 0x05;
}



/*--------------------------- timer_poll ------------------------------------*/

static void timer_poll () {
  /* System tick timer running in poll mode */

  if (SysTick->CTRL & 0x10000) {
    /* Timer tick every 100 ms */
    timer_tick ();
    tick = __TRUE;
  }
}

/*--------------------------- init_io ---------------------------------------*/

static void init_io () {

  /* Set the clocks. */
  SystemInit();
  RCC->APB2ENR |= 0x00000279;

  /* Configure the GPIO for Push Buttons */
  GPIOB->CRH &= 0xFFFFFF0F;
  GPIOB->CRH |= 0x00000040;
  GPIOC->CRH &= 0xFFF0FFFF;
  GPIOC->CRH |= 0x00040000;

  /* Configure the GPIO for LEDs. */
  GPIOD->CRL &= 0xFFF00FFF;
  GPIOD->CRL |= 0x00033000;
  GPIOD->CRH &= 0xFF0FFFFF;
  GPIOD->CRH |= 0x00300000;
  GPIOE->CRH &= 0xF0FFFFFF;
  GPIOE->CRH |= 0x03000000;

  /* Configure UART2 for 115200 baud. */
  AFIO->MAPR |= 0x00000008;
  GPIOD->CRL &= 0xF00FFFFF;
  GPIOD->CRL |= 0x04B00000;

  RCC->APB1ENR |= 0x00020000;
  USART2->BRR = 0x0135;
  USART2->CR3 = 0x0000;
  USART2->CR2 = 0x0000;
  USART2->CR1 = 0x200C;

  /* Configure ADC.14 input. */
  GPIOC->CRL &= 0xFFF0FFFF;
  ADC1->SQR1  = 0x00000000;
  ADC1->SMPR1 = (5<<12);
  ADC1->SQR3  = (14<<0);
  ADC1->CR1   = 0x00000100;
  ADC1->CR2   = 0x000E0003;

  /* Reset calibration */
  ADC1->CR2  |= 0x00000008;
  while (ADC1->CR2 & 0x00000008);

  /* Start calibration */
  ADC1->CR2  |= 0x00000004;
  while (ADC1->CR2 & 0x00000004);
  ADC1->CR2  |= 0x00500000;
}


/*--------------------------- fputc -----------------------------------------*/

int fputc (int ch, FILE *f)  {
  /* Debug output to serial port. */

  if (ch == '\n')  {
    while (!(USART2->SR & 0x0080));
    USART2->DR = 0x0D;
  }
  while (!(USART2->SR & 0x0080));
  USART2->DR = (ch & 0xFF);
  return (ch);
}

/*--------------------------- LED_out ---------------------------------------*/

void LED_out (U32 val) 
{
  U32 rv;

  rv = 0;
  if (val & 0x01) rv |= 0x00004000;
  GPIOE->BSRR = rv;
  GPIOE->BRR  = rv ^ 0x00004000;

  rv = 0;
  if (val & 0x02) rv |= 0x00002000;
  if (val & 0x04) rv |= 0x00000008;
  if (val & 0x08) rv |= 0x00000010;
  GPIOD->BSRR = rv;
  GPIOD->BRR  = rv ^ 0x0002018;
}


/*--------------------------- AD_in -----------------------------------------*/

U16 AD_in (U32 ch) 
{
  /* Read ARM Analog Input */
  U32 val = 0;

  if (ch < 1) {
    val = ADC1->DR & 0x0FFF;
  }
  return (val);
}


/*--------------------------- get_button ------------------------------------*/

U8 get_button (void) 
{
  /* Read ARM Digital Input */
  U32 val = 0;

  if ((GPIOB->IDR & (1 << 9)) == 0) {
    /* Key button */
    //val |= 0x01;
		val =1;
		
  }
  if ((GPIOC->IDR & (1 << 13)) == 0) {
    /* Wakeup button */
    //val |= 0x02;
		val =2; 
  }
  return (val);
}


/*--------------------------- upd_display -----------------------------------*/
/*
static void upd_display () {
  // Update LCD Module display text. 

  GLCD_clearLn (Line7);
  GLCD_displayStringLn(Line7, lcd_text[0]);
  GLCD_clearLn (Line8);
  GLCD_displayStringLn(Line8, lcd_text[1]);

  LCDupdate =__FALSE;
}
*/

/*--------------------------- init_display ----------------------------------*/

static void init_display () 
{
  /* LCD Module init */

  GLCD_init();

  GLCD_clear(Navy);
  GLCD_setBackColor(Navy);
  GLCD_setTextColor(White);
    
  GLCD_displayStringLn(Line0, "       VBEB Co.,Ltd");
  GLCD_displayStringLn(Line1, "        TCP/IP Test");

  GLCD_bitmap (5, 10, 95, 35, VBEBLogo); 

//  upd_display ();

}


/*--------------------------- dhcp_check ------------------------------------*/

static void dhcp_check () 
{
  /* Monitor DHCP IP address assignment. */

  if (tick == __FALSE || dhcp_tout == 0) {
    return;
  }

  if (mem_test (&MY_IP, 0, IP_ADRLEN) == __FALSE && !(dhcp_tout & 0x80000000)) {
    /* Success, DHCP has already got the IP address. */
    dhcp_tout = 0;
    sprintf((char *)lcd_text[0],"IP address:");
    sprintf((char *)lcd_text[1],"%d.%d.%d.%d", MY_IP[0], MY_IP[1],
                                               MY_IP[2], MY_IP[3]);
    LCDupdate = __TRUE;
    return;
  }
  if (--dhcp_tout == 0) {
    /* A timeout, disable DHCP and use static IP address. */
//    dhcp_disable ();
    sprintf((char *)lcd_text[1]," DHCP failed    " );
    LCDupdate = __TRUE;
    dhcp_tout = 30 | 0x80000000;
    return;
  }
  if (dhcp_tout == 0x80000000) {
    dhcp_tout = 0;
    sprintf((char *)lcd_text[0],"IP address:");
    sprintf((char *)lcd_text[1],"%d.%d.%d.%d", MY_IP[0], MY_IP[1],
                                               MY_IP[2], MY_IP[3]);
    LCDupdate = __TRUE;
  }
}


/*--------------------------- blink_led -------------------------------------*/

static void blink_led () 
{
  /* Blink the LEDs on an eval board */
  const U8 led_val[8] = { 0x01,0x03,0x07,0x0F,0x0E,0x0C,0x08,0x00 };
  static U32 cnt;

  if (tick == __TRUE) {
    /* Every 100 ms */
    tick = __FALSE;
    if (LEDrun == __TRUE) {
      LED_out (led_val[cnt]);
      if (++cnt >= sizeof(led_val)) {
        cnt = 0;
      }
    }
    if (LCDupdate == __TRUE) {
      //upd_display ();
    }
  }
}

/*---------------------------------------------------------------------------*/

int main (void) 
{
  /* Main Thread of the TcpNet */
	int i=1;
	U32 buttonget;
  init ();
	
 // printf ("Program started\n");
  //LEDrun = __TRUE;
  ///dhcp_tout = DHCP_TOUT;

  tcp_soc = tcp_get_socket (TCP_TYPE_CLIENT, 0, 120, tcp_callback);
  soc_state = 0;
	//tcp_connect (tcp_soc, rem_IP, 2020, 0);
	//send_datalog ();
	//send_data ();
	while (1) 
		{
			timer_poll ();
			main_TcpNet ();
			buttonget = get_button();
			if ( buttonget == 2)
			{
				
			  send_data ("SANG",4);
			}
	  }
}

void send_data (U8 *data_buf, U8 data_length) 
{
  
  static int bcount;
	//int i;
	U32 max;
  U8 *sendbuf;

  switch (soc_state) {
    case 0:
      tcp_connect (tcp_soc, rem_IP, 2020, 0);
      bcount    = 0;
      wait_ack  = __FALSE;
      soc_state = 1;
			GLCD_clearLn (Line8);
			GLCD_displayStringLn(Line8, "Connecting...");
      return;
		case 2:
      
			if (wait_ack == __TRUE) {
        return;
      }
			
			if (tcp_check_send (tcp_soc))
				{
      //max = tcp_max_dsize (tcp_soc);
      sendbuf = tcp_get_buf (data_length);
      
			/*for (i = 0; i < max; i += 2) {
        sendbuf[i]   = bcount >> 8;
        sendbuf[i+1] = bcount & 0xFF;
        if (bcount >= 32768) {
          soc_state = 3;
          break;
        }
      }*/
			memcpy (sendbuf, data_buf, data_length);
      tcp_send (tcp_soc, sendbuf, data_length);
			//tcp_send (tcp_soc, sendbuf, max);
			GLCD_clearLn (Line8);
			GLCD_displayStringLn(Line8, "Message Sent");
      wait_ack = __TRUE;
      return;
				}
			case 3:
      if (wait_ack == __TRUE) {
        return;
      }
      tcp_close (tcp_soc);
      soc_state = 4;
      return;
  }
}
U16 tcp_callback (U8 soc, U8 event, U8 *ptr, U16 par) 
{
  /* This function is called on TCP event */
	U8 data_get;
  switch (event) {
     
    case TCP_EVT_CONNECT:
      /* Socket is now connected and ready to send data. */
      soc_state = 2;
      break;
    case TCP_EVT_ACK:
      /* Our sent data has been acknowledged by remote peer */
      wait_ack = __FALSE;
      break;
		case TCP_EVT_DATA:
      /* TCP data frame has been received, 'ptr' points to data */
      /* Data length is 'par' bytes */
       data_get = *ptr;
      break;
       
		
  }
  return (0);
}
			
BOOL send_datalog () 
{
  U8 *sendbuf;
  U16 maxlen;

  if (tcp_check_send (tcp_soc)) {
    /* The socket is ready to send the data. */
    maxlen = tcp_max_dsize (tcp_soc);
    sendbuf = tcp_get_buf (maxlen);
    memcpy (sendbuf, data1_buf, maxlen);
    tcp_send (tcp_soc, sendbuf, maxlen);
    return (__TRUE);
  }
  return (__FALSE);
}
/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
