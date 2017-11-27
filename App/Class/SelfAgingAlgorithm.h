/*==========================================================*/ 	
/* Title		:	SelfAgingAlgorithm.h											*/ 	
/* Description	: 	Class_SelfAgingAlgorithm definition								*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/	
#ifndef 	SELFAGINGALGORITHM_H
        
#define     SELFAGINGALGORITHM_H

	class Class_SelfAgingAlgorithm
	{
	private:
		INT16		m_i16AgingIindMdTgt;				//自老化功能相关变量--电感电流目标模值
 		INT16		m_i16AgingRefID_0;
 		INT16		m_i16AgingRefIQ_0;
 		INT16		m_i16AgingBpModeRef_0;		//Ref	
		INT16		m_i16AgingVolRefCnt_0;	
		INT16		m_i16AgingCurRefCnt_0;	
 		            
		INT16		m_i16AgingCos0_0;			
		INT16		m_i16AgingSin0_0;
	
		UNLONG		m_unAgingSumPLL_0;
		INT16		m_i16AgingPLLCnt_0;		
		UNLONG		m_unAgingIoutTheta_0;	
		
		INT16		m_i16AgingID_0;			
		INT16		m_i16AgingIQ_0;	
		INT16		m_i16AgingIZ_0;				//TRANS	
	
		UNLONG		m_unAgingSumID_0;			//Reg	
		UNLONG		m_unAgingSumIQ_0;	
		UNLONG		m_unAgingSumIZ_0;	

		INT16		m_i16AgingKpCur_0;			
		INT16		m_i16AgingKiCur_0;			
                    
		INT16		m_i16AgingPwmD_0;			
		INT16		m_i16AgingPwmQ_0;			
		INT16		m_i16AgingPwmZ_0;
		
		INT16		m_i16wUirD_0;
		INT16		m_i16wUirQ_0;
		INT16		m_i16wUirZ_0;
		
		INT16		m_i161DivVbusP_0;
		INT16		m_i161DivVbusN_0;
		
		INT16		m_i16VaAct_0;
		INT16		m_i16VbAct_0;
		INT16		m_i16VcAct_0;	
		
		INT16		m_i16Iaref_0;
		INT16		m_i16Ibref_0;
		INT16		m_i16Icref_0;
		
		UNLONG		unIarefFilter_0;
		UNLONG		unIbrefFilter_0;		
		UNLONG		unIcrefFilter_0;		
		
		INT16		m_i16PWMaDeadCmp_0;
		INT16		m_i16PWMbDeadCmp_0;
		INT16		m_i16PWMcDeadCmp_0;
		
		INT16		m_i16KSPWM;
		INT16		m_i16PulseOffCount;
	                
		INT16		m_i16AgingSinDphase_0;	//SPWM	
		INT16		m_i16AgingCosDphase_0;
		
	public:
		void		App_SelfAgingAlgVarInit(void);	
		void 		App_SelfAgingAlgParamConfig(void);
		        	
		void 		Alg_Int_SelfAging(void);
		        	
	private:    	
		void 		Alg_Int_AgingPhaseLock(void);
		void 		Alg_Int_AgingRefGen(void);
		void 		Alg_Int_AgingPreDeal(void);
		void 		Alg_Int_AgingCurrReg(void);
		void 		Alg_Int_AgingSPWM(void);
		void 		Alg_Int_AgingOffReset(void);

	};

#endif
//===========================================================================
// End of file.
//===========================================================================
