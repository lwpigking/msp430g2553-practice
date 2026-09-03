/**
 * modified by lwPigKing
 * description: 浙江理工大学科技与艺术学院单片机实训，型号为msp430g2553
 * 需求：
 * 1. 二极管模拟东西、南北两个方向的信号灯
 * 2. 东西、南北这两个方向的信号灯工作模式一致，其中当其中一个方向量。另一个方向就灭
 * 3. 两个方向的信号灯工作周期为35s
 * 4. 当紧急事件发生的时候，信号灯全部变成红色
 * 5. 扩展板上的LED模拟东西、南北两个方向
 * 6. LCD数码显示灯还要多久亮
 * 7. Key1绿灯增5s，最大60s，超过回到30s
 * 8. Key2绿灯减5s，最小10s，小于回到30s
 * 9. Key3紧急状态键，Key4解除紧急状态，回到原来的状态
 */

#include"MSP430G2553.h"
#include"TCA6416A.h"
#include"HT1621.h"
#include"LCD_128.h"
#include"BCSplus_init.h"

void I2C_IODect();
void P13_Onclick();
void Out_all_redlight();
void WDT_init();
void Timer1_ISR();
void Out_LED(char led_state);
void Timer_A0_int(void);
void disp();

// 显示缓冲
char dis_buf[4]={0,0,0,0};
/**
 * 东和西的状态必须一致
 * 南和北的状态必须一致
 * 但是东西、南北的状态不能一致，除非是全红，所以只有如下五中状态S0-4，不符合数学上的组合
 * S0：东西绿-南北红
 * S1：东西黄-南北红
 * S2：东西红-南北绿
 * S3：东西红-南北黄
 * S4:紧急状态全红
 *
 * P1.7,P1.6,P1.5对应南北的红黄绿灯
 * P1.7对应LED8，P1.6对应LED7，P1.5对应LED6
 *
 * P1.2,P1.1,P1.0对应东西的红黄绿灯
 * P1.2对应LED3，P1.1对应LED2，P1。0对应LED1
 * */
// 存放S0-4这五个状态的字符数组
char c[5]={0x7e,0x7d,0xdb,0xbb,0x7b};
// 绿灯为30秒，黄灯为5秒
char green=30,yellow=5;
// m、n、s分别为南北与东西转换时间、绿与黄转换时间、状态值
char m=35,n=30,s=0;
// 紧急状态，0为非紧急状态
char state_of_emergency = 0;
// 0-不显示
char disp_state= 0;

/**
 * LCD、I2C、定时器等，并启动了低功耗模式（LPM0）以及全局中断，使得系统能够通过中断来响应外部事件（如按键）。
 */
void main(void) {
	// 关闭哈士奇~~
	WDTCTL = WDTPW + WDTHOLD;
	// 初始化定时器
	BCSplus_graceInit();
	// 初始化LCD(开启LCD驱动)
	HT1621_init();
	// 初始化I2C扩展IO单元
	TCA6416A_Init();

	// 设定WDT定时中断为1000ms，开启WDT定时中断使能
	// 看门狗定时器
	WDT_init();

	// 定时器模块初始化
	Timer_A0_int();

	// 清除屏幕
	LCD_Clear();

	// 更新显示缓存
	HT1621_Reflash(LCD_Buffer);

	// 屏幕输出
	// 输出c数组里的状态
	Out_LED(c[s]);

	// 低功耗模式
	_bis_SR_register(LPM0_bits+GIE);

	while(1) {
		PinIN();
		I2C_IODect();
		if(disp_state == 1) disp();
		disp_state = 0;
	    _bis_SR_register(LPM0_bits);
      }
}

/**
 * 设置定时器的工作模式和启用中断
 */
void Timer_A0_int(void) {
   TACTL = TASSEL_2 + ID_1 + MC_2 + TAIE;
}

/**
 * 忽略了捕获/比较寄存器的中断，并仅关心 Timer_A 溢出中断
 */
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A(void) {
  switch( TA0IV ) {
    case  TA0IV_TACCR1: break;                                 // CCR1 not used
    case  TA0IV_TACCR2: break;                                 // CCR2 not used
    case  TA0IV_TAIFG:  _bic_SR_register_on_exit(LPM0_bits );  // overflow，
                        break;
    default:  break;
  }
}

/**
 * Key1 - Key4的功能
 * Key1: 增加绿灯持续时间
 * Key2: 减少绿灯持续时间
 * Key3: 变为紧急状态
 * Key4: 从紧急状态变回原来状态
 */

// Key1
void I2C_IO10_Onclick() {
	// 绿灯增加5秒
	green = green+5;
	// 超过60秒恢复到30秒
	if(green>60) {
		green=30;
	}
	//根据不同状态，对m、n赋值，n-东西绿灯,m-南北绿灯
	// m、n、s分别为南北与东西转换时间、绿与黄转换时间、状态值
	switch(s) {
		   case 0:
			   //m=35，n=30
			   m=green+yellow,n=green;
			   break;

		   case 1:
			   //n=5
			   m=yellow,n=yellow;
			   break;

		   case 2:
			   //m=30，n=35
			   m=green,n=green+yellow;
			   break;

		   case 3:
			   //m=5
			   m=yellow,n=yellow;
			   break;

		   default: break;
	}
}

// Key2
void I2C_IO11_Onclick() {
	// 绿灯减少5秒
	green= green-5;
	// 低于10秒恢复为30秒
	if(green<10) green=30;
	//根据不同状态，对m、n赋值，n-东西绿灯或红灯剩余时间,m-南北绿灯或红灯乘余时间
	switch(s) {
		   case 0:m=green+yellow,n=green;break;			    //m=35，n=30
		   case 1:m=yellow,n=yellow;break;           		//n=5
		   case 2:m=green,n=green+yellow;break;      		//m=30，n=35
		   case 3:m=yellow,n=yellow;break;           		//m=5
		   default: break;
	}
}

/**
 * 当 Key3被按下时，切换至紧急状态。在紧急状态下，所有信号灯会变为红色。
 * LED3和LED8亮
 */
void I2C_IO12_Onclick() {
	state_of_emergency^= BIT0;
	if(state_of_emergency) Out_all_redlight();
}

/**
 * Key4被按下时，继续当前状态
 */
void I2C_IO13_Onclick() {
	state_of_emergency^= BIT0;
	Out_LED(c[s]);
	dis_buf[0]=n%10;
	dis_buf[1]=n/10;
	dis_buf[2]=m%10;
	dis_buf[3]=m/10;
	LCD_DisplayDigit(dis_buf[0],2);
	LCD_DisplayDigit(dis_buf[1],1);
	LCD_DisplayDigit(dis_buf[2],6);
	LCD_DisplayDigit(dis_buf[3],5);
	HT1621_Reflash(LCD_Buffer);
}


/**
 * 该函数通过读取 TCA6416A I/O 扩展器的输入缓冲区，检测按键的按下事件。
 * 根据按键的状态变化调用相应的按键处理函数。
 * 见2实验平台篇（辅助教材）.pdf的图 1.18 TCA6416A原理图
 * 此函数监测 Key1 (I2C_IO10)、Key2 (I2C_IO11)、Key3 (I2C_IO12)、Key4 (I2C_IO13)，
 * 分别对应信号灯的时间调节、紧急状态切换和继续状态等功能。
 */
void I2C_IODect() {
	static unsigned char KEY_Now=0;
	unsigned char KEY_Past;
	KEY_Past=KEY_Now;
	//----判断I2C_IO10所连的KEY1按键是否被按下------
	if((TCA6416A_InputBuffer&BIT8) == BIT8)
		KEY_Now |=BIT0;
	else
		KEY_Now &=~BIT0;
	if(((KEY_Past&BIT0)==BIT0)&&(KEY_Now&BIT0) !=BIT0)
		I2C_IO10_Onclick();
	//----判断I2C_IO11所连的KEY2按键是否被按下------
	if((TCA6416A_InputBuffer&BIT9)== BIT9)
		KEY_Now |=BIT1;
	else
		KEY_Now &=~BIT1;
	if(((KEY_Past&BIT1)==BIT1)&&(KEY_Now&BIT1)!=BIT1)
		I2C_IO11_Onclick();
	//----判断I2C_IO12所连的KEY3按键是否被按下------
	if((TCA6416A_InputBuffer&BITA) == BITA)
		KEY_Now |=BIT2;
	else
		KEY_Now &=~BIT2;
	if(((KEY_Past&BIT2)==BIT2)&&(KEY_Now&BIT2) ==0)
		I2C_IO12_Onclick();
	//----判断I2C_IO13所连的KEY4按键是否被按下------
	if((TCA6416A_InputBuffer&BITB) ==  BITB)
		KEY_Now |=BIT3;
	else
		KEY_Now &=~BIT3;
	if(((KEY_Past&BIT3) == BIT3)&& (KEY_Now&BIT3) == 0)
		I2C_IO13_Onclick();
}


/******************************************************************************************************
 * 名       称：Out_all_redlight()
 * 功       能：P1.3的中断事件处理函数，即当P1.3键被按下后，下一步干什么
 * 入口参数：无
 * 出口参数：无
 * 说       明：使用事件处理函数的形式，可以增强代码的移植性和可读性
 * 范       例：无
 ******************************************************************************************************/
void Out_all_redlight()
{
	Out_LED(c[4]);
}

/******************************************************************************************************
 * 名       称：WDT_init()
 * 功       能：设定WDT定时中断为1000ms，开启WDT定时中断使能
 * 入口参数：无
 * 出口参数：无
 * 说       明：WDT定时中断的时钟源选择ACLK，可以用LPM3休眠。
 * 范       例：无
 ******************************************************************************************************/
// 看门狗初始化，1秒
void WDT_init()
{
	WDTCTL=WDT_ADLY_1000;
    IE1|=WDTIE;
}

/******************************************************************************************************
 * 名       称：WDT_ISR()
 * 功       能：响应WDT定时中断服务,每秒中断一次。
 * 入口参数：无
 * 出口参数：无
 * 说       明：WDT定时中断独占中断向量，所以无需进一步判断中断事件，也无需人工清除标志位。
 *         所以，在WDT定时中断服务子函数中，直接调用WDT事件处理函数就可以了。
 * 范       例：无
 ******************************************************************************************************/
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	disp_state = 1;
}

/******************************************************************************************************
 * 名       称：disp()
 * 功       能：秒计时处理，输出红绿灯状态和显示剩余时间
 * 入口参数：无
 * 出口参数：无
 * 说       明：
 * 范       例：无
 ******************************************************************************************************/
void disp()
{
if(!state_of_emergency)           //如果是紧急状态，退出中断
   {
	   m--;		                   //m-1
	   n--;				           //n-1
	   if(m==0||n==0)			       //m=0或者n=0时,状态转换
	   {
	       s++;
		   if(s>3)	s=0;		                    //如果状态大于３，则返回0
	       switch(s)						        //根据不同状态，对m、n赋值，n-东西绿灯或红灯剩余时间,m-南北绿灯或红灯乘余时间
		   {
		      case 0:m=green+yellow,n=green;break;			    //m=35，n=30
		      case 1:m=yellow,n=yellow;break;           		//n=5
		      case 2:m=green,n=green+yellow;break;      		//m=30，n=35
		      case 3:m=yellow,n=yellow;break;           		//m=5
		      default: break;
	       }
	       Out_LED(c[s]);                                      //显示LED灯
	   }
	   dis_buf[0]=n%10;                                    //dis_buf[0]-dis_buf[3]分别存放ｎ的个位十位、ｍ的个位十位
       dis_buf[1]=n/10;
	   dis_buf[2]=m%10;
	   dis_buf[3]=m/10;
	   LCD_DisplayDigit(dis_buf[0],2);
	   LCD_DisplayDigit(dis_buf[1],1);
	   LCD_DisplayDigit(dis_buf[2],6);
	   LCD_DisplayDigit(dis_buf[3],5);
	   HT1621_Reflash(LCD_Buffer);             //控制芯片RAM更新
   }
}


/**
 * 此函数通过 I2C总线控制 TCA6416A扩展 I/O模块输出相应的信号灯状态。
 * 通过调用 TCA6416_Tx_Frame() 向 I/O 扩展器发送命令字，从而控制东西和南北方向的信号灯
 * @Param led_state LED的状态字节
 */
void Out_LED(char led_state)
{
	unsigned char con[2]={0,0};
	con[0] = Out_CMD0;
	con[1] = led_state;						// 某位置1，输出为高，0为低
	TCA6416_Tx_Frame(con,2);			             //  写入命令字
}
