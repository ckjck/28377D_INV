/*==========================================================*/
/* Title		:	RAMDrv.h								*/
/* Description	: 	Class_RAMDrv definition					*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#ifndef		RAMDRV_H 
             
#define     RAMDRV_H

class Class_RAMDrv
	{
	
	public:
		void 	Drv_MemCheck(void);
		void 	Drv_MemCopy(UINT16 *SourceAddr, UINT16 *SourceEndAddr, UINT16 *DestAddr);
		
	};
	
#endif

//===========================================================================
// End of file.
//===========================================================================

