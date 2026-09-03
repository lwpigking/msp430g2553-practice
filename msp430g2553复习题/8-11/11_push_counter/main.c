#include "MSP430G2553.h"
#include"TCA6416A.h"
#include "LCD_128.h"
#include"HT1621.h"


//-----在main函数前提前申明函数-----
void P1_IODect();
void GPIO_init();
void WDT_init();

int counter=0;

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;	//关狗
	GPIO_init();
	WDT_init();
	BCSCTL1 = CALBC1_12MHZ;      //Set DCO to 12MHz
    DCOCTL = CALDCO_12MHZ;
	TCA6416A_Init();			// 初始化IO扩展口
	HT1621_init();
	LCD_DisplayDigit(LCD_DIGIT_CLEAR,1);
	LCD_DisplayDigit(LCD_DIGIT_CLEAR,2);
	LCD_DisplayDigit(LCD_DIGIT_CLEAR,3);
	LCD_DisplayDigit(LCD_DIGIT_CLEAR,4);
	LCD_DisplayDigit(LCD_DIGIT_CLEAR,5);
	LCD_DisplayDigit(LCD_DIGIT_CLEAR,6);
		//-----更新缓存，真正显示-----
	HT1621_Reflash(LCD_Buffer);
	_enable_interrupts();							//开总中断
	_bis_SR_register(LPM3_bits);				//LPM3休眠
}

void GPIO_init()
{
	//-----设定P1.0和P1.3-----
	P1DIR &= ~BIT3;    				    //设定P1.3为输出，其它为输入
	P1REN |= BIT3;				        //启用P1.3内部上下拉电阻
	P1OUT |= BIT3;			            //将电阻设置为上拉

}

void WDT_init()
{
	//-----设定WDT为16ms中断-----
	WDTCTL=WDT_ADLY_16;
	//-----WDT中断使能-----
    IE1|=WDTIE;
}

#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	//-----启用Port1事件检测函数-----
	P1_IODect();		//检测通过，则会调用事件处理函数
}

void P1_IODect()
{
	static unsigned char KEY_Now=0;	//变量值出函数时需保留
	unsigned char KEY_Past=0;
	KEY_Past=KEY_Now;
	//-----查询IO的输入寄存器-----
	if(P1IN&BIT3) 	KEY_Now=1;
	else    	    KEY_Now=0;
	//-----前一次高电平、后一次低电平，说明按键按下-----
	if((KEY_Past==1)&&(KEY_Now==0))
		{
		  counter++;
		  if (counter>=9) counter=0;
		  LCD_DisplayDigit(LCD_DIGIT_CLEAR,6);   //没键按下，清LCD
		  LCD_DisplayDigit(counter,6);
		  HT1621_Reflash(LCD_Buffer);
  		}
	
}



