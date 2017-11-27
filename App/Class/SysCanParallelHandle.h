/***************************************************************************************
* 
* Filename: 		SysCanParallelHandle.h
* Contributor:
* Author: 		liujie	
* Date: 			7/28/2008
* Copyright 		2008 ENPC Corporation, All Rights Reserved
*
* Description: 	
* 
* 
* Version
* Last rev by:		liujie
* Last rev date:	7/28/2008
* Last rev desc:	
****************************************************************************************/
#ifndef SysCanParallelHandle_H
#define SysCanParallelHandle_H


//CANͨ����س���
//#define		BroadcastIdCNST			15			//�㲥֡��ID
//#define 	CANRxFlag				0x0000003F	//�ж��Ƿ��յ��µ�����
//#define 	VersionNumber			0x0020		//����汾��
//#define 	MessageID_Version		0x0002		//����������Ϣ֡ID
//#define 	MessageID_High			0x0003		//�����ȼ���Ϣ֡ID
//#define 	MessageID_Low			0x0004		//�����ȼ���Ϣ֡ID



struct	ParaData
	{
		INT16		m_i16Pouta;					//����A�๦�� Q6.10
		INT16		m_i16Poutb;					//����B�๦��
		INT16		m_i16Poutc;					//����C�๦��

		INT16		m_i16Souta;					//����A������
		INT16		m_i16Soutb;					//����B������
		INT16		m_i16Soutc;					//����C������

		INT16		m_i16ModelKVA;				//������������
		INT16		m_i16wGrantKVA;				//������Ȩ����
		
		UINT16	uSystemFlag;			//������Ϣ��־
										//bit0:	1-��������� 0-������ر�
										//bit1:	1-��������� 0-�����û�й���
										//bit2:	1-������������澯
										//bit3:	1-���������乩��
										//bit4:	1-�������ع���
										//bit5:	1-����һ�����
										//bit6:	1-�������ظ澯
										//bit7:	1-����һ��澯
										//bit8:	1-��·���� 0-��·û�й���
										//bit9:	1-�������߱�־ 
										//bit10,11: 	00����
										//01���л���Χ
										//11��������Χ
										//bit12:��·��ֵ��Χ��־
										//bit13~bit15: unused
	};

struct	CANTempReg		//��ʱ�Ĵ���
	{
		INT16	iMessageID;		//���ID
		
		INT16 iTempBox0;		//���MBXnA
		
		INT16	iTempBox1;		//���MBXnB							

		INT16	iTempBox2;		//���MBXnC
		
		INT16	iTempBox3;		//���MBXnD
	};	

struct	MACIDFlagWord
		{
			UINT16 bMACID0		:1; // ��·��ID 
			UINT16 bMACID1		:1;	// UPS1 ID
			UINT16 bMACID2		:1;	// UPS2 ID
			UINT16 bMACID3		:1;	// UPS3 ID
			UINT16 bMACID4		:1;	// UPS4 ID
			UINT16 bMACID5		:1;	// UPS5 ID
			UINT16 bMACID6		:1;	// UPS6 ID
			UINT16 bMACID1Backup	:1;	//UPS1 ID ����
			UINT16 bMACID2Backup	:1;	//UPS1 ID ����
			UINT16 bMACID3Backup	:1;	//UPS1 ID ����
			UINT16 bMACID4Backup	:1;	//UPS1 ID ����
			UINT16 bMACID5Backup	:1;	//UPS1 ID ����
			UINT16 bMACID6Backup	:1;	//UPS1 ID ����
			UINT16 bRev1315		:3;	//20051117
		};
	union	MACIDFlag
	{
		INT16	 iMACIDFlagWord;
		struct	MACIDFlagWord	 sMACIDFlagBits;
	};


struct	CanFlag
	{
		UINT16	bCANTxPrdStatus:1;
		/*		BIT 00:		CANͨѶ���ڱ�־
						1:ͨ������42ms��ʱ������1
						0:42ms���ڶ�ʱδ���������ȴ�
		*/
		UINT16	bCommErrJudgePeriod:2;
		/*		BIT 01-02:	CANͨ�Ź����жϱ�־
							2: 82ms��ʱ�����ж��Ƿ����ͨ�Ź���
						����:��������CANͨ�ŷ���״̬
		*/
		UINT16	bErr:1;
		/*		BIT 03:		CANͨ�Ź��ϱ�־
						1:����
						0:�޹���
		*/
		UINT16	bAlone:1;
		/*		BIT 04:		CANͨ�ŵ�����־
						1:����ϵͳ��ֻ�б�������/����ͨ���߹���(�൱�ڱ�������)
						0:����
		*/
		UINT16	bBadData:1;
		/*		BIT 05:		CANͨ�Ž��մ������ݱ�־
						1:ͨ���������յ��������ݻ�������ݳ�ʱ/�ӻ����յ���������
						0:���յ���ȷ����
		*/
		UINT16	bAutoBp:1;
		/*		BIT 06:		CANͨ�Ž�����·�����ݱ�־
						1:ͨ���������յ���·������
						0:û�н��յ�����/��·��û������
		*/
		UINT16	bVersionErr :1;//CANͨ�Ű汾��־
						//1:�뱾����һ��
						//0:�뱾��һ��
		UINT16	bReceived:1;
		/*		BIT 08:		CANͨ�Ž��ձ�־
						1:���յ��µ�����
						0:û�н��յ�������
		*/
		UINT16	bVersion:1;
		/*		BIT 09:		����汾���ͱ�־
						1:�跢��
						0:���跢��
		*/
		UINT16	bUPSSingleOline:1;
		/*		BIT 10:   ����ϵͳ�������߱�־ 
						1: ��������
						0: �������
		*/
		
		UINT16	bRev1015:5;
	};






//---------------------------------------------------------------------------------//

class SysCanParallelHandle
{
public:
	/* ������ͨ��ϵͳ�㲥�õ������� */
	INT16		iPoutTotala;				//����ϵͳA���ܹ��� *10
	INT16		iPoutTotalb;				//����ϵͳB���ܹ���
	INT16		iPoutTotalc;				//����ϵͳC���ܹ���
	INT16		iPoutTotalMax;				//����ϵͳ�ܹ��ʸ������ֵ

	INT16		iSoutTotala;				//����ϵͳA��������
	INT16		iSoutTotalb;				//����ϵͳB��������
	INT16		iSoutTotalc;				//����ϵͳC��������
	INT16		iSoutTotalMax;				//����ϵͳ�������������ֵ
		
	INT16		iPSoutTotalMax;				//����ϵͳ�ܹ��ʡ������������ֵ

	INT16		iNumInv;					//����ϵͳ��俪����̨��
	INT16		iNumOn;						//����ϵͳ��乩����̨��
	INT16		iNumBp;						//����ϵͳ��·����̨��
	INT16		iNumUPS;					//����ϵͳʵ���ϵ���̨��

/* ������ͨ�ű���������ص����� */
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

/* �����Ǳ���ά�������� */
	
	volatile	struct 	ParaData		sParaData[ParaNumCNST];
	
	INT16  iCapacityOn[ParaNumCNST];

/* �����Ǹ�����ά�������� */

	INT16		iFrameIDLow;					//ͨѶ֡�����
	INT16		iFrameIDHigh;

	UINT16	uCanDelay;					//����ǰ��ʱ��¼ʱ��
	INT16		iStartTimeCount;			//�������ʱ��(��40msΪ��λ���޷���7500����5����)
	
//cb 030422
//	INT16		iCommErrJudgePeriod;		//�������ߡ�����ͨ�Ź�����ʱ������
//cb 030604
	INT16		iSpanCanErr;				//����ͨ�Ź�����ʱ������
	
//	INT16		iCANTxPrdStatus;			//1��	��������42msʱ�䵽������������
	INT16		iLocalStatusChanged;		//1:	����״̬���͸ı�
										//0��	δ�ı�
	INT16		iVersionTxFlagCounter;		//�汾�������ڼ�����
				
	

	
	volatile	struct	CANTempReg	iCANTempReg;
	
	//�����ȼ���Ϣ֡���ձ�־
	
	union	MACIDFlag	uMACIDFlag;//������Ϣ��־�����յ������ȼ���Ϣ���ö�Ӧ��λΪ1��

	//ͨ�Ź��Ϸ����󣬸ü���������ͳ������δ���յ�ĳUPS��Ϣ�Ĵ�����
	//���ڵ���5�Σ�����ӦUPS������buffers���㡣20061008
	volatile unsigned int sMACIDErrCounter[ParaNumCNST];		
	volatile	struct 	CanFlag	sCanFlag;

		INT16			iCanAloneConfirmCnt;				//CanBeAloneȷ�ϼ������ڴ��ڼ䲻���κβ���
		
		INT16 		iCapacitySumBasic;					//�����ϵ��������֮�� Ϊ5Kva�ı���.

		INT16 		iCapacitySum_InvOn;					//������俪����������֮�� Ϊ5Kva�ı���.

		INT16 		iCapacitySum_Invmod;				//������乩��������� Ϊ5Kva�ı���.

		INT16			iMaxCapacityInv;					//��俪���������
		INT16			iMaxCapacityOn;						//��乩���������
		INT16			iInvMasterPriorINT16er;

		INT16			iKCapSum_Invmod;					//ϵ����sun(Cai/Cpi)--��乩��,Q4.12
		INT16			iKCapAdj;							//������͵Ĳ�ͬ��������У��ϵ��,Q4.12

//��ع����������
		INT16 		iSumOfInvONCapacity;				//������乩��״̬�µĸ�̨������GrandKVA֮��
		INT16			iSumOfBattInvCapacity;				//���ڵ�����״̬�µĸ�̨������GrandKVA֮��;
		INT16			iSumOfBattWarCapacity;				//���ڵ��Ԥ�澯״̬�µĸ�̨������GrandKVA֮��;



public:
	void vCAN_Agent(void);
	void vCAN_Var_Init(void);

private:	
	void vCAN_Rx( void );
	void vCANDataRemove ( UINT16 i );
	void vCANDataStore( void );
	void vCANCalculation( void );   
	void vCANLocalData( void );  
	void vCANHigh_Tx( void );
	void vCANLow_Tx( void );    
	void vCAN_Com_Err( void );   
	void vCANVersion_Tx ( void );

};



#endif





