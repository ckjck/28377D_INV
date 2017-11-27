/*==========================================================*/
/* Title		:	WatchDogDrv.h								*/
/* Description	: 	Class_WatchDogDrv definition					*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#ifndef		WATCHDOGDRV_H 
             
#define     WATCHDOGDRV_H

class Class_WatchDogDrv
	{
	private:
		INT16		m_i16KickDogCntPerMloop;			//kick dog CNT
		
	public:
		void 		Drv_DisableDog(void);
		void 		Drv_EnableDog(void);
		void 		Drv_KickDog(void);
		
	};
	
#endif

//===========================================================================
// End of file.
//===========================================================================

