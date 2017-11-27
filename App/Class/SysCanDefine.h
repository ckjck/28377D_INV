/***************************************************************************************
* 
* Filename: 		SysCanApp.h
* Contributor:
* Author: 		Wang Zhihua	
* Date: 			4/05/2008
* Copyright 		2008 ENPC Corporation, All Rights Reserved
*
* Description: 	
* 
* 
* Version
* Last rev by:		Wang Zhihua
* Last rev date:	4/21/2008
* Last rev desc:	
****************************************************************************************/

#ifndef SYS_CAN_DEFINE_H
#define SYS_CAN_DEFINE_H

//---------------------------------------------------------------------------------
//Head files
//#include "Define.h"

/**********************************************************************************
����:ģ���ϴ�״̬�����Ͷ���
���: 1 ����, 2 ���, 3��·
**********************************************************************************/	
//-----------------------------------------------------------------------------------

//����ϴ�״̬�����ݽṹ���Ͷ���

	#define MODULE_NUM	objMonInterface.ModuleNum	
	#define STATE_MSG_OBJ_SIZE objMonInterface.ModuleNum
	#define ALRM_MSG_OBJ_SIZE objMonInterface.ModuleNum


	#define MODULE_INV	0
	#define MODULE_BYP  1

	//#define INV_STATE_MSG_OBJ_SIZE 1
	//#define INV_ALRM_MSG_OBJ_SIZE 1

	#define INV_SW1_LENGTH	 (sizeof(_INV_SW1_T) * 2)
	#define BYP_SW1_LENGTH	 (sizeof(_BYP_SW1_T) * 2)

	#define INV_ALRM1_LENGTH	 (sizeof(_INV_WARN1_T) * 2)
	#define BYP_ALRM1_LENGTH	 (sizeof(_BYP_WARN1_T) * 2)
	
	typedef struct
	{
		union
		{
			/*����״̬��1*/
			UINT16 u16Word1All;		
			struct
			{
				UINT16 bOutBrk:			1;	/*�������״̬0:�Ͽ�1:�պ�*/ 
				UINT16 bMaintainBypBrk:	1;	/*ά����·�տ�״̬0:�Ͽ�1:�պ�*/    
				UINT16 bAutoBypBrk:		1;	/*�Զ���·�տ�״̬0:�Ͽ�1:�պ�*/    		
				UINT16 bSpinBrk:			3;	/*��ת�տ�״̬
												0:�ر�
												1:����״̬
												2:����
												3:��·
												4:ά��*/
				UINT16 bParamSetReq:	1;	
				UINT16 sleep:			1;	
				UINT16 bChargerOnNum:			3;	//20Kʹ�ã�UPS�Ĳ������������ڹ��õ������������
				UINT16 bCmdCharge:		1;		/*��䷢�͵�ǿ��ת��������*/
				UINT16 bEnablePreWarn:	1;		/*ʹ�ܵ��Ԥ�澯*/
				UINT16 bEnableSeftTest:    1;		/*bat seft test*/
				UINT16 bReserved:		2;												
			}tWord1Bits;
		}Word1_U;

		union
		{
			/*����״̬��2*/
			UINT16 u16Word2All;		
			struct
			{
				UINT16 bStartupEna:		1;	/*��������״̬0:������1:����*/
				UINT16 bInvState:		1;	/*�����״̬0:�ر�1:����*/  	 	
				UINT16 bInvSupplyState:	2;	/*���๩��״̬0:��������1:Byp2:Inv 3:��ֹ���*/ 
				UINT16 bEpoPrompt:		1;	/*�����ػ���ʾ0:����ʾ1:����ʾ*/
				UINT16 bModelID:        4;	/*ģ��ID��*/
				UINT16 bPwrCANState:	1;	/*Power CANͨѶ״̬*/

				//TBD
				UINT16 bProcSwPrompt:	3;	/*	�����л���ʾ
												0:����ʾ
												1:��·����л�
												2:������·�쳣�ػ���ʾ
												3:������·�쳣�ػ���ʾ
												4:������������
												5:�޷�����л���ʾ*/
				UINT16 bECOMode:		1;	/*ECOģʽ״̬  0:normal  1:ECOģʽ*/
				UINT16 bReserved:		2;
			}tWord2Bits;
		}Word2_U;

	}_INV_SW1_T;


	//-------------�����ڲ���״̬�ֶ���---------------------------
	#define PARALLEL_STATE_MSG_OBJ_SIZE 1

	// �����ڲ���̨��
	#define		ParaNumCNST				6			//����̨��
	
	//������Ϣ��־
	typedef struct
	{
		union
		{
			/*�����ڲ���״̬��1*/
			UINT16 u16Word1All;		
			struct
			{
				UINT16 bInvOn:				1;	/*bit0:	1-��������� 0-������ر�*/
				UINT16 bInvPowerOn:			1;	/*bit1:	1-��������� 0-�����û�й���*/
				UINT16 bBattCapAlarm:		1;	/*bit2:	1-������������澯*/
				UINT16 bBattInvPower:		1;	/*bit3:	1-���������乩��*/
				UINT16 bOwnSevereFault:		1;	/*bit4:	1-�������ع���*/
				UINT16 bOwnGenericFault:	1;	/*bit5:	1-����һ�����*/
				UINT16 bOwnSevereAlarm:	1;	/*bit6:	1-�������ظ澯*/
				UINT16 bOwnGenericAlarm:	1;	/*bit7:	1-����һ��澯*/

				UINT16 bBypPowerOn:		1;	/*bit8:	1-��·���� 0-��·û�й���*/
				UINT16 bSingleOnLine:		1;	/*bit9:	1-�������߱�־ */
				UINT16 bBypState:			2;	/*bit10,11: 	00����01������11��������Χ*/
				UINT16 bBypRange:		1;	/*bit12:��·��ֵ��Χ��־ */
														
				UINT16 bReserved:			3;	/*bit13~bit15: unused*/
				
			}tWord1Bits;
		}Word1_U;

		//����TBD	To Be Determined
		
	}_PARALLEL_SW1_T;

	#define PARALLEL_SW1_LENGTH	 (sizeof(_PARALLEL_SW1_T) * 2)

//������·��������õ�״̬��,ֻ��������,���������ӵ�����δ
//�ŵ�ԭ����״̬����
//-------------�����ڲ���״̬�ֶ���---------------------------
	#define BYPMAINT_STATE_MSG_OBJ_SIZE 1

	
	//��·����Ϣ��־
	typedef struct
	{
		union
		{
			/*��������·��״̬��1*/
			UINT16 u16Word1All;		
			struct
			{
				UINT16 bQ3Bp:				1;	/*bit0:	1-�ڲ�ά�޿տ��պ� 0-�ڲ�ά�޿տ��Ͽ�*/
				UINT16 bExyQ3Bp:			1;	/*bit1:	1-��·��ά�޿տ��պ� 0-��·��ά�޿տ��Ͽ�*/
				UINT16 bQ5			:		1;	/*bit2:	1-��·������ź�*/
										
				UINT16 bReserved:			12;	/*bit3~bit15: unused*/
				UINT16 bReservedFlag:			1;
				
			}tWord1Bits;
		}Word1_U;

		//����TBD
		
	}_BYPMAINT_SW1_T;

	#define BYPMAINT_SW1_LENGTH	 (sizeof(_BYPMAINT_SW1_T) * 2)

//����������·�ϴ������Ļ��ܻ��ܲ���״̬�������/��������
//ע�����4��������
//��ҪԤ����
typedef struct
{
	union
	{
		UINT16 u16Word1All;
		struct
		{
			//����ϵͳ��俪����̨��
			UINT16 bRackInvOnNum:		5;
			//����ϵͳ��乩����̨��
			UINT16 bRackInvSupplyNum:	5;
			//����ϵͳ��ع���̨��
			UINT16 bRackBattSupplyNum:	2;
			//����ϵͳ��·����̨��
			UINT16 bRackBypSupplyNum:	2;
			
			UINT16 bReserved:	2;
			
		}tWord1Bits;
		
	}Word1_U;

	union
	{
		UINT16 u16Word2All;
		struct
		{
			//����ϵͳ��������澯�ܻ�����
			UINT16 bRackNumBattWarn:		2;
			//����ϵͳ�����乩���ܻ�����
			UINT16 bRackNumBattInv:	2;
			//����ϵͳ���Ԥ�澯̨��---�ɼ��ͨ��ʵʱ��Ϣ�·�
			UINT16 bRackBattPreWarnNum:	2;
			//����ϵͳ�澯�����ۺϱ�־
			UINT16 bRackWarnFlag:	4;
										//bit0:	1-����ϵͳ�����ع���
										//bit1:	1-����ϵͳ��һ�����
										//bit2:	1-����ϵͳ�����ظ澯
										//bit3:	1-����ϵͳ��һ��澯
			//����ϵͳUPS�ϵ��־�������ܻ�ӻ��ϴ�0xFF
			UINT16	bRackRunFlag:	2;				
										//bit0:	Reserved
										//bit1:	1-1#������
										//bit2:	1-2#������
										//bit3:	1-3#������
			UINT16 bReserved:	4;
										
			
		}tWord2Bits;
		
	}Word2_U;

	//����չ
	
}_RACK_RECV_STATE_T;

#define RACK_SW_LENGTH	 (sizeof(_RACK_RECV_STATE_T) * 2)
//----------------------------------

//����ۺ�״̬��Ϣ,���ڶ�ʱ�·���DSP
typedef struct
{
	union
	{
		//����ۺ�״̬��
		UINT16 u16Word1All;	
		struct
		{
			UINT16  bBcbState	:1;				/*BCB�ѿ�״̬*/
			UINT16  bBypOverProtectState	:1;		/*��·������״̬*/
			UINT16  bBattSupplyState:1;			/*=1�����ܵ�ع���״̬*/
			UINT16  bBattPreWarnFlag:1;			/*=1���Ԥ�澯��־*/
			UINT16  bBatSeftTest    :1;     /*=1����Լ�*/
								
			UINT16	bReserved		:11;			//BIT01-15
		}tWord1Bits;
	}Word1_U;
	
}_MON_REAL_MSG_T;

	//------------------------------------------------------------------------


/**********************************************************************************
����:ģ���ϴ��澯�����Ͷ���
���: 1 ����, 2 ���, 3��·
**********************************************************************************/	

//-----------------------------------------------------------------------------------	
//����ϴ��澯�����ݽṹ���Ͷ���

	typedef struct
	{
		union
		{
			/*����澯��״̬��1*/
			UINT16 u16Word1All;				
			struct
			{
				UINT16 bInvSync:			1;	/*0�����ͬ��,1��ͬ��*/ 
				UINT16 bInvFault:			1;	/*���������*/  
				UINT16 bInvOverTemp:		1;	/*���������*/ 
				UINT16 bSigOverLoad:		1;	/*�����������*/  
				UINT16 bSigOverLoadTimeOut:	1;	/*�������س�ʱ*/  
				UINT16 bOutOverVlt:			1;	/*���������ѹ����*/   
				UINT16 bOutVltFualt:		1;	/*�����ѹ�쳣: NXb+��*/
				UINT16 bDcCurrOffsetOver:	1;	/*���ֱ����������*/
				UINT16 bInvOverCurr:		1;	/*���������*/ 
				UINT16 bBusAbnormalOff:		1;	/*ĸ���쳣�ػ�*/ 
				UINT16 bBusOverVlt:			1;	/*ֱ��ĸ�߹�ѹ����*/ 
				UINT16 bInvInductOverTemp:	1;	/*����й���*/   
				UINT16 bFanFault:			1;	/*���ȹ���*/  
				UINT16 bInvRelay:			1;	/*���̵�������*/
				UINT16 bOutFuseFault:		1;	/*�����˿��*/ 
				UINT16 bAuxPwr2Down:		1;	/*������Դ2����*/  
			}tWord1Bits;
		}Word1_U;

		union
		{
			/*����澯��״̬��2*/
			UINT16 u16Word2All;				
			struct
			{
				UINT16 bUsrOpFault				:1;	/*�û���������*/  
				UINT16 ucBypAbnor				:1;	/*��·�쳣*/
				UINT16 ucOutBkPowerFault		:1;	/*����Ӵ�����Դ����*/	
				UINT16 ucInvIGBT1OverCurr		:1;	/*���IGBT1����*/  
				UINT16 ucInvIGBT2OverCurr		:1;	/*���IGBT2����*/																																																																																												
				UINT16 ucInvIGBT3OverCurr		:1;	/*���IGBT3����*/ 
				UINT16 ucAverCurInductOverTemp	:1;	//������й���
				UINT16 ucBypLineFault			:1;	//��·���¹���
				UINT16 ucBypFanPowerFault		:1;	//��·���ȵ�Դ����
				UINT16 ucBypOverTemp			:1;	//��·����
				UINT16 bLBSstatus				:2;	/*LBS״̬0:δ���� 1:����2:�쳣*/
				UINT16 ucInvBrk 				:1;	/*���Ӵ��������������*/ 
				UINT16 bOutShortFault			:1;	//�����·
				UINT16 bReserved				:2;	
			}tWord2Bits;
		}Word2_U;


		union
		{
			/*����澯��״̬��3*/
			UINT16 u16Word3All;				
			struct
			{
				//UINT16 bBypScrFault:		1;	/*��·��բ�ܹ���*/      
				UINT16 bBypState:			2;	/*��·���(��ѹ��Ƶ��)0:normal 1:������2:������*/ 
				UINT16 bBypPhaseReverse:	1;	/*��·����*/   	
				UINT16 bToBypByLoadPuls: 	1;	/*���س��ת��·*/ 
				UINT16 bBypSwTimesLtd:		1;	/*��·�л���������*/ 
				UINT16 bBypOverCurr:		1;	/*��·��������*/
				UINT16 bBypScrFault:		2;	/*��·��բ�ܹ���0:normal 1:����2:��·3:��·*/  
				UINT16 bBypAbnorOff:		1;	/*��·�쳣�ػ��澯*/
							
				UINT16 bParaBoardFault:		1;	/*���������*/  
				UINT16 bSysShareCurr:		1;	/*������������*/   
				UINT16 bParaLineFault:		1;	/*���������ӹ���*/  
				UINT16 bParaCom:			1;	/*����ͨѶ����*/
				UINT16 bSysOverLoad:		1;	/*����ϵͳ����*/   

				UINT16 bToBypReq:			1;	/*��module����ת��·*/ 
				UINT16 bParaFault:			1;	//�����߼��ܹ���
				
			}tWord3Bits;
		}Word3_U;
		
	}_INV_WARN1_T;

	
//---------------------------------------------------------------------------

/**********************************************************************************
����:ģ���ϴ�ģ�������Ͷ���
���: 1 ����, 2 ���, 3��·
**********************************************************************************/	
//--------------------------------------------------------------------------------
//ģ���ϴ�ģ�������Ͷ���

	// 3������Ϣ
	typedef struct
	{
		UINT16 u16Phase_a;	//A����Ϣ
		UINT16 u16Phase_b;	//B����Ϣ
		UINT16 u16Phase_c;	//C����Ϣ
	}_PHASE_INFO_T;

	// 3������Ϣ
	typedef struct
	{
		UINT16 u16Line_ab;	//AB����Ϣ
		UINT16 u16Line_bc;	//BC����Ϣ
		UINT16 u16Line_ca;	//CA����Ϣ
	}_LINE_INFO_T;

	//ֱ����Ϣ
	typedef struct
	{
		UINT16 u16Post;		//����
		UINT16 u16Neg;		//����	
	}_DC_INFO_T;

	//������Ϣ
	typedef struct
	{
		UINT16 u16Dbg1;		//������Ϣ1
		UINT16 u16Dbg2;		//������Ϣ2
		UINT16 u16Dbg3;		//������Ϣ3
	}_DBG_INFO_T;

	//������Ϣ
	typedef struct
	{
		UINT16 u16Self;		// ������Ϣ
	}_SINGLE_INFO_T;
	
//----------------------------------------------------------------------------


//-------------------------------------------------------------------------------
// 2����ϱ�ģ�������ݽṹ���Ͷ���
	//ע:��·��ѹ��Ƶ������·�ϱ�����,���Գ���Ϊ12,����14
	//20081211 ����·��ѹ��Ƶ���ָĳɸ���ģ�����Ҳ�ϱ�,���Գ����ָĳ�14
	#define INV_ANALOG_MSG_OBJ_SIZE 14 //12
	
	typedef struct
	{
		_PHASE_INFO_T OutActPwr;			/*������й����� 1024 -- �ֵ*/
		_PHASE_INFO_T OutApparentPwr;		/*��������ڹ��� 1024 -- �ֵ*/
		_PHASE_INFO_T OutFactor;				/*������������� X 100*/             

		_PHASE_INFO_T OutPhaseVlt;			/*������ѹ X 10*/
		_PHASE_INFO_T OutPHaseCurr;			/*�������� 1024 -- �ֵ*/
		_PHASE_INFO_T InvPhaseVlt;			/*������ѹ X 10*/

		_PHASE_INFO_T InvPhaseCurr;			/*�������� 1024 -- �ֵ*/                
		_PHASE_INFO_T InductCurr;			/*��������1024 -- �ֵ*/
		_PHASE_INFO_T BypPhaseVlt;			/*��·���ѹ X 10*/
		
		_PHASE_INFO_T OutLoadRate;			/*��������ذٷֱ� X 10*/
		_PHASE_INFO_T OutPeakRate;			/*�������ֵ�� X 10*/  
				
		_SINGLE_INFO_T OutFreq;				/*���Ƶ�� X 100*/     
		_SINGLE_INFO_T BypFreq;				/*��·Ƶ�� X 100*/

		_DBG_INFO_T DbgVar;					/*�����Ա���*/
			
	}_INV_ANALOG_T;

#define PHASE_INFO_LEN	(sizeof(_PHASE_INFO_T))
#define LINE_INFO_LEN (sizeof(_LINE_INFO_T))
#define DC_INFO_LEN (sizeof(_DC_INFO_T))
#define SINGLE_INFO_LEN (sizeof(_SINGLE_INFO_T))
#define DBG_INFO_LEN (sizeof(_DBG_INFO_T))



//-----------------------------------------------------------------------------
// 3��·�ϱ�ģ�����ṹ�嶨��
	//����¶���Ϣ
	typedef struct
	{
		UINT16 u16BattInTemp;
		UINT16 u16BattOutTemp;
	}_BATT_TEMP_INFO_T;

	#define BYP_ANALOG_MSG_OBJ_SIZE  12	//((sizeof(BypCanAnlogMsgObjs))/(sizeof(ANALOG_MSG_OBJ_T)))	//
	
	typedef struct
	{
		_PHASE_INFO_T	BypPhaseVlt;		/*��·���ѹ X 10*/
		_PHASE_INFO_T	BypOutPhaseVlt;		/*������ѹ X 10*/
		_PHASE_INFO_T	BypPhaseCurr;		/*��·����� 1024 -- �ֵ*/
		_PHASE_INFO_T	OutFactor;			/*��������*/
		_SINGLE_INFO_T   BypFreq;			/*��·Ƶ�� X 100*/
		_SINGLE_INFO_T   BypOutFreq;			/*���Ƶ�� X 100*/
		_PHASE_INFO_T 	OutActPwr;			/*������й����� 1024 -- �ֵ*/
		_PHASE_INFO_T	OutApparentPwr;		/*��������ڹ���1024*/
		
		_PHASE_INFO_T	OutLoadRate;		/*���������*/
		_PHASE_INFO_T	OutPeakRate;		/*�����ֵ��*/

		//������·�ϴ��Ļ���ϵͳ�й����ʺ����ڹ���1024�
		_PHASE_INFO_T 	RackOutActPwr;			/*RACK����ϵͳ������й����� 1024 -- �ֵ*/
		_PHASE_INFO_T	RackOutApparentPwr;	/*RACK����ϵͳ��������ڹ���1024*/
		
//		_BATT_TEMP_INFO_T	BatteryInTemp;		/*����¶�*/ 
//		_SINGLE_INFO_T	EnvirTemp;			/*�����¶�*/
//		_DBG_INFO_T		BypDebugVari;		/*��·���Ա���*/		//TBD, �¶�
	}_BYP_ANALOG_T;

/**********************************************************************************
����:ģ�����ģ�������Ͷ���
���: 1 ����, 2 ���, 3��·
**********************************************************************************/	
	typedef struct
	{
		UINT16	u16PostVlt;	/*������鸡���ѹ*/
		UINT16	u16NegVlt;	/*������鸡���ѹ*/
	}BATT_FLT_CHARG_VLT_T;

//---------------------------------------------------------------------------------
// 1����
	#define REC_ANALOG_RECV_MSG_SIZE 2

	typedef struct
	{
		BATT_FLT_CHARG_VLT_T FltVlt;
		UINT16	u16LocalUnitPower;	/*ģ�鹦��*/
	}_REC_RECV_ANALOG_T;

//--------------------------------------------------------------------------------
// 2 ���
	#define INV_ANALOG_RECV_MSG_SIZE 5
	typedef struct
	{
		_PHASE_INFO_T   	OutActPwr;	/*ϵͳ����й�����*/
		_PHASE_INFO_T   	OutApparentPwr;	/*ϵͳ������ڹ���*/
		UINT16	u16EnvirTemp;	/*�����¶�*/

		//��������ϵͳ���й�����
		_PHASE_INFO_T   	RackOutActPwr;
		//��������ϵͳ�����ڹ���
		_PHASE_INFO_T   	RackOutApparentPwr;
		
	}_INV_RECV_ANALOG_T;


//-------------------------------------------------------------------------------
// 3 ��·
	#define BYP_ANALOG_RECV_MSG_SIZE 
	typedef struct
	{

	}_BYP_RECV_ANALOG_T;
	
/**********************************************************************************
����:������Ϣ���
���: 1 Dsp �ۺ����ò��� 2 ����,3 ���, 4��·, 5 ʱ��
**********************************************************************************/	
//#define REC_CFG_FROM_MON_OBJ_SIZE 10 //tbd
//#define REC_CFG_TO_MON_OBJ_SIZE 10//tbd

#define INV_CFG_FROM_MON_OBJ_SIZE  46	//41+�������������	//37 //tbd(sizeof(CfgFromMonObjs) / sizeof(CFG_MSG_T))
#define INV_SYS_CFG_FROM_MON_OBJ_SIZE 25	//22
#define INV_CFG_TO_MON_OBJ_SIZE 5//tbd


//---------------------------------------------------------------------------------
// 1 Dsp�ۺ����ò�������,//TBD���Ƿ�REC/INV/BYP������ز����ֿ�?
	typedef struct
	{
		UINT16 u16SetParamFresh;		//�����Ƿ����FALSE: û�и���; TRUE:����
		
		struct
		{
			//UINT16 u16UpsType;			//Nxa,b,e,a+,b+,f,rm
			UINT16 u16CapType;			/*������������: 10,15,20,30KVA*/ 
			UINT16 u16SysVltGrade;		/*ϵͳ��ѹ�ȼ����� 200V,208V,380V,400V,415V,480*/ 
			UINT16 u16RunMod;			/*����ģʽ���� 1:NOR 2:ECO*/
			//UINT16 u16AutoOutEna;		/*�Զ��ָ������������ 1:DIS 2:ENA*/
			//UINT16 u16AutoOutDelay;		/*�Զ��ָ������ʱ���� (10) 1-1440*/       
			UINT16 u16InFreq;			/*����Ƶ�ʵȼ����� 50HZ,60HZ */  
			UINT16 u16OutFreq;			/*���Ƶ�ʵȼ����� 50HZ,60HZ */ 
			//UINT16 u16InTransf;			/*�Ƿ��������ѹ��	1:û��  2:�� ȱʡ1*/
			//UINT16 u16OutTransf;			/*�Ƿ��������ѹ��	1:û��  2:�� ȱʡ1*/
			//UINT16 u16InTransfRate;		/*�����ѹ�����1~10000,ȱʡ100 */
			//UINT16 u16OutTransfRate;	/*�����ѹ�����1~10000,ȱʡ100*/

			UINT16 u16GenMod;			/*���ܷ����ģʽ���� 1:ENA  2:DIS*/
			UINT16 u16GenPwrLtd;		/*���ܷ�����޹������� (0) 0-240*/
			UINT16 u16GenStartDelay;	/*���ܷ���������ӳ�ʱ������ (10) 5-30*/
			//UINT16 u16ChargerType;		/*������ڷ����ģʽ�Լ���ط��澯ʱ�ĳ�
			//								�繦������0: 0(0)  1:20%  2:100% 3:Reserved*/
												
			UINT16 u16AgingOutCurrRate;	/*���ϻ���������ٷֱ����� 0~130(25)*/    
			UINT16 u16OutSwitchTime;	/*�������л�ʱ������ms 0:NONE 1:40 2:60 3:80 4:100*/      
			UINT16 u16OutFreqTrackRate;	/*���Ƶ�ʸ����������� 1~30---0.1~3Hz/s (1)*/ 
			UINT16 u16InvRunMode;		/*���������ģʽ����0:���� 1:���ϻ� 2:����*/
			UINT16 u16InvOutFactor;		/*����������������0:0.8; 1: 1*/
			UINT16 u16OutVltMicroAdj;	/*�����ѹ΢��ϵ������ (230) 200~250*///SYS
			
			UINT16 u16BypSwitchTime;	/*��·�л��������� (5) 1-10*/ 
			UINT16 u16BypFreqTrackLtd;	/*��·Ƶ�ʸ��ٷ�Χ���� 1:2HZ 2:1HZ 3:3HZ 4:0.5HZ*/
			UINT16 u16BypVltUpLtd;		/*��·��ѹ������Χ����(����) 1:15% 2:10% 3:20%*/    
			UINT16 u16BypVltDownLtd;	/*��·��ѹ������Χ����(����) 1:20% 2:10% 3:30% 4:40%*/
			UINT16 u16BypFreqLtd;		/*��·Ƶ�ʱ�����Χ���� 1:10% 2:20%* 3:5% 4: 2.5% */  
			UINT16 u16BypProtectBrk;	/*��·���ౣ���ö�·�� 0:NONE 1:HAVE*/  

			UINT16 u16RMIdx;			/*���ܺ�*/
			UINT16 u16RMNum;			/*������������*/
			UINT16 u16ReRMNum;			/*�������������*/
			UINT16 u16BasicModNum;		/*��������ģ����*/
			UINT16 u16ReModNum;		/*��������ģ����*/
			UINT16 u16LBSMode;			/*LBS mode 0:none 1: master 2: slaver*/

			UINT16	u16BattGrpIdx;		/*������ 1��1�ŵ���飻 2��2�ŵ����*/	
			UINT16	u16Cap;				/*�����������7~3000AH*/
			UINT16	u16EqChgVlt;		/*��ؾ����ѹ */	
			UINT16	u16ChgCurrLtd;		/*��س������ֵ X 100 */
			UINT16	u16SelfChkDischgPowLtd;			/*����Լ�ŵ繦������*/
			UINT16	u16UpsConfigType;			/*��������ʽ*/
			UINT16   u16BatShare;					/*1:share module ,0: no share.*/
			UINT16	u16OutputPhase;	/*0:����1:����*/
		}SetParam;
	}_SYS_DSP_SET_INFO_T;	

	//-----------------------------------------------------------------------------------
	

	//-----------------------------------------------------------------------------------
	// 3����У����������
	typedef struct
	{
		UINT16 u16SetParamFresh;		//�����Ƿ����FALSE: û�и���; TRUE:����

		//u16SetParamFresh1--������
		UINT16 u16SetParamFresh1;
		UINT16 u16SetParamFresh2;
		struct
		{
			UINT16 u16OutVltRevCoef_a;			/*A�������ѹУ��ϵ�� (4096)*/
			UINT16 u16OutVltRevCoef_b;			/*B�������ѹУ��ϵ�� (4096)*/
			UINT16 u16OutVltRevCoef_c;			/*C�������ѹУ��ϵ�� (4096)*/
			UINT16 u16OutCurrRevCoef_a;			/*A���������У��ϵ�� (4096)*/
			UINT16 u16OutCurrRevCoef_b;			/*B���������У��ϵ�� (4096)*/
			UINT16 u16OutCurrRevCoef_c;			/*C���������У��ϵ�� (4096)*/
			
			UINT16 u16InvVltRevCoef_a;			/*A������ѹУ��ϵ������ (4096)*/ 
			UINT16 u16InvVltRevCoef_b;			/*B������ѹУ��ϵ������ (4096)*/ 
			UINT16 u16InvVltRevCoef_c;			/*C������ѹУ��ϵ������ (4096)*/ 
			UINT16 u16InvCurrRevCoef_a;			/*A��������У��ϵ������ (4096)*/
			UINT16 u16InvCurrRevCoef_b;			/*B��������У��ϵ������ (4096)*/
			UINT16 u16InvCurrRevCoef_c;			/*C��������У��ϵ������ (4096)*/
			
			UINT16 u16BypVltRevCoef_a;			/*A����·��ѹУ��ϵ������ (4096)*/
			UINT16 u16BypVltRevCoef_b;			/*B����·��ѹУ��ϵ������ (4096)*/
			UINT16 u16BypVltRevCoef_c;			/*C����·��ѹУ��ϵ������ (4096)*/

			UINT16 u16DebugVar1Addr;			/*���Ա�����ַ1*/
			UINT16 u16DebugVar2Addr;			/*���Ա�����ַ2*/
			UINT16 u16DebugVar3Addr;			/*���Ա�����ַ3*/

			UINT16 u16DegVar1;		/*���Բ���1*/
			UINT16 u16DegVar2;		/*���Բ���2*/
			UINT16 u16DegVar3;		/*���Բ���3*/
		}SetParam;
	}_INV_SET_INFO_T;

	#define INV_SET_INFO_SIZE  (sizeof(_INV_SET_INFO_T) / sizeof(UINT16) * 2 -2)

	
	//------------------------------------------------------------------------------------
	// 5 ʱ������
	typedef struct
	{
//		UINT32 u32Senconds;	//Unix ��Ԫʼ����
		UINT16 u16TimeS_H;
		UINT16 u16TimeS_L;
		UINT16 u16TimeMS;//ms
		//=1��ʾ��ͬ����,=0δͬ����
		UINT16 u16HaveSyncClkFlag;
	}_RT_CLK_T;


/**********************************************************************************
����:ģ�������Ϣ
���: 
**********************************************************************************/	
	typedef struct
	{
		UINT16 u16ProductVer;	
		UINT16 u16BaseLineVer;
		UINT16 u16DevelopVer;
	}_VER_T;

	typedef struct
	{
		//System Can protocol version
		_VER_T ProtocolVer;
		
		//Dsp firmware version
		_VER_T DspFwVer;
		
		//Cpld firmware version
		_VER_T CpldFwVer;

		//���к�
		//TBD
	}_MODULE_BASE_INFO_T;


/**********************************************************************************
����:������Ϣ
���: ��������λ��Ϊ1��ʾ���µ����0��ʾû���µ�����
**********************************************************************************/	
	typedef struct
	{
		//�������
		UINT16 u16PostBattFltChgCmd	: 	1;//������ת����
		UINT16 u16NegBattFltChgCmd:		1;//������ת����
		UINT16 u16PostBattEqualChgCmd:	1;//������ת����
		UINT16 u16NegBattEqualChgCmd:	1;//������ת����
		UINT16 u16SelfChk20PercentCmd:	1;//20%�Լ�
		UINT16 u16SelfChkDeepCmd:		1;//����Լ�
		UINT16 u16SelfChkEndCmd:		1;//�Լ����
		UINT16 u16EodCmd:				1;//EOD
		UINT16 u16BattTripCmd:			1;//���trip
		UINT16 u16OutputEnableCmd:		1;//�������
		UINT16 u16OutputDisableCmd:		1;//�����ֹ
		UINT16 u16BattGndPtcCmd:		1;//�ӵر���
		UINT16 u16Reserved				:4;

		//ϵͳ����	
		UINT16 u16FaultClrCmd:			1;//�������
		UINT16 u16ManualInvOnCmd:		1;//�ֶ�����
		UINT16 u16ManulaInvOffCmd:		1;//�ֶ��ػ�
		UINT16 u16EpoCmd:				1;//EPO
		UINT16 u16AbnrmlShutConfirmCmd:	1;//�쳣�ػ�ȷ��
		UINT16 u16AbnrmllShutCancelCmd:	1;//�쳣�ػ�ȡ��
		UINT16 u16IndirectSwitchCmd:	1;//����л�
		UINT16 u16RecFwUpdateCmd:		1;//�����̼�����
		UINT16 u16InvFwUpdateCmd:		1;//���̼�����
		UINT16 u16BypFwUpdateCmd:		1;//��·�̼�����
		UINT16 u16Reserved1				:6;
				
	}_CTRL_INFO_T;

	enum SYS_CAN_CTRL_CMD_LIST
	{
		POST_BATT_FLT_CHARG_CMD = 0x01,	//������ת����
		NEG_BATT_FLT_CHARG_CMD,			//������ת����
		POST_BATT_EQUAL_CHARG_CMD,		//������ת����
		NEG_BATT_EQUAL_CHARG_CMD,		//������ת����
		SELF_CHK_20PERCENT_CMD,		//20%�Լ�
		SELF_CHK_DEEP_CMD,			//����Լ�
		SELF_CHK_END_CMD,				//�Լ����
		EOD_CMD,						//EOD
		BATT_TRIP_CMD,					//���trip
		OUTPUT_ENABLE_CMD,			//�������
		OUTPUT_DISENABLE_CMD,		//�����ֹ
		BATT_GND_PTC_CMD,				//�ӵر���

		FAULT_CLEAR_CMD = 0x10,		//�������
		MANUAL_INV_ON_CMD,			//�ֶ�����
		MANUAL_INV_OFF_CMD,			//�ֶ��ػ�
		EPO_CMD,						//EPO
		ABNORMAL_SHUT_CONFIRM_CMD,		//�쳣�ػ�ȷ��
		ABNORMAL_SHUT_CANCEL_CMD,		//�쳣�ػ�ȡ��
		INDIRECT_SWITCH_CMD,			//����л�
		REC_FW_UPDATE_CMD,			//�����̼�����
		INV_FW_UPDATE_CMD,			//���̼�����
		BYP_FW_UPDATE_CMD			//��·�̼�����

	};



//��·�ϴ�״̬�����ݽṹ���Ͷ���
	typedef struct
	{
		union
		{
			//��·��״̬��
			UINT16 u16Word1All;	
			struct
			{
				UINT16  bPwrCANState	:1;				/*Power CANͨѶ״̬*/
				UINT16  bEpoPrompt		:1;				/*�����ػ���ʾ0:����ʾ1:����ʾ*/
//				UINT16	bBCBConnect		:1;				//BIT002	BCB����״̬0��δ���� 1������

				UINT16	bBCBConnect		:2;	
				//20081107 �����ϴ�����״̬2bit
				//=2,��ؼӹ�����״̬�����/��������
				//=00 ��������,=01��·����=02�����ֹ
				UINT16	bBpSupplyState :2;				//BIT03-04 ����״̬��·�ϱ�����״̬
//				UINT16	bRecordTxRequest :1;			//BIT05 �����ϴ�����

				UINT16	bOutBreak		:1;				/*�������״̬0:�Ͽ�1:�պ�*/
				UINT16	bBypMaintBreak	:1;				/*��·ά�޿տ�״̬ 0:�Ͽ�1:�պ�*/
				UINT16	bAutoBypBreak	:1;				/*�Զ���·�տ�״̬0:�Ͽ�1:�պ�*/
				UINT16	bInsideBattBreak:1;
				UINT16	bBypOnlinestate	:1;		//��·���߱�־
				UINT16	bReserved		:5;		//BIT09-15
			}tWord1Bits;
		}Word1_U;
		
	}_BYP_SW1_T;

	// ��·�ϴ��澯��Ϣ���ݽṹ����//TBD

	typedef struct
	{
		union
		{
			/*��·�澯��״̬��1*/
			UINT16 u16Word1All;				
			struct
			{
				UINT16 bBypOverCurr				:1;			/*��·��������*/
				UINT16 bBypFeedBackFault		:2;			/*��·��բ�ܹ���0:normal 1:����*/ 
				UINT16 bFanFault				:1;			//���ȹ���
				UINT16 bBatEarthFault			:1;			//��ؽӵع���
				UINT16 bBatRoomEnvirAbnormal	:1;			//��ط������쳣
				UINT16 bModOvTemp				:1;			//��·ģ�����
				UINT16 bAuxPwr1Down				:1;			//������Դ1����
				UINT16 bAuxPwr2Down				:1;			//������Դ2����	
			//	UINT16 bPowOK					:1;			//Power ok����	
				UINT16 bBypState				:2;			/*��·���(��ѹ��Ƶ��)0:normal 1:������2:������*/ 
				UINT16 bBypPhaseReverse			:1;			/*��·����*/   		
				//INV TBD
//				UINT16 bLbsActive:			2;	/*LBS����״̬0: noactive 1: active 2: abnormal*/
//				UINT16 bToBypReq:			1;	/*�ڻ�����ת��·*/ 
				UINT16 bOvload					:1;				//�������
				UINT16 bParallelCommErr		:1;		/*���ܼ䲢��ͨѶ����*/
				UINT16 bReqBypScrFault			:1;	/*�ڻ���բ�ܹ���*/
				UINT16 bTrasOverTemp			:1;	//�ⲿ�����ѹ������
			}tWord1Bits;
		}Word1_U;

	}_BYP_WARN1_T;


//-----------------------------------------
//��������ɢ�ź�״̬�ṹ����
//Ŀǰֻ����·�����ź�,�Ժ���Ը��������ڴ�����
//	#define REC_DISCRETE_MSG_OBJ_SIZE 1
	typedef struct
	{
		union
		{
			/*������ɢ�ź�״̬��1*/
			UINT16 u16Word1All;			
			struct
			{
				UINT16 bInBrk:			1;	/*���뷴���ź�0:��Ч; 1:��Ч*/
				UINT16 bChargerWorkP:	1;	/*BIT2	���������������������*/
				UINT16 bChargerWorkN:	1;	/*BIT3	����������������������*/
				UINT16 bBoostState: 	2;  /*Boost״̬*/
				UINT16 bReserved:		11;
			}tWord1Bits;
		}Word1_U;

		
	}_REC_DISCRETE_SW1_T;

#endif

