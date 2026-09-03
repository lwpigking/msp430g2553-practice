#include <msp430.h> 
/**
 * P1OUT &= ~BIT0: LED0灭
 * P1OUT ^= BIT0: LED0灭或者亮，交替
 * P1OUT |= BIT0: LED0量
 */
/**
 * 4. 编程实现用P1.6按键点动控制P1.0的LED灯，按下时LED灯0.1秒亮，0.9秒灭周期闪烁，释放时熄灭
 * 5. 编程实现利用Timer1主定时器连续模式产生溢出定时中断，定时长度0.1秒。用该中断控制P1.0引脚的LED灯闪烁，0.1秒亮、0.1秒灭。
 * 6. 编程实现利用Timer1主定时器UP模式，通道0的定时中断，定时长度0.1秒。用该中断控制P1.0引脚的LED灯闪烁，0.1秒亮、0.1秒灭。
 * 7. 程实现Timer0_A3的通道1的PWM功能（用P1.2脚）控制LED灯，周期为0.5秒，占空比为10%，用LED灯观察。
 */
void Blink();
int t = 65536-32768/10;
int main(void) {
	// 四、编程实现用P1.6按键点动控制P1.0的LED灯，按下时LED灯0.1秒亮，0.9秒灭周期闪烁，释放时熄灭
//	WDTCTL = WDTPW + WDTHOLD;
//	P1DIR |= BIT0;
//	P1OUT &= ~BIT0;
//
//	P1DIR &= ~BIT6;
//	P1REN |= BIT6;
//	P1OUT |= BIT6;
//
//	while (1) {
//		// 没按下
//		if ((BIT6 & P1IN)) {
//			P1OUT &= ~BIT0;
//			// 按下
//			} else {
//				while(!(BIT6 & P1IN)) {
//					P1OUT ^= BIT0;
//					_delay_cycles(100000);
//					P1OUT ^= BIT0;
//					_delay_cycles(900000);
//				}
//			}
//	}

	// 五、编程实现利用Timer1主定时器连续模式产生溢出定时中断，定时长度0.1秒。用该中断控制P1.0引脚的LED灯闪烁，0.1秒亮、0.1秒灭。
//	WDTCTL = WDTPW + WDTHOLD;
//	P1DIR |= BIT0;
//	TA1R = t;
//	// 配置Timer1_A
//	TA1CTL = TASSEL_1 + MC_2 + TAIE;
//	_BIS_SR(LPM3_bits + GIE);

	// 六、编程实现利用Timer1主定时器UP模式，通道0的定时中断，定时长度0.1秒。用该中断控制P1.0引脚的LED灯闪烁，0.1秒亮、0.1秒灭。
//	WDTCTL = WDTPW + WDTHOLD;
//	P1DIR |= BIT0;
//	TA1CCTL0 = CCIE;
//	TA1CCR0 = 32678/10;
//	// 配置Timer1_A
//	TA1CTL = TASSEL_1 + MC_1;
//	_BIS_SR(LPM0_bits + GIE);

	// 七、程实现Timer0_A3的通道1的PWM功能（用P1.2脚）控制LED灯，周期为0.5秒，占空比为10%，用LED灯观察。
	WDTCTL = WDTPW + WDTHOLD;
	P1DIR |= BIT2;
	P1SEL |= BIT2;
	CCR0 = 32678/2 - 1;
	CCTL1 = OUTMOD_7;
	CCR1 = (32768/2 - 1)/10;
	TACTL = TASSEL_1 + MC_1;
	_BIS_SR(CPUOFF);


	return 0;
}

// 定时器1中断服务程序
//#pragma vector =TIMER1_A1_VECTOR
//__interrupt void Timer_A(void) {
//	switch(TA1IV) {
//	case 2: break;
//	case 4: break;
//	case 10:
//		P1OUT ^= BIT0;
//		TA1R = t;
//		break;
//	}
//}

//#pragma vector = TIMER1_A0_VECTOR
//__interrupt void Timer_A(void) {
//	P1OUT ^= BIT0;
//}
