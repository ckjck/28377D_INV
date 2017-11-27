/*==========================================================*/ 	
/* Title		:	Inverter.h													*/ 	
/* Description	: 	Class_Inverter definition										*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#ifndef    INVERTER_H
       
#define    INVERTER_H


		struct	structFlagVinvABC
		{
			UINT16	bVinvRMS					:1;					//BIT00		��Чֵ��־  0����  1�쳣													
			UINT16	bVoutAbnormal				:1;					//BIT01		�����ѹ�쳣��Чֵ��־:[120%,70%] 0����  1�쳣	
			UINT16	bInvCurrUnbalanced			:1;
			UINT16	bInvCurrUnbalancedWarn		:1;
			UINT16	bRev1504					:12;                //BIT15-02		
		};							
	
		struct	structFlagInv
		{
			UINT16	bInv						:2;					//BIT01--00	 �����״̬ 	00�ر�	01������11�������			
			UINT16	bDspCrazyOff				:1;					//BIT02		���DSP�쳣�ػ�  0�������DSP�쳣�ػ�1�����DSP�쳣�ػ�			
			UINT16	bPLL2PhaseTrace				:1;			        //BIT03  ������������ٱ�־ 0������· 1������Ƶͬ���ź�
			UINT16	bPhaseTrace					:2;					//BIT05--04	��λ���ٱ�־ 	00����	01����· 10����� 11��LBSԴ			
			UINT16	bAmpTrace					:2;					//BIT07--06	���ȸ��ٱ�־ 	00�	01����·(Ԥ��)10�����			
			UINT16	bPhaseLock					:1;					//BIT08		��λ������־ 	bPhaseLock			
			UINT16	bAmpLock					:1;					//BIT09		����������־ 	bAmpLock			
			UINT16  bMaxcapacityInvFlag			:1;					//BIT10		������ѿ�����������������־	0�����	1���			
			UINT16  bMaxcapacityOnFlag			:1;					//BIT11		������ѹ����������������־	0�����	1���			
			UINT16	bInvTraceConf				:1;					//BIT12		Inv nomal trace confirm signal(0: not confirm, 1: confirm)			
			UINT16	bRev1513					:3;
		};


class Class_Inverter
	{
	public:
		volatile struct	structFlagInv		m_st_wFlagInv;		//���״̬��־
		volatile struct	structFlagVinvABC	m_st_FlagVinvABC;	//����ѹ��־

		INT16			m_i16wVrefTgtMd;
		INT16  			m_i16VrefSetMd;				//���ģֵ(����)
		INT16			m_i16Ila_0;					//A���е��� 
		INT16			m_i16Ilb_0;					//B���е��� 
		INT16			m_i16Ilc_0;					//C���е��� 

		INT16			m_i16IlaFilter_0;			//A���е���ֱ������ 
		INT16			m_i16IlbFilter_0;			//B���е���ֱ������ 
		INT16			m_i16IlcFilter_0;	 		//C���е���ֱ������ 		

		INT16			m_i16Iinva_0;				//A��������     
		INT16			m_i16Iinvb_0;				//B��������     
		INT16			m_i16Iinvc_0;				//C��������     
		
		INT16			m_i16Vinva_0;				//A������ѹ
		INT16			m_i16Vinvb_0;				//B������ѹ
		INT16			m_i16Vinvc_0;				//C������ѹ						 

		INT16			m_i16VinvaDc_0;				//A������ѹֱ������������
		INT16			m_i16VinvbDc_0;				//B������ѹֱ������������
		INT16			m_i16VinvcDc_0;

		

		INT16			m_i16VinvMd_0;				//����ѹģֵd�����
		INT16			m_i16VinvMq_0;				//����ѹq�����  
		INT16			m_i16VinvMz_0;              //����ѹz����� 
		
		INT16			m_i16SinQInv_0;				//����������ѹ���
		INT16			m_i16CosQInv_0;				//���ʸ��������ֵ	
		UNLONG			m_unVinvMMean_0;			//����ѹģ�˲�ֵ
		INT16			m_i16FinvMean;				//���ƽ��Ƶ��
		INT16			m_i16PointcntInvM_0;		//����ѹ���ڵ���,����������Чֵ��
		INT16			m_i16PointcntInv_0;			//����ѹ�ۼӵ���,�ж��ۼ���

		UNLONG			m_unVinvaDcFilter_0;	                                 
		UNLONG			m_unVinvbDcFilter_0;                                    
		UNLONG			m_unVinvcDcFilter_0;  
		
		INT16      	    m_i16IinvMz_0;              //������z����� 	
		INT16			m_i16IinvMd_0;				//������d�����
		INT16			m_i16IinvMq_0;				//������q�����  
		
		INT16      	    m_i16IinvMz_1;              //������z�����,���ڵ���ǰ�� 	
		INT16			m_i16IinvMd_1;				//������d�����,���ڵ���ǰ�� 
		INT16			m_i16IinvMq_1;				//������q�����,���ڵ���ǰ��   
		    	               	
		UNLONG			m_unVbusSubFilter_0;		//����ĸ�߲�Z�ỷ���Ŀ���     	               	
		UNLONG			m_unIinvMdFilter_0; 		//����ͨ��CAN�ڲ����䴫��
		UNLONG			m_unIinvMqFilter_0;			//����ͨ��CAN�ڲ����䴫��
		UNLONG			m_unIinvMzFilter_0;			//����ͨ��CAN�ڲ����䴫��
		      	         
		INT16  	        m_i16IlMz_0;       			//��е���z�����
		INT16			m_i16IlMd_0;				//��е���d�����
		INT16			m_i16IlMq_0;				//��е���q�����
		
		INT16			m_i16VinvaPrms;				//���A���ѹ��ʱ��Чֵ	
		INT16			m_i16VinvbPrms;				//���B���ѹ��ʱ��Чֵ
		INT16			m_i16VinvcPrms;				//���C���ѹ��ʱ��Чֵ
		UNLONG			m_unVinvaRms;				//���A���ѹ��Чֵ(���˲�β��)	
		UNLONG			m_unVinvbRms;				//���B���ѹ��Чֵ(���˲�β��)		
		UNLONG			m_unVinvcRms;				//���C���ѹ��Чֵ(���˲�β��)	
		
		INT16			m_i16VinvabPrms;			//���AB�ߵ�ѹ��ʱ��Чֵ		
		INT16			m_i16VinvbcPrms;			//���BC�ߵ�ѹ��ʱ��Чֵ
		INT16			m_i16VinvcaPrms;			//���CA�ߵ�ѹ��ʱ��Чֵ
		UNLONG			m_unVinvabRms;				//���AB�ߵ�ѹ��Чֵ(���˲�β��)
		UNLONG			m_unVinvbcRms;				//���BC�ߵ�ѹ��Чֵ(���˲�β��)		
		UNLONG			m_unVinvcaRms;				//���CA�ߵ�ѹ��Чֵ(���˲�β��)		

		
		INT16			m_i16IindaPrms;				//�����A�������ʱ��Чֵ	
		INT16			m_i16IindbPrms;				//�����B�������ʱ��Чֵ
		INT16			m_i16IindcPrms;				//�����C�������ʱ��Чֵ
		UNLONG			m_unIindaRms;				//�����A�������Чֵ(���˲�β��)	
		UNLONG			m_unIindbRms;				//�����B�������Чֵ(���˲�β��)		
		UNLONG			m_unIindcRms;				//�����C�������Чֵ(���˲�β��)	
		
		INT16			m_i16IinvaPrms;				//���A�������ʱ��Чֵ	
		INT16			m_i16IinvbPrms;				//���B�������ʱ��Чֵ
		INT16			m_i16IinvcPrms;				//���C�������ʱ��Чֵ
		UNLONG			m_unIinvaRms;				//���A�������Чֵ(���˲�β��)	
		UNLONG			m_unIinvbRms;				//���B�������Чֵ(���˲�β��)		
		UNLONG			m_unIinvcRms;				//���C�������Чֵ(���˲�β��)
		
		INT16			m_i16Pinva;					//���A�๦��,���ۼ�ƽ����ȡ
		INT16			m_i16Pinvb;					//���B�๦��,���ۼ�ƽ����ȡ
		INT16			m_i16Pinvc;					//���C�๦��,���ۼ�ƽ����ȡ 

		INT16			m_i16PinvaforShow;
		INT16			m_i16PinvbforShow;
		INT16			m_i16PinvcforShow;
		
		INT16			m_i16Sinva;					//���A������,����Ӧ��Чֵ��ȡ
		INT16			m_i16Sinvb;					//���B������,����Ӧ��Чֵ��ȡ
		INT16			m_i16Sinvc;					//���C������,����Ӧ��Чֵ��ȡ

		INT16			m_i16SinvaforShow;
		INT16			m_i16SinvbforShow;
		INT16			m_i16SinvcforShow;
		               
		INT16			m_i16SLinva;				//�����A������,����Ӧ��Чֵ��ȡ
		INT16			m_i16SLinvb;				//�����B������,����Ӧ��Чֵ��ȡ
		INT16			m_i16SLinvc;				//�����C������,����Ӧ��Чֵ��ȡ
		               
		INT16			m_i16IinvaforShow;
		INT16			m_i16IinvbforShow;
		INT16			m_i16IinvcforShow;


		INT16			m_u16IinvaCrest_0;				//��������ֵ��
		INT16			m_u16IinvbCrest_0;				//��������ֵ��
		INT16			m_u16IinvcCrest_0;				//��������ֵ��

		INT16			m_i16IinvaCrestAck_0;			//��������ֵ��
		INT16			m_i16IinvbCrestAck_0;			//��������ֵ��
		INT16			m_i16IinvcCrestAck_0;			//��������ֵ��


		INT16			m_i16IlaforShow;
		INT16			m_i16IlbforShow;
		INT16			m_i16IlcforShow;

		INT16			m_i16wEnvirTemp;                //�����¶�
		INT16			m_i16TempKWGrant;				//�¶���Ȩ�ʣ������¶ȶԹ��ʽ��е���,�������������������
		INT16			m_i16TempKVAGrant;				//�¶���Ȩ�ʣ������¶ȶԹ��ʽ��е���		
		
		
		INT16			m_i16wComIndSatConfFlag;
		/*			0--������
					1--���ͣ�ȷ��9.5s
					2--���ͣ�ȷ��30s
		*/
	
		//zqf 060413--����������ģ���俪��ʱ������䲻ͬ����Ϣ�ϱ�
		INT16			m_i16wInvWarn1LockConfCntPer40ms;
		INT16                    m_i16_InvaOverCurrentChkCnt;		//��������ȷ�ϼ�����
		INT16                    m_i16_InvbOverCurrentChkCnt;
		INT16                    m_i16_InvcOverCurrentChkCnt;
		INT16                    m_i16_InvOverCurrentChkCnt;
		INT16			m_i16_InvOverCurrentChkCnt1;		//��������ȷ�ϼ��������������Ӵ�����·����

		//��������
		INT16		m_i16PFinva;
		INT16		m_i16PFinvb;
		INT16		m_i16PFinvc;
		INT16		m_i16LoadRatea;
		INT16		m_i16LoadRateb;
		INT16		m_i16LoadRatec;

		//�����������
		INT16		m_i16SystemPF;
	private:
		INT16			m_i16VoutAbnormalCntPer14ms;//�����ѹ�쳣������
		INT16			m_i16Vinvab_0;				//AB����ߵ�ѹ 
		INT16			m_i16Vinvbc_0;				//BC����ߵ�ѹ 
		INT16			m_i16Vinvca_0;				//CA����ߵ�ѹ 

		UNLONG			m_unIlaFilter_0;			//A���е���ֱ�������˲�ֵ
		UNLONG			m_unIlbFilter_0;			//B���е���ֱ�������˲�ֵ
		UNLONG			m_unIlcFilter_0;			//C���е���ֱ�������˲�ֵ
		 
//		INT16			m_i16VinvaDc_0;				//A������ѹֱ������	
//		INT16			m_i16VinvbDc_0;				//B������ѹֱ������	
//		INT16			m_i16VinvcDc_0;				//C������ѹֱ������ 
		                                                              
		INT16			m_i16Iinva2_0;				//A��������+��е���ֱ������
		INT16			m_i16Iinvb2_0;				//B��������+��е���ֱ������
		INT16			m_i16Iinvc2_0;				//C��������+��е���ֱ������

		INT16			m_i16IinvMd2_0;			//������+��е���ֱ���������d�����
		INT16			m_i16IinvMq2_0;			//������+��е���ֱ���������q�����		
		INT16			m_i16IinvMz2_0;			//������+��е���ֱ���������z�����
		
		INT16			m_i16KVinvNormal;
		INT16			m_i16KIinvNormal;
		
		INT16			m_i16KVinva_0;				//���AB�ߵ�ѹ����ϵ��  
		INT16			m_i16KVinvb_0;				//���BC�ߵ�ѹ����ϵ��  
		INT16			m_i16KVinvc_0;				//���CA�ߵ�ѹ����ϵ��  
		INT16			m_i16KIl_0;					//��е�������ϵ��      
		INT16			m_i16KIlFilter_0;			//�˲����е�������ϵ
		INT16			m_i16KIinva_0;				//A������������ϵ��  
		INT16			m_i16KIinvb_0;				//B������������ϵ��  
		INT16			m_i16KIinvc_0;				//C������������ϵ��  
		INT16			m_i16KVinvdc_0;				//�����ѹֱ����������ϵ��	

		INT16			m_i16VinvM_0;				//����ѹģֵ
		  	                    	
		UNLONG			m_unDthetaRefBase_0;		//����ʸ���ǻ�������
		
                
		UNLONG			m_unVinvaSum_0;				//���A���ѹƽ���ۼӺ�,�ж��ۼ���
		UNLONG			m_unVinvaSumM_0;			//���A���ѹƽ�����ں�,����������Чֵ��
		UNLONG			m_unVinvbSum_0;				//���B���ѹƽ���ۼӺ�,�ж��ۼ���
		UNLONG			m_unVinvbSumM_0;			//���B���ѹƽ�����ں�,����������Чֵ��
		UNLONG			m_unVinvcSum_0;				//���C���ѹƽ���ۼӺ�,�ж��ۼ���
		UNLONG			m_unVinvcSumM_0;			//���C���ѹƽ�����ں�,����������Чֵ��
	            		                    	
		UNLONG			m_unVinvabSum_0;			//���AB�ߵ�ѹƽ���ۼӺ�,�ж��ۼ���
		UNLONG			m_unVinvabSumM_0;			//���AB�ߵ�ѹƽ�����ں�,����������Чֵ��
		UNLONG			m_unVinvbcSum_0;			//���BC�ߵ�ѹƽ���ۼӺ�,�ж��ۼ���
		UNLONG			m_unVinvbcSumM_0;			//���BC�ߵ�ѹƽ�����ں�,����������Чֵ��
		UNLONG			m_unVinvcaSum_0;			//���CA�ߵ�ѹƽ���ۼӺ�,�ж��ۼ���
		UNLONG			m_unVinvcaSumM_0;			//���CA�ߵ�ѹƽ�����ں�,����������Чֵ��
        
		UNLONG			m_unIinvaSum_0;				//���A�����ƽ���ۼӺ�,�ж��ۼ���
		UNLONG			m_unIinvaSumM_0;			//���A�����ƽ�����ں�,����������Чֵ��
		UNLONG			m_unIinvbSum_0;				//���B�����ƽ���ۼӺ�,�ж��ۼ���
		UNLONG			m_unIinvbSumM_0;			//���B�����ƽ�����ں�,����������Чֵ��
		UNLONG			m_unIinvcSum_0;				//���C�����ƽ���ۼӺ�,�ж��ۼ���
		UNLONG			m_unIinvcSumM_0;			//���C�����ƽ�����ں�,����������Чֵ��
		              	
		UNLONG			m_unIindaSum_0;				//�����A�����ƽ���ۼӺ�,�ж��ۼ���
		UNLONG			m_unIindaSumM_0;			//�����A�����ƽ�����ں�,����������Чֵ��
		UNLONG			m_unIindbSum_0;				//�����B�����ƽ���ۼӺ�,�ж��ۼ���
		UNLONG			m_unIindbSumM_0;			//�����B�����ƽ�����ں�,����������Чֵ��
		UNLONG			m_unIindcSum_0;				//�����C�����ƽ���ۼӺ�,�ж��ۼ���
		UNLONG			m_unIindcSumM_0;			//�����C�����ƽ�����ں�,����������Чֵ��
		
		UNLONG			m_unDthetaRefMean_0;		//����ʸ����ƽ������
		               	
		UNLONG			m_unPinvaAcc_0;				//���A�๦���ۼӺ�,�ж��ۼ���
		UNLONG			m_unPinvaAccM_0;			//���A�๦�����ں�,������������
                      	
		UNLONG			m_unPinvbAcc_0;				//���B�๦���ۼӺ�,�ж��ۼ���
		UNLONG			m_unPinvbAccM_0;			//���B�๦�����ں�,������������
                       	
		UNLONG			m_unPinvcAcc_0;				//���C�๦���ۼӺ�,�ж��ۼ���
		UNLONG			m_unPinvcAccM_0;			//���C�๦�����ں�,������������
		        	          
		INT16			m_i16VinvFault1CntPer14ms;	//����ѹ���ϼ�����
		INT16			m_i16VinvFault2CntPer14ms;

		INT16			m_i16TempGrant1Cnt;			//�¶���Ȩ��ȷ����ʱ
		INT16			m_i16TempGrant9Cnt;
		INT16			m_i16TempGrant8Cnt;
		//�������ж���·SCR�������KM����һ�࿪·
		INT16		m_i16IinvUnbalancedCntPer40ms;	//�����������ű۲�������־

		INT16		m_i16BpSTSShortPWMShutCnt_0;

		INT32		m_i32IinvaPSum_0;
		INT32		m_i32IinvaPSumM_0;
		INT32		m_i32IinvaQSum_0;
		INT32		m_i32IinvaQSumM_0;
		
		INT32		m_i32IinvbPSum_0;
		INT32		m_i32IinvbPSumM_0;
		INT32		m_i32IinvbQSum_0;
		INT32		m_i32IinvbQSumM_0;
		
		INT32		m_i32IinvcPSum_0;
		INT32		m_i32IinvcPSumM_0;
		INT32		m_i32IinvcQSum_0;
		INT32		m_i32IinvcQSumM_0;


		INT16		m_i16IinvaP_0;
		INT16		m_i16IinvaQ_0;

		
		INT16		m_i16IinvbP_0;
		INT16		m_i16IinvbQ_0;

		
		INT16		m_i16IinvcP_0;
		INT16		m_i16IinvcQ_0;

		INT16		m_i16IlaFilter2_0;
		INT16		m_i16IlbFilter2_0;
		INT16		m_i16IlcFilter2_0;

		INT16		m_i16VbusDiff_0;

//		INT16		m_i16VaNoDc_0;
//		INT16		m_i16VbNoDc_0;
//		INT16		m_i16VcNoDc_0;

		INT16		m_i16PointcntInvMDtheta;
		
	public:
		void			App_InvVarInit(void);
		void			App_InvParamConfig();
		        		
		void			Dat_Int_InvPreDeal(void);
		void 			Dat_Int_InvCal(void);
		void			Dat_VinvRMS(void);
		void			Dat_IinvRMS(void);
		void			Dat_TempPowerGrant(void);
		void			Dat_PinvRMS(void);
		            	
		void			Dat_VinvFlag(void);
		        		
		void			Alg_InvFault(void);

		void			Alg_Int_InvOverCurrentChk(void);
		void			Alg_IinvUnbalanced(void);
		void			Dat_Int_IinvDFTCal(void);
		void			Dat_InvPQRMS(void);
		void			Dat_IinvPQRMS(void);

		friend	class	Class_Debug;	
		friend	class	Class_PowCANScheduler;
		friend	class	Class_InverterAlgorithm;
	};
	
#endif		
	


//===========================================================================
// End of file.
//===========================================================================
