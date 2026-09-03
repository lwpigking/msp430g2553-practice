#include <msp430.h>

int t=65536-32768/10;
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR |= BIT0;                            // P1.0 output
  TA1R = t;
  TA1CTL = TASSEL_1 + MC_2 + TAIE;           // ACLK, contmode, interrupt
  _BIS_SR(LPM3_bits + GIE);                 // Enter LPM3 w/ interrupt
}

// Timer_A3 Interrupt Vector (TA0IV) handler
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer_A(void)
{
  switch( TA1IV )
  {
    case  2:  break;                        // CCR1 not used
    case  4:  break;                        // CCR2 not used
    case 10:  P1OUT ^= BIT0;                // overflow
              TA1R = t;
              break;
  }
}
