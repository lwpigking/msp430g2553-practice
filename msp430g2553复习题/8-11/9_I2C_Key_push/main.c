/*警告：由于G2 LaunchPad上的LED2使用了P1.6（I2C引脚），所以所有涉及到I2C的实验都必须把P1.6跳线拔除,包含本例程！
 * main.c
 */
#include"MSP430G2553.h"
#include"TCA6416A.h"

void WDT_init();
void I2C_IODect()	;		                 //检测事件确实发生了

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;	//关狗
    BCSCTL1 = CALBC1_12MHZ;      /* Set DCO to 12MHz */
    DCOCTL = CALDCO_12MHZ;
    TCA6416A_Init();			// 初始化IO扩展口
//----提示初始化成功----
	PinOUT(0,1);           // 指定输出为1，灯灭
	PinOUT(1,1);
	PinOUT(2,1);
	PinOUT(3,1);
	PinOUT(4,1);
	PinOUT(5,1);
	PinOUT(6,1);
	PinOUT(7,1);
	WDT_init();
	while(1)
	{
		PinIN();
		I2C_IODect();
		_bis_SR_register(LPM0_bits);
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
	if((TCA6416A_InputBuffer&BIT8) == BIT8)
		PinOUT(0,1);
	else
		PinOUT(0,0);
	//----判断I2C_IO11所连的KEY2按键是否被按下------
	if((TCA6416A_InputBuffer&BIT9)== BIT9)
		PinOUT(1,1);
	else
		PinOUT(1,0);
	//----判断I2C_IO12所连的KEY3按键是否被按下------
	if((TCA6416A_InputBuffer&BITA) == BITA)
		PinOUT(2,1);
	else
		PinOUT(2,0);
	//----判断I2C_IO13所连的KEY4按键是否被按下------
	if((TCA6416A_InputBuffer&BITB) ==  BITB)
		PinOUT(3,1);
	else
		PinOUT(3,0);
}
