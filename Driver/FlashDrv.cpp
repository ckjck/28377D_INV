/*==========================================================*/
/* Title		:	FlashDrv.cpp								*/
/* Description	: 	Class_ADCDrv realization 				*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#include "FlashDrv.h"


extern UINT16 RamfuncsLoadStart;
extern UINT16 RamfuncsLoadEnd;
extern UINT16 RamfuncsRunStart;

/**********************************************************************************
函数名： Drv_Encrypt()
功能描述： 对整个逆变程序进行加密
函数调用：无
*************************************************************************************/
	
	void	Class_FlashDrv::Drv_Encrypt(void)
	{
	 	volatile INT16 *CSMSCR = (volatile INT16 *)0x0000AEF; 	//CSMSCR register
   		volatile UINT16 	*Passaddr;
	 	UINT16 j;
	
	 	Passaddr = (UINT16 *) 0x3F7F80;
	    for (j = 0x0000; j < 0x76; j++)			
 	   	{
			*(Passaddr + j) = 0x0;
		}


		//写密码到PWL寄存器
		// 0x9106 8909 2991 9952 3659 9235 0911 1902 is used.
	  //	asm(" .sect csmpasswds ");                       
	 // 	asm(" .WORD 9268H,3909H,2991H,9952H,3659H,9235H,0911H,1902H ");
	 // 	asm(" .text ");
	
	  	EALLOW;
	  	*CSMSCR = 0x8000; 									//Set FORCESEC bit*/ 
	  	EDIS;
 		
	}

/************************************************************************************
//函数名称：    Drv_InitFlash()
//功能描述：    初始化Flash
************************************************************************************/

#pragma CODE_SECTION("ramfuncs") 

void 	Class_FlashDrv::Drv_InitFlash(void)
{
    EALLOW;

    //
    // Set VREADST to the proper value for the flash banks to power up
    // properly. This sets the bank power up delay.
    //
    Flash0CtrlRegs.FBAC.bit.VREADST = 0x14;

    //
    // At reset bank and pump are in sleep. A Flash access will power up the
    // bank and pump automatically.
    //
    // After a Flash access, bank and pump go to low power mode (configurable
    // in FBFALLBACK/FPAC1 registers) if there is no further access to flash.
    //
    // Power up Flash bank and pump. This also sets the fall back mode of
    // flash and pump as active.
    //
    Flash0CtrlRegs.FPAC1.bit.PMPPWR = 0x1;
    Flash0CtrlRegs.FBFALLBACK.bit.BNKPWR0 = 0x3;

    //
    // Disable Cache and prefetch mechanism before changing wait states
    //
    Flash0CtrlRegs.FRD_INTF_CTRL.bit.DATA_CACHE_EN = 0;
    Flash0CtrlRegs.FRD_INTF_CTRL.bit.PREFETCH_EN = 0;

    //
    // Set waitstates according to frequency
    //
    //      *CAUTION*
    // Minimum waitstates required for the flash operating at a given CPU rate
    // must be characterized by TI. Refer to the datasheet for the latest
    // information.
    //
    Flash0CtrlRegs.FRDCNTL.bit.RWAIT = 0x3;


    //
    // Enable Cache and prefetch mechanism to improve performance of code
    // executed from Flash.
    //
    Flash0CtrlRegs.FRD_INTF_CTRL.bit.DATA_CACHE_EN = 1;
    Flash0CtrlRegs.FRD_INTF_CTRL.bit.PREFETCH_EN = 1;

    //
    // At reset, ECC is enabled. If it is disabled by application software and
    // if application again wants to enable ECC.
    //
    Flash0EccRegs.ECC_ENABLE.bit.ENABLE = 0xA;

    EDIS;

    //
    // Force a pipeline flush to ensure that the write to the last register
    // configured occurs before returning.
    //
    
    asm(" RPT #7 || NOP");
}


//===========================================================================
// End of file.
//===========================================================================
