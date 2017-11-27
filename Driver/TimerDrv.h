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
		UINT16		m_u16ClkUsBack;				//ʱ�ӵ�λ���ݣ����ڼ��Ӷ�ʱ�ж�
		UINT16		m_u16ClkUs_0;				//ʱ�ӵ�λ,166.7us
		UINT16		m_u16ClkS_0;				//ʱ�ӵ�λ,1s :6000*166.7uS
		UINT16		m_u16ClkH_0;				//ʱ�ӵ�λ,1h :3600*1S
		UINT32		m_u32ClkIntStart_0;
	private:
		UINT16 		m_u16ClkHOld;				//Сʱ����
		INT16		m_i16NumForBig1ms;			//1.125ms��ʱ��
		INT16		m_i16NumFor1ms;				//1ms��ʱ��
		INT16		m_i16NumFor14ms;			//14ms��ʱ��
		INT16		m_i16NumFor40ms;			//40ms��ʱ��
		INT16		m_i16NumForRuning;			//40ms���е���˸������
		INT16		m_i16LED;					//LEDָʾ��
		
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

