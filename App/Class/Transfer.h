/*==========================================================*/ 	
/* Title		:	Transfer.h													*/ 	
/* Description	: 	Class_Transfer definition										*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
 #ifndef	TRANSFER_H
            
 #define    TRANSFER_H


//DSP异常切旁路用
#define 	DSPCrazyA	*(volatile UINT16 *)0xAFFC		//DSP CRAZY TEST BIT A
#define 	DSPCrazyB	*(volatile UINT16 *)0xAFFD		//DSP CRAZY TEST BIT B
#define 	DSPCrazyC	*(volatile UINT16 *)0xAFFE		//DSP CRAZY TEST BIT C
#define 	DSPCrazyD	*(volatile UINT16 *)0xAFFF		//DSP CRAZY TEST BIT D
                                                      		
	struct	structXferCmd
	{
		UINT16	bSwitchStatus				:1;				//BIT00		SWITCH_STATUS  0:旁路侧1:逆变侧								
		UINT16	bRev0301					:3;		
		UINT16	bSwitchMode					:2;				//BIT05-04	SWITCH_MODE	0:自主 1:不间断 2:间断	
		UINT16	bRev0607					:2;				//BIT07-06
		UINT16	bSwitchDesire				:1;				//BIT08		SWITCH_DESIRE	0:旁路侧1:逆变侧							
		UINT16	bIGBTOCPBak					:1;				//BIT09		 逆变器硬件过流备份信号 0:无过流1:有过流
		UINT16	bWaitingZeroCross			:1;				//BIT10		旁路切逆变间断切换时等待逆变过零点延迟标志 0:延时未开始或者已结束 1:延迟中
		UINT16	bSwOverlapDelay				:1;				//BIT11		208V重叠切换标志 0非重叠延时中	1重叠延时中			
		UINT16	bSwitchInterval				:1;				//BIT12		0:非切换间断中	1:切换间断中
		UINT16	bRev13						:2;				//BIT14-13
		UINT16	bOvloadSwitch				:1;				//BIT15		过载95％容量回切 0:非过载回切1:过载回切
	};				


	struct	structXferResult
	{
		UINT16	bInvKM						:1;				//BIT00		逆变STS	0:封锁 1:开放
		UINT16	bBpSTS						:1;				//BIT01		旁路STS	0:封锁 1:开放	
		UINT16	bInvKMBak					:1;				//BIT02		逆变STS备份bInvKMBak 0:封锁 1:开放
		UINT16	bEcoInvMdUpFlag				:1;				//BIT03	ECO模式下逆变不间断切旁路时有效值提高标志
																		//0:不需提升1:需提升
																
		UINT16	bEcoInvMdUpOk				:1;				//BIT04	ECO模式下逆变不间断切旁路时有效值提高结束标志
																		//0:未完成1:已完成
		UINT16	bRev1105					:7;				//BIT11-05
		UINT16	bInvSideDelay				:2;				//BIT13-12	逆变5分钟延时标志	
																		//00逆变侧不需延时
																		//10逆变侧需延时,但未开始
																		//11逆变侧需延时,已开始;延时到后转化为00
		UINT16	bBpSideDelay				:2;				//BIT15-14	旁路5分钟延时标志		
																		//00旁路侧不需延时
																		//10旁路侧需延时,但未开始
																		//11旁路侧需延时,已开始; 延时到后转化为00		
	};
	
	
	class Class_Transfer
	{
	public:	
			
		volatile struct	structXferCmd		m_st_wXferCmd,m_st_XferCmdBack;			//切换命令形成
		volatile struct	structXferResult	m_st_XferResult;						//切换结果

		INT16		m_i16BPDelayCnt;	
	    INT16		m_i16InvFreeRunBasedOnBpFreqCount;
		INT16		m_i16FlagInvFreeRunBasedOnBpFreq;
		INT16		m_i16wFlagInvFreeRunBasedOnBpFreqTraceMinus;	
		            
		INT16		m_i16wBptoinvTimes;			     	                   			//过载旁路切逆变次数,
		                                                                   			
		INT16	 	m_i16wBptoinvSwCntPer40ms;	     	                   			//旁路至逆变切换等待延时  
	  	INT16	m_i16BpSTSOffDelay;											//旁路晶闸管断开延时						                                           			
	  	INT16	m_i16BpSTSOffCnt;											//旁路晶闸管断开计数器						                                           			

		INT16	m_i16KMOffDelayFlag;
		INT16	m_i16KMOffDelay;
		INT16	m_i16KMOnCntPer14ms;
	private:                                                               			
		UINT16 		m_u16ClkUsIntSw;			     	                   			//间断切换延时起点, 12ms	            	                                                       			
		UINT16 		m_u16ClkSBpside;			     	                   			//过载跳旁路计时起点, 5分延时用	            	                                                       			
		UINT16 		m_u16ClkSInvside;			     	                   			//ECO跳逆变计时起点, 5分延时用	                                            				 
		INT16		m_i16SystemOvloadCntPerMloop;									//邻机过载确认计数器--每个小时区段(CLK_H)限制在设定次数		                                                                    		
		INT16 		m_i16XferBpID;													//切旁路代码
		//0:期望逆变侧   1:Q5断 2:Q3BP合 3:逆变器关闭 4:本机过载延时到  5:本机冲击跳旁路  6:邻机过载冲击转旁路 7:ECO                                              					                                                 					                                                              		
		INT16		m_i16XferBpIDBack;												//切旁路代码                        
		INT16 		m_i16XferBpIDArray[10];											//加强诊断用

	public:
		void 		App_XferOut(void);
			    	
	private:    	
		void		App_XferCommand(void);
		void		App_XferProcess(void);
		void		App_XferExecute(void);
	
	};
	
	
#endif	




//===========================================================================
// End of file.
//===========================================================================
