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
名称:模块上传状态量类型定义
类别: 1 整流, 2 逆变, 3旁路
**********************************************************************************/	
//-----------------------------------------------------------------------------------

//逆变上传状态量数据结构类型定义

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
			/*逆变侧状态字1*/
			UINT16 u16Word1All;		
			struct
			{
				UINT16 bOutBrk:			1;	/*输出开关状态0:断开1:闭合*/ 
				UINT16 bMaintainBypBrk:	1;	/*维修旁路空开状态0:断开1:闭合*/    
				UINT16 bAutoBypBrk:		1;	/*自动旁路空开状态0:断开1:闭合*/    		
				UINT16 bSpinBrk:			3;	/*旋转空开状态
												0:关闭
												1:测试状态
												2:正常
												3:旁路
												4:维修*/
				UINT16 bParamSetReq:	1;	
				UINT16 sleep:			1;	
				UINT16 bChargerOnNum:			3;	//20K使用，UPS的并机数量，用于共用电池组充电器管理
				UINT16 bCmdCharge:		1;		/*逆变发送的强制转浮充命令*/
				UINT16 bEnablePreWarn:	1;		/*使能电池预告警*/
				UINT16 bEnableSeftTest:    1;		/*bat seft test*/
				UINT16 bReserved:		2;												
			}tWord1Bits;
		}Word1_U;

		union
		{
			/*逆变侧状态字2*/
			UINT16 u16Word2All;		
			struct
			{
				UINT16 bStartupEna:		1;	/*开机允许状态0:不允许1:允许*/
				UINT16 bInvState:		1;	/*逆变器状态0:关闭1:开启*/  	 	
				UINT16 bInvSupplyState:	2;	/*逆变侧供电状态0:均不供电1:Byp2:Inv 3:禁止输出*/ 
				UINT16 bEpoPrompt:		1;	/*紧急关机提示0:无提示1:有提示*/
				UINT16 bModelID:        4;	/*模块ID号*/
				UINT16 bPwrCANState:	1;	/*Power CAN通讯状态*/

				//TBD
				UINT16 bProcSwPrompt:	3;	/*	过程切换提示
												0:无提示
												1:旁路间接切换
												2:单机旁路异常关机提示
												3:并机旁路异常关机提示
												4:启动容量不足
												5:无法间接切换提示*/
				UINT16 bECOMode:		1;	/*ECO模式状态  0:normal  1:ECO模式*/
				UINT16 bReserved:		2;
			}tWord2Bits;
		}Word2_U;

	}_INV_SW1_T;


	//-------------机架内并机状态字定义---------------------------
	#define PARALLEL_STATE_MSG_OBJ_SIZE 1

	// 机架内并机台数
	#define		ParaNumCNST				6			//并机台数
	
	//单机信息标志
	typedef struct
	{
		union
		{
			/*机架内并机状态字1*/
			UINT16 u16Word1All;		
			struct
			{
				UINT16 bInvOn:				1;	/*bit0:	1-逆变器开启 0-逆变器关闭*/
				UINT16 bInvPowerOn:			1;	/*bit1:	1-逆变器供电 0-逆变器没有供电*/
				UINT16 bBattCapAlarm:		1;	/*bit2:	1-单机电池容量告警*/
				UINT16 bBattInvPower:		1;	/*bit3:	1-单机电池逆变供电*/
				UINT16 bOwnSevereFault:		1;	/*bit4:	1-单机严重故障*/
				UINT16 bOwnGenericFault:	1;	/*bit5:	1-单机一般故障*/
				UINT16 bOwnSevereAlarm:	1;	/*bit6:	1-单机严重告警*/
				UINT16 bOwnGenericAlarm:	1;	/*bit7:	1-单机一般告警*/

				UINT16 bBypPowerOn:		1;	/*bit8:	1-旁路供电 0-旁路没有供电*/
				UINT16 bSingleOnLine:		1;	/*bit9:	1-单机在线标志 */
				UINT16 bBypState:			2;	/*bit10,11: 	00正常01超跟踪11超保护范围*/
				UINT16 bBypRange:		1;	/*bit12:旁路幅值范围标志 */
														
				UINT16 bReserved:			3;	/*bit13~bit15: unused*/
				
			}tWord1Bits;
		}Word1_U;

		//可扩TBD	To Be Determined
		
	}_PARALLEL_SW1_T;

	#define PARALLEL_SW1_LENGTH	 (sizeof(_PARALLEL_SW1_T) * 2)

//增加旁路传给逆变用的状态量,只有逆变接收,属于新增加的所以未
//放到原来的状态量中
//-------------机架内并机状态字定义---------------------------
	#define BYPMAINT_STATE_MSG_OBJ_SIZE 1

	
	//旁路柜信息标志
	typedef struct
	{
		union
		{
			/*机架内旁路柜状态字1*/
			UINT16 u16Word1All;		
			struct
			{
				UINT16 bQ3Bp:				1;	/*bit0:	1-内部维修空开闭合 0-内部维修空开断开*/
				UINT16 bExyQ3Bp:			1;	/*bit1:	1-旁路柜维修空开闭合 0-旁路柜维修空开断开*/
				UINT16 bQ5			:		1;	/*bit2:	1-旁路柜输出信号*/
										
				UINT16 bReserved:			12;	/*bit3~bit15: unused*/
				UINT16 bReservedFlag:			1;
				
			}tWord1Bits;
		}Word1_U;

		//可扩TBD
		
	}_BYPMAINT_SW1_T;

	#define BYPMAINT_SW1_LENGTH	 (sizeof(_BYPMAINT_SW1_T) * 2)

//以下是由旁路上传过来的机架机架并机状态传给监控/逆变的数据
//注意最多4并机机架
//主要预留用
typedef struct
{
	union
	{
		UINT16 u16Word1All;
		struct
		{
			//并机系统逆变开机总台数
			UINT16 bRackInvOnNum:		5;
			//并机系统逆变供电总台数
			UINT16 bRackInvSupplyNum:	5;
			//并机系统电池供电台数
			UINT16 bRackBattSupplyNum:	2;
			//并机系统旁路供电台数
			UINT16 bRackBypSupplyNum:	2;
			
			UINT16 bReserved:	2;
			
		}tWord1Bits;
		
	}Word1_U;

	union
	{
		UINT16 u16Word2All;
		struct
		{
			//并机系统电池容量告警总机架数
			UINT16 bRackNumBattWarn:		2;
			//并机系统电池逆变供电总机架数
			UINT16 bRackNumBattInv:	2;
			//并机系统电池预告警台数---由监控通过实时信息下发
			UINT16 bRackBattPreWarnNum:	2;
			//并机系统告警故障综合标志
			UINT16 bRackWarnFlag:	4;
										//bit0:	1-并机系统有严重故障
										//bit1:	1-并机系统有一般故障
										//bit2:	1-并机系统有严重告警
										//bit3:	1-并机系统有一般告警
			//并机系统UPS上电标志，单机架或从机上传0xFF
			UINT16	bRackRunFlag:	2;				
										//bit0:	Reserved
										//bit1:	1-1#机开机
										//bit2:	1-2#机开机
										//bit3:	1-3#机开机
			UINT16 bReserved:	4;
										
			
		}tWord2Bits;
		
	}Word2_U;

	//可扩展
	
}_RACK_RECV_STATE_T;

#define RACK_SW_LENGTH	 (sizeof(_RACK_RECV_STATE_T) * 2)
//----------------------------------

//监控综合状态信息,用于定时下发给DSP
typedef struct
{
	union
	{
		//监控综合状态字
		UINT16 u16Word1All;	
		struct
		{
			UINT16  bBcbState	:1;				/*BCB脱扣状态*/
			UINT16  bBypOverProtectState	:1;		/*旁路超保护状态*/
			UINT16  bBattSupplyState:1;			/*=1本机架电池供电状态*/
			UINT16  bBattPreWarnFlag:1;			/*=1电池预告警标志*/
			UINT16  bBatSeftTest    :1;     /*=1电池自检*/
								
			UINT16	bReserved		:11;			//BIT01-15
		}tWord1Bits;
	}Word1_U;
	
}_MON_REAL_MSG_T;

	//------------------------------------------------------------------------


/**********************************************************************************
名称:模块上传告警量类型定义
类别: 1 整流, 2 逆变, 3旁路
**********************************************************************************/	

//-----------------------------------------------------------------------------------	
//逆变上传告警量数据结构类型定义

	typedef struct
	{
		union
		{
			/*逆变侧告警量状态字1*/
			UINT16 u16Word1All;				
			struct
			{
				UINT16 bInvSync:			1;	/*0逆变器同步,1不同步*/ 
				UINT16 bInvFault:			1;	/*逆变器故障*/  
				UINT16 bInvOverTemp:		1;	/*逆变器过温*/ 
				UINT16 bSigOverLoad:		1;	/*单机输出过载*/  
				UINT16 bSigOverLoadTimeOut:	1;	/*单机过载超时*/  
				UINT16 bOutOverVlt:			1;	/*交流输出过压故障*/   
				UINT16 bOutVltFualt:		1;	/*输出电压异常: NXb+用*/
				UINT16 bDcCurrOffsetOver:	1;	/*输出直流分量过大*/
				UINT16 bInvOverCurr:		1;	/*逆变器过流*/ 
				UINT16 bBusAbnormalOff:		1;	/*母线异常关机*/ 
				UINT16 bBusOverVlt:			1;	/*直流母线过压故障*/ 
				UINT16 bInvInductOverTemp:	1;	/*逆变电感过温*/   
				UINT16 bFanFault:			1;	/*风扇故障*/  
				UINT16 bInvRelay:			1;	/*逆变继电器故障*/
				UINT16 bOutFuseFault:		1;	/*输出熔丝断*/ 
				UINT16 bAuxPwr2Down:		1;	/*辅助电源2掉电*/  
			}tWord1Bits;
		}Word1_U;

		union
		{
			/*逆变侧告警量状态字2*/
			UINT16 u16Word2All;				
			struct
			{
				UINT16 bUsrOpFault				:1;	/*用户操作错误*/  
				UINT16 ucBypAbnor				:1;	/*旁路异常*/
				UINT16 ucOutBkPowerFault		:1;	/*输出接触器电源故障*/	
				UINT16 ucInvIGBT1OverCurr		:1;	/*逆变IGBT1过流*/  
				UINT16 ucInvIGBT2OverCurr		:1;	/*逆变IGBT2过流*/																																																																																												
				UINT16 ucInvIGBT3OverCurr		:1;	/*逆变IGBT3过流*/ 
				UINT16 ucAverCurInductOverTemp	:1;	//均流电感过温
				UINT16 ucBypLineFault			:1;	//旁路线缆故障
				UINT16 ucBypFanPowerFault		:1;	//旁路风扇电源故障
				UINT16 ucBypOverTemp			:1;	//旁路过温
				UINT16 bLBSstatus				:2;	/*LBS状态0:未激活 1:激活2:异常*/
				UINT16 ucInvBrk 				:1;	/*逆变接触器辅助触点故障*/ 
				UINT16 bOutShortFault			:1;	//输出短路
				UINT16 bReserved				:2;	
			}tWord2Bits;
		}Word2_U;


		union
		{
			/*逆变侧告警量状态字3*/
			UINT16 u16Word3All;				
			struct
			{
				//UINT16 bBypScrFault:		1;	/*旁路晶闸管故障*/      
				UINT16 bBypState:			2;	/*旁路情况(电压或频率)0:normal 1:超保护2:超跟踪*/ 
				UINT16 bBypPhaseReverse:	1;	/*旁路相序反*/   	
				UINT16 bToBypByLoadPuls: 	1;	/*负载冲击转旁路*/ 
				UINT16 bBypSwTimesLtd:		1;	/*旁路切换次数限制*/ 
				UINT16 bBypOverCurr:		1;	/*旁路过流故障*/
				UINT16 bBypScrFault:		2;	/*旁路晶闸管故障0:normal 1:反灌2:开路3:短路*/  
				UINT16 bBypAbnorOff:		1;	/*旁路异常关机告警*/
							
				UINT16 bParaBoardFault:		1;	/*并机板故障*/  
				UINT16 bSysShareCurr:		1;	/*并机均流故障*/   
				UINT16 bParaLineFault:		1;	/*并机线连接故障*/  
				UINT16 bParaCom:			1;	/*并机通讯故障*/
				UINT16 bSysOverLoad:		1;	/*并机系统过载*/   

				UINT16 bToBypReq:			1;	/*邻module请求转旁路*/ 
				UINT16 bParaFault:			1;	//并机逻辑总故障
				
			}tWord3Bits;
		}Word3_U;
		
	}_INV_WARN1_T;

	
//---------------------------------------------------------------------------

/**********************************************************************************
名称:模块上传模拟量类型定义
类别: 1 整流, 2 逆变, 3旁路
**********************************************************************************/	
//--------------------------------------------------------------------------------
//模块上传模拟量类型定义

	// 3相相信息
	typedef struct
	{
		UINT16 u16Phase_a;	//A相信息
		UINT16 u16Phase_b;	//B相信息
		UINT16 u16Phase_c;	//C相信息
	}_PHASE_INFO_T;

	// 3相线信息
	typedef struct
	{
		UINT16 u16Line_ab;	//AB相信息
		UINT16 u16Line_bc;	//BC相信息
		UINT16 u16Line_ca;	//CA相信息
	}_LINE_INFO_T;

	//直流信息
	typedef struct
	{
		UINT16 u16Post;		//正极
		UINT16 u16Neg;		//负极	
	}_DC_INFO_T;

	//调试信息
	typedef struct
	{
		UINT16 u16Dbg1;		//调试信息1
		UINT16 u16Dbg2;		//调试信息2
		UINT16 u16Dbg3;		//调试信息3
	}_DBG_INFO_T;

	//单相信息
	typedef struct
	{
		UINT16 u16Self;		// 单相信息
	}_SINGLE_INFO_T;
	
//----------------------------------------------------------------------------


//-------------------------------------------------------------------------------
// 2逆变上报模拟量数据结构类型定义
	//注:旁路电压和频率由旁路上报即可,所以长度为12,不是14
	//20081211 将旁路电压和频率又改成各个模块逆变也上报,所以长度又改成14
	#define INV_ANALOG_MSG_OBJ_SIZE 14 //12
	
	typedef struct
	{
		_PHASE_INFO_T OutActPwr;			/*相输出有功功率 1024 -- 额定值*/
		_PHASE_INFO_T OutApparentPwr;		/*相输出视在功率 1024 -- 额定值*/
		_PHASE_INFO_T OutFactor;				/*相输出功率因数 X 100*/             

		_PHASE_INFO_T OutPhaseVlt;			/*输出相电压 X 10*/
		_PHASE_INFO_T OutPHaseCurr;			/*输出相电流 1024 -- 额定值*/
		_PHASE_INFO_T InvPhaseVlt;			/*逆变相电压 X 10*/

		_PHASE_INFO_T InvPhaseCurr;			/*逆变相电流 1024 -- 额定值*/                
		_PHASE_INFO_T InductCurr;			/*电感相电流1024 -- 额定值*/
		_PHASE_INFO_T BypPhaseVlt;			/*旁路相电压 X 10*/
		
		_PHASE_INFO_T OutLoadRate;			/*相输出负载百分比 X 10*/
		_PHASE_INFO_T OutPeakRate;			/*相输出峰值比 X 10*/  
				
		_SINGLE_INFO_T OutFreq;				/*输出频率 X 100*/     
		_SINGLE_INFO_T BypFreq;				/*旁路频率 X 100*/

		_DBG_INFO_T DbgVar;					/*逆变测试变量*/
			
	}_INV_ANALOG_T;

#define PHASE_INFO_LEN	(sizeof(_PHASE_INFO_T))
#define LINE_INFO_LEN (sizeof(_LINE_INFO_T))
#define DC_INFO_LEN (sizeof(_DC_INFO_T))
#define SINGLE_INFO_LEN (sizeof(_SINGLE_INFO_T))
#define DBG_INFO_LEN (sizeof(_DBG_INFO_T))



//-----------------------------------------------------------------------------
// 3旁路上报模拟量结构体定义
	//电池温度信息
	typedef struct
	{
		UINT16 u16BattInTemp;
		UINT16 u16BattOutTemp;
	}_BATT_TEMP_INFO_T;

	#define BYP_ANALOG_MSG_OBJ_SIZE  12	//((sizeof(BypCanAnlogMsgObjs))/(sizeof(ANALOG_MSG_OBJ_T)))	//
	
	typedef struct
	{
		_PHASE_INFO_T	BypPhaseVlt;		/*旁路相电压 X 10*/
		_PHASE_INFO_T	BypOutPhaseVlt;		/*输出相电压 X 10*/
		_PHASE_INFO_T	BypPhaseCurr;		/*旁路相电流 1024 -- 额定值*/
		_PHASE_INFO_T	OutFactor;			/*功率因素*/
		_SINGLE_INFO_T   BypFreq;			/*旁路频率 X 100*/
		_SINGLE_INFO_T   BypOutFreq;			/*输出频率 X 100*/
		_PHASE_INFO_T 	OutActPwr;			/*相输出有功功率 1024 -- 额定值*/
		_PHASE_INFO_T	OutApparentPwr;		/*相输出视在功率1024*/
		
		_PHASE_INFO_T	OutLoadRate;		/*输出负载率*/
		_PHASE_INFO_T	OutPeakRate;		/*输出峰值比*/

		//增加旁路上传的机架系统有功功率和视在功率1024额定
		_PHASE_INFO_T 	RackOutActPwr;			/*RACK机架系统相输出有功功率 1024 -- 额定值*/
		_PHASE_INFO_T	RackOutApparentPwr;	/*RACK机架系统相输出视在功率1024*/
		
//		_BATT_TEMP_INFO_T	BatteryInTemp;		/*电池温度*/ 
//		_SINGLE_INFO_T	EnvirTemp;			/*环境温度*/
//		_DBG_INFO_T		BypDebugVari;		/*旁路调试变量*/		//TBD, 温度
	}_BYP_ANALOG_T;

/**********************************************************************************
名称:模块接收模拟量类型定义
类别: 1 整流, 2 逆变, 3旁路
**********************************************************************************/	
	typedef struct
	{
		UINT16	u16PostVlt;	/*正电池组浮充电压*/
		UINT16	u16NegVlt;	/*负电池组浮充电压*/
	}BATT_FLT_CHARG_VLT_T;

//---------------------------------------------------------------------------------
// 1整流
	#define REC_ANALOG_RECV_MSG_SIZE 2

	typedef struct
	{
		BATT_FLT_CHARG_VLT_T FltVlt;
		UINT16	u16LocalUnitPower;	/*模块功率*/
	}_REC_RECV_ANALOG_T;

//--------------------------------------------------------------------------------
// 2 逆变
	#define INV_ANALOG_RECV_MSG_SIZE 5
	typedef struct
	{
		_PHASE_INFO_T   	OutActPwr;	/*系统输出有功功率*/
		_PHASE_INFO_T   	OutApparentPwr;	/*系统输出视在功率*/
		UINT16	u16EnvirTemp;	/*环境温度*/

		//并机机架系统总有功功率
		_PHASE_INFO_T   	RackOutActPwr;
		//并机机架系统总视在功率
		_PHASE_INFO_T   	RackOutApparentPwr;
		
	}_INV_RECV_ANALOG_T;


//-------------------------------------------------------------------------------
// 3 旁路
	#define BYP_ANALOG_RECV_MSG_SIZE 
	typedef struct
	{

	}_BYP_RECV_ANALOG_T;
	
/**********************************************************************************
名称:设置信息相关
类别: 1 Dsp 综合设置参数 2 整流,3 逆变, 4旁路, 5 时钟
**********************************************************************************/	
//#define REC_CFG_FROM_MON_OBJ_SIZE 10 //tbd
//#define REC_CFG_TO_MON_OBJ_SIZE 10//tbd

#define INV_CFG_FROM_MON_OBJ_SIZE  46	//41+并机冗余机架数	//37 //tbd(sizeof(CfgFromMonObjs) / sizeof(CFG_MSG_T))
#define INV_SYS_CFG_FROM_MON_OBJ_SIZE 25	//22
#define INV_CFG_TO_MON_OBJ_SIZE 5//tbd


//---------------------------------------------------------------------------------
// 1 Dsp综合设置参数定义,//TBD，是否将REC/INV/BYP各自相关参数分开?
	typedef struct
	{
		UINT16 u16SetParamFresh;		//参数是否更新FALSE: 没有根新; TRUE:更新
		
		struct
		{
			//UINT16 u16UpsType;			//Nxa,b,e,a+,b+,f,rm
			UINT16 u16CapType;			/*机型容量类型: 10,15,20,30KVA*/ 
			UINT16 u16SysVltGrade;		/*系统电压等级设置 200V,208V,380V,400V,415V,480*/ 
			UINT16 u16RunMod;			/*工作模式设置 1:NOR 2:ECO*/
			//UINT16 u16AutoOutEna;		/*自动恢复输出允许设置 1:DIS 2:ENA*/
			//UINT16 u16AutoOutDelay;		/*自动恢复输出延时设置 (10) 1-1440*/       
			UINT16 u16InFreq;			/*输入频率等级设置 50HZ,60HZ */  
			UINT16 u16OutFreq;			/*输出频率等级设置 50HZ,60HZ */ 
			//UINT16 u16InTransf;			/*是否有输入变压器	1:没有  2:有 缺省1*/
			//UINT16 u16OutTransf;			/*是否有输出变压器	1:没有  2:有 缺省1*/
			//UINT16 u16InTransfRate;		/*输入变压器变比1~10000,缺省100 */
			//UINT16 u16OutTransfRate;	/*输出变压器变比1~10000,缺省100*/

			UINT16 u16GenMod;			/*智能发电机模式设置 1:ENA  2:DIS*/
			UINT16 u16GenPwrLtd;		/*智能发电机限功率设置 (0) 0-240*/
			UINT16 u16GenStartDelay;	/*智能发电机启机延迟时间设置 (10) 5-30*/
			//UINT16 u16ChargerType;		/*充电器在发电机模式以及电池房告警时的充
			//								电功率设置0: 0(0)  1:20%  2:100% 3:Reserved*/
												
			UINT16 u16AgingOutCurrRate;	/*自老化输出电流百分比设置 0~130(25)*/    
			UINT16 u16OutSwitchTime;	/*输出间断切换时间设置ms 0:NONE 1:40 2:60 3:80 4:100*/      
			UINT16 u16OutFreqTrackRate;	/*输出频率跟踪速率设置 1~30---0.1~3Hz/s (1)*/ 
			UINT16 u16InvRunMode;		/*逆变器工作模式设置0:正常 1:自老化 2:调测*/
			UINT16 u16InvOutFactor;		/*逆变器输出功率因数0:0.8; 1: 1*/
			UINT16 u16OutVltMicroAdj;	/*输出电压微调系数设置 (230) 200~250*///SYS
			
			UINT16 u16BypSwitchTime;	/*旁路切换次数设置 (5) 1-10*/ 
			UINT16 u16BypFreqTrackLtd;	/*旁路频率跟踪范围设置 1:2HZ 2:1HZ 3:3HZ 4:0.5HZ*/
			UINT16 u16BypVltUpLtd;		/*旁路电压保护范围设置(上限) 1:15% 2:10% 3:20%*/    
			UINT16 u16BypVltDownLtd;	/*旁路电压保护范围设置(下限) 1:20% 2:10% 3:30% 4:40%*/
			UINT16 u16BypFreqLtd;		/*旁路频率保护范围设置 1:10% 2:20%* 3:5% 4: 2.5% */  
			UINT16 u16BypProtectBrk;	/*旁路反灌保护用短路器 0:NONE 1:HAVE*/  

			UINT16 u16RMIdx;			/*机架号*/
			UINT16 u16RMNum;			/*并机机架数量*/
			UINT16 u16ReRMNum;			/*并机冗余机架数*/
			UINT16 u16BasicModNum;		/*本机基本模块数*/
			UINT16 u16ReModNum;		/*本机冗余模块数*/
			UINT16 u16LBSMode;			/*LBS mode 0:none 1: master 2: slaver*/

			UINT16	u16BattGrpIdx;		/*电池组号 1：1号电池组； 2：2号电池组*/	
			UINT16	u16Cap;				/*电池容量设置7~3000AH*/
			UINT16	u16EqChgVlt;		/*电池均充电压 */	
			UINT16	u16ChgCurrLtd;		/*电池充电限流值 X 100 */
			UINT16	u16SelfChkDischgPowLtd;			/*电池自检放电功率限制*/
			UINT16	u16UpsConfigType;			/*单并机方式*/
			UINT16   u16BatShare;					/*1:share module ,0: no share.*/
			UINT16	u16OutputPhase;	/*0:三单1:三三*/
		}SetParam;
	}_SYS_DSP_SET_INFO_T;	

	//-----------------------------------------------------------------------------------
	

	//-----------------------------------------------------------------------------------
	// 3逆变侧校正参数定义
	typedef struct
	{
		UINT16 u16SetParamFresh;		//参数是否更新FALSE: 没有根新; TRUE:更新

		//u16SetParamFresh1--扩充用
		UINT16 u16SetParamFresh1;
		UINT16 u16SetParamFresh2;
		struct
		{
			UINT16 u16OutVltRevCoef_a;			/*A相输出电压校正系数 (4096)*/
			UINT16 u16OutVltRevCoef_b;			/*B相输出电压校正系数 (4096)*/
			UINT16 u16OutVltRevCoef_c;			/*C相输出电压校正系数 (4096)*/
			UINT16 u16OutCurrRevCoef_a;			/*A相输出电流校正系数 (4096)*/
			UINT16 u16OutCurrRevCoef_b;			/*B相输出电流校正系数 (4096)*/
			UINT16 u16OutCurrRevCoef_c;			/*C相输出电流校正系数 (4096)*/
			
			UINT16 u16InvVltRevCoef_a;			/*A相逆变电压校正系数设置 (4096)*/ 
			UINT16 u16InvVltRevCoef_b;			/*B相逆变电压校正系数设置 (4096)*/ 
			UINT16 u16InvVltRevCoef_c;			/*C相逆变电压校正系数设置 (4096)*/ 
			UINT16 u16InvCurrRevCoef_a;			/*A相逆变电流校正系数设置 (4096)*/
			UINT16 u16InvCurrRevCoef_b;			/*B相逆变电流校正系数设置 (4096)*/
			UINT16 u16InvCurrRevCoef_c;			/*C相逆变电流校正系数设置 (4096)*/
			
			UINT16 u16BypVltRevCoef_a;			/*A相旁路电压校正系数设置 (4096)*/
			UINT16 u16BypVltRevCoef_b;			/*B相旁路电压校正系数设置 (4096)*/
			UINT16 u16BypVltRevCoef_c;			/*C相旁路电压校正系数设置 (4096)*/

			UINT16 u16DebugVar1Addr;			/*调试变量地址1*/
			UINT16 u16DebugVar2Addr;			/*调试变量地址2*/
			UINT16 u16DebugVar3Addr;			/*调试变量地址3*/

			UINT16 u16DegVar1;		/*调试参数1*/
			UINT16 u16DegVar2;		/*调试参数2*/
			UINT16 u16DegVar3;		/*调试参数3*/
		}SetParam;
	}_INV_SET_INFO_T;

	#define INV_SET_INFO_SIZE  (sizeof(_INV_SET_INFO_T) / sizeof(UINT16) * 2 -2)

	
	//------------------------------------------------------------------------------------
	// 5 时钟设置
	typedef struct
	{
//		UINT32 u32Senconds;	//Unix 纪元始秒数
		UINT16 u16TimeS_H;
		UINT16 u16TimeS_L;
		UINT16 u16TimeMS;//ms
		//=1表示已同步过,=0未同步过
		UINT16 u16HaveSyncClkFlag;
	}_RT_CLK_T;


/**********************************************************************************
名称:模块基本信息
类别: 
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

		//序列号
		//TBD
	}_MODULE_BASE_INFO_T;


/**********************************************************************************
名称:控制信息
类别: 所用命令位段为1表示有新的命令；0表示没有新的命令
**********************************************************************************/	
	typedef struct
	{
		//电池命令
		UINT16 u16PostBattFltChgCmd	: 	1;//正组电池转浮充
		UINT16 u16NegBattFltChgCmd:		1;//负组电池转浮充
		UINT16 u16PostBattEqualChgCmd:	1;//正组电池转均充
		UINT16 u16NegBattEqualChgCmd:	1;//负组电池转均充
		UINT16 u16SelfChk20PercentCmd:	1;//20%自检
		UINT16 u16SelfChkDeepCmd:		1;//深度自检
		UINT16 u16SelfChkEndCmd:		1;//自检结束
		UINT16 u16EodCmd:				1;//EOD
		UINT16 u16BattTripCmd:			1;//电池trip
		UINT16 u16OutputEnableCmd:		1;//输出允许
		UINT16 u16OutputDisableCmd:		1;//输出禁止
		UINT16 u16BattGndPtcCmd:		1;//接地保护
		UINT16 u16Reserved				:4;

		//系统命令	
		UINT16 u16FaultClrCmd:			1;//故障清除
		UINT16 u16ManualInvOnCmd:		1;//手动开机
		UINT16 u16ManulaInvOffCmd:		1;//手动关机
		UINT16 u16EpoCmd:				1;//EPO
		UINT16 u16AbnrmlShutConfirmCmd:	1;//异常关机确认
		UINT16 u16AbnrmllShutCancelCmd:	1;//异常关机取消
		UINT16 u16IndirectSwitchCmd:	1;//间接切换
		UINT16 u16RecFwUpdateCmd:		1;//整流固件升级
		UINT16 u16InvFwUpdateCmd:		1;//逆变固件升级
		UINT16 u16BypFwUpdateCmd:		1;//旁路固件升级
		UINT16 u16Reserved1				:6;
				
	}_CTRL_INFO_T;

	enum SYS_CAN_CTRL_CMD_LIST
	{
		POST_BATT_FLT_CHARG_CMD = 0x01,	//正组电池转浮充
		NEG_BATT_FLT_CHARG_CMD,			//负组电池转浮充
		POST_BATT_EQUAL_CHARG_CMD,		//正组电池转均充
		NEG_BATT_EQUAL_CHARG_CMD,		//负组电池转均充
		SELF_CHK_20PERCENT_CMD,		//20%自检
		SELF_CHK_DEEP_CMD,			//深度自检
		SELF_CHK_END_CMD,				//自检结束
		EOD_CMD,						//EOD
		BATT_TRIP_CMD,					//电池trip
		OUTPUT_ENABLE_CMD,			//输出允许
		OUTPUT_DISENABLE_CMD,		//输出禁止
		BATT_GND_PTC_CMD,				//接地保护

		FAULT_CLEAR_CMD = 0x10,		//故障清除
		MANUAL_INV_ON_CMD,			//手动开机
		MANUAL_INV_OFF_CMD,			//手动关机
		EPO_CMD,						//EPO
		ABNORMAL_SHUT_CONFIRM_CMD,		//异常关机确认
		ABNORMAL_SHUT_CANCEL_CMD,		//异常关机取消
		INDIRECT_SWITCH_CMD,			//间接切换
		REC_FW_UPDATE_CMD,			//整流固件升级
		INV_FW_UPDATE_CMD,			//逆变固件升级
		BYP_FW_UPDATE_CMD			//旁路固件升级

	};



//旁路上传状态量数据结构类型定义
	typedef struct
	{
		union
		{
			//旁路侧状态字
			UINT16 u16Word1All;	
			struct
			{
				UINT16  bPwrCANState	:1;				/*Power CAN通讯状态*/
				UINT16  bEpoPrompt		:1;				/*紧急关机提示0:无提示1:有提示*/
//				UINT16	bBCBConnect		:1;				//BIT002	BCB接入状态0：未接入 1：接入

				UINT16	bBCBConnect		:2;	
				//20081107 增加上传供电状态2bit
				//=2,监控加工供电状态由逆变/整流决定
				//=00 均不供电,=01旁路供电=02输出禁止
				UINT16	bBpSupplyState :2;				//BIT03-04 供电状态旁路上报供电状态
//				UINT16	bRecordTxRequest :1;			//BIT05 故障上传请求

				UINT16	bOutBreak		:1;				/*输出开关状态0:断开1:闭合*/
				UINT16	bBypMaintBreak	:1;				/*旁路维修空开状态 0:断开1:闭合*/
				UINT16	bAutoBypBreak	:1;				/*自动旁路空开状态0:断开1:闭合*/
				UINT16	bInsideBattBreak:1;
				UINT16	bBypOnlinestate	:1;		//旁路在线标志
				UINT16	bReserved		:5;		//BIT09-15
			}tWord1Bits;
		}Word1_U;
		
	}_BYP_SW1_T;

	// 旁路上传告警信息数据结构定义//TBD

	typedef struct
	{
		union
		{
			/*旁路告警量状态字1*/
			UINT16 u16Word1All;				
			struct
			{
				UINT16 bBypOverCurr				:1;			/*旁路过流故障*/
				UINT16 bBypFeedBackFault		:2;			/*旁路晶闸管故障0:normal 1:反灌*/ 
				UINT16 bFanFault				:1;			//风扇故障
				UINT16 bBatEarthFault			:1;			//电池接地故障
				UINT16 bBatRoomEnvirAbnormal	:1;			//电池房环境异常
				UINT16 bModOvTemp				:1;			//旁路模块过温
				UINT16 bAuxPwr1Down				:1;			//辅助电源1掉电
				UINT16 bAuxPwr2Down				:1;			//辅助电源2掉电	
			//	UINT16 bPowOK					:1;			//Power ok故障	
				UINT16 bBypState				:2;			/*旁路情况(电压或频率)0:normal 1:超保护2:超跟踪*/ 
				UINT16 bBypPhaseReverse			:1;			/*旁路相序反*/   		
				//INV TBD
//				UINT16 bLbsActive:			2;	/*LBS激活状态0: noactive 1: active 2: abnormal*/
//				UINT16 bToBypReq:			1;	/*邻机请求转旁路*/ 
				UINT16 bOvload					:1;				//输出过载
				UINT16 bParallelCommErr		:1;		/*机架间并机通讯故障*/
				UINT16 bReqBypScrFault			:1;	/*邻机晶闸管故障*/
				UINT16 bTrasOverTemp			:1;	//外部输入变压器过温
			}tWord1Bits;
		}Word1_U;

	}_BYP_WARN1_T;


//-----------------------------------------
//整流侧零散信号状态结构定义
//目前只有主路反灌信号,以后可以根据需求在此扩充
//	#define REC_DISCRETE_MSG_OBJ_SIZE 1
	typedef struct
	{
		union
		{
			/*整流零散信号状态字1*/
			UINT16 u16Word1All;			
			struct
			{
				UINT16 bInBrk:			1;	/*输入反灌信号0:无效; 1:有效*/
				UINT16 bChargerWorkP:	1;	/*BIT2	１：充电器正调节器工作*/
				UINT16 bChargerWorkN:	1;	/*BIT3	１：负电器正调节器工作*/
				UINT16 bBoostState: 	2;  /*Boost状态*/
				UINT16 bReserved:		11;
			}tWord1Bits;
		}Word1_U;

		
	}_REC_DISCRETE_SW1_T;

#endif

