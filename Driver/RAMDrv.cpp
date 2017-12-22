/*==========================================================*/
/* Title		:	RAMDrv.cpp								*/
/* Description	: 	Class_ADCDrv realization 				*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#include "RAMDrv.h"

/**********************************************************************************
函数名：	Drv_MemCheck()			
功能描述：	F2808 DSP RAM检查模块
函数调用：	无
***********************************************************************************/
void	Class_RAMDrv::Drv_MemCheck()
{
		volatile UINT32 	*memaddr;
		UINT32 	j, u16memerr;

		u16memerr = 0;

		//M0,M1 SARAM(2K*16) Check
    	memaddr = (UINT32 *) 0x0000;
    	for (j = 0x0000; j < 0x0800; j++)
    	{
			*(memaddr + j) = 0x5555;
			if (*(memaddr + j) != 0x5555)
				u16memerr=1;

			*(memaddr + j) = 0xaaaa;
			if (*(memaddr + j) != 0xaaaa)
				u16memerr=1;

			*(memaddr + j) = 0x0000;
    	}


		//CPUx.CLA1 to CPUx MSGRAM (128*16) Check
    	memaddr = (UINT32 *) 0x1480;
    	for (j = 0x0000; j < 0x0080; j++)
    	{
			*(memaddr + j) = 0x5555;
			if (*(memaddr + j) != 0x5555)
				u16memerr=1;

			*(memaddr + j) = 0xaaaa;
			if (*(memaddr + j) != 0xaaaa)
				u16memerr=1;

			*(memaddr + j) = 0x0000;
    	}

		//CPUx to CPUx.CLA1 MSGRAM (128*16) Check
    	memaddr = (UINT32 *) 0x1500;
    	for (j = 0x0000; j < 0x0080; j++)
    	{
			*(memaddr + j) = 0x5555;
			if (*(memaddr + j) != 0x5555)
				u16memerr=1;

			*(memaddr + j) = 0xaaaa;
			if (*(memaddr + j) != 0xaaaa)
				u16memerr=1;

			*(memaddr + j) = 0x0000;
    	}


		//LS0~LS5 SARAM(2K*16*6) Check
    	memaddr = (UINT32 *) 0x8000;
    	for (j = 0x0000; j < 0x3000; j++)
    	{
			*(memaddr + j) = 0x5555;
			if (*(memaddr + j) != 0x5555)
				u16memerr=1;

			*(memaddr + j) = 0xaaaa;
			if (*(memaddr + j) != 0xaaaa)
				u16memerr=1;

			*(memaddr + j) = 0x0000;
    	}


		//D0 D1 SARAM(2K*16,2K*16) Check
    	memaddr = (UINT32 *) 0xB000;
    	for (j = 0x0000; j < 0x1000; j++)
    	{
			*(memaddr + j) = 0x5555;
			if (*(memaddr + j) != 0x5555)
				u16memerr=1;

			*(memaddr + j) = 0xaaaa;
			if (*(memaddr + j) != 0xaaaa)
				u16memerr=1;

			*(memaddr + j) = 0x0000;
    	}

		//GS0~GS15 SARAM(4K*16*16) Check
    	memaddr = (UINT32 *) 0x00C000;
    	for (j = 0x0000; j < 0x10000; j++)					
    	{
			*(memaddr + j) = 0x5555;
			if (*(memaddr + j) != 0x5555)
				u16memerr=1;

			*(memaddr + j) = 0xaaaa;
			if (*(memaddr + j) != 0xaaaa)
				u16memerr=1;

			*(memaddr + j) = 0x0000;
    	}

	//定义回定变量的地址为：0xAFFC-0xAFFF
	if ((DSPCrazyA == 0xEB95) 
		 && (DSPCrazyB == 0xEB95)
		 && (DSPCrazyC == 0xEB95)  
		 && (DSPCrazyD == 0xEB95)  )
	{
		objInverter.m_st_wFlagInv.bDspCrazyOff = 1;					//有DSP死机切旁路开机标志
	}  
	else
	{
	    //Mem  Check for 0xAFFC-0xAFFF
   	 	memaddr = (UINT32 *) 0xAFFC;
	    for (j = 0x0000; j < 0x4; j++)			
 	   	{
			*(memaddr + j) = 0x5555;
			if (*(memaddr + j) != 0x5555)
				u16memerr=1;

			*(memaddr + j) = 0xaaaa;
			if (*(memaddr + j) != 0xaaaa)
				u16memerr=1;

			*(memaddr + j) = 0x0000;
	    }	
	}

		if(u16memerr==1)
		{
			objSystem.m_st_wFlagSystem.bRAMError=1;
			
			while(1)									//死循环		
			{
				//zqf 070111--Limp mode时CPU时钟为1-5MHz，取典型值3MHz
				objTimerDrv.Drv_usDelay(30000);  	//33.3us*30000=1s, LED闪烁
				GpioDataRegs.GPBTOGGLE.bit.GPIO40 = 1;

			}	 	
		}

}

void	Class_RAMDrv::Drv_MemCheckMaster(void)
{	
	MemCfgRegs.DxINIT.all = 0x000F;
	MemCfgRegs.LSxINIT.all = 0x003F;
	MemCfgRegs.GSxINIT.all = 0xFFFF;
	MemCfgRegs.MSGxINIT.all = 0x0007;
	while((MemCfgRegs.DxINITDONE.all != 0x0F) || (MemCfgRegs.LSxINITDONE.all != 0x003f)||
		  (MemCfgRegs.GSxINITDONE.all != 0xFFFF) || (MemCfgRegs.MSGxINITDONE.all != 0x0007))
	{
		//zqf 070111--Limp mode时CPU时钟为1-5MHz，取典型值3MHz
		objTimerDrv.Drv_usDelay(2000000);  	//2000000*0.5us=1s, LED闪烁
		GpioDataRegs.GPBTOGGLE.bit.GPIO40 = 1;

	}	 	
			
}


//-----------------------------------
void 	Class_RAMDrv::Drv_MemCopy(UINT16 *SourceAddr, UINT16* SourceEndAddr, UINT16* DestAddr)
{
    while(SourceAddr < SourceEndAddr)
    { 
       *DestAddr++ = *SourceAddr++;
    }
    return;
}


//===========================================================================
// End of file.
//===========================================================================
