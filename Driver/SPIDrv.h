/*==========================================================*/
/* Title		:	SPIDrv.h								*/
/* Description	: 	Class_SPIDrv definition					*/
/* Date			:	Apr.2008								*/
/*==========================================================*/

#ifndef    SPIDRV_H
       
#define	   SPIDRV_H

class Class_SPIDrv
	{
	
	public:
		
		UINT16	SPITransmitCNT;
		void 	Drv_SPIInit(void);
		void	Drv_SPIAct(void);
		UINT16	Drv_SPIReceiveData(void);
		
	};
	
 #endif	
//===========================================================================
// End of file.
//===========================================================================
