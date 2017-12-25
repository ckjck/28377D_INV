/*==========================================================*/
/* Title		:	PWMDrv.cpp								*/
/* Description	: 	Class_ADCDrv realization 				*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#include "PWMDrv.h"

/************************************************************************************
//函数名称：    Drv_PWMActive()
//功能描述：	PWM脉冲使能
************************************************************************************/
#pragma CODE_SECTION(".Epwm1Funcs")

void 	Class_PWMDrv::Drv_PWMActive(void)
{
	
	EALLOW;						
	EPwm1Regs.TZCLR.bit.OST = 1;
	EPwm2Regs.TZCLR.bit.OST = 1;					// Clear Flag for One-Shot Trip (OST) Latch
	EPwm3Regs.TZCLR.bit.OST = 1;
	EPwm4Regs.TZCLR.bit.OST = 1;
	EPwm5Regs.TZCLR.bit.OST = 1;
	EPwm6Regs.TZCLR.bit.OST = 1;
	EDIS;
}

/************************************************************************************
//函数名称：    Drv_PWMInactive()
//功能描述：	PWM脉冲封锁
************************************************************************************/
//#pragma CODE_SECTION(".Epwm1Funcs")

void 	Class_PWMDrv::Drv_PWMInactive(void)
{
	EALLOW;						
	EPwm1Regs.TZFRC.bit.OST = 1;
	EPwm2Regs.TZFRC.bit.OST = 1;					// Clear Flag for One-Shot Trip (OST) Latch
	EPwm3Regs.TZFRC.bit.OST = 1;
	EPwm4Regs.TZFRC.bit.OST = 1;
	EPwm5Regs.TZFRC.bit.OST = 1;
	EPwm6Regs.TZFRC.bit.OST = 1;
	EDIS;

}

/************************************************************************************
//函数名称：    Drv_PWMInit()
//功能描述：	PWM初始化
************************************************************************************/
void 	Class_PWMDrv::Drv_PWMInit(void)
{
	//EPWM1
	EALLOW;
	EPwm1Regs.TZCTL.bit.TZA = TZ_FORCE_LO;			// Forced Low
	EPwm1Regs.TZCTL.bit.TZB = TZ_FORCE_LO;			// Forced Low
	EPwm1Regs.TZCLR.bit.OST = 1;					// Forces a fault on the OST latch and sets the OSTFLG bit.
	EDIS;
	EPwm1Regs.TBPRD = 2083; 				  // Switch Period Counter
	EPwm1Regs.TBPHS.bit.TBPHS = 0;
	EPwm1Regs.CMPA.bit.CMPA = 0;
	EPwm1Regs.TBCTL.bit.PHSDIR = TB_UP;
	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
	EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE; 		 // Phase loading enabled
	EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; 	 //
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		// TBCLK = SYSCLK
	EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;				//
	EPwm1Regs.TBCTR = 0x0000;
	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;	// load on CTR = Zero or PRD
	EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;	// load on CTR = Zero or PRD
	EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;			// CNT = CMP up   ->1
	EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;			// CNT = CMP down ->0
	EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;			// CNT = CMP up   ->0
	EPwm1Regs.AQCTLB.bit.CBD = AQ_SET;				// CNT = CMP down ->1
	EPwm1Regs.AQSFRC.bit.RLDCSF = 3;				// the active register load immediately
	EPwm1Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;	   // Software forcing disabled, i.e., has no effect
	EPwm1Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
	//SET DB
	EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	EPwm1Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm1Regs.DBRED.all = KDeadTimeCnst;
	EPwm1Regs.DBFED.all = KDeadTimeCnst;
	EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;		//
	EPwm1Regs.ETSEL.bit.INTEN = 1;
	EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;


	//EPWM2
	EALLOW;
	EPwm2Regs.TZCTL.bit.TZA = TZ_FORCE_LO;			// Forced Low
	EPwm2Regs.TZCTL.bit.TZB = TZ_FORCE_LO;			// Forced Low
	EPwm2Regs.TZCLR.bit.OST = 1;					// Forces a fault on the OST latch and sets the OSTFLG bit.
	EDIS;
	EPwm2Regs.TBPRD = 2083; 				  		// Switch Period Counter
	EPwm2Regs.TBPHS.bit.TBPHS = 0;
	EPwm2Regs.CMPA.bit.CMPA = 0;
	EPwm2Regs.TBCTL.bit.PHSDIR = TB_UP;
	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
	EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE; 		 	// Phase loading enabled
	EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; 	//
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		// TBCLK = SYSCLK
	EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;				
	EPwm2Regs.TBCTR = 0x0000;
	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;	// load on CTR = Zero or PRD
	EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;	// load on CTR = Zero or PRD
	EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;			// CNT = CMP up   ->1
	EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;				// CNT = CMP down ->0
	EPwm2Regs.AQCTLB.bit.CBU = AQ_CLEAR;			// CNT = CMP up   ->0
	EPwm2Regs.AQCTLB.bit.CBD = AQ_SET;				// CNT = CMP down ->1
	EPwm2Regs.AQSFRC.bit.RLDCSF = 3;				// the active register load immediately
	EPwm2Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;	    // Software forcing disabled, i.e., has no effect
	EPwm2Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
	//SET DB
	EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	EPwm2Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm2Regs.DBRED.all = KDeadTimeCnst;
	EPwm2Regs.DBFED.all = KDeadTimeCnst;
	EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;		
	EPwm2Regs.ETSEL.bit.INTEN = 1;
	EPwm2Regs.ETPS.bit.INTPRD = ET_1ST;


	//EPWM3
	EALLOW;
	EPwm3Regs.TZCTL.bit.TZA = TZ_FORCE_LO;			// Forced Low
	EPwm3Regs.TZCTL.bit.TZB = TZ_FORCE_LO;			// Forced Low
	EPwm3Regs.TZCLR.bit.OST = 1;					// Forces a fault on the OST latch and sets the OSTFLG bit.
	EDIS;
	EPwm3Regs.TBPRD = 2083; 				  		// Switch Period Counter
	EPwm3Regs.TBPHS.bit.TBPHS = 0;
	EPwm3Regs.CMPA.bit.CMPA = 0;
	EPwm3Regs.TBCTL.bit.PHSDIR = TB_UP;
	EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
	EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE; 		 	// Phase loading enabled
	EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; 	//
	EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		// TBCLK = SYSCLK
	EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;				
	EPwm3Regs.TBCTR = 0x0000;
	EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;	// load on CTR = Zero or PRD
	EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;	// load on CTR = Zero or PRD
	EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;			// CNT = CMP up   ->1
	EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;				// CNT = CMP down ->0
	EPwm3Regs.AQCTLB.bit.CBU = AQ_CLEAR;			// CNT = CMP up   ->0
	EPwm3Regs.AQCTLB.bit.CBD = AQ_SET;				// CNT = CMP down ->1
	EPwm3Regs.AQSFRC.bit.RLDCSF = 3;				// the active register load immediately
	EPwm3Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;	    // Software forcing disabled, i.e., has no effect
	EPwm3Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
	//SET DB
	EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	EPwm3Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm3Regs.DBRED.all = KDeadTimeCnst;
	EPwm3Regs.DBFED.all = KDeadTimeCnst;
	EPwm3Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;		
	EPwm3Regs.ETSEL.bit.INTEN = 1;
	EPwm3Regs.ETPS.bit.INTPRD = ET_1ST;



	//EPWM4
	EALLOW;
	EPwm4Regs.TZCTL.bit.TZA = TZ_FORCE_LO;			// Forced Low
	EPwm4Regs.TZCTL.bit.TZB = TZ_FORCE_LO;			// Forced Low
	EPwm4Regs.TZCLR.bit.OST = 1;					// Forces a fault on the OST latch and sets the OSTFLG bit.
	EDIS;
	EPwm4Regs.TBPRD = 2083; 				  		// Switch Period Counter
	EPwm4Regs.TBPHS.bit.TBPHS = 0;
	EPwm4Regs.CMPA.bit.CMPA = 0;
	EPwm4Regs.TBCTL.bit.PHSDIR = TB_UP;
	EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
	EPwm4Regs.TBCTL.bit.PHSEN = TB_ENABLE; 		 	// Phase loading enabled
	EPwm4Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; 	//
	EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		// TBCLK = SYSCLK
	EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV1;				
	EPwm4Regs.TBCTR = 0x0000;
	EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;	// load on CTR = Zero or PRD
	EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;	// load on CTR = Zero or PRD
	EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;			// CNT = CMP up   ->1
	EPwm4Regs.AQCTLA.bit.CAD = AQ_SET;				// CNT = CMP down ->0
	EPwm4Regs.AQCTLB.bit.CBU = AQ_CLEAR;			// CNT = CMP up   ->0
	EPwm4Regs.AQCTLB.bit.CBD = AQ_SET;				// CNT = CMP down ->1
	EPwm4Regs.AQSFRC.bit.RLDCSF = 3;				// the active register load immediately
	EPwm4Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;	    // Software forcing disabled, i.e., has no effect
	EPwm4Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
	//SET DB
	EPwm4Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm4Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	EPwm4Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm4Regs.DBRED.all = KDeadTimeCnst;
	EPwm4Regs.DBFED.all = KDeadTimeCnst;
	EPwm4Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;		
	EPwm4Regs.ETSEL.bit.INTEN = 1;
	EPwm4Regs.ETPS.bit.INTPRD = ET_1ST;



	//EPWM5
	EALLOW;
	EPwm5Regs.TZCTL.bit.TZA = TZ_FORCE_LO;			// Forced Low
	EPwm5Regs.TZCTL.bit.TZB = TZ_FORCE_LO;			// Forced Low
	EPwm5Regs.TZCLR.bit.OST = 1;					// Forces a fault on the OST latch and sets the OSTFLG bit.
	EDIS;
	EPwm5Regs.TBPRD = 2083; 				  		// Switch Period Counter
	EPwm5Regs.TBPHS.bit.TBPHS = 0;
	EPwm5Regs.CMPA.bit.CMPA = 0;
	EPwm5Regs.TBCTL.bit.PHSDIR = TB_UP;
	EPwm5Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
	EPwm5Regs.TBCTL.bit.PHSEN = TB_ENABLE; 		 	// Phase loading enabled
	EPwm5Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; 	//
	EPwm5Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		// TBCLK = SYSCLK
	EPwm5Regs.TBCTL.bit.CLKDIV = TB_DIV1;				
	EPwm5Regs.TBCTR = 0x0000;
	EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;	// load on CTR = Zero or PRD
	EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;	// load on CTR = Zero or PRD
	EPwm5Regs.AQCTLA.bit.CAU = AQ_CLEAR;			// CNT = CMP up   ->1
	EPwm5Regs.AQCTLA.bit.CAD = AQ_SET;				// CNT = CMP down ->0
	EPwm5Regs.AQCTLB.bit.CBU = AQ_CLEAR;			// CNT = CMP up   ->0
	EPwm5Regs.AQCTLB.bit.CBD = AQ_SET;				// CNT = CMP down ->1
	EPwm5Regs.AQSFRC.bit.RLDCSF = 3;				// the active register load immediately
	EPwm5Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;	    // Software forcing disabled, i.e., has no effect
	EPwm5Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
	//SET DB
	EPwm5Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm5Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	EPwm5Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm5Regs.DBRED.all = KDeadTimeCnst;
	EPwm5Regs.DBFED.all = KDeadTimeCnst;
	EPwm5Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;		
	EPwm5Regs.ETSEL.bit.INTEN = 1;
	EPwm5Regs.ETPS.bit.INTPRD = ET_1ST;

	
	
	//EPWM6
	//
	EALLOW;
	EPwm6Regs.TZCTL.bit.TZA = TZ_FORCE_LO;			// Forced Hi (EPWM2A = High state)
	EPwm6Regs.TZCTL.bit.TZB = TZ_FORCE_LO;			// Forced Hi (EPWM2B = High state)
	EPwm6Regs.TZCLR.bit.OST = 1;					// Forces a fault on the OST latch and sets the OSTFLG bit.
	EDIS;
	EPwm6Regs.TBPRD = 2083; 				  // Switch Period Counter
	EPwm6Regs.TBPHS.bit.TBPHS = 0;
	EPwm6Regs.CMPA.bit.CMPA = 0; 	  //
	EPwm6Regs.TBCTL.bit.PHSDIR = TB_UP;
	EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
	EPwm6Regs.TBCTL.bit.PHSEN = TB_ENABLE;			// Phase loading enabled
	EPwm6Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;		//
	EPwm6Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		// TBCLK = SYSCLK
	EPwm6Regs.TBCTL.bit.CLKDIV = TB_DIV1;
	EPwm6Regs.TBCTR = 0x0000;
	EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;	// load on CTR = Zero or PRD
	EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;	// load on CTR = Zero or PRD
	EPwm6Regs.AQCTLA.bit.CAU = AQ_CLEAR;			// CNT = CMP up   ->1
	EPwm6Regs.AQCTLA.bit.CAD = AQ_SET;			// CNT = CMP down ->0
	EPwm6Regs.AQCTLB.bit.CBU = AQ_CLEAR;			// CNT = CMP up   ->0
	EPwm6Regs.AQCTLB.bit.CBD = AQ_SET;				// CNT = CMP down ->1
	EPwm6Regs.AQSFRC.bit.RLDCSF = 3;				// the active register load immediately
	EPwm6Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;	   // Software forcing disabled, i.e., has no effect
	EPwm6Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
	//SET DB
	EPwm6Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm6Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	EPwm6Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm6Regs.DBRED.all = KDeadTimeCnst;
	EPwm6Regs.DBFED.all = KDeadTimeCnst;
	

	//Epwm11
	//
	//改用CANS收发器 20170826	
	EALLOW;
	EPwm11Regs.TZCTL.bit.TZA = TZ_FORCE_HI;		  // Forced high (EPWM6A = low state)
	EPwm11Regs.TZCTL.bit.TZB = TZ_FORCE_HI;		  // Forced high (EPWM6B = low state)
	EPwm11Regs.TZFRC.bit.OST = 1;					  // Forces a fault on the OST latch and sets the OSTFLG bit.
	EDIS;
	EPwm11Regs.TBPRD = 30000;						  // 30000*32*10*10ns=96ms
	EPwm11Regs.CMPB.bit.CMPB = 2500;						  // TBPRD,[3906*2,2232*2]=[40Hz,70Hz]
	EPwm11Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;
	EPwm11Regs.TBCTL.bit.PHSEN = TB_DISABLE;		  // Phase loading disabled
	EPwm11Regs.TBCTL.bit.PRDLD = TB_IMMEDIATE;
	EPwm11Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE; // DISABLE
	EPwm11Regs.TBCTL.bit.HSPCLKDIV = 5;			  // /10
	EPwm11Regs.TBCTL.bit.CLKDIV = 5;				  // /32
	EPwm11Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm11Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm11Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
	EPwm11Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
	EPwm11Regs.AQCTLA.bit.CAU = AQ_SET;			  // CNT = CMP up	->0
	EPwm11Regs.AQCTLA.bit.CAD = AQ_CLEAR;			  // CNT = CMP down ->1
	EPwm11Regs.AQCTLB.bit.CBU = AQ_SET;		  // CNT = CMP up	->0
	EPwm11Regs.AQCTLB.bit.PRD = AQ_CLEAR;			  // PRD			->1
	EPwm11Regs.AQSFRC.bit.RLDCSF = 3;				  // the active register load immediately
	EPwm11Regs.AQCSFRC.bit.CSFA = AQC_NO_ACTION;	  // Software forcing disabled, i.e., has no effect
	EPwm11Regs.AQCSFRC.bit.CSFB = AQC_NO_ACTION;
	EPwm11Regs.DBCTL.bit.OUT_MODE = DB_DISABLE;
	EPwm11Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;  
	EPwm11Regs.DBRED.all = 0;
	EPwm11Regs.DBFED.all = 0;


	
    EPwm1Regs.TBPHS.bit.TBPHS = 0;     
	EPwm2Regs.TBPHS.bit.TBPHS = 0;     
	EPwm3Regs.TBPHS.bit.TBPHS = 0;     
	EPwm4Regs.TBPHS.bit.TBPHS = 0;     
	EPwm5Regs.TBPHS.bit.TBPHS = 0;     
	EPwm6Regs.TBPHS.bit.TBPHS = 0;     
	EPwm11Regs.TBPHS.bit.TBPHS = 0;  
	
    EPwm1Regs.TBCTR = 0;                         	
    EPwm2Regs.TBCTR = 0;                         	
    EPwm3Regs.TBCTR = 0;                         	
    EPwm4Regs.TBCTR = 0;
	EPwm5Regs.TBCTR = 0;
    EPwm6Regs.TBCTR = 0;
	EPwm11Regs.TBCTR = 0;
  
}


//===========================================================================
// End of file.
//===========================================================================

