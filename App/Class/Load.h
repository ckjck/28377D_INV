/*==========================================================*/ 	
/* Title		:	Load.h														*/ 	
/* Description	: 	Class_Load definition											*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#ifndef		LOAD_H
       
#define     LOAD_H


	struct	structFlagInvLoadAB	
	{
		UINT16	bLoadA					:5;					//BIT04-00	本机A相逆变负载标志	bLoadA
																		//00000 95%以下
																		//00001 95%-105%
																		//00011 105%-110%
																		//00111 110%-125%
																		//01111 125%-150%
																		//11111 150%以上		
		UINT16	bRev0705				:3;					//BIT07-05
		UINT16	bLoadB					:5;					//BIT12-08	本机B相逆变负载标志 bLoadB
																		//00000 95%以下
																		//00001 95%-105%
																		//00011 105%-110%
																		//00111 110%-125%
																		//01111 125%-150%
																		//11111 150%以上    	
    	UINT16	bRev1513				:3;					//BIT15-13
    	
	};			

	
	struct	structFlagInvLoadCT
	{
		UINT16	bLoadC					:5;					//BIT04-00	本机C相逆变负载标志	bLoadC
																		//00000 95%以下
																		//00001 95%-105%
																		//00011 105%-110%
																		//00111 110%-125%
																		//01111 125%-150%
																		//11111 150%以上		
		UINT16	bLoadT					:5;					//BIT09-05	本机逆变负载总标志	bLoadT
																		//00000 95%以下
																		//00001 95%-105%
																		//00011 105%-110%
																		//00111 110%-125%
																		//01111 125%-150%
																		//11111 150%以上		
		UINT16	bLoadTotalT				:5;					//BIT14-10	本机逆变负载总标志	bLoadT
																		//00000 95%以下
																		//00001 95%-105%
																		//00011 105%-110%
																		//00111 110%-125%
																		//01111 125%-150%
																		//11111 150%以上
		UINT16	bRev15					:1;
		
	};			

	struct	structFlagILLoadAB	
	{
		UINT16	bLoadA					:5;					//BIT04-00	本机A相逆变电感负载标志	bLoadA
																		//00000 95%以下
																		//00001 95%-105%
																		//00011 105%-110%
																		//00111 110%-125%
																		//01111 125%-150%
																		//11111 150%以上		
		UINT16	bRev0705				:3;					//BIT07-05
		UINT16	bLoadB					:5;					//BIT12-08	本机B相逆变电感负载标志 bLoadB
																		//00000 95%以下
																		//00001 95%-105%
																		//00011 105%-110%
																		//00111 110%-125%
																		//01111 125%-150%
																		//11111 150%以上    	
    	UINT16	bRev1513				:3;					//BIT15-13   	
	};	


	struct	structFlagILLoadCT
	{
		UINT16	bLoadC					:5;					//BIT04-00	本机C相逆变电感负载标志	bLoadC
																		//00000 95%以下
																		//00001 95%-105%
																		//00011 105%-110%
																		//00111 110%-125%
																		//01111 125%-150%
																		//11111 150%以上		
		UINT16	bRev0705				:3;					//BIT07-05
		UINT16	bLoadT					:5;					//BIT12-08	本机逆变电感负载总标志	bLoadT
																		//00000 95%以下
																		//00001 95%-105%
																		//00011 105%-110%
																		//00111 110%-125%
																		//01111 125%-150%
																		//11111 150%以上		
		UINT16	bRev1513				:3;					//BIT15-13	
	};			

	struct	structFlagOutLoadABCT
	{
		UINT16	bLoadA					:4;						//BIT03-00	本机A相输出负载标志 bLoadA
																			//0000 95%以下
																			//0001 95%-100%
																			//0011 100%-105%
																			//0111 105%以上		
		UINT16	bLoadB					:4;						//BIT07-04	本机B相输出负载标志 bLoadB
																			//0000 95%以下
																			//0001 95%-100%
																			//0011 100%-105%
																			//0111 105%以上
		
		UINT16	bLoadC					:4;						//BIT11-08	本机C相输出负载标志	bLoadC
																			//0000 95%以下
																			//0001 95%-100%
																			//0011 100%-105%
																			//0111 105%以上
		
		UINT16	bLoadT					:4;						//BIT15-12	本机输出负载总标志	bLoadT
																			//0000 95%以下
																			//0001 95%-100%
																			//0011 100%-105%
																			//0111 105%以上
	 	
	};

	struct	structFlagload
	{
		UINT16	bRev0700				:8;		
		UINT16	bPSoutHigh				:1;						//BIT8		输出检测过载（仅根据P/SOUT确认）bPSoutHigh						//相当于原PSOUT_FLAG		
		UINT16	bRev1009				:2;		        		//BIT10-09
		UINT16	bReqSwitch				:1;						//BIT11		邻机过载冲击跳旁路				bReqSwitch		
		UINT16	bImpactSwitch			:1;						//BIT12		本机冲击跳旁路(旁路正常下)		bImpactSwitch		
		UINT16	bShortCircuit			:1;						//BIT13		三单下快检输出短路
		UINT16	bOvload					:1;						//BIT14		输出总过载(包括旁路/逆变侧)	bOvload		
		UINT16	bInvOvloadTimeout		:1;						//BIT15		逆变过载延时到				 			
	};


	class Class_Load
	{
	public:			
		volatile struct structFlagload			m_st_wFlagLoad;				//负载相关标志
		INT16		m_i16PinvaGrant;								//A相有功功率降额后容量
		INT16		m_i16PinvbGrant;								//B相有功功率降额后容量
		INT16		m_i16PinvcGrant;								//C相有功功率降额后容量
		
		INT16		m_i16SinvaGrant;								//A相视在功率降额后容量
		INT16		m_i16SinvbGrant;								//B相视在功率降额后容量
		INT16		m_i16SinvcGrant;								//C相视在功率降额后容量
		
		INT16		m_i16PSinvaGrantMAX;							////A相有功无功功率降额后容量最大值
		INT16		m_i16PSinvbGrantMAX;							////B相有功无功功率降额后容量最大值
		INT16		m_i16PSinvcGrantMAX;							////C相有功无功功率降额后容量最大值
		
		INT16		m_i16PoutaGrant;								//A相有功功率降额后容量
		INT16		m_i16PoutbGrant;								//B相有功功率降额后容量
		INT16		m_i16PoutcGrant;								//C相有功功率降额后容量
		
		INT16		m_i16SoutaGrant;								//A相视在功率降额后容量
		INT16		m_i16SoutbGrant;								//B相视在功率降额后容量
		INT16		m_i16SoutcGrant;								//C相视在功率降额后容量
		
		INT16		m_i16PSoutaGrantMAX;							////A相有功无功功率降额后容量最大值
		INT16		m_i16PSoutbGrantMAX;							////B相有功无功功率降额后容量最大值
		INT16		m_i16PSoutcGrantMAX;							////C相有功无功功率降额后容量最大值
		
	
	private:
		volatile struct	structFlagInvLoadAB		m_st_FlagInvLoadAB;			//本机逆变负载标志
		volatile struct	structFlagInvLoadCT		m_st_FlagInvLoadCT;			//本机逆变负载标志
		volatile struct	structFlagILLoadAB		m_st_FlagILLoadAB;		
		volatile struct	structFlagILLoadCT		m_st_FlagILLoadCT;			//本机逆变电感负载标志
		volatile struct	structFlagOutLoadABCT	m_st_FlagOutLoadABCT;		//本机输出负载标志   		
			
		INT16	 	m_i16OvloadSignalCntPerMloop;							//INV_OVERLOAD信号持续时间 (20个主循环周期)
																			//确保并机系统其它各机收到该信号
		INT16		m_i16OvloadConfCntPer40ms;								//逆变过载确认次数, 用于确认过载
		INT16		m_i16PSoutHighConfCntPer40ms;							//输出过载确认次数，用于确认过载
		INT32		m_i32OvloadDlyCntPer40ms;								//过载延时计数器(主程序平均每40ms累加/递减一次)
	                                            						
		INT16		m_i16Powerbacka;										//旁路A相回切逆变比例增加功率
		INT16		m_i16Powerbackb;										//旁路B相回切逆变比例增加功率
		INT16		m_i16Powerbackc;										//旁路C相回切逆变比例增加功率

public:		
		void 		Dat_LoadDeal(void);	
		        	
	private:   
		void	Dat_LoadGrant(void);
		void	Alg_ShortCircuitDetect(void);
		void		Dat_LoadFlag(void);
		void		Dat_OvloadFlag(void);
		void		App_OvloadDelay(void);
		void		App_OvloadSignal(void);
		
	};
	
	
//负载标志处理模块vLoad_Flag()
const FLAG_TABLE InvLoadTable =
{ 
	6,
  	26,
  	{0, 973, 1075, 1126, 1280, 1536, 10000},
  	{0x0000, 0x0001, 0x0003, 0x0007, 0x000f, 0x001f}
};

const FLAG_TABLE OutLoadTable =
{ 
	4,
  	26,
  	{0, 973, 1024, 1075, 10000},
  	{0x0000, 0x0001, 0x0003, 0x0007}
};

#endif
//===========================================================================
// End of file.
//===========================================================================
