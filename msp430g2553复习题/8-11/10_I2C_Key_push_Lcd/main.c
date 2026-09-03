/*警告：由于G2 LaunchPad上的LED2使用了P1.6（I2C引脚），所以所有涉及到I2C的实验都必须把P1.6跳线拔除,包含本例程！
 * main.c
 */
#include"MSP430G2553.h"
#include"TCA6416A.h"
#include "LCD_128.h"
#include"HT1621.h"

void WDT_init();
void I2C_IODect()	;		                 //检测事件确实发生了

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;	 //关狗
    BCSCTL1 = CALBC1_12MHZ;      //Set DCO to 12MHz
    DCOCTL = CALDCO_12MHZ;
    TCA6416A_Init();			// 初始化IO扩展口
    HT1621_init();
//----提示初始化成功----
	PinOUT(0,1);           // 指定0号管脚输出为1
	PinOUT(1,1);           // 指定1号管脚输出为0
	PinOUT(2,1);           // 指定2号管脚输出为1
	PinOUT(3,1);           // 指定3号管脚输出为0
	PinOUT(4,1);           // 指定4号管脚输出为1
	PinOUT(5,1);           // 指定5号管脚输出为0
	PinOUT(6,1);           // 指定6号管脚输出为1
	PinOUT(7,1);           // 指定7号管脚输出为0
//--LCD清零
	LCD_DisplayDigit(LCD_DIGIT_CLEAR,1);
	LCD_DisplayDigit(LCD_DIGIT_CLEAR,2);
	LCD_DisplayDigit(LCD_DIGIT_CLEAR,3);
	LCD_DisplayDigit(LCD_DIGIT_CLEAR,4);
	LCD_DisplayDigit(LCD_DIGIT_CLEAR,5);
	LCD_DisplayDigit(LCD_DIGIT_CLEAR,6);
	//-----更新缓存，真正显示-----
	HT1621_Reflash(LCD_Buffer);
	WDT_init();
	while(1)
	{
		PinIN();
		I2C_IODect();
		_bis_SR_register(LPM0_bits + GIE);
	}


}
/******************************************************************************************************
 * 名       称：WDT_init()
 * 功       能：初始化WDT定时中断为16ms
 * 入口参数：无
 * 出口参数：无
 * 说       明：无
 * 范       例：无
 ******************************************************************************************************/
void WDT_init()
{
	//-----设定WDT为-----------
	WDTCTL=WDT_ADLY_16;
	//-----WDT中断使能----------------------
    IE1 |= WDTIE;
}
/******************************************************************************************************
 * 名       称：WDT_ISR()
 * 功       能：WDT定时中断服务
 * 入口参数：无
 * 出口参数：无
 * 说       明：无
 * 范       例：无
 ******************************************************************************************************/
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
  _bic_SR_register_on_exit(LPM0_bits );
}
/******************************************************************************************************
 * 名       称： I2C_IODect()
 * 功       能：TCA6416A的扩展IO事件检测函数
 * 入口参数：无
 * 出口参数：无
 * 说       明：检测具体哪个扩展IO被按下
 * 范       例：无
 ******************************************************************************************************/
void I2C_IODect()			                 //检测事件确实发生了
{
	//----判断I2C_IO10所连的KEY1按键是否被按下------
	if((TCA6416A_InputBuffer&0x0f00)!=0)
		LCD_DisplayDigit(LCD_DIGIT_CLEAR,6);  //没键按下，清LCD
	if((TCA6416A_InputBuffer&BIT8) == 0)
		LCD_DisplayDigit(1,6);
	if((TCA6416A_InputBuffer&BIT9)== 0)
		LCD_DisplayDigit(2,6);
	if((TCA6416A_InputBuffer&BITA) == 0)
		LCD_DisplayDigit(3,6);
	if((TCA6416A_InputBuffer&BITB) == 0)
		LCD_DisplayDigit(4,6);
	HT1621_Reflash(LCD_Buffer);
}
