/*==========================================================*/
/* Title		:	MonitorInterface.h												*/
/* Description	: 	Class_MonitorInterface definition									*/
/* Date		:	Apr.2008														*/
/*==========================================================*/
#ifndef      MONITORINTERFACE_H

#define      MONITORINTERFACE_H
//Head files
//#include "SysCanApp.h"
//�汾��Ϣ���������³�����ʾ����汾ΪV100B000D000

#define	ProtocolVerVCnst		100
#define	ProtocolVerBCnst		0x1
#define	ProtocolVerDCnst		0x0

#define	DspVerVCnst				100
#define	DspVerBCnst				0
#define	DspVerDCnst				2

#define	CpldVerVCnst			0x0
#define	CpldVerBCnst			0x6
#define	CpldVerDCnst			0x0

#define Voltage80CNST	373				//373=1024*80/2
#define Current5CNST	51				//51=1024*5/100
#define Sout5CNST		51				//51=1024*5/100


struct structMonitorOrder
	{
		UINT16	bOn							:1;			//BIT00		Inverter On  0:�� 1:��
		UINT16	bOff						:1;			//BIT01		Inverter Off 0:�� 1:��
		UINT16	bEPO						:1;			//BIT02		EPO          0:�� 1:��
		UINT16	bRev03						:1;			//BIT03		(BpShutԤ����
		UINT16	bReset						:1;			//BIT04		Fault Reset  0��
		UINT16	bIntSwitchConf			:1;			//BIT05		����л�ȷ�� 0:�� 1:��
		UINT16	bRiskOffConf				:2;			//BIT07-06	���չػ�ȷ�� 0:��1:ȷ��2:ȡ��
		UINT16	bRev1108					:4;			//BIT11-08
		UINT16	bInstantOff					:1;			//BIT12		�����ػ�,���عػ��� 0:�� 1:��
		UINT16	bOutDisable					:1;			//BIT13		�����ֹ����   0:�� 1:��
		UINT16	bOutEnable					:1;			//BIT14		�����������   0:�� 1:��
		UINT16	bFlashUpdate				:1;			//BIT15		FlashUpdate����0:�� 1:��
	};
	struct 	structUPSModeRun
	{
 		UINT16	bConfig						:2;			//BIT01-00	UPS��̬
																	//00 ����
																	//01 �ȱ��ݴӻ�
																	//10 �ȱ�������
																	//11 ����
		UINT16	bMode						:1;			//BIT02		UPSģʽ 	0: ���� 1: ECO 	(����)
		UINT16	bID208						:1;			//BIT03	   	208V��ϵ��ʶ0:400��ϵ 1:208��ϵ
		UINT16	bCabinet					:2;			//BIT05��04 ��·����ʽ  0: ��1: ά��2: �Զ�
		UINT16	bPowerFactor				:1;			//BIT06	  	�����������ѡ�� 0:0.9 1:1
		UINT16	bLBS						:2;			//BIT08��07  LBS���� 0:LBS��ֹ1:LBS�ӻ�2:LBS����
		UINT16	bFeedback					:1;			//BIT9		 BACK_FEEDѡ������ 0:����·CIRCUIT BREAKER 1:����·CIRCUIT BREAKER
		UINT16	bIntSwMode					:1;			//BIT10		 ����л�ԭ�� 0:��С���ʱ��1:��С����ֱ������
		UINT16	bFreqOut					:1;			//BIT11  	 ���Ƶ��  0:50HZ 1:60HZ
		UINT16	bVoltOut					:3;			//BIT14��12  ��ѹ����
														//000 400V  ����
														//001 380V  ����
														//010 415V  ����
														//011 208V  ����
														//100 200V  ����
														//101	480V  ����
		UINT16	bMasterECO					:1;			//BIT15		�ȱ���������ǰģʽ 0: ���� 1: ECO
	};

	struct 	structUPSModeRun2
	{
 		UINT16	bInvRunMode					:2;			//BIT01	���������ģʽ����0:���� 1:���ϻ� 2:����
		UINT16	bBattShare					:1;			//���õ����1:���ã�0:������
		UINT16	bRev1501					:13;		//BIT15��03
	};
//-------------------------------------------------------------------------------

/*****************************************************************************
���ƣ���������ϴ���
���״̬���澯��ģ�������汾��Ϣ
********************************************************************************/
	typedef struct
	{

		_INV_SW1_T 				m_st_InvBreakerStatus;					//������ϴ�״̬��
		_INV_WARN1_T			m_st_InvWarn;							//����ϴ��澯��
		_INV_ANALOG_T			m_st_InvAnalog;							//����ϴ�ģ����
		_MODULE_BASE_INFO_T    	m_st_ModuleVerInfo;						//ģ��汾��Ϣ

	}_INV_SEND_MONITOR_T;

/*****************************************************************************
���ƣ���·�����ϴ���
���״̬���澯��ģ�������汾��Ϣ
********************************************************************************/
	typedef struct
	{

		_BYP_SW1_T 				m_st_BypBreakerStatus;					//��·���ϴ�״̬��
		_BYP_WARN1_T			m_st_BypWarn;							//��·�ϴ��澯��
		_BYP_ANALOG_T			m_st_BypAnalog;							//��·�ϴ�ģ����
		//_MODULE_BASE_INFO_T    		m_st_BypVerInfo;						//ģ��汾��Ϣ

	}_BYP_SEND_MONITOR_T;

//---------------------------------------

//�����ڲ������ݽ�������
	typedef struct
	{
		_PARALLEL_SW1_T  		ParallelState[ParaNumCNST];

		INT16		iNumInv;					//�������ڲ���ϵͳ��俪����̨��
		INT16		iNumOn;						//�������ڲ���ϵͳ��乩����̨��
		INT16		iNumBp;						//�������ڲ���ϵͳ��·����̨��
		INT16		iNumUPS;					//�������ڲ���ϵͳʵ���ϵ���̨��

		INT16		iTotalNumInv;                          //�������ܼ䲢��ϵͳ������俪����̨��
		INT16		iTotalNumOn;                           //�������ܼ䲢��ϵͳ���й��翪����̨��


		//������ͨ�ű���������ص�����
		INT16		iNumBattWarn;				//����ϵͳ��������澯��̨��
		INT16		iNumBattInv;				//����ϵͳ�����乩����̨��
		INT16		iParaWarnFlag;				//����ϵͳ�澯�����ۺϱ�־
											//bit0:	1-����ϵͳ�����ع���
											//bit1:	1-����ϵͳ��һ�����
											//bit2:	1-����ϵͳ�����ظ澯
											//bit3:	1-����ϵͳ��һ��澯

		INT16		iParaRunFlag;				//����ϵͳUPS�ϵ��־��������ӻ��ϴ�0xFF
											//bit0:	Reserved
											//bit1:	1-1#������
											//bit2:	1-2#������
											//bit3:	1-3#������
											//bit4:	1-4#������
											//bit5:	1-5#������
											//bit6:	1-6#������
											//bit7-15:	Reserved

	}_PARALLEL_EXCHG_DATA_T;

/*****************************************************************************
���ƣ����������
���
********************************************************************************/
	typedef struct
	{
		_SYS_DSP_SET_INFO_T			m_st_SysDSPSettings;					//ϵͳ������
		_INV_SET_INFO_T				m_st_InvSetInfo;						//���У������
		_CTRL_INFO_T				m_st_SysCtrCmd;							//������� ����
		_INV_RECV_ANALOG_T			m_st_InvRecvAnalog;
	}_INV_RECEIVE_MONITOR_T;


	//ģ��ID�����־λ
	 typedef struct
	{
		//��⵽��ID�ų���Χ,DSP�߼���,CANʹ��
		UINT16	bUsIdNumExpire 		:1;
		//��⵽���������ظ���ID,CAN��λ,�����߼�Ҫʹ��
		UINT16	bUsIdNumOverlap		:1;
		UINT16 	bReserved				:14;

	}_ID_ERROR_TYPE_T;
//-----------------------------------------------------------------------------


	class Class_MonitorInterface
	{
	public:

		volatile struct structMonitorOrder		m_st_wMonitorOrder;				//�������
		volatile struct structUPSModeRun		m_st_wUPSModeRun;				//UPS����ģʽ
		volatile struct structUPSModeRun2		m_st_wUPSModeRun2;				//UPS����ģʽ

		_INV_SEND_MONITOR_T			m_st_InvSendMonitor; 			//����ϴ������Ϣ
		_BYP_SEND_MONITOR_T			m_st_BypSendMonitor;			//��·�ϴ������Ϣ
		_INV_RECEIVE_MONITOR_T			m_st_wInvReceiveMonitor;		//�����ܼ����Ϣ
//		_MODULE_BASE_INFO_T 			m_stCfgInfoCheck_D;		//����������Ϣ��ѯ

		_REC_DISCRETE_SW1_T 			m_st_DiscreteState[ParaNumCNST];
		//Parallel state data
		_PARALLEL_EXCHG_DATA_T			m_st_wParallelData;		//�����ڲ����䴫������

		//Byp maint state data
		_BYPMAINT_SW1_T				m_st_wBypBreakData;	//��·��������״̬��Ϣ

		//Rack parallel sw msg
		_RACK_RECV_STATE_T				m_st_wRackStateMsg;

		_RT_CLK_T						 m_SyncTime;				//���ͬ����ʱ�䣬DSPά��msֵ

		_ID_ERROR_TYPE_T				m_st_IdErrorFlag; 		//ģ��ID�����־λ

		_MON_REAL_MSG_T				m_st_MonRealMsg;			//���Լ�ص���Ϣ

		INT16 		m_i16BptoinvTimesSet;										//������·������������/��ͨѶ����
		INT16		m_i16IntSwTimeSet;											//�������л�ʱ������
																				//0:��С�л�ʱ�� 1:40ms2:60ms 3:80ms 4:100ms
		INT16 		m_i16FbpTraceSet;											//��·Ƶ�ʸ��ٷ�Χ����	/��ͨѶ����
						                                  						//  0: +-2Hz1: +-1Hz  2: +-3Hz 3:+-0.5Hz
		INT16 		m_i16VbpUplimitSet;											//��·������Χ��������/��ͨѶ����   0:+10%  1:+15%
		INT16 		m_i16VbpDnlimitSet;											//��·������Χ��������/��ͨѶ����
			                                   			            			//   0:-20%  1:-10%   2: -30%  3: -40%
		INT16 		m_i16FbpLimitSet;											//��·Ƶ�ʱ�����Χ����/��ͨѶ����
		INT16		m_i16VoltDispScale;						        					                      //0: +-10%  1: +-20%   2: +-5%  3: +-2.5%

		INT16		m_i16FreqSlewRate;											//�������ʣ�0.1-3Hz/s ===>1-30
		INT16 		m_i16VrefSetRms;											//�����Чֵ(�û�)

//---------------------------------------------------------
		UINT16 		m_u16ParamFresh;											//У���������±�־����λ(0-14)��Ӧ����У������
		INT16		m_i16KsVoutaSet;
		INT16		m_i16KsVoutbSet;
		INT16		m_i16KsVoutcSet;
		INT16		m_i16KsIoutaSet;
		INT16		m_i16KsIoutbSet;
		INT16		m_i16KsIoutcSet;
		INT16		m_i16KsVinvaSet;
		INT16		m_i16KsVinvbSet;
		INT16		m_i16KsVinvcSet;
		INT16		m_i16KsIinvaSet;
		INT16		m_i16KsIinvbSet;
		INT16		m_i16KsIinvcSet;

		INT16		m_i16KsVbpaSet;
		INT16		m_i16KsVbpbSet;
		INT16		m_i16KsVbpcSet;
//----------------------------------------------------------------
		INT16		m_i16KsVdcpSet;
		INT16		m_i16KsVdcnSet;

		INT16		m_i16SelfAgingCurrRate;										//���ϻ�������ر���--����������������
		INT16		m_i16wSerialNumSet_0;										//�������,��ͨѶ����
		INT16		m_i16BasicNum;												//��������̨��
		INT16		m_i16RedunNum;												//��������̨��
		INT16		m_i16AlterPeriodSet;										//�ȱ������������ӹ��罻��ʱ��
		INT16		m_i16ModelKVA;												//��������
		INT16		m_i16GrantKVA;												//��Ȩ����
		//INT16		m_i16ModelKW;

		UINT16		m_u16TimeLabel1_0;
		UINT16		m_u16TimeLabel2_0;
		UINT16		m_u16TimeLabel3_0;
																				//���Լ�ص�ͬ��ʱ��


		INT16		m_i16TimeSynCommand_0;										//ʱ��ͬ������
		UINT16		m_u16HistoryRecord[256];									//��ʷ��¼������
		UINT16 		ModuleNum;			//�������а������������ 150K����� 20KΪ���+��·

		/*���Բ���*/
		UINT16		m_i16_DegVar1;//2017.06.10
		UINT16		m_i16_DegVar2;
		UINT16		m_i16_DegVar3;

		//���������Լ�������Ч
		UINT16		m_u16BatDeepSelfRecved;

	private:
		UINT16		*pDSPVar1Adr;												//��ϱ����ĵ�ַ1ָ��
		UINT16		*pDSPVar2Adr;												//��ϱ����ĵ�ַ2ָ��
		UINT16		*pDSPVar3Adr;												//��ϱ����ĵ�ַ3ָ��

		INT16		m_i16SupplyStatusCnt;
		INT16		m_i16SystemInvOnCnt;
		INT16		m_i16OutputDelayCntPerMloop;
		INT16		m_i16ParaSignalExitMaskDelayCntPerMloop;
		INT16		m_i16FaultResetCntPerMloop;
		INT16		m_i16PromptCntPer40ms;										//��ʾȷ����ʱ

	public:
		void		App_MonitorVarInit(void);
		void		App_MonitorDeal(void);
		void		App_Int_TimeLabel(void);

	private:
		void		App_MonInterfaceWrite(void);
		void		Byp_App_MonInterfaceWrite(void);
		void		App_MonitorOrder(void);
		void		App_SendtoMonitor(void);
		void		App_MonitorParam(void);

		void        App_MonInterfaceRead(void);
		VOID 		App_ParallelLocalData(VOID );

	};

	#pragma DATA_SECTION(".FaultRecord")
	UINT16	u16FaultRecordRead[128];			//���ϼ�¼���ݶ�������
#endif

//===========================================================================
// End of file.
//===========================================================================
