///*警告：由于G2 LaunchPad上的LED2使用了P1.6（I2C引脚），所以所有涉及到I2C的实验都必须把P1.6跳线拔除,包含本例程！
// * main.c
// */#include <msp430.h>
//#include "LCD_128.h"
//#include "HT1621.h"
//#include "TCA6416A.h"
//
//long temp;
//long IntDeg;
//void ADC10_ISR(void);
//void ADC10_init(void);
//void LCD_Init();
//void LCD_Display();
//
//void main()
//{
//	// 十二、编程实现对单片机电源电压的检测，结果保留两位小数，显示在LCD屏上。
//	WDTCTL=WDTPW+WDTHOLD;
//	ADC10_init();
//	LCD_Init();
//	HT1621_Reflash(LCD_Buffer);
//	while(1)
//    {
//      ADC10CTL0 |= ENC + ADC10SC;                // Sampling and conversion start
//      _bis_SR_register(CPUOFF + GIE);       	 // LPM0 with interrupts enabled
//      //-----ADC转换完成中断唤醒CPU后才执行以下代码-----
//      temp = ADC10MEM;							//读取AD采样值
//      IntDeg= temp*500/1024;		            //转换为电压值，2.5V参考电压100倍处理，再乘2
//      LCD_Display();							//调用LCD显示函数
//    }
//}
///******************************************************************************************************
// * 名       称：LCD_Init()
// * 功       能：初始化LCD显示相关的硬件，并预设固定不变的显示内容
// * 入口参数：无
// * 出口参数：无
// * 说       明：预设显示内容小数点。
// * 范       例：无
// ******************************************************************************************************/
//void LCD_Init()
//{
//    TCA6416A_Init();
//    HT1621_init();
//    LCD_DisplayDigit(LCD_DIGIT_CLEAR,6);
//    LCD_DisplayDigit(LCD_DIGIT_CLEAR,5);
//    LCD_DisplayDigit(LCD_DIGIT_CLEAR,4);
//	LCD_DisplaySeg(52);		//小数点
//}
///******************************************************************************************************
// * 名       称：LCD_Displaly()
// * 功       能：将温度值显示出来
// * 入口参数：无
// * 出口参数：无
// * 说       明：包括对负温度的处理、拆分数字等几部分
// * 范       例：无
// ******************************************************************************************************/
//void LCD_Display()
//{
//	      LCD_DisplayDigit(LCD_DIGIT_CLEAR,5);
//	      LCD_DisplayDigit(LCD_DIGIT_CLEAR,6);
//	      LCD_DisplayDigit(LCD_DIGIT_CLEAR,4);
//	      //-----拆分3位并显示数字-----
//	      LCD_DisplayDigit(IntDeg/100,4);
//	      LCD_DisplayDigit((IntDeg%100)/10,5);
//	      LCD_DisplayDigit((IntDeg%100)%10,6);
//	      //-----更新缓存，真正显示-----
//	      HT1621_Reflash(LCD_Buffer);
//}
///******************************************************************************************************
// * 名       称：ADC10_init(void)
// * 功       能：初始化ADC10采集内部温度传感器，单次手动采样
// * 入口参数：无
// * 出口参数：无
// * 说       明：直接从CCS的Example中移植过来
// * 范       例：无
// ******************************************************************************************************/
//void ADC10_init(void)
//{
//	ADC10CTL0 &= ~ENC;
//	ADC10CTL0 = ADC10IE + ADC10ON + REFON + REF2_5V + SREF_1;
//    ADC10CTL1 = INCH_11;
//    ADC10CTL0 |= ENC;
//}
///******************************************************************************************************
// * 名       称：ADC10_ISR_HOOK
// * 功       能：ADC转换完成后唤醒CPU
// * 入口参数：无
// * 出口参数：无
// * 说       明：直接从CCS的Example中移植过来
// * 范       例：无
// ******************************************************************************************************/
//#pragma vector=ADC10_VECTOR
//__interrupt void ADC10_ISR_HOOK(void)
//{
//	_bic_SR_register_on_exit(LPM0_bits);
//}


// 编程实现P1.3按键控制P1.0的LED灯，按一次灯亮0.2秒，程序采用低功耗结构。
#include <msp430.h>

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    P1DIR &= ~BIT3;             //SET P1.3 input
    P1REN |= BIT3;
    P1OUT |= BIT3;
    P1IE  |= BIT3;
    P1IES |= BIT3;
    P1DIR |= BIT0;              //set P1.0 output
    P1OUT &= ~BIT0;
   	_bis_SR_register(LPM0_bits+GIE);
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
  P1OUT |= BIT0;
  _delay_cycles(200000);
  P1OUT &= ~BIT0;
  P1IFG &= ~BIT3;
  _bic_SR_register_on_exit(LPM0_bits);
}
