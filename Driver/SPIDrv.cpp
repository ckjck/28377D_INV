/*==========================================================*/
/* Title		:	SPIDrv.cpp								*/
/* Description	: 	Class_ADCDrv realization 				*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#include "SPIDrv.h"

/**********************************************************************************
函数名：	Drv_SPIInit()			
功能描述：	SPI模块初始化
函数调用：	无
***********************************************************************************/
void	Class_SPIDrv::Drv_SPIInit()
{
	/* <=== SPI FIFO and SPI Registers initialization ===> */

    //SpibRegs.SPICCR.all = 0x000F;	         // Reset on, rising edge, 16-bit char bits  
	SpicRegs.SPICTL.all = 0x000E;    		 // Enable master mode, delay half_cycle phase,
                                             // enable talk, and SPI int disabled.
	SpicRegs.SPIBRR.bit.SPI_BIT_RATE = 0x0001;				 // Baud Rate = 6.25MHz(最快20M)
	SpicRegs.SPICCR.all = 0x008F;		     // Relinquish SPI from Reset   
	SpicRegs.SPIPRI.bit.FREE = 1;            // Set so breakpoints don't disturb xmission

}
//#pragma CODE_SECTION(".Epwm1Funcs")

//SPI接收数据
UINT16	Class_SPIDrv::Drv_SPIReceiveData(void)
{
	INT16	i16_temp_0;
	UINT16	u16_temp1_0;

  	for(i16_temp_0 = 0;((i16_temp_0 < 30) && (SpibRegs.SPISTS.bit.INT_FLAG != 1));i16_temp_0++)
  	{
  		asm(" nop");
  	}
	u16_temp1_0 = SpibRegs.SPIRXBUF;

	return(u16_temp1_0);
}


//SPI操作函数
//#pragma CODE_SECTION(".Epwm1Funcs")

void Class_SPIDrv::Drv_SPIAct(void)						
{
    SpibRegs.SPITXBUF = 0x00;
	objDigitalIO.Dat_Int_SetExInReg(objSPIDrv.Drv_SPIReceiveData());
	asm(" rpt #5 ||nop");
	SpibRegs.SPITXBUF = objDigitalIO.m_un_ExOutAReg.all;
	objDigitalIO.Dat_Int_SetExInReg(objSPIDrv.Drv_SPIReceiveData());
	asm(" rpt #5 ||nop");
	SpibRegs.SPITXBUF = objDigitalIO.m_un_ExOutBReg.all;
	objDigitalIO.Dat_Int_SetExInReg(objSPIDrv.Drv_SPIReceiveData());
	asm(" rpt #5 ||nop");
	SpibRegs.SPITXBUF = objDigitalIO.m_un_ExOutCReg.all;
	objDigitalIO.Dat_Int_SetExInReg(objSPIDrv.Drv_SPIReceiveData());

	/*switch(SPITransmitCNT)
	{
		case 1:

			SpibRegs.SPITXBUF = objDigitalIO.m_un_ExOutAReg.all;
			break;

		case 2:

			SpibRegs.SPITXBUF = objDigitalIO.m_un_ExOutBReg.all;
			break;

		case 3:

			SpibRegs.SPITXBUF = objDigitalIO.m_un_ExOutCReg.all;

		default:
		{
			break;
		}
	}

	objDigitalIO.Dat_Int_SetExInReg(objSPIDrv.Drv_SPIReceiveData());
	SPITransmitCNT++;
	if((SPITransmitCNT > 3)||(SPITransmitCNT < 1))
		SPITransmitCNT = 1;
	*/
		
}



//===========================================================================
// End of file.
//===========================================================================

