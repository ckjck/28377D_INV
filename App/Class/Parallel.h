/*==========================================================*/ 	
/* Title		:	Parallel.h														*/ 	
/* Description	: 	Class_Parallel definition											*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#ifndef 	PARALLEL_H
        	
#define 	PARALLEL_H
	
	struct	structParaSignal
	{
		UINT16	bInvOverload					:1;				//BIT00		INV_OVERLOAD		
		UINT16	bSystemUnlock					:1;				//BIT01		SYSTEM_UNLOCK		
		UINT16	bInvOnStandby					:1;				//BIT02		INV_ON_STANDBY		
		UINT16	bInvBpStatus					:1;				//BIT03		INV_BP_STATUS		
		UINT16	bInvState						:1;				//BIT04		bInvState					
		UINT16	bLocalMaster					:1;				//BIT05		LocalMaster	
		UINT16	bLBSAct							:1;				//BIT06		bMasterOnline confirm signal(0: not confirm, 1: confirm)				
		UINT16	bLBSWide						:1;				//BIT07		MASTER_ID					
		UINT16	bMasterOnline					:1;				//BIT08		1: Enable, 0: Disable		
		UINT16	bREV2							:7;				//BIT09-15		MASTER_ONLINE(note:need to update at once)				
	};                                                                  	
	struct	structParaSignalF
	{
		UINT16	bInvOverloadF					:1;				//BIT0		INV_OVERLOAD_F		
		UINT16	bSystemUnlockF					:1;				//BIT1		SYSTEM_UNLOCK_F		
		UINT16	bInvOnStandbyF					:1;				//BIT2		INV_ON_STANDBY_F		
		UINT16	bInvBpStatusF					:1;				//BIT3		INV_BP_STATUS_F		
		UINT16	bMasterOnlineF					:1;				//BIT04		bMasterOnlineF
		UINT16	bExtOutStatusF					:1;				//BIT05		bExtOutStatusF					
		UINT16	bCabQ3BpStatusF					:1;				//BIT06		bCabQ3BpStatusF	
		UINT16	bLBSDetect						:1;				//BIT07		bLBSDetect		
		UINT16	bLeader_Rack					:1;	        	//BIT08		bLeader_Rack
		UINT16	bRev							:6;				//BIT14-09		
		UINT16	bConnectOKF						:1;				//BIT15		CONNECT_OK_F		
	};

	struct	structParaMaint
	{
		UINT16	bOutIsoDisable					:1;				//BIT00		OutIsoDisable位，输出隔离禁止位		
		UINT16	bIsolation						:1;				//BIT01		Isolation位，置位表示并机进入隔离态					
		UINT16	bParaSignalMask					:1;				//BIT02		ParaSignalMask位，用来屏蔽并机信号，置位有效				
		UINT16	bUPSTestModeBak					:1;				//BIT03		调测模式备份位(仅用于NXa系列)		
		UINT16	bTestAbnormal					:1;				//BIT04		调测模式异常状态(仅用于NXa系列)0无	1有		
		UINT16	bTestingState					:1;				//BIT05		用于面板显示的调测模式(仅用于NXa系列)0无	1有		
		UINT16	bRev1506						:10;			//BIT15-06		
	};
	
	class Class_Parallel
	{
	public:

		INT16		i16TempInvOn;
		volatile struct	structParaMaint		m_st_wParaMaint; //并机不拔并机线维护状态
		volatile struct	structParaSignal		m_st_wParaSignal,m_st_wParaSignalBak;			
				//对应EXT_OUT B A, 备份变量屏蔽其中的BIT08:MASTER_ID 及BIT10:MASTER_COMM,这两位由中断程序操作
		volatile struct	structParaSignalF		m_st_wParaSignalF,m_st_wParaSignalFBak;	//对应EXT_IN D C	
		                                  			
//		INT16		m_i16IcirMd_0;							//环流d轴分量
//		INT16		m_i16IcirMq_0;							//环流q轴分量
//		INT16		m_i16IcirMdAvg_0;						//环流d轴分量
//		INT16		m_i16IcirMqAvg_0;						//环流q轴分量		            		
		
	private:
		
		INT16		m_i16IoutaMean_0;						//A相均分电流
		INT16		m_i16IoutbMean_0;						//B相均分电流
		INT16		m_i16IoutcMean_0;						//C相均分电流
		
		INT16		m_i16KIavg_0;							//平均电流定标系数
//		INT16		m_i16KIavgAdj_0;	    				//平均电流调整后的定标系数(AC+Unlike)  
		                                   			
	                                     			
		INT16		m_i16SpanParaFault;						//并机均流失败计数器
		INT16		m_i16ParaBrdCntPerMloop;				//并机板异常计数器
		INT16		m_i16ParaFaultCntPer40ms;   			
		INT16		m_i16ShareFaultCnt_0;					//逆变均流异常计数器
		
	public:
//		inline	void		App_ParaVarInit(void);	
//		void		App_ParaParamConfig(void);
		        	
//		void		Dat_Int_ParaPreDeal(void);
		        	
		void		Alg_ParaStatus(void);
		void		Alg_ParaFault(void);
	
	};

#endif
//===========================================================================
// End of file.
//===========================================================================
