/*==========================================================*/ 	
/* Title		:	Load.h														*/ 	
/* Description	: 	Class_Load definition											*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#ifndef		LOAD_H
       
#define     LOAD_H


	struct	structFlagInvLoadAB	
	{
		UINT16	bLoadA					:5;					//BIT04-00	����A����为�ر�־	bLoadA
																		//00000 95%����
																		//00001 95%-105%
																		//00011 105%-110%
																		//00111 110%-125%
																		//01111 125%-150%
																		//11111 150%����		
		UINT16	bRev0705				:3;					//BIT07-05
		UINT16	bLoadB					:5;					//BIT12-08	����B����为�ر�־ bLoadB
																		//00000 95%����
																		//00001 95%-105%
																		//00011 105%-110%
																		//00111 110%-125%
																		//01111 125%-150%
																		//11111 150%����    	
    	UINT16	bRev1513				:3;					//BIT15-13
    	
	};			

	
	struct	structFlagInvLoadCT
	{
		UINT16	bLoadC					:5;					//BIT04-00	����C����为�ر�־	bLoadC
																		//00000 95%����
																		//00001 95%-105%
																		//00011 105%-110%
																		//00111 110%-125%
																		//01111 125%-150%
																		//11111 150%����		
		UINT16	bLoadT					:5;					//BIT09-05	������为���ܱ�־	bLoadT
																		//00000 95%����
																		//00001 95%-105%
																		//00011 105%-110%
																		//00111 110%-125%
																		//01111 125%-150%
																		//11111 150%����		
		UINT16	bLoadTotalT				:5;					//BIT14-10	������为���ܱ�־	bLoadT
																		//00000 95%����
																		//00001 95%-105%
																		//00011 105%-110%
																		//00111 110%-125%
																		//01111 125%-150%
																		//11111 150%����
		UINT16	bRev15					:1;
		
	};			

	struct	structFlagILLoadAB	
	{
		UINT16	bLoadA					:5;					//BIT04-00	����A������и��ر�־	bLoadA
																		//00000 95%����
																		//00001 95%-105%
																		//00011 105%-110%
																		//00111 110%-125%
																		//01111 125%-150%
																		//11111 150%����		
		UINT16	bRev0705				:3;					//BIT07-05
		UINT16	bLoadB					:5;					//BIT12-08	����B������и��ر�־ bLoadB
																		//00000 95%����
																		//00001 95%-105%
																		//00011 105%-110%
																		//00111 110%-125%
																		//01111 125%-150%
																		//11111 150%����    	
    	UINT16	bRev1513				:3;					//BIT15-13   	
	};	


	struct	structFlagILLoadCT
	{
		UINT16	bLoadC					:5;					//BIT04-00	����C������и��ر�־	bLoadC
																		//00000 95%����
																		//00001 95%-105%
																		//00011 105%-110%
																		//00111 110%-125%
																		//01111 125%-150%
																		//11111 150%����		
		UINT16	bRev0705				:3;					//BIT07-05
		UINT16	bLoadT					:5;					//BIT12-08	��������и����ܱ�־	bLoadT
																		//00000 95%����
																		//00001 95%-105%
																		//00011 105%-110%
																		//00111 110%-125%
																		//01111 125%-150%
																		//11111 150%����		
		UINT16	bRev1513				:3;					//BIT15-13	
	};			

	struct	structFlagOutLoadABCT
	{
		UINT16	bLoadA					:4;						//BIT03-00	����A��������ر�־ bLoadA
																			//0000 95%����
																			//0001 95%-100%
																			//0011 100%-105%
																			//0111 105%����		
		UINT16	bLoadB					:4;						//BIT07-04	����B��������ر�־ bLoadB
																			//0000 95%����
																			//0001 95%-100%
																			//0011 100%-105%
																			//0111 105%����
		
		UINT16	bLoadC					:4;						//BIT11-08	����C��������ر�־	bLoadC
																			//0000 95%����
																			//0001 95%-100%
																			//0011 100%-105%
																			//0111 105%����
		
		UINT16	bLoadT					:4;						//BIT15-12	������������ܱ�־	bLoadT
																			//0000 95%����
																			//0001 95%-100%
																			//0011 100%-105%
																			//0111 105%����
	 	
	};

	struct	structFlagload
	{
		UINT16	bRev0700				:8;		
		UINT16	bPSoutHigh				:1;						//BIT8		��������أ�������P/SOUTȷ�ϣ�bPSoutHigh						//�൱��ԭPSOUT_FLAG		
		UINT16	bRev1009				:2;		        		//BIT10-09
		UINT16	bReqSwitch				:1;						//BIT11		�ڻ����س������·				bReqSwitch		
		UINT16	bImpactSwitch			:1;						//BIT12		�����������·(��·������)		bImpactSwitch		
		UINT16	bShortCircuit			:1;						//BIT13		�����¿�������·
		UINT16	bOvload					:1;						//BIT14		����ܹ���(������·/����)	bOvload		
		UINT16	bInvOvloadTimeout		:1;						//BIT15		��������ʱ��				 			
	};


	class Class_Load
	{
	public:			
		volatile struct structFlagload			m_st_wFlagLoad;				//������ر�־
		INT16		m_i16PinvaGrant;								//A���й����ʽ��������
		INT16		m_i16PinvbGrant;								//B���й����ʽ��������
		INT16		m_i16PinvcGrant;								//C���й����ʽ��������
		
		INT16		m_i16SinvaGrant;								//A�����ڹ��ʽ��������
		INT16		m_i16SinvbGrant;								//B�����ڹ��ʽ��������
		INT16		m_i16SinvcGrant;								//C�����ڹ��ʽ��������
		
		INT16		m_i16PSinvaGrantMAX;							////A���й��޹����ʽ�����������ֵ
		INT16		m_i16PSinvbGrantMAX;							////B���й��޹����ʽ�����������ֵ
		INT16		m_i16PSinvcGrantMAX;							////C���й��޹����ʽ�����������ֵ
		
		INT16		m_i16PoutaGrant;								//A���й����ʽ��������
		INT16		m_i16PoutbGrant;								//B���й����ʽ��������
		INT16		m_i16PoutcGrant;								//C���й����ʽ��������
		
		INT16		m_i16SoutaGrant;								//A�����ڹ��ʽ��������
		INT16		m_i16SoutbGrant;								//B�����ڹ��ʽ��������
		INT16		m_i16SoutcGrant;								//C�����ڹ��ʽ��������
		
		INT16		m_i16PSoutaGrantMAX;							////A���й��޹����ʽ�����������ֵ
		INT16		m_i16PSoutbGrantMAX;							////B���й��޹����ʽ�����������ֵ
		INT16		m_i16PSoutcGrantMAX;							////C���й��޹����ʽ�����������ֵ
		
	
	private:
		volatile struct	structFlagInvLoadAB		m_st_FlagInvLoadAB;			//������为�ر�־
		volatile struct	structFlagInvLoadCT		m_st_FlagInvLoadCT;			//������为�ر�־
		volatile struct	structFlagILLoadAB		m_st_FlagILLoadAB;		
		volatile struct	structFlagILLoadCT		m_st_FlagILLoadCT;			//��������и��ر�־
		volatile struct	structFlagOutLoadABCT	m_st_FlagOutLoadABCT;		//����������ر�־   		
			
		INT16	 	m_i16OvloadSignalCntPerMloop;							//INV_OVERLOAD�źų���ʱ�� (20����ѭ������)
																			//ȷ������ϵͳ���������յ����ź�
		INT16		m_i16OvloadConfCntPer40ms;								//������ȷ�ϴ���, ����ȷ�Ϲ���
		INT16		m_i16PSoutHighConfCntPer40ms;							//�������ȷ�ϴ���������ȷ�Ϲ���
		INT32		m_i32OvloadDlyCntPer40ms;								//������ʱ������(������ƽ��ÿ40ms�ۼ�/�ݼ�һ��)
	                                            						
		INT16		m_i16Powerbacka;										//��·A��������������ӹ���
		INT16		m_i16Powerbackb;										//��·B��������������ӹ���
		INT16		m_i16Powerbackc;										//��·C��������������ӹ���

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
	
	
//���ر�־����ģ��vLoad_Flag()
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
