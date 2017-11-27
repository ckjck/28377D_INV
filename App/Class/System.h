/*==========================================================*/ 	
/* Title		:	System.h														*/ 	
/* Description	: 	Class_System definition											*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#ifndef 	SYSTEM_H

#define		SYSTEM_H

	struct structFlagStatus
	{	
		UINT16	bVbp					:2;					//BIT01-00	旁路状态 0：正常1：超切换范围3：超保护范围		
		UINT16	bInv					:2;					//BIT03-02	逆变状态 0：关闭1：软启动3：启动完毕		
		UINT16	bLock					:1;					//BIT04		逆变锁相状态	0：不同步1：同步		
		UINT16	bUserOffBak				:1;					//BIT05		用户关机命令备份，用于解决2ms问题0:无1:有		
		UINT16	bSupply					:2;					//BIT07-06	系统供电状态00均不供电01旁路供电10逆变供电	
		UINT16	bEPO					:1;					//BIT08		EPO状态	0否	1是(状态)		
		UINT16	bInvFault				:1;					//BIT09		逆变器故障	0否	1是(状态)		
		UINT16	bMainFeedBack			:1;					//BIT10		IGBT过流	0否	1是(状态)		
		UINT16	bOutDisable				:1;					//BIT11		输出禁止	0否	1是(状态)		
		UINT16	bIntSwitch				:1;					//BIT12		间断切换提示0否	1是(提示)		
		UINT16	bSingleRiskOff			:1;					//BIT13		单机关机风险提示0否	1是(提示)		
		UINT16	bParaRiskOff			:1;					//BIT14		并机关机风险提示	0否	1是(提示)		
		UINT16	bPow15vFaultBak			:1;					//BIT15	 	POW_OK信号备份 		0否	1是			
	};
	
	struct	structFlagPrompt
	{	
		UINT16	bDCAbnormalOff			:1;					//BIT00		 母线电压异常关机  		0否	1是		
		UINT16	bSwitchTimesClamp		:1;					//BIT01		 本小时过载切换次数到	0否	1是		
		UINT16	bImpactSwitch			:1;					//BIT02		 本机逆变冲击跳旁路   	0否	1是		
		UINT16	bReqSwitch				:1;					//BIT03		 系统请求转旁路（邻机过载冲击）0否1是		
		UINT16	bParaShareFault			:1;					//BIT04		 并机均流故障 	0否	1是		
		UINT16	bDCAbnormalOff1			:1;					//BIT05		
		UINT16	bParaCableFault			:1;					//BIT06		 并机线连接故障 	0否	1是		
		UINT16	bInvOnEnable			:1;					//BIT07		 逆变器开机允许标志  	0否	1是		
		UINT16	bUnitCapPrompt			:1;					//BIT08		 系统需间断切换, 负载大于单机容量 	0否	1是		
		UINT16	bTotalCapPrompt0		:1;					//BIT09		 系统负载大于当前有效UPS总容量(提示) 	0否1是		
		UINT16	bTotalCapPrompt			:1;					//BIT10		 系统负载大于当前有效UPS总容量(经延时)	0否1是		
		UINT16	bParaOvload				:1;					//BIT11		 0否1是		
		UINT16	bShortCircuit			:1;					//BIT12		 逆变输出短路（状态）0否1是		
		UINT16	bVoutAbnormal			:1;					//BIT13		 输出电压异常告警	0否	1是			
		UINT16	bPow24vFaultConf			:1;					//BIT14		 确认后的POWOKFAULT或PowFault2故障 	0否	1是			
		UINT16	bPow15vFault				:1;					//BIT15		 POW_OK故障 	0否	1是		
	};	
	
	struct	structFlagFault
	{
		UINT16	bInvFault				:1;					//BIT00		逆变器故障	0否	1是		
		UINT16	bInvCurrentUnbalanced	:1;					//BIT01		三单下逆变三相电流不平衡 0否 1是					
		UINT16	bBpCurrentUnbalanced	:1;					//BIT02		三单下旁路三相电流不平衡 0否 1是
		UINT16	bFanFault				:1;					//BIT03		风扇故障	0否	1是		
		UINT16	bDCOV					:1;					//BIT04		母线过压	0否	1是		
		UINT16	bVoutAbnormal			:1;					//BIT05		输出电压异常0否	1是		
		UINT16	bBpOverCurrent			:1;					//BIT06		旁路过流	0否	1是		
		UINT16	bInvDcOffset			:1;					//BIT07		输出直流分量高	0否	1是		
		UINT16	bOperFault				:1;					//BIT08		用户操作错误	0否	1是		
		UINT16	bOvload					:1;					//BIT09		输出过载		0否	1是	
		UINT16	bInvOvloadTimeout		:1;					//BIT10		单机输出过载超时0否	1是		
		UINT16	bBypFeedBackFault		:1;					//BIT11		IGBT过流		0否	1是		
		UINT16	bAuxPS2Fault			:1;					//BIT12		辅助电源故障	
		UINT16	bDCAbnormal				:1;					//BIT13		母线异常		0否	1是		
		UINT16	bBpInvtSeq				:1;					//BIT14		旁路反相		0否	1是		
		UINT16	bBpFailOff				:1;					//BIT15		旁路异常关机	0否	1是	
	};
	
	struct	structFlagSystem
	{
		UINT16 	bAfterParamDly			:1;					//BIT00				0-无1-参数设置后延时结束				
		UINT16 	bKeyModuleDly			:1;					//BIT01				0-无1-核心模块运行前延时完毕		
		UINT16 	bParamSetReq			:1;					//BIT02				1-请求参数设置	
		UINT16 	bUPSTestMode			:1;					//BIT03				1-调测模式设置位
		UINT16 	bParamFresh				:1;					//BIT04				1-参数设置更新标志			
		UINT16	bNewFaultArise			:1;					//BIT05				1-新故障出现标志				
		UINT16	bRecordSaveRequest		:1;					//BIT06				1-故障记录上传请求标志			
		UINT16	bSelfAgingAction		:1;					//BIT07				1-自老化模式作用位，只在上电设置时有效			
		UINT16 	bRAMError				:1;					//BIT08				内存自检错		
		UINT16 	bServiceMode			:1;					//BIT09				维修检测模式		
		UINT16 	bWDStart				:1;					//BIT10				开始喂狗标志			
		UINT16	bTimer1ms				:1;					//BIT11				1ms定时标志		每1ms置位一次,供后续模块查询.该标志在进入时清除.		
		UINT16	bSelfAgingSet			:1;					//BIT12				1-自老化模式设置位，开放给SCI模块			
		UINT16 	bIoTestMode				:1;					//BIT13 				DSP工装模式				
		UINT16 	bTimer14ms				:1;					//BIT14  			14ms定时标志 每14ms置位一次,供后续模块查询.该标志在进入时清除.		
		UINT16 	bTimer40ms				:1;					//BIT15  			40ms定时标志 每40ms置位一次,供后续模块查询.该标志在进入时清除.		
	};
	class Class_System
	{
	public:
		volatile struct structFlagStatus	m_st_wFlagStatus,m_st_wFlagStatusBak;	//系统状态, 发往监控
		volatile struct	 structFlagPrompt	m_st_wFlagPrompt,m_st_wFlagPromptBak;	//辅助监控状态
		volatile struct	 structFlagFault		m_st_wFlagFault,m_st_wFlagFaultBak;		//故障状态，发往监控
		volatile struct	 structFlagSystem	m_st_wFlagSystem;					//系统状态标志
				
		INT16		m_i16wNumMainloop;											//主循环标号0-9循环, 用于分时处理
		                                  								
		INT16		m_i161DivGrantKVA;											//iGrantKVA的倒数,用于将CAN 算法中的除法运算改为乘法
		INT16		m_i161DivGrantRate;											//授权率的倒数
//		INT16		m_i16GrantKVABak; 											//iGrantKVA变量的备份
        UINT16		m_u16TimerBig1ms;                                   								
		UINT16		m_u16DSPRevision;    
		INT16		m_i16wFlagCPLDVerErr;
		                                 								
		INT16		m_i16wAfterParamCntPer40ms;									//参数设置后延时, 以40ms为单位
		INT16		m_i16wKeyModuleCntPer40ms;									//核心模块运行前计数, 以40ms为单位
		INT16		m_i16wWDStartCntPer40ms;									//硬件狗延时计数器
		
		INT16		m_i16PoutaTotal;					//并机A相总输出有功功率(来自旁路)
		INT16		m_i16PoutbTotal;					//并机B相总输出有功功率(来自旁路)
		INT16		m_i16PoutcTotal;					//并机C相总输出有功功率(来自旁路)
		INT16		m_i16SoutaTotal;					//并机A相总输出视在功率(来自旁路)
		INT16		m_i16SoutbTotal;					//并机B相总输出视在功率(来自旁路)
		INT16		m_i16SoutcTotal;					//并机C相总输出视在功率(来自旁路)

		INT16		m_i16RackPoutaTotal;			//机架间并联并机--- A相总输出有功功率(来自旁路)
		INT16		m_i16RackPoutbTotal;			//机架间并联并机--- B相总输出有功功率(来自旁路)
		INT16		m_i16RackPoutcTotal;			//机架间并联并机--- C相总输出有功功率(来自旁路)
		INT16		m_i16RackSoutaTotal;			//机架间并联并机--- A相总输出视在功率(来自旁路)
		INT16		m_i16RackSoutbTotal;			//机架间并联并机--- B相总输出视在功率(来自旁路)
		INT16		m_i16RackSoutcTotal;			//机架间并联并机--- C相总输出视在功率(来自旁路)


		INT16		m_i16PoutTotalMax;
		INT16		m_i16SoutTotalMax;
		INT16		m_i16RackPoutTotalMax;
		INT16		m_i16RackSoutTotalMax;
		  
		INT16		m_i16CPLDVerD1;											//CPLD版本号
		INT16		m_i16CPLDVerD2;				//CPLD版本号								//CPLD版本号
		INT16		m_i16CPLDVerD3;	                                								
	private: 
		INT16		m_i16FaultLedCnt;
		INT16		m_i16NormalLedCnt;
		
		INT16		m_i16FanFaultCntPer1ms;									//风扇故障确认计数器								
		INT16		m_i16PowerFaultCntPer40ms;
		INT16 		m_i16PowerOKWrongCntPerMloop;								//POW_OK 故障确认计数器	
		INT16		m_i16ModuleOkCnt;										//ModuleReady信号确认计数器
		INT16		m_i16ModuleNotOkCnt;
	public:
		void		App_SysVarInit(void);	
		void		App_ModelOnline(void);
		void		App_SysParamConfig(void);
			    	
		void		App_DSPIDCheck(void);		
		void		App_CPLD_Ver(void); 
		 void     	App_Module_Ok(void);
		 void	App_Module_LED(void);
		void		Alg_SysFault(void);		
		void		Alg_OperFault(void);
	
		
	};
	
#endif	
//===========================================================================
// End of file.
//===========================================================================
