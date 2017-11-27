/*==========================================================*/
/* Title		:	CAPDrv.cpp								*/
/* Description	: 	Class_ADCDrv realization 				*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#include "CAPDrv.h"

/**********************************************************************************
函数名：	Drv_CAPInit()			
功能描述：	CAP模块初始化
函数调用：	无
***********************************************************************************/
void	Class_CAPDrv::Drv_CAPInit()
{
	/* <=== eCAP Registers initialization ===> */

    // 捕获口1作为载波同步，接收PWM 同步信号，但禁止同步信号输出
    // continue模式cap，不触发reset
    ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;      // Make sure the counter is stopped
    ECap1Regs.ECEINT.all = 0x0000;           // Disable CAP interrupts
 
    // Configure peripheral registers
    ECap1Regs.ECCTL1.bit.PRESCALE = 0;	     // 时钟周期不分频
    ECap1Regs.ECCTL2.bit.CONT_ONESHT = 0;    // Continuous模式
    ECap1Regs.ECCTL2.bit.STOP_WRAP = 0;      // Stop after Caputure Event1
    ECap1Regs.ECCTL1.bit.CAP1POL = 1;        // trailing edge   改用CANS收发器 20170826
    ECap1Regs.ECCTL2.bit.SYNCI_EN = 1;       // 使能同步信号输入    当有FromPWM SYNC IN,TSCTR装载CSTPHS
    ECap1Regs.ECCTL2.bit.SYNCO_SEL = 3;      // 同步信号输出禁止
    ECap1Regs.ECCTL1.bit.CAPLDEN = 1;        // Enable capture units
 
    ECap1Regs.ECCLR.all = 0xFFFF;            // Clear all CAP interrupt flags
    ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;      // TSCTR free-running
    ECap1Regs.ECCTL2.bit.REARM = 1;          // arm one-shot
 
    // 捕获口2作为LBS过零捕获
    // 不接收PWM 同步信号，使能输出同步信号
    // continue模式cap，软件触发reset
    ECap2Regs.ECCTL2.bit.TSCTRSTOP = 0;      // Make sure the counter is stopped
    ECap2Regs.ECEINT.all = 0x0000;           // Disable CAP interrupts
    
    // Configure peripheral registers
    ECap2Regs.ECCTL1.bit.PRESCALE = 0;	     // 时钟周期不分频 
    ECap2Regs.ECCTL2.bit.CONT_ONESHT = 0;    // Continuous模式
    ECap2Regs.ECCTL2.bit.STOP_WRAP = 0;      // Stop after Caputure Event1
    ECap2Regs.ECCTL1.bit.CAP1POL = 1;        // trailing edge   改用CANS收发器 20170826
    ECap2Regs.ECCTL2.bit.SYNCI_EN = 0;       // 禁止同步信号输入
    ECap2Regs.ECCTL2.bit.SYNCO_SEL = 1;      // 同步信号输出CTR=PRD
 
    ECap2Regs.ECCLR.all = 0xFFFF;            // Clear all CAP interrupt flags

    // 捕获口3作为输出过零捕获
    // 接收同步信号，禁止输出同步信号
    // continue模式cap
    ECap3Regs.ECCTL2.bit.TSCTRSTOP = 0;      // Make sure the counter is stopped
    ECap3Regs.ECEINT.all = 0x0000;           // Disable CAP interrupts
    
    // Configure peripheral registers
    ECap3Regs.ECCTL1.bit.PRESCALE = 0;	     // 时钟周期不分频
    ECap3Regs.ECCTL2.bit.CONT_ONESHT = 0;    // Continuous模式
    ECap3Regs.ECCTL2.bit.STOP_WRAP = 0;      // Stop after Caputure Event1
    ECap3Regs.ECCTL1.bit.CAP1POL = 0;        // Rising edge
    ECap3Regs.ECCTL2.bit.SYNCI_EN = 1;       // 同步信号输入使能
    ECap3Regs.ECCTL2.bit.SYNCO_SEL = 3;      // 同步信号输出禁止

    ECap3Regs.ECCLR.all = 0xFFFF;            // Clear all CAP interrupt flags

	// 捕获口4作为工频同步信号过零捕捉
    // 接收同步信号，禁止输出同步信号
    // continue模式cap
    ECap4Regs.ECCTL2.bit.TSCTRSTOP = 0;      // Make sure the counter is stopped
    ECap4Regs.ECEINT.all = 0x0000;           // Disable CAP interrupts

    // Configure peripheral registers
    ECap4Regs.ECCTL1.bit.PRESCALE = 0;	     // 时钟周期不分频
    ECap4Regs.ECCTL2.bit.CONT_ONESHT = 0;    // Continuous模式
    ECap4Regs.ECCTL2.bit.STOP_WRAP = 0;      // Stop after Caputure Event1
    ECap4Regs.ECCTL1.bit.CAP1POL = 1;        // trailing edge   改用CANS收发器 20170826
    ECap4Regs.ECCTL2.bit.SYNCI_EN = 0;       // 同步信号输入禁止
    ECap4Regs.ECCTL2.bit.SYNCO_SEL = 3;      // 同步信号输出禁止

    ECap4Regs.ECCLR.all = 0xFFFF;            // Clear all CAP interrupt flags

	
    
    ECap2Regs.ECCTL1.bit.CAPLDEN = 1;        // Enable capture units
    ECap3Regs.ECCTL1.bit.CAPLDEN = 1;        // Enable capture units 
    ECap4Regs.ECCTL1.bit.CAPLDEN = 1;        // Enable capture units   
    ECap2Regs.ECCTL2.bit.REARM = 1;          // arm one-shot
    ECap3Regs.ECCTL2.bit.REARM = 1;          // arm one-shot
    ECap4Regs.ECCTL2.bit.REARM = 1;          // arm one-shot
    ECap2Regs.ECCTL2.bit.TSCTRSTOP = 1;      // TSCTR free-running
    ECap3Regs.ECCTL2.bit.TSCTRSTOP = 1;      // TSCTR free-running
    ECap4Regs.ECCTL2.bit.TSCTRSTOP = 1;      // TSCTR free-running

}


//===========================================================================
// End of file.
//===========================================================================
