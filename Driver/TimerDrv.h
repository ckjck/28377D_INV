/*==========================================================*/
/* Title		:	TimerDrv.h								*/
/* Description	: 	Class_TimerDrv definition					*/
/* Date			:	Apr.2008								*/

/*==========================================================*/
#ifndef		TIMERDRV_H 
             
#define     TIMERDRV_H

class Class_TimerDrv
	{
	public:	
		UINT16		m_u16ClkUsBack;				//时钟单位备份，用于监视定时中断
		UINT16		m_u16ClkUs_0;				//时钟单位,166.7us
		UINT16		m_u16ClkS_0;				//时钟单位,1s :6000*166.7uS
		UINT16		m_u16ClkH_0;				//时钟单位,1h :3600*1S
		UINT32		m_u32ClkIntStart_0;
	private:
		UINT16 		m_u16ClkHOld;				//小时备份
		INT16		m_i16NumForBig1ms;			//1.125ms定时用
		INT16		m_i16NumFor1ms;				//1ms定时用
		INT16		m_i16NumFor14ms;			//14ms定时用
		INT16		m_i16NumFor40ms;			//40ms定时用
		INT16		m_i16NumForRuning;			//40ms运行灯闪烁计数器
		INT16		m_i16LED;					//LED指示灯
		
	public:
		void		Drv_SysTimer(void);
		void 		Drv_Int_ClockMaintain(void);
		void 		Drv_usDelay(INT32 Count);
		void		Drv_CPUTimerInit(void);
	};

#endif

//===========================================================================
// End of file.
//===========================================================================

