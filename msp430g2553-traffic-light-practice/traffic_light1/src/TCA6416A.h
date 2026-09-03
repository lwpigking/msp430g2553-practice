/*
 * TCA6416A.h
 *
 *  Created on: 2013-4-6
 *      Author: Administrator
 */

#ifndef TCA6416A_H_
#define TCA6416A_H_

extern void PinIN();
extern void PinOUT(unsigned char pin,unsigned char status);
extern void TCA6416A_Init();
extern volatile unsigned int TCA6416A_InputBuffer;
//-----控制寄存器定义-----
#define		In_CMD0			0x00	//读取管脚输入状态寄存器；只读
#define		In_CMD1			0x01
#define		Out_CMD0		0x02	//控制管脚输出状态寄存器；R/W
#define		Out_CMD1		0x03
#define		PIVS_CMD0		0x04	//反向控制管脚输出状态寄存器；R/W
#define		PIVS_CMD1		0x05
#define		CFG_CMD0		0x06	//管脚方向控制：1：In；0:：Out。
#define		CFG_CMD1		0x07

#endif /* TCA6416A_H_ */
