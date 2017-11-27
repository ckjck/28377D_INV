/*==========================================================*/
/* Title		:	SCIDrv.cpp								*/
/* Description	: 	Class_ADCDrv realization 				*/
/* Date			:	Apr.2008								*/
/*==========================================================*/

#include "SCIDrv.h"

/**********************************************************************************
函数名：	Drv_SCIInit()			
功能描述：	SCI模块初始化
函数调用：	无
***********************************************************************************/
void	Class_SCIDrv::Drv_SCIInit()
{
	/* <=== SCI-A Registers initialization ===> */
    SciaRegs.SCICCR.all = 0x0007;            // 1 stop bit,  No loopback
                                             // No parity,8 char bits,
                                             // async mode, idle-line protocol
    SciaRegs.SCICTL1.all = 0x0003;           // enable TX, RX, internal SCICLK,
                                             // Disable RX ERR, SLEEP, TXWAKE
    SciaRegs.SCIHBAUD.bit.BAUD = 0x0001;            	 // RATE IS 9600 bit/s
    SciaRegs.SCILBAUD.bit.BAUD = 0x0045;
//    SciaRegs.SCICTL1.all = 0x0023;     		 // Relinquish SCI from Reset
//    SciaRegs.SCICTL2.bit.RXBKINTENA = 1;

/* <=== SCI-B Registers initialization ===> */
   ScidRegs.SCICCR.all = 0x0007;    		 // 1 stop bit,  No loopback 
                                             // No parity,8 char bits,
                                             // async mode, idle-line protocol
   ScidRegs.SCICTL1.all =0x0003;             // enable TX, RX, internal SCICLK, 
                                             // Disable RX ERR, SLEEP, TXWAKE
   ScidRegs.SCIHBAUD.bit.BAUD = 0x0001;
   ScidRegs.SCILBAUD.bit.BAUD = 0x0045;
   ScidRegs.SCICTL1.all = 0x0023;     		 // Relinquish SCI from Reset 

}


//===========================================================================
// End of file.
//===========================================================================

