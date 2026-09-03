#include <msp430G2553.h>
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P1DIR |= BIT0;                            // Set P1.0 to output direction
  P1OUT &= ~BIT0;
  P1DIR &= ~BIT6;                           // Set P1.6 to input direction
  P1REN |= BIT6;		     		        //启用P1.3内部上下拉电阻
  P1OUT |= BIT6;			                //将电阻设置为上拉
  while (1)                                  // Test P1.3
  {
   	if(BIT6 & P1IN) P1OUT &= ~BIT0;          //
    else
    {


    	while (!(BIT6 & P1IN))
    	{
    		P1OUT ^= BIT0;                      // else set
    		_delay_cycles(100000);
    		P1OUT ^= BIT0;                      // else set
    		_delay_cycles(900000);
    	}
     }
  }
}
