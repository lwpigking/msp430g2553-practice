#include <msp430.h> 

/*下降沿可用P1.3按键产生 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
   	//-----初始化TA1为SMCLK 输入，连续模式，通道1停止，通道2捕获(P2.4)-----
   	P2SEL |= BIT4;
    P2DIR &= ~BIT4;                                           //SET P2.4 capture input
    P2REN |= BIT4;
    P2OUT |= BIT4;
    TA1CTL = TASSEL_2 + MC_2 + TACLR;
    TA1CCTL2 = CM_2 + CCIS_0 + CAP + SCS + CCIE;              //下降沿捕获
   	TA1CCTL2 &= ~COV;
   	P1DIR |= BIT0;                                            //set P1.0 output
   	_bis_SR_register(LPM0_bits+GIE);
}

#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer_A1(void)
{
  int a;
  switch(TA1IV)
  {
  case  TA1IV_TACCR1: break;                      // CCR1 not used
  case  TA1IV_TACCR2:                             // CCR2
	    a = TA1CCR2;                                 //-----启用中断服务函数-----
	    P1OUT ^= BIT0;                            // capture indicate
        break;
  case TA1IV_TAIFG: break;
  default :  break;
  }
}


