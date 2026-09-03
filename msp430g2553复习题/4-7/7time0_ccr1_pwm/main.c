#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR |= BIT2;                            // P1.2 output
  P1SEL |= BIT2;                            // P1.2 options
  CCR0 = 32768/2-1;                         // PWM Period
  CCTL1 = OUTMOD_7;                         // CCR1 reset/set
  CCR1 = (32768/2-1)/10;                    // CCR1 PWM duty cycle
  TACTL = TASSEL_1 + MC_1;                  // AMCLK, up mode
  _BIS_SR(CPUOFF);                          // Enter LPM0
}
