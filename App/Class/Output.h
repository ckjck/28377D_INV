/*==========================================================*/ 	
/* Title		:	Output.h														*/ 	
/* Description	: 	Class_Output definition											*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/ 
#ifndef       OUTPUT_H
       
#define       OUTPUT_H

struct OutFlag
{
	UINT16	bShortCircuit				:1;		//�����·��־
	UINT16	bDthetaOutFast				:1;				//������쳬����
	UINT16	bVoutMFast					:1;				//ģֵ��쳬����
	UINT16	bVoutFast					:1;				//��·��ѹ������
	UINT16	bRev0415					:12;	//Bit0415 ����
};

class Class_Output
	{
	public:
		struct OutFlag m_st_OutFlag;
		INT16			m_i16SinQOut_0;				//���ʸ��������ֵ
		INT16			m_i16CosQOut_0;				//���ʸ��������ֵ
		UNLONG			m_unVoutMMean_0;			//�����ѹģ�˲�ֵ
		UNLONG			m_unDthetaOutBase_0;		//���ʸ���ǻ�������		
		
		INT16			m_i16Vouta_0;				//A�������ѹ   
		INT16			m_i16Voutb_0;				//B�������ѹ   
		INT16			m_i16Voutc_0;				//C�������ѹ
		
		INT16			m_i16VoutaPrms;				//���A���ѹ��ʱ��Чֵ	
		INT16			m_i16VoutbPrms;				//���B���ѹ��ʱ��Чֵ
		INT16			m_i16VoutcPrms;				//���C���ѹ��ʱ��Чֵ
		                
		UNLONG			m_unVoutaRms;				//���A���ѹ��Чֵ(���˲�β��)	
		UNLONG			m_unVoutbRms;				//���B���ѹ��Чֵ(���˲�β��)		
		UNLONG			m_unVoutcRms;				//���C���ѹ��Чֵ(���˲�β��)	

		//�޸Ŀ�ʼ��Adpater20K�������������������ر���
		INT16			m_i16Iouta_0;				//A��������� 
		INT16			m_i16Ioutb_0;				//B���������  
		INT16			m_i16Ioutc_0;				//C���������
		
		INT16			m_i16IoutaPrms;				//���A�������ʱ��Чֵ	
		INT16			m_i16IoutbPrms;				//���B�������ʱ��Чֵ
		INT16			m_i16IoutcPrms;				//���C�������ʱ��Чֵ
		               
		INT16			m_i16IoutaforShow;			//����ʹ��ʱ���͵���ʱ���ߺܶ��֧�����㣬��ר���ñ������������������ʾ
		INT16			m_i16IoutbforShow;
		INT16			m_i16IoutcforShow; 

		UINT16			m_u16IoutaCrest_0;     	//A�����������ֵ��
		UINT16			m_u16IoutbCrest_0;     		//B�����������ֵ��
		UINT16			m_u16IoutcCrest_0;     		//C�����������ֵ��
		//�޸Ľ�����           
		
		INT16			m_i16Souta;					//���A������,����Ӧ��Чֵ��ȡ
		INT16			m_i16Soutb;					//���B������,����Ӧ��Чֵ��ȡ
		INT16			m_i16Soutc;					//���C������,����Ӧ��Чֵ��ȡ
                	   
		INT16			m_i16Pouta;					//���A�๦��,���ۼ�ƽ����ȡ
		INT16			m_i16Poutb;					//���B�๦��,���ۼ�ƽ����ȡ
		INT16			m_i16Poutc;					//���C�๦��,���ۼ�ƽ����ȡ
		              
		INT16			m_i16PFouta;
		INT16			m_i16PFoutb;
		INT16			m_i16PFoutc;
		                
		INT16			m_i16LoadRatea;
		INT16			m_i16LoadRateb;
		INT16			m_i16LoadRatec;
		
		
		INT16			m_i16wPFoutLeada;			//A �೬ǰ�صĹ��ذٷֱ�; 1024=����
		INT16			m_i16wPFoutLeadb;			//B �೬ǰ�صĹ��ذٷֱ�; 1024=����
		INT16			m_i16wPFoutLeadc;			//c �೬ǰ�صĹ��ذٷֱ�; 1024=����
		                
		INT16			m_i16FoutMean;				//���ƽ��Ƶ��
		                
		INT16			m_i16wVoutFastRef1;			//����m_i16VbpDnlimit�仯
		INT16			m_i16wVoutFastRef2;			//-40%
		INT16			m_i16wVoutFastRef3;			//70%
		INT16			m_i16wVoutFastRef4;			//120%	
		
	                				
		
		INT16                    m_i16VoutFastchkCnt_0;	       //�����������
	private:
		UNLONG			m_unVoutabRms;				//���AB�ߵ�ѹ��Чֵ(���˲�β��)
		UNLONG			m_unVoutbcRms;				//���BC�ߵ�ѹ��Чֵ(���˲�β��)
		UNLONG			m_unVoutcaRms;				//���CA�ߵ�ѹ��Чֵ(���˲�β��)
		
		INT16			m_i16Voutab_0;				//AB����ߵ�ѹ
		INT16			m_i16Voutbc_0;				//BC����ߵ�ѹ
		INT16			m_i16Voutca_0;				//CA����ߵ�ѹ
				
		INT16			m_i16KVoutNormal;
		INT16			m_i16KIoutNormal;
		               
		INT16			m_i16KVouta_0;				//���AB�ߵ�ѹ����ϵ��  
		INT16			m_i16KVoutb_0;				//���BC�ߵ�ѹ����ϵ��  
		INT16			m_i16KVoutc_0;				//���CA�ߵ�ѹ����ϵ�� 
				        	                  	
		INT16			m_i16KIouta_0;				//���A���������ϵ��  
		INT16			m_i16KIoutb_0;				//���B���������ϵ��  
		INT16			m_i16KIoutc_0;				//���C���������ϵ�� 

		UNLONG			m_unIoutaRms;				//���A�������Чֵ(���˲�β��)	
		UNLONG			m_unIoutbRms;				//���B�������Чֵ(���˲�β��)		
		UNLONG			m_unIoutcRms;				//���C�������Чֵ(���˲�β��)	
	
		INT16			m_i16CFouta;
		INT16			m_i16CFoutb;
		INT16			m_i16CFoutc;

		UNLONG			m_unIoutaSum_0;				//���A�����ƽ���ۼӺ�,�ж��ۼ���***
		UNLONG			m_unIoutaSumM_0;			//���A�����ƽ�����ں�,����������Чֵ��
		UNLONG			m_unIoutbSum_0;				//���B�����ƽ���ۼӺ�,�ж��ۼ���
		UNLONG			m_unIoutbSumM_0;			//���B�����ƽ�����ں�,����������Чֵ��
		UNLONG			m_unIoutcSum_0;				//���C�����ƽ���ۼӺ�,�ж��ۼ���
		UNLONG			m_unIoutcSumM_0;			//���C�����ƽ�����ں�,����������Чֵ��
		
		INT16			m_i16IoutaCrestAck_0;  		//A�����������ֵ�ȷ�ֵ��ȷ�ϴ���
		INT16			m_i16IoutbCrestAck_0;  		//B�����������ֵ�ȷ�ֵ��ȷ�ϴ���
		INT16			m_i16IoutcCrestAck_0;  		//C�����������ֵ�ȷ�ֵ��ȷ�ϴ���	
		
		
		INT16			m_i16PSouta;				
		INT16			m_i16PSoutb;					
		INT16			m_i16PSoutc;	
	
		INT16			m_i16VoutabPrms;			//���AB�ߵ�ѹ��ʱ��Чֵ				
		INT16			m_i16VoutbcPrms;			//���BC�ߵ�ѹ��ʱ��Чֵ
		INT16			m_i16VoutcaPrms;			//���CA�ߵ�ѹ��ʱ��Чֵ
			


		           
		UNLONG			m_unVoutaFilter_0;			                              
		UNLONG			m_unVoutbFilter_0;			                              
		UNLONG			m_unVoutcFilter_0;
	 
		INT16			m_i16VoutM_0;				//�����ѹģֵ
		 	                    	
		INT16			m_i16DthetaOutInst_0;		//���ʸ����˲ʱ����	
		  	                    	
		UNLONG			m_unVoutaSum_0;				//���A���ѹƽ���ۼӺ�,�ж��ۼ���***
		UNLONG			m_unVoutaSumM_0;			//���A���ѹƽ�����ں�,����������Чֵ��
		UNLONG			m_unVoutbSum_0;				//���B���ѹƽ���ۼӺ�,�ж��ۼ���
		UNLONG			m_unVoutbSumM_0;			//���B���ѹƽ�����ں�,����������Чֵ��
		UNLONG			m_unVoutcSum_0;				//���C���ѹƽ���ۼӺ�,�ж��ۼ���
		UNLONG			m_unVoutcSumM_0;			//���C���ѹƽ�����ں�,����������Чֵ��
         		
		UNLONG			m_unVoutabSum_0;			//���AB�ߵ�ѹƽ���ۼӺ�,�ж��ۼ���***
		UNLONG			m_unVoutabSumM_0;			//���AB�ߵ�ѹƽ�����ں�,����������Чֵ��
		UNLONG			m_unVoutbcSum_0;			//���BC�ߵ�ѹƽ���ۼӺ�,�ж��ۼ���
		UNLONG			m_unVoutbcSumM_0;			//���BC�ߵ�ѹƽ�����ں�,����������Чֵ��
		UNLONG			m_unVoutcaSum_0;			//���CA�ߵ�ѹƽ���ۼӺ�,�ж��ۼ���
		UNLONG			m_unVoutcaSumM_0;			//���CA�ߵ�ѹƽ�����ں�,����������Чֵ��
	            	                    	
		        	                   	
		INT16			m_i16PointcntOut_0;			//�����ѹ�ۼӵ���,�ж��ۼ���
		INT16			m_i16PointcntOutM_0;		//�����ѹ���ڵ���,����������Чֵ�� 
		        	                    	
		UNLONG			m_unDthetaOutMean_0;		//���ʸ����ƽ������
		
		INT16			m_i16SinQoutFilter_0;
		INT16			m_i16CosQoutFilter_0;
		INT16			m_i16SinQoutOld_0;			//���ʸ��������ֵ
		INT16			m_i16CosQoutOld_0;			//���ʸ��������ֵ
		        	                    	
		UNLONG			m_unPoutaAcc_0;				//���A�๦���ۼӺ�,�ж��ۼ���
		UNLONG			m_unPoutaAccM_0;			//���A�๦�����ں�,������������
                          	
		UNLONG			m_unPoutbAcc_0;				//���B�๦���ۼӺ�,�ж��ۼ���
		UNLONG			m_unPoutbAccM_0;			//���B�๦�����ں�,������������
                         	
		UNLONG			m_unPoutcAcc_0;				//���C�๦���ۼӺ�,�ж��ۼ���
		UNLONG			m_unPoutcAccM_0;			//���C�๦�����ں�,������������
		
		INT16			m_i16VoutaFastchkCnt_0;		//���A���ѹ���ټ�������
		INT16			m_i16VoutbFastchkCnt_0;		//���B���ѹ���ټ�������
		INT16			m_i16VoutcFastchkCnt_0;		//���C���ѹ���ټ�������
		INT16			m_i16SpanVbpFastAct_0;		//��·��ѹ���ټ������������
		INT16			m_i16ClkSVbpFastchk_0;		//��·���ټ������ʱ�ӱ���
		INT16			m_i16VoutaFtFastchkCnt_0;
		INT16			m_i16VoutbFtFastchkCnt_0;
		INT16			m_i16VoutcFtFastchkCnt_0;
		        	    
		INT16			m_i16VoutaFastAbnormalCnt_0;	//zqf 060714--���ӡ������ѹ�쳣����������
		INT16			m_i16VoutbFastAbnormalCnt_0;
		INT16			m_i16VoutcFastAbnormalCnt_0;
		
		INT16			m_i16VoutFastAbnormalStartCnt_0;// ����쳣��⿪ʼ��ʱ������
		INT16           m_i16VinvFastchkStartCnt;		//�����ѹ��쿪ʼ��ʱ������

		//Adapter20 ��������������
		INT16		m_i16VoutaFilter0_0;						//�����ѹ�˲����ֵ
		INT16		m_i16VoutaFilter1_0;						//�����ѹ�˲����ֵ
		INT16		m_i16VoutaFilter2_0;						//�����ѹ�˲����ֵ
		INT16		m_i16VoutaFilter3_0;						//�����ѹ�˲����ֵ
		INT16		m_i16VoutaFilter4_0;						//�����ѹ�˲����ֵ
		INT16		m_i16VoutaFilter5_0;	
		INT16			m_i16ToutCnt_0;
		INT16			m_i16VoutaOld_0;
		INT16			m_i16VoutCrossCnt;
		UINT32			m_u32Tout;
		UINT32			m_u32ToutClk_0;
		UINT32			m_u32ToutClkOld_0;
		//

		//��·
		INT16		m_i16ShortPointCnt;
		INT16		m_i16IoutaShortCnt;
		INT16		m_i16IoutbShortCnt;
		INT16		m_i16IoutcShortCnt;
		INT16		m_i16IaShortFlag;
		INT16		m_i16IbShortFlag;
		INT16		m_i16IcShortFlag;
		//ECO���
		INT16		m_i16VoutMFastChkCnt_0;
		INT16		m_i16DthetaoutFastChkCnt_0;
	public:
		void			App_OutVarInit(void);
		void			App_OutParamConfig(void);
		            	
		void			Dat_Int_OutPreDeal(void);		
		void 			Dat_Int_OutCal(void);
		void			Dat_VoutRMS(void);
		void			Dat_IoutRMS(void);
		void			Dat_PoutRMS(void);
		            	
		void			Alg_Int_VoutFastChk(void);
		void			Alg_Int_VoutAbnormal(void);
		void			Alg_VoutAbnormal(void);
		void			Alg_Int_VoutFastChk2(void);
		//void			Alg_Int_ShortCircuit(void);
		void		Alg_Int_VOutRangeFastChk(void);

		friend class Class_Transfer;
	
	};

#endif
//===========================================================================
// End of file.
//===========================================================================
