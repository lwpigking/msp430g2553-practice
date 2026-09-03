#include <msp430.h> 

/**
 * 1. P1.0点亮LED，进入LPM3模式
 * 2. P1.0点亮LED一秒钟，进入LPM3模式
 * 3. P1.x引脚控制一个LED灯的闪烁，周期1秒（0.1秒亮，0.9秒灭）
 * 4. P1.3点动控制P1.6的LED灯控制
 * 5. 设置P1.0、P1.6引脚为按键，P1.2、P1.4为输出，分别控制2个LED灯。P1.0键按住，P1.2的LEd亮，释放灭；P1.6键按住，P1.4的LED灭，释放亮
 */
int main(void) {
//    WDTCTL = WDTPW | WDTHOLD;

//    // 一、
//    // 1. 配置引脚方向(方向寄存器)
//    // 2. 配置引脚输出(IO寄存器)
//    // P1.0方向
//    P1DIR |= BIT0;
//    // P1.0输出(P1.0为高电平)
//    P1OUT |= BIT0;
//    // LPM3模式(低功耗休眠模式)
//    _bis_SR_register(LPM3_bits);

//    // 二、
//    P1DIR |= BIT0;
//    // 点亮LED
//    P1OUT |= BIT0;
//    // 延时一秒为1M(一百万)!!!!!!!
//    _delay_cycles(1000000);
//    // 关闭LED
//    P1OUT &= ~BIT0;
//    _bis_SR_register(LPM3_bits);

//    // 三、
//    P1DIR |= BIT0;
//    while(1) {
//    	P1OUT ^= BIT0;
//    	_delay_cycles(100000);
//    	P1OUT ^= BIT0;
//    	_delay_cycles(900000);
//    }

//    // 四、按下亮，不按灭
//	WDTCTL = WDTPW + WDTHOLD;
//    P1DIR |= BIT6;
//    // P1.3方向配置,P1.3为输入
//    P1DIR &= ~BIT3;
//    // 上拉电阻，保证不按下的情况下是高电平
//    // 下拉：接地
//    // 上拉：Vcc
//    P1REN |= BIT3;
//    P1OUT |= BIT3;
//    P1OUT &= ~BIT6;
//    while (1) {
//    	// 没按下
//    	if ((BIT3 & P1IN)) {
//    		P1OUT &= ~BIT6;
//    	} else {
//    		P1OUT |= BIT6;
//    	}
//    }

	// 五:设置P1.0、P1.6引脚为按键，P1.2、P1.4为输出，分别控制2个LED灯。P1.0键按住，P1.2的LEd亮，释放灭；P1.6键按住，P1.4的LED灭，释放亮
	WDTCTL = WDTPW + WDTHOLD;
    // P1.0为输入
    P1DIR &= ~BIT0;
    // P1.6为输入
    P1DIR &= ~BIT6;
    // 启用P1.0和P1.6的上拉/下拉电阻
    P1REN |= BIT0 + BIT6;
    // 设置为上拉电阻
    P1OUT |= BIT0 + BIT6;

    // p1.2，p1.4输出
    P1DIR |= BIT2 + BIT4;



    while (1) {
    	// 检测P1.0按键是否按下（低电平有效）
    	// P1.0没被按下
    	if (P1IN & BIT0) {
    		P1OUT &= ~BIT2;
    	} else {
    		P1OUT |= BIT2;
    	}

    	 // 检测P1.6按键是否按下（低电平有效）
    	 if (P1IN & BIT6) {
    		 P1OUT &= ~BIT4;
    	 } else {
    		 P1OUT |= BIT4;
    	 }
    }

	return 0;
}
