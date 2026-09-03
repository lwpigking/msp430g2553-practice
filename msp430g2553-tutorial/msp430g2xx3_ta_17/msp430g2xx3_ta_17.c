/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 * 
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430G2xx3 Demo - Timer_A, PWM TA1, Up Mode, 32kHz ACLK
//
//  Description: This program generates one PWM output on P1.2 using
//  Timer_A configured for up mode. The value in CCR0, 512-1, defines the PWM
//  period and the value in CCR1 the PWM duty cycles. Using 32kHz ACLK
//  as TACLK, the timer period is 15.6ms with a 75% duty cycle on P1.2.
//  Normal operating mode is LPM3.
//  ACLK = TACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO.
//  //* External watch crystal installed on XIN XOUT is required for ACLK *//	
//
//               MSP430G2xx3
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |         P1.2/TA1|--> CCR1 - 75% PWM
//
//  D. Dang
//  Texas Instruments, Inc
//  December 2010
//   Built with CCS Version 4.2.0 and IAR Embedded Workbench Version: 5.10
//******************************************************************************

#include <msp430.h>


int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  TA0CTL = 0;
  // 设置时钟
  DCOCTL = CALDCO_1MHZ;
  BCSCTL1 = CALBC1_1MHZ;

  // 占空比为0.1
  TA0CCR0 = 25000;
  TA0CCR1 = TA0CCR0 / 10;

  // 设置SMCLK和UPMODE
  TA0CTL = TASSEL_2 | MC_1 | ID_3;
  TA0CCTL1 = OUTMOD_6;

  // 复用1.3口
  P1DIR |= BIT3;
  P1SEL |= BIT3;

  __bis_SR_register(CPUOFF + GIE); // 关闭CPU，进入低功耗模式

}

/**
 * 7. 程实现Timer0_A3的通道1的PWM功能（用P1.2脚）控制LED灯，周期为0.5秒，占空比为10%，用LED灯观察。
 */


#include <msp430.h>

void setup_timer0_a3_pwm(void) {
    // 停止计时器
    TA0CTL = 0;

    P1DIR |= BIT6;
    P1SEL |= BIT6;

    // 配置Timer0_A3
    TA0CTL = TASSEL_2 | MC_1 | ID_3;

    TA0CCR0 = 25000;  // PWM周期为500ms (250,000 / 10)

    // 设置PWM占空比 (10%)
    TA0CCR1 = TA0CCR0 / 10;  // 10%的占空比

    TA0CCTL1 = OUTMOD_7;
}

int main(void) {
    WDTCTL = WDTPW + WDTHOLD; // 停止看门狗定时器

    // 设置时钟为1MHz
    BCSCTL1 = CALBC1_1MHZ;    // 设置DCO为1MHz
    DCOCTL = CALDCO_1MHZ;

    // 配置PWM
    setup_timer0_a3_pwm();

    // 进入低功耗模式，等待PWM周期自动执行
    __bis_SR_register(CPUOFF + GIE); // 关闭CPU，进入低功耗模式
}
