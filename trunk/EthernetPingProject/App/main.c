#include <stm32f10x_cl.h>
#include <Net_Config.h>


BOOL tick;
static void timer_poll () {
  /* System tick timer running in poll mode */

  if (SysTick->CTRL & 0x10000) {
    /* Timer tick every 100 ms */
    timer_tick ();
    tick = __TRUE;
  }
}
int main (void)  { 
	
  //timer_init (); 
  init_TcpNet (); 
  while (1)  { 
    timer_poll (); 
    main_TcpNet (); 
  } 
} 