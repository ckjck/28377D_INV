/*==========================================================*/ 	
/* Title		:	InverterControl.h												*/ 	
/* Description	: 	Class_InverterControl definition									*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#ifndef		INVERTERCONTROL_H
       
#define     INVERTERCONTROL_H

struct	structFlagInvOnoff
	{
		UINT16	bMonitorOn					:1;				//BIT00		监控开机    	1 开机0 无		
		UINT16	bFaultAutoOn				:1;				//BIT01		故障恢复自开机	1 开机  (如过热恢复)0 无		
		UINT16	bSwitchAutoOn				:1;				//BIT02		输出切换自开机  1 开机	0 无		
//cb 030724 用户关机改为先切换，后关机，bUserOff只切换，bUserOffAct才关机		
//注意这里进行了位定义交换：原BIT4移至BIT3， 新BIT4作为关机命令!
		UINT16	bUserOff					:1;				//BIT03		用户关机切换	1 关机切换	0 无		
		UINT16	bUserOffAct					:1;				//BIT04		用户关机命令	1 关机	0 无		
		UINT16	bEPO						:1;				//BIT05		紧急关机		1 关机	0 无		
		UINT16	bInvFaultOff				:1;				//BIT06		逆变故障关机	1 关机	0 无		
		UINT16	bIGBTOCPOff					:1;				//BIT07		IGBT过流关机	1 关机	0 无		
		UINT16	bInstantOff					:1;				//BIT08		立即关机		1 关机	0 无		
		UINT16	bOutDisableOff				:1;				//BIT09		禁止态关机  	1 关机	0 无		
		UINT16	bBpFailOff					:1;				//BIT10		旁路异常关机	1 关机  (逆变过载时间到旁路异常 旁路过载下异常 过载后旁路5分钟内异常 回切次数到旁路异常)0 无
		UINT16	bAutoRecoverOff				:1;				//BIT11		可自行恢复故障关机 	1 关机	0 无		
		UINT16	bManualRecoverOff			:1;				//BIT12		可人为恢复故障关机 	1 关机	0 无		
		UINT16	bNeverRecoverOff			:1;				//BIT13		不可恢复故障关机  	1 关机	0 无		
		UINT16	bSwitchAutoOff				:1;				//BIT14		输出切换自关机	 	1 关机	0 无		
		UINT16	bFinalOnOff					:1;				//BIT15		总开关机  			1 开机	0 关机 		
	};	
	
	class Class_InverterControl
	{
	public:
		// 以下变量允许外界执行写操作，用于从故障，监控，切换等模块收集各种开关机命令！
		volatile struct	structFlagInvOnoff	m_st_wFlagInvOnoff;						//逆变开关机状态
		volatile struct	structFlagInvOnoff	m_st_FlagInvOnoffBak;					//逆变开关机状态
		
		INT16		m_i16InvSSCntPer40ms;											//逆变器软启动计时, 以40ms为单位
                                                									
		INT16		m_i16FlagOffReason;												//逆变关机原因	 	
																					//			BIT03--00	关机原因(分等级,可覆盖)
																					//	0000--开机状态
																					//	0001--切换关机
																					//	0010--可自恢复故障关机
																					//	0011--正常关机或可人为 恢复故障关机
																					//	0100--需清除故障类关机 如紧急关机,禁止态关机，旁	
																					//           路过流关机
																					//	0101--不可恢复故障关机
																					//

	private:
		INT16		m_i16wVrefTgtMd;													//电压目标矢量d轴分量	
		INT16		m_i16AutoOnDlyCntPer40ms;
		INT16		m_i16SoftStartCount;				//test
		INT16		m_i16MasterOnlineConfCnt;										//主机竞争延时计数器
		INT16		m_i16ModelReadyCnt;
		INT16		m_i16AmpLockBpCnt;				//幅值锁定延时计数器				
		INT16		m_i16AmpLockTgtCnt;				//幅值锁定延时计数器				

	public:
		inline	void		App_InvCtrlVarInit(void);
		void		App_InvCtrl(void);
			    	
	private:   
		void		App_InvOffReason(void); 	
		void		App_InvOnoffAgent(void);
		void		App_InvOnoffAction(void);
		void		App_InvMaster(void);
		void		App_InvTrace(void);
		void		App_InvSoftstart(void);
		void		App_InvStandby(void);
                	
//		void		App_InvOffReason(void);
                	
		void		App_TraceSignal( void );
		void		App_TraceControl( void );
		void		App_TraceJudge( void );
		
	};

#endif
//===========================================================================
// End of file.
//===========================================================================
