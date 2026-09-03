#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR |= BIT0;                            // P1.0 output
  TA1CCTL0 = CCIE;                             // CCR0 interrupt enabled
  TA1CCR0 = 32768/10;
  TA1CTL = TASSEL_1 + MC_1;                  // ACLK, upmode
  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}

// Timer A0 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer_A (void)
{
  P1OUT ^= BIT0;                             // Toggle P1.0
}
