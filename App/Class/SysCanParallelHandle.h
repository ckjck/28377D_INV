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


//CAN通信相关常数
//#define		BroadcastIdCNST			15			//广播帧的ID
//#define 	CANRxFlag				0x0000003F	//判断是否收到新的数据
//#define 	VersionNumber			0x0020		//软件版本号
//#define 	MessageID_Version		0x0002		//本版配置信息帧ID
//#define 	MessageID_High			0x0003		//高优先级信息帧ID
//#define 	MessageID_Low			0x0004		//低优先级信息帧ID



struct	ParaData
	{
		INT16		m_i16Pouta;					//单机A相功率 Q6.10
		INT16		m_i16Poutb;					//单机B相功率
		INT16		m_i16Poutc;					//单机C相功率

		INT16		m_i16Souta;					//单机A相容量
		INT16		m_i16Soutb;					//单机B相容量
		INT16		m_i16Soutc;					//单机C相容量

		INT16		m_i16ModelKVA;				//单机物理容量
		INT16		m_i16wGrantKVA;				//单机授权容量
		
		UINT16	uSystemFlag;			//单机信息标志
										//bit0:	1-逆变器开启 0-逆变器关闭
										//bit1:	1-逆变器供电 0-逆变器没有供电
										//bit2:	1-单机电池容量告警
										//bit3:	1-单机电池逆变供电
										//bit4:	1-单机严重故障
										//bit5:	1-单机一般故障
										//bit6:	1-单机严重告警
										//bit7:	1-单机一般告警
										//bit8:	1-旁路供电 0-旁路没有供电
										//bit9:	1-单机在线标志 
										//bit10,11: 	00正常
										//01超切换范围
										//11超保护范围
										//bit12:旁路幅值范围标志
										//bit13~bit15: unused
	};

struct	CANTempReg		//临时寄存器
	{
		INT16	iMessageID;		//存放ID
		
		INT16 iTempBox0;		//存放MBXnA
		
		INT16	iTempBox1;		//存放MBXnB							

		INT16	iTempBox2;		//存放MBXnC
		
		INT16	iTempBox3;		//存放MBXnD
	};	

struct	MACIDFlagWord
		{
			UINT16 bMACID0		:1; // 旁路柜ID 
			UINT16 bMACID1		:1;	// UPS1 ID
			UINT16 bMACID2		:1;	// UPS2 ID
			UINT16 bMACID3		:1;	// UPS3 ID
			UINT16 bMACID4		:1;	// UPS4 ID
			UINT16 bMACID5		:1;	// UPS5 ID
			UINT16 bMACID6		:1;	// UPS6 ID
			UINT16 bMACID1Backup	:1;	//UPS1 ID 备份
			UINT16 bMACID2Backup	:1;	//UPS1 ID 备份
			UINT16 bMACID3Backup	:1;	//UPS1 ID 备份
			UINT16 bMACID4Backup	:1;	//UPS1 ID 备份
			UINT16 bMACID5Backup	:1;	//UPS1 ID 备份
			UINT16 bMACID6Backup	:1;	//UPS1 ID 备份
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
		/*		BIT 00:		CAN通讯周期标志
						1:通信周期42ms定时到，置1
						0:42ms周期定时未到，继续等待
		*/
		UINT16	bCommErrJudgePeriod:2;
		/*		BIT 01-02:	CAN通信故障判断标志
							2: 82ms定时到，判断是否存在通信故障
						其他:本机处于CAN通信发送状态
		*/
		UINT16	bErr:1;
		/*		BIT 03:		CAN通信故障标志
						1:故障
						0:无故障
		*/
		UINT16	bAlone:1;
		/*		BIT 04:		CAN通信单机标志
						1:并机系统中只有本机在线/并机通信线故障(相当于本机在线)
						0:正常
		*/
		UINT16	bBadData:1;
		/*		BIT 05:		CAN通信接收错误数据标志
						1:通信主机接收到错误数据或接收数据超时/从机接收到错误数据
						0:接收到正确数据
		*/
		UINT16	bAutoBp:1;
		/*		BIT 06:		CAN通信接收旁路柜数据标志
						1:通信主机接收到旁路柜数据
						0:没有接收到数据/旁路柜没有启动
		*/
		UINT16	bVersionErr :1;//CAN通信版本标志
						//1:与本机不一致
						//0:与本机一致
		UINT16	bReceived:1;
		/*		BIT 08:		CAN通信接收标志
						1:接收到新的数据
						0:没有接收到新数据
		*/
		UINT16	bVersion:1;
		/*		BIT 09:		软件版本发送标志
						1:需发送
						0:不需发送
		*/
		UINT16	bUPSSingleOline:1;
		/*		BIT 10:   并机系统单机在线标志 
						1: 单机在线
						0: 多机在线
		*/
		
		UINT16	bRev1015:5;
	};






//---------------------------------------------------------------------------------//

class SysCanParallelHandle
{
public:
	/* 以下是通过系统广播得到的数据 */
	INT16		iPoutTotala;				//并机系统A相总功率 *10
	INT16		iPoutTotalb;				//并机系统B相总功率
	INT16		iPoutTotalc;				//并机系统C相总功率
	INT16		iPoutTotalMax;				//并机系统总功率各相最大值

	INT16		iSoutTotala;				//并机系统A相总容量
	INT16		iSoutTotalb;				//并机系统B相总容量
	INT16		iSoutTotalc;				//并机系统C相总容量
	INT16		iSoutTotalMax;				//并机系统总容量各相最大值
		
	INT16		iPSoutTotalMax;				//并机系统总功率、容量各相最大值

	INT16		iNumInv;					//并机系统逆变开机总台数
	INT16		iNumOn;						//并机系统逆变供电总台数
	INT16		iNumBp;						//并机系统旁路供电台数
	INT16		iNumUPS;					//并机系统实际上电总台数

/* 以下是通信本机传给监控的数据 */
	INT16		iNumBattWarn;				//并机系统电池容量告警总台数
	INT16		iNumBattInv;				//并机系统电池逆变供电总台数
	INT16		iParaWarnFlag;				//并机系统告警故障综合标志
										//bit0:	1-并机系统有严重故障
										//bit1:	1-并机系统有一般故障
										//bit2:	1-并机系统有严重告警
										//bit3:	1-并机系统有一般告警

	INT16		iParaRunFlag;				//并机系统UPS上电标志，单机或从机上传0xFF
										//bit0:	Reserved
										//bit1:	1-1#机开机
										//bit2:	1-2#机开机
										//bit3:	1-3#机开机
										//bit4:	1-4#机开机
										//bit5:	1-5#机开机
										//bit6:	1-6#机开机
										//bit7-15:	Reserved

/* 以下是本机维护的数据 */
	
	volatile	struct 	ParaData		sParaData[ParaNumCNST];
	
	INT16  iCapacityOn[ParaNumCNST];

/* 以下是各单机维护的数据 */

	INT16		iFrameIDLow;					//通讯帧的序号
	INT16		iFrameIDHigh;

	UINT16	uCanDelay;					//发送前延时记录时刻
	INT16		iStartTimeCount;			//开机后计时器(以40ms为单位，限幅在7500，即5分钟)
	
//cb 030422
//	INT16		iCommErrJudgePeriod;		//单机在线、并机通信故障延时计数器
//cb 030604
	INT16		iSpanCanErr;				//并机通信故障延时计数器
	
//	INT16		iCANTxPrdStatus;			//1：	发送周期42ms时间到，发送新数据
	INT16		iLocalStatusChanged;		//1:	工作状态发送改变
										//0：	未改变
	INT16		iVersionTxFlagCounter;		//版本发送周期计数器
				
	

	
	volatile	struct	CANTempReg	iCANTempReg;
	
	//低优先级信息帧接收标志
	
	union	MACIDFlag	uMACIDFlag;//接收消息标志，接收到低优先级消息后置对应的位为1。

	//通信故障发生后，该计数器用来统计连续未接收到某UPS消息的次数。
	//大于等于5次，则将响应UPS的数据buffers清零。20061008
	volatile unsigned int sMACIDErrCounter[ParaNumCNST];		
	volatile	struct 	CanFlag	sCanFlag;

		INT16			iCanAloneConfirmCnt;				//CanBeAlone确认计数，在此期间不作任何操作
		
		INT16 		iCapacitySumBasic;					//所有上电机型容量之和 为5Kva的倍数.

		INT16 		iCapacitySum_InvOn;					//所有逆变开启机型容量之和 为5Kva的倍数.

		INT16 		iCapacitySum_Invmod;				//所有逆变供电机型容量 为5Kva的倍数.

		INT16			iMaxCapacityInv;					//逆变开机最大容量
		INT16			iMaxCapacityOn;						//逆变供电最大容量
		INT16			iInvMasterPriorINT16er;

		INT16			iKCapSum_Invmod;					//系数：sun(Cai/Cpi)--逆变供电,Q4.12
		INT16			iKCapAdj;							//降额机型的不同容量并机校正系数,Q4.12

//电池管理所需变量
		INT16 		iSumOfInvONCapacity;				//处在逆变供电状态下的各台机器的GrandKVA之和
		INT16			iSumOfBattInvCapacity;				//处在电池逆变状态下的各台机器的GrandKVA之和;
		INT16			iSumOfBattWarCapacity;				//处在电池预告警状态下的各台机器的GrandKVA之和;



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





