/*==========================================================*/ 	
/* Title		:	DCBus.h														*/ 	
/* Description	: 	Class_DCBus definition											*/ 	
/* Date			:	Apr.2008													*/ 	
/*==========================================================*/
 #ifndef		DCBUS_H 
             
 #define     	DCBUS_H
 
	class Class_DCBus
	{
	public:
		INT16		m_i16VdcOverCnt_0;
		INT16		m_i16Vdc_0;									//直流母线电压 
	    INT16		m_i16VbusP_0;								//直流正母线电压
	    INT16		m_i16VbusN_0;								//直流负母线电压
	                                        					
	private:   
		INT16		m_i16KVbusPAdjust_0;						//正母线电压校正系数
		INT16		m_i16KVbusNAdjust_0;						//负母线电压校正系数		
		                             					
		INT16		m_i16VdcSmp_0;								//直流母线电压采样值 	
		INT16		m_i16KVdcbus_0;								//母线电压定标系数
		UNLONG		m_unVdcSmp_0;								//母线电压(含滤波尾数)
		                                    					
		INT16		m_i16VdcLowCntPer40ms;						//母线低于330V延时	
		INT16		m_i16DCOVPWMShutCnt_0;
		INT16		m_i16DcOverDelayCnt_0;
		
	public:
		inline void		App_DCBusVarInit(void);
		inline void		App_DCBusParamConfig(void);
		        	
		void		Dat_Int_DCBusPreDeal(void);
		void		Dat_DCBusRMS(void);
		        	
		void		Alg_DCBusFault(void); 
		void 		Alg_Int_DCOVProtect(void);
		friend		class Class_Service;
		
	};	
#endif

//===========================================================================
// End of file.
//===========================================================================
