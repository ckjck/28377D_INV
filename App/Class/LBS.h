/*==========================================================*/ 	
/* Title		:	LBS.h														*/ 	
/* Description	: 	Class_LBS definition											*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#ifndef			LBS_H
        
#define         LBS_H
	
 struct	structLBSStatus
	{
		UINT16	bLBSDetect					:2;					//BIT01--00	 LBS总线状态	00 LBS_NO NE01 LBS_INV	10 LBS_BYP		
		UINT16  bLbsAbnormal				:1; 				//BIT02	 	LBS总线通断状态  0  通 1  不正常			
		UINT16	bLbsActive					:1;					//BIT03		LBS激活
		UINT16	bCAPLBSFlag					:1;					//BIT04		0:本中断未捕捉到LBS信号，1：本中断捕捉到LBS信号
		UINT16	bRev1505					:11;                //BIT15-05
	}; 
	
	
									
	class Class_LBS
	{
	public:
		volatile struct	structLBSStatus		m_st_LBSStatus;				//LBS状态
		
		INT32		m_i32TLBS_0;
		UINT32		m_u32TLBSClk_0;								//LBS捕获值
	
		
	private:
		UINT32		m_u32TLBSClkOld_0;							//LBS捕获值的旧值
		INT16		m_i16T1pCntLBSDct_0;								//LBS_SYNC_F脉冲检测值(周期1500)
		INT16		m_i16T1pCntLBSDct0_0;								//LBS_SYNC_F脉冲检测累加计数器
		                                        						
		INT16		m_i16T1pCntLBS_0;									//LBS_SYNC_F脉冲检测定时计数器	
		INT16		m_i16TLBSCnt_0;
		INT16		m_i16LBSTimeDelayCnt_0;
		INT16		m_i16LBSAbnormalCnt_0;
			
	public:
		void 		Dat_Int_LBSCapDeal(void);
		void		Alg_Int_LBSDetect(void);
		void		Alg_LBSSignal(void);
		
		
	};

#endif
	
//===========================================================================
// End of file.
//===========================================================================
