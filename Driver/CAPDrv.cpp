/*==========================================================*/
/* Title		:	CAPDrv.cpp								*/
/* Description	: 	Class_ADCDrv realization 				*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#include "CAPDrv.h"

/**********************************************************************************
��������	Drv_CAPInit()			
����������	CAPģ���ʼ��
�������ã�	��
***********************************************************************************/
void	Class_CAPDrv::Drv_CAPInit()
{
	/* <=== eCAP Registers initialization ===> */

    // �����1��Ϊ�ز�ͬ��������PWM ͬ���źţ�����ֹͬ���ź����
    // continueģʽcap��������reset
    ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;      // Make sure the counter is stopped
    ECap1Regs.ECEINT.all = 0x0000;           // Disable CAP interrupts
 
    // Configure peripheral registers
    ECap1Regs.ECCTL1.bit.PRESCALE = 0;	     // ʱ�����ڲ���Ƶ
    ECap1Regs.ECCTL2.bit.CONT_ONESHT = 0;    // Continuousģʽ
    ECap1Regs.ECCTL2.bit.STOP_WRAP = 0;      // Stop after Caputure Event1
    ECap1Regs.ECCTL1.bit.CAP1POL = 1;        // trailing edge   ����CANS�շ��� 20170826
    ECap1Regs.ECCTL2.bit.SYNCI_EN = 1;       // ʹ��ͬ���ź�����    ����FromPWM SYNC IN,TSCTRװ��CSTPHS
    ECap1Regs.ECCTL2.bit.SYNCO_SEL = 3;      // ͬ���ź������ֹ
    ECap1Regs.ECCTL1.bit.CAPLDEN = 1;        // Enable capture units
 
    ECap1Regs.ECCLR.all = 0xFFFF;            // Clear all CAP interrupt flags
    ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;      // TSCTR free-running
    ECap1Regs.ECCTL2.bit.REARM = 1;          // arm one-shot
 
    // �����2��ΪLBS���㲶��
    // ������PWM ͬ���źţ�ʹ�����ͬ���ź�
    // continueģʽcap���������reset
    ECap2Regs.ECCTL2.bit.TSCTRSTOP = 0;      // Make sure the counter is stopped
    ECap2Regs.ECEINT.all = 0x0000;           // Disable CAP interrupts
    
    // Configure peripheral registers
    ECap2Regs.ECCTL1.bit.PRESCALE = 0;	     // ʱ�����ڲ���Ƶ 
    ECap2Regs.ECCTL2.bit.CONT_ONESHT = 0;    // Continuousģʽ
    ECap2Regs.ECCTL2.bit.STOP_WRAP = 0;      // Stop after Caputure Event1
    ECap2Regs.ECCTL1.bit.CAP1POL = 1;        // trailing edge   ����CANS�շ��� 20170826
    ECap2Regs.ECCTL2.bit.SYNCI_EN = 0;       // ��ֹͬ���ź�����
    ECap2Regs.ECCTL2.bit.SYNCO_SEL = 1;      // ͬ���ź����CTR=PRD
 
    ECap2Regs.ECCLR.all = 0xFFFF;            // Clear all CAP interrupt flags

    // �����3��Ϊ������㲶��
    // ����ͬ���źţ���ֹ���ͬ���ź�
    // continueģʽcap
    ECap3Regs.ECCTL2.bit.TSCTRSTOP = 0;      // Make sure the counter is stopped
    ECap3Regs.ECEINT.all = 0x0000;           // Disable CAP interrupts
    
    // Configure peripheral registers
    ECap3Regs.ECCTL1.bit.PRESCALE = 0;	     // ʱ�����ڲ���Ƶ
    ECap3Regs.ECCTL2.bit.CONT_ONESHT = 0;    // Continuousģʽ
    ECap3Regs.ECCTL2.bit.STOP_WRAP = 0;      // Stop after Caputure Event1
    ECap3Regs.ECCTL1.bit.CAP1POL = 0;        // Rising edge
    ECap3Regs.ECCTL2.bit.SYNCI_EN = 1;       // ͬ���ź�����ʹ��
    ECap3Regs.ECCTL2.bit.SYNCO_SEL = 3;      // ͬ���ź������ֹ

    ECap3Regs.ECCLR.all = 0xFFFF;            // Clear all CAP interrupt flags

	// �����4��Ϊ��Ƶͬ���źŹ��㲶׽
    // ����ͬ���źţ���ֹ���ͬ���ź�
    // continueģʽcap
    ECap4Regs.ECCTL2.bit.TSCTRSTOP = 0;      // Make sure the counter is stopped
    ECap4Regs.ECEINT.all = 0x0000;           // Disable CAP interrupts

    // Configure peripheral registers
    ECap4Regs.ECCTL1.bit.PRESCALE = 0;	     // ʱ�����ڲ���Ƶ
    ECap4Regs.ECCTL2.bit.CONT_ONESHT = 0;    // Continuousģʽ
    ECap4Regs.ECCTL2.bit.STOP_WRAP = 0;      // Stop after Caputure Event1
    ECap4Regs.ECCTL1.bit.CAP1POL = 1;        // trailing edge   ����CANS�շ��� 20170826
    ECap4Regs.ECCTL2.bit.SYNCI_EN = 0;       // ͬ���ź������ֹ
    ECap4Regs.ECCTL2.bit.SYNCO_SEL = 3;      // ͬ���ź������ֹ

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
