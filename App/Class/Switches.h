/*==========================================================*/ 	
/* Title		:	Switches.h													*/ 	
/* Description	: 	Class_Switches definition										*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#ifndef 	SWITCHES_H

#define		SWITCHES_H

	struct	structFlagBreaker
	{
		UINT16	bQ5					:1;				//BIT00		输出空开状态 0断	1合		
		UINT16	bBp					:1;				//BIT01		旁路供电状态 0否 1是		
		UINT16	bInv				:1;				//BIT02		逆变器开启状态 0否	1是		
		UINT16	bOn					:1;				//BIT03		逆变器开机 0否	1是		
		UINT16	bQ3bp				:1;				//BIT04		维修旁路空开 0断	1合		
		UINT16	bReqBpSTSShort		:1;				//BIT05		是否邻机旁路晶闸管短路故障 0本机	1邻机
		UINT16	bFuse				:1;				//BIT06		输出熔丝 0断	1合		
		UINT16	bBackFeed			:1;				//BIT07     旁路反灌标志，送给监控板，以及用以触发故障记录					
		UINT16	bBpSTS				:1;				//BIT08		旁路晶闸管驱动 0断	1合		
		UINT16	bInvKM				:1;				//BIT09		逆变晶闸管驱动 0断	1合		
		UINT16	bBpOn				:1;				//BIT10		旁路正供电标志	0否	1是		
		UINT16	bKM5FClose			:1;				//BIT11		208V逆变接触器可靠导通标志（连续确认若干个主循环）				0否				1是		
		UINT16	bBpSTSOpen			:1;				//BIT12		旁路晶闸管开路故障 0无故障	1断路		
		UINT16	bBpSTSShort			:1;				//BIT13		旁路晶闸管短路故障 0无故障	1短路		
		UINT16	bInvKMOpen			:1;				//BIT14		逆变晶闸管开路故障 0无故障	1断路		
		UINT16	bInvKMShort			:1;				//BIT15		逆变晶闸管短路 0无故障	1短路		
	};			
	class Class_Switches
	{
	public:
		volatile struct	structFlagBreaker	m_st_FlagBreaker,m_st_wFlagBreakerBak;	//空开状态, 发往监控
//		volatile struct	structFlagTrueBreaker	sFlagTrueBreaker;			//空开状态, 发往监控
		INT16		m_i16VbackFeed_0; 						//旁路反灌保护零地电压检测量
//		INT16		m_i16KbackFeed_0;						//旁路反灌保护零地电压定标系数
		UNLONG		m_unVbackFeedRms;						//旁路反灌保护电压有效值(含滤波尾数)
		
	private:	
		UNLONG		m_unVbackFeedSum_0;						//旁路反灌保护零地电压平方累加和,中断累加用
		UNLONG		m_unVbackFeedSumM_0;					//旁路反灌保护零地电压平方周期和,主程序求有效值用
		INT16		m_i16VbackFeedPrms;						//旁路反灌保护电压即时有效值
		  
		UNLONG		m_unIinvaFilter_0;   			        //逆变电流滤波值
		UNLONG		m_unIinvbFilter_0;                      //逆变电流滤波值
		UNLONG		m_unIinvcFilter_0;                      //逆变电流滤波值
		           
		INT16		m_i16BpSTSBackwardFaultCnt_0;			//旁路晶闸管短路确认计数
		INT16		m_i16BpSTSForwardFaultCntPer40ms;		//旁路晶闸管短路确认计数 FEED FORWARD
		INT16		m_i16BpSTSOpenCntPer40ms;				//旁路晶闸管开路确认计数
		INT16 		m_i16FeedbackVcomCntPer14ms;			//旁路反灌保护
		INT16		m_i16InvSTSFaultCntPer40ms;				//逆变晶闸管短路确认计数
		INT16		m_i16InvSTSFault1CntPer14ms;
		INT16		m_i16InvSTSOpenCntPer40ms;				//逆变晶闸管开路确认计数
		INT16		m_i16SCRDiffFastChkCnt_0;				//输出各相电压压差快速检测计数器	
		INT16		m_i16InvKMFastChkCnt;
		INT16		m_i16InvSTSFaultDisableCntPer40ms; //旁路晶闸管短路禁止计数器---当三相电压全有时,不通过检输出电压报旁路晶闸管故障
		INT16		m_i16STSShortCntPer40ms;			//旁路短路确认计数器
		
	public:
		inline	void		App_SwitchesVarInit(void);
//		inline	void		App_SwitchesParamConfig(void);
		        	
//		void 		Dat_Int_VBackFeedCal(void);
//		void		Dat_VBackFeedRMS(void);
		        	
		void		Dat_BreakerStatus(void);
		        	
//		void		Alg_Int_BpSTSShort(void);
		void		Alg_Int_STSDiffFastChk(void);
		void		Alg_Int_INVKMFastChk(void);
		void		Alg_STSShortFault(void);
		void		Alg_STSOpenFault(void);
		
	};

#endif
//===========================================================================
// End of file.
//===========================================================================
