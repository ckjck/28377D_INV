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
		INT16		m_i16Vdc_0;									//ֱ��ĸ�ߵ�ѹ 
	    INT16		m_i16VbusP_0;								//ֱ����ĸ�ߵ�ѹ
	    INT16		m_i16VbusN_0;								//ֱ����ĸ�ߵ�ѹ
	                                        					
	private:   
		INT16		m_i16KVbusPAdjust_0;						//��ĸ�ߵ�ѹУ��ϵ��
		INT16		m_i16KVbusNAdjust_0;						//��ĸ�ߵ�ѹУ��ϵ��		
		                             					
		INT16		m_i16VdcSmp_0;								//ֱ��ĸ�ߵ�ѹ����ֵ 	
		INT16		m_i16KVdcbus_0;								//ĸ�ߵ�ѹ����ϵ��
		UNLONG		m_unVdcSmp_0;								//ĸ�ߵ�ѹ(���˲�β��)
		                                    					
		INT16		m_i16VdcLowCntPer40ms;						//ĸ�ߵ���330V��ʱ	
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
