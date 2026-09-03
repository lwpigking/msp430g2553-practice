#include <msp430.h> 
#include <stdint.h>

/**
 * 初始化系统时钟
 */
void InitSystemClock(void) {
	// 配置DCO为1MHZ
	DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
    // 配置SMCLK的时钟源为DCO
    BCSCTL2 &= ~SELS;
    // SMCLK的分频系数
    BCSCTL2 &= ~(DIVS0 | DIVS1);
}


/**
 * 初始化串口
 */
void InitUART(void) {
	// 复位USCI_Ax
	UCA0CTL1 |= UCSWRST;
	// 选择USCI_Ax为UART模式
    UCA0CTL0 &= ~UCSYNC;

    // 配置UART时钟源为SMCLK
    UCA0CTL1 |= UCSSEL1;

    // 设置波特率为9600MHZ
    UCA0BR0 = 0x68;
    UCA0BR1 = 0x00;
    UCA0MCTL = 1 << 1;

    // 使能端口复用
    P1SEL |= BIT1 + BIT2;
    P1SEL2 |= BIT1 + BIT2;
    // 清除复位，使能UART
    UCA0CTL1 &= ~UCSWRST;
}

/**
 * 通过串口发送字符串
 * @Param pbuff 指向要发送字符串的指针
 * @Param num 要发送的字符个数
 */
void UARTSendString(uint8_t *pbuff, uint8_t num) {
	uint8_t cnt = 0;
	for (cnt = 0; cnt < num; cnt++) {
		while(UCA0STAT & UCBUSY)
		UCA0TXBUF = *(pbuff + cnt);
	}
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    InitSystemClock();
    InitUART();

    while(1) {
    	UARTSendString("ASDFGH", 6);
    	__delay_cycles(500000);
    }




    return 0;
}
