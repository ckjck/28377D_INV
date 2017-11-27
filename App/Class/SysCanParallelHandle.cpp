/***************************************************************************************
* 
* Filename: 		SysCanParallelHandle.cpp
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

#include "SysCanParallelHandle.h"



/* ***********************************************************
文件名称：	void Class_CAN::vCAN_Var_Init( )
功能描述：	CAN非零变量初始化，在默认情况下都认为是从机
算法说明：	none

入口参数：	none
出口参数：	sCanFlag
			ECanaRegs
			
局部变量：	i
				tempECanReg
函数调用：	none
*********************************************************** */

void SysCanParallelHandle::vCAN_Var_Init( )
{
	UINT16 i;
	struct	ECAN_REGS	tempECanReg;
	
	S2U ( sCanFlag ) = 0;
	iCanAloneConfirmCnt = 0;    
	//iCANTxPrdStatus = 1;
	sCanFlag.bCANTxPrdStatus = 1;
	iFrameIDLow = 1;  
	iFrameIDHigh = 1;
	sCanFlag.bAlone = 0; 
	sCanFlag.bUPSSingleOline = 0;		
	sCanFlag.bErr = 0;
	sCanFlag.bVersion = 1;
	sCanFlag.bReceived = 0;
	uMACIDFlag.iMACIDFlagWord = 0;
	sCanFlag.bCommErrJudgePeriod = 0; 
	sCanFlag.bVersionErr = 0;
	iVersionTxFlagCounter = 0;
		
	//清除并机表数据
	for (i=0; i<=ParaNumCNST-1; i++)
	{
		sParaData[i].m_i16Pouta = 0;
		sParaData[i].m_i16Poutb = 0;
		sParaData[i].m_i16Poutc = 0;
		sParaData[i].m_i16Souta = 0;
		sParaData[i].m_i16Soutb = 0;
		sParaData[i].m_i16Soutc = 0;
		sParaData[i].m_i16ModelKVA = 0;
		sParaData[i].m_i16wGrantKVA = 0;
		sParaData[i].uSystemFlag = 0;
		sMACIDErrCounter[i] = 0;//20061008
	}	

	//enable receive mailbox0~5
	ECanaRegs.CANME.all = 0;

	tempECanReg.CANME.all = ECanaRegs.CANME.all;
	tempECanReg.CANME.bit.ME0 = 1;
	tempECanReg.CANME.bit.ME1 = 1;
	tempECanReg.CANME.bit.ME2 = 1;
	tempECanReg.CANME.bit.ME3 = 1;
	tempECanReg.CANME.bit.ME4 = 1;
	tempECanReg.CANME.bit.ME5 = 1;
	ECanaRegs.CANME.all = tempECanReg.CANME.all;
	
	iLocalStatusChanged = 0; 						

	iPoutTotala = 0;
	iPoutTotalb = 0;
	iPoutTotalc = 0;
	iSoutTotala = 0;
	iSoutTotalb = 0;
	iSoutTotalc = 0;
	
//zqf 060615--降额机型不同容量并联
	iKCapAdj=4096;
}



/* ***********************************************************
文件名称：	void Class_CAN::vCAN_Agent( )
功能描述：	CAN通信主程序，在此进本机接收、发送、故障判断等子程序的调度
算法说明：	none

入口参数：	iStartTimeCount
			objSystem.m_st_wFlagSystem
			objMonInterface.m_st_wUPSModeRun
			ECanaRegs
			K180sBased40msCnst
			ParaNumCNST
			uCanDelay
			objTimerDrv.u16ClkUs_0
			iFrameIDLow
			m_i16Pouta
			m_i16Poutb
			m_i16Poutc
			m_i16Souta
			m_i16Soutb
			m_i16Soutc
			K42msCnst
			
			
出口参数：	iStartTimeCount
			sCanFlag
			ECanaRegs
			sParaData
			uFrameNo
			uCanDelay
			iParaRunFlag
			iSoutTotala
			iSoutTotalb
			iSoutTotalc
			iPoutTotala
			iPoutTotalb
			iPoutTotalc
			iPoutTotalMax
			iSoutTotalMax
			iPSoutTotalMax
			iNumInv
			iNumOn
			iVersionTxFlagCounter
			iCommErrJudgePeriod
			
			
局部变量：	
			i
			
函数调用：
			vCANLocalData( )
			vCAN_Rx()
			vCANCalculation( )
			vCANVersion_Tx()
			vCANHigh_Tx()
			vCANLow_Tx( )
			vCAN_Com_Err()
			
*********************************************************** */
	
void SysCanParallelHandle::vCAN_Agent( )
{
	UINT16	i;
//	struct	ECAN_REGS	tempECanReg;

	//cb 030808  CAN 电池预告警及电池供电台数容错
	if(objXfer.m_st_XferResult.bInvKM==0)
	{
//				objMonitor.uInvRealInfo.bBattWarn=0;
//				objMonitor.uInvRealInfo.bBattInv=0;
		
	}			
	//cb_H 030414 5分钟延时缩短为3分钟
	if ( ( iStartTimeCount < K180sBased40msCnst ) && ( objSystem.m_st_wFlagSystem.bTimer40ms == 1 ) )
	//cb_E 030414
	{
		iStartTimeCount ++;		//从开机开始计时，计时3分钟后停止计时
	}
	
	if ( ECanaRegs.CANES.bit.BO != 0 ) 
	{
		EALLOW;
		ECanaRegs.CANMC.bit.ABO = 1;
		EDIS;
	}
/*
	//20060419 并机系统测试模式下，可开启逆变；
	if ( objSystem.m_st_wFlagSystem.bUPSTestMode == 1 )//调测模式?2
	{
		S2U( sCanFlag ) = 0;
		
		if ( objSwitches.m_st_FlagBreaker.bInv == 0 )
		{
			iNumInv = 0;
		}
		else
		{
			iNumInv = 1;
		}

		if ( objSwitches.m_st_FlagBreaker.bOn == 0 )
		{
			iNumOn = 0;
		}
		else
		{
			iNumOn = 1;
		}

		if ( objSwitches.m_st_FlagBreaker.bQ5 == 1 )
		{
			iPoutTotala = m_i16Pouta;
			iPoutTotalb = m_i16Poutb;
			iPoutTotalc = m_i16Poutc;

			iSoutTotala = m_i16Souta;
			iSoutTotalb = m_i16Soutb;
			iSoutTotalc = m_i16Soutc;
		}
		else
		{
			iPoutTotala = 0;
			iPoutTotalb = 0;
			iPoutTotalc = 0;

			iSoutTotala = 0;
			iSoutTotalb = 0;
			iSoutTotalc = 0;
		}

		iCapacitySum_InvOn = m_i16wGrantKVA;
	
		return;
	}
	
	//20060419 end
*/

	
	//zqf 061225--增加旁路供电台数维护
	//单机或 并机信号隔离态 或UPSTestMode
	if ( objMonInterface.m_st_wUPSModeRun.bConfig != 3 || objParallel.m_st_wParaMaint.bParaSignalMask == 1 || objSystem.m_st_wFlagSystem.bUPSTestMode == 1 )
	{
		S2U( sCanFlag ) = 0;
		
		if ( objSwitches.m_st_FlagBreaker.bInv == 0 )
		{
			iNumInv = 0;
			iCapacitySum_InvOn = 0;
		}
		else
		{
			iNumInv = 1;
			iCapacitySum_InvOn = objMonInterface.m_i16GrantKVA;					//保证UPSTestMode下逆变可开机
		}

		if ( objSwitches.m_st_FlagBreaker.bOn == 0 )
		{
			iNumOn = 0;
		}
		else
		{
			iNumOn = 1;
		}

		//更新旁路供电状态，解决由并机状态切换到并机隔离态时输出灯状态不能刷新问题(输出灯仍点亮)
		if ( objSwitches.m_st_FlagBreaker.bBpOn == 1 )						//旁路供电
		{
			iNumBp = 1;
		}
		else
		{
			iNumBp = 0;
		}

		if ( objSwitches.m_st_FlagBreaker.bQ5 == 1 )
		{
			iPoutTotala = objOutput.m_i16Pouta;
			iPoutTotalb = objOutput.m_i16Poutb;
			iPoutTotalc = objOutput.m_i16Poutc;

			iSoutTotala = objOutput.m_i16Souta;
			iSoutTotalb = objOutput.m_i16Soutb;
			iSoutTotalc = objOutput.m_i16Soutc;
		}
		else
		{
			iPoutTotala = 0;
			iPoutTotalb = 0;
			iPoutTotalc = 0;

			iSoutTotala = 0;
			iSoutTotalb = 0;
			iSoutTotalc = 0;
		}

		return;
	}
	
	/*  当并机系统里只有一台UPS开启时，如果本机处于发送状态，则本机的发送请求位将始终为1，
		且CAN总线会报出错，从而导致CAN通信无法正常进行。此时把本机的容量和功率及逆变开启
		状态作为并机系统的总容量和总功率。
	*/
	
	//从单机在线过渡到多机在线
	if ( ( sCanFlag.bAlone == 1 ) && ( sCanFlag.bUPSSingleOline == 0 ) )
	{
		sCanFlag.bAlone = 0;

		//清除错误标志20061020
		ECanaRegs.CANES.all = ECanaRegs.CANES.all;
						
		for(i=1;i<=ParaNumCNST-1;i++)		//清除原来并机表数据
		{
			sParaData[i].m_i16Pouta = 0;
			sParaData[i].m_i16Poutb = 0;
			sParaData[i].m_i16Poutc = 0;
			sParaData[i].m_i16Souta = 0;
			sParaData[i].m_i16Soutb = 0;
			sParaData[i].m_i16Soutc = 0;
			sParaData[i].uSystemFlag = 0;
		}
	
		//cb 030904 保留bErr标志，克服BE_ALONE 和BROADCAST之间切换时的故障漏报。
//wzh20051212
		//S2U ( sCanFlag ) &= CBIT3;
//wzh
		sCanFlag.bCANTxPrdStatus = 1;
		iFrameIDLow = 1;					//从1号帧开始
	}
	
	//读取本机数据
	vCANLocalData( );
	
	// 数据接收函数
	vCAN_Rx();

	//如果并机系统单机在线
	if ( sCanFlag.bUPSSingleOline == 1 )
	{
		//vCAN_Be_Alone( );
		vCANCalculation( );
		sCanFlag.bAlone = 1;
	}

	//如果接收到新数据
	if ( sCanFlag.bReceived == 1 )
	{
		vCANCalculation( );
		sCanFlag.bReceived = 0;
	}

	if ( ECanaRegs.CANTRS.bit.TRS10 == 0 )//如果发送邮箱10为空

	{
	
		//发送本机软件版本号
		if ( sCanFlag.bVersion == 1 )
		{
			vCANVersion_Tx();
			sCanFlag.bVersion = 0;
			iVersionTxFlagCounter = 0;
		}
	
		//如果本机工作状态改变，则发送高优先级信息帧
		else if (iLocalStatusChanged == 1) 
		{
			vCANHigh_Tx();
			iLocalStatusChanged = 0;
		}
	
		
		else  
		{
			//发送低优先级信息帧
			if ((iFrameIDLow <= 3) && (iFrameIDLow >= 1))
			{
				vCANLow_Tx( );
			}
		}
		
	}
	// 发送函数和通信故障判断
	if( sCanFlag.bCommErrJudgePeriod == 2 )
	{
		//iCANTxPrdStatus = 0;
		vCAN_Com_Err();//通信故障处理
		sCanFlag.bCommErrJudgePeriod = 0;
	}

				
	// 发送周期42ms定时
	if ( sCanFlag.bCANTxPrdStatus == 1 )
	{
		sCanFlag.bCANTxPrdStatus = 0;		//置开始延时标志
		uCanDelay = objTimerDrv.m_u16ClkUs_0;	//记录开始延时时刻
		return;
	}

	if ( ( ( ( objTimerDrv.m_u16ClkUs_0 + CLKUSPRDCNST ) < ( uCanDelay + K42msCnst ) ) && ( objTimerDrv.m_u16ClkUs_0 < uCanDelay ) )
		|| ( ( objTimerDrv.m_u16ClkUs_0 < ( uCanDelay + K42msCnst ) ) && ( objTimerDrv.m_u16ClkUs_0 > uCanDelay ) ) )
	{
		return;			//等待延时42ms还没有到，返回
	}
	
	sCanFlag.bCANTxPrdStatus = 1;    //延时42ms时间到,则置标志位
	iFrameIDLow = 1;
	sCanFlag.bCommErrJudgePeriod += 1;
	iVersionTxFlagCounter += 1;

	if ( iVersionTxFlagCounter == 120 )
	{
		sCanFlag.bVersion = 1;
	}
	
	
}



/*************************************************************
文件名称：	vCan_Rx( )
功能描述：	并机系统中，查询接收邮箱是否有新的数据。如有则调用函数
			vCANDataRemove(),将收到的新数据放到临时buffers中，并通过
			vCANDataStore()将数据进行分类后放到相应UPS存储单元中。
			
算法说明：	none

入口参数：	CANRxFlag
				ECanaRegs.CANRMP
				ECanaRegs.CANRML
				
出口参数：	
			
局部变量：	vMailBoxFlag
				vCANReceived
函数调用：vCANDataRemove( )
				vCANDataStore( )

*************************************************************/
void SysCanParallelHandle::vCAN_Rx( )
{
	char vMailBoxFlag;

	if ( ECanaRegs.CANRMP.all & CANRxFlag )
	{
		
		//查看邮箱5是否接收到数据
		if ( ECanaRegs.CANRMP.bit.RMP5 == 1 )
		{
			vMailBoxFlag = 5;
			vCANDataRemove( vMailBoxFlag );	//将数据移到临时buffer
			//ECanaRegs.CANRMP.bit.RMP5 = 1;	//释放邮箱5，可重新接收新的数据
			ECanaRegs.CANRMP.all = 0x00000020;//释放邮箱5，可重新接收新的数据20061008
			vCANDataStore( );	//将数据放到相应的buffers
		}
		
		//查看邮箱4是否接收到数据
		if ( ECanaRegs.CANRMP.bit.RMP4 == 1 )
		{
			vMailBoxFlag = 4;
			vCANDataRemove( vMailBoxFlag );	//将数据移到临时buffer
			//ECanaRegs.CANRMP.bit.RMP4 = 1;	//释放邮箱4，可重新接收新的数据
			ECanaRegs.CANRMP.all = 0x00000010;	//释放邮箱4，可重新接收新的数据20061008
			vCANDataStore( );	//将数据放到相应的buffers
		}
		
		//查看邮箱3是否接收到数据
		if ( ECanaRegs.CANRMP.bit.RMP3 == 1 )
		{
			vMailBoxFlag = 3;
			vCANDataRemove( vMailBoxFlag );	//将数据移到临时buffer
			//ECanaRegs.CANRMP.bit.RMP3 = 1;	//释放邮箱3，可重新接收新的数据
			ECanaRegs.CANRMP.all = 0x00000008;	//释放邮箱3，可重新接收新的数据20061008
			vCANDataStore( );	//将数据放到相应的buffers
		}
		
		//查看邮箱2是否接收到数据
		if ( ECanaRegs.CANRMP.bit.RMP2 == 1 )
		{
			vMailBoxFlag = 2;
			vCANDataRemove( vMailBoxFlag );	//将数据移到临时buffer
			//ECanaRegs.CANRMP.bit.RMP2= 1;	//释放邮箱2，可重新接收新的数据
			ECanaRegs.CANRMP.all = 0x00000004;	//释放邮箱2，可重新接收新的数据20061008
			vCANDataStore( );	//将数据放到相应的buffers
		}

		//查看邮箱1是否接收到数据
		if ( ECanaRegs.CANRMP.bit.RMP1 == 1 )
		{
			vMailBoxFlag = 1;
			vCANDataRemove( vMailBoxFlag );	//将数据移到临时buffer
			//ECanaRegs.CANRMP.bit.RMP1= 1;	//释放邮箱1，可重新接收新的数据
			ECanaRegs.CANRMP.all = 0x00000002;	//释放邮箱1，可重新接收新的数据20061008
			vCANDataStore( );	//将数据放到相应的buffers
		}

		//查看邮箱0是否接收到数据
		if ( ECanaRegs.CANRMP.bit.RMP0== 1 )
		{
			vMailBoxFlag = 0;
			vCANDataRemove( vMailBoxFlag );	//将数据移到临时buffer
			//ECanaRegs.CANRMP.bit.RMP0= 1;	//释放邮箱0，可重新接收新的数据
			ECanaRegs.CANRMP.all = 0x00000001;	//释放邮箱0，可重新接收新的数据20061008
			vCANDataStore( );	//将数据放到相应的buffers
		}
	}

	//清除错误标志
	ECanaRegs.CANRML.all |=0x0000003F;
	
}
/*************************************************************
文件名称：	vCanDataRemove( UINT16 i )
功能描述：	将收到的数据放到临时buffer
算法说明：	none

入口参数：	
				ECanaMboxes
		
出口参数：	iCANTempReg

			
局部变量：	i
函数调用：	none


*************************************************************/
void SysCanParallelHandle::vCANDataRemove ( UINT16 i )
{

	switch ( i )
	{
		case 5:
			//将接收邮箱5中的ID和数据内容放到临时寄存器
			iCANTempReg.iMessageID =ECanaMboxes.MBOX5.MSGID.bit.STDMSGID;//存放ID
			iCANTempReg.iTempBox0 = ECanaMboxes.MBOX5.MDL.word.LOW_WORD;//存放第一个字的内容
			iCANTempReg.iTempBox1 = ECanaMboxes.MBOX5.MDL.word.HI_WORD;//存放第二个字的内容
			iCANTempReg.iTempBox2 = ECanaMboxes.MBOX5.MDH.word.LOW_WORD;//存放第三个字的内容
			iCANTempReg.iTempBox3 = ECanaMboxes.MBOX5.MDH.word.HI_WORD;//存放第四个字的内容
		break;

		case 4:
			//将接收邮箱4中的ID和数据内容放到临时寄存器
			iCANTempReg.iMessageID = ECanaMboxes.MBOX4.MSGID.bit.STDMSGID;//存放ID
			iCANTempReg.iTempBox0 = ECanaMboxes.MBOX4.MDL.word.LOW_WORD;//存放第一个字的内容
			iCANTempReg.iTempBox1 = ECanaMboxes.MBOX4.MDL.word.HI_WORD;//存放第二个字的内容
			iCANTempReg.iTempBox2 = ECanaMboxes.MBOX4.MDH.word.LOW_WORD;//存放第三个字的内容
			iCANTempReg.iTempBox3 = ECanaMboxes.MBOX4.MDH.word.HI_WORD;//存放第四个字的内容
		break;
		
		case 3:
			//将接收邮箱3中的ID和数据内容放到临时寄存器
			iCANTempReg.iMessageID = ECanaMboxes.MBOX3.MSGID.bit.STDMSGID;//存放ID
			iCANTempReg.iTempBox0 = ECanaMboxes.MBOX3.MDL.word.LOW_WORD;//存放第一个字的内容
			iCANTempReg.iTempBox1 = ECanaMboxes.MBOX3.MDL.word.HI_WORD;//存放第二个字的内容
			iCANTempReg.iTempBox2 = ECanaMboxes.MBOX3.MDH.word.LOW_WORD;//存放第三个字的内容
			iCANTempReg.iTempBox3 = ECanaMboxes.MBOX3.MDH.word.HI_WORD;//存放第四个字的内容
		break;
			
		case 2:
			//将接收邮箱2中的ID和数据内容放到临时寄存器
			iCANTempReg.iMessageID = ECanaMboxes.MBOX2.MSGID.bit.STDMSGID;//存放ID
			iCANTempReg.iTempBox0 = ECanaMboxes.MBOX2.MDL.word.LOW_WORD;//存放第一个字的内容
			iCANTempReg.iTempBox1 = ECanaMboxes.MBOX2.MDL.word.HI_WORD;//存放第二个字的内容
			iCANTempReg.iTempBox2 = ECanaMboxes.MBOX2.MDH.word.LOW_WORD;//存放第三个字的内容
			iCANTempReg.iTempBox3 = ECanaMboxes.MBOX2.MDH.word.HI_WORD;//存放第四个字的内容
		break;
			
		case 1:
			//将接收邮箱1中的ID和数据内容放到临时寄存器
			iCANTempReg.iMessageID = ECanaMboxes.MBOX1.MSGID.bit.STDMSGID;//存放ID
			iCANTempReg.iTempBox0 = ECanaMboxes.MBOX1.MDL.word.LOW_WORD;//存放第一个字的内容
			iCANTempReg.iTempBox1 = ECanaMboxes.MBOX1.MDL.word.HI_WORD;//存放第二个字的内容
			iCANTempReg.iTempBox2 = ECanaMboxes.MBOX1.MDH.word.LOW_WORD;//存放第三个字的内容
			iCANTempReg.iTempBox3 = ECanaMboxes.MBOX1.MDH.word.HI_WORD;//存放第四个字的内容
		break;
		
		case 0:
			//将接收邮箱0中的ID和数据内容放到临时寄存器
			iCANTempReg.iMessageID = ECanaMboxes.MBOX0.MSGID.bit.STDMSGID;//存放ID
			iCANTempReg.iTempBox0 = ECanaMboxes.MBOX0.MDL.word.LOW_WORD;//存放第一个字的内容
			iCANTempReg.iTempBox1 = ECanaMboxes.MBOX0.MDL.word.HI_WORD;//存放第二个字的内容
			iCANTempReg.iTempBox2 = ECanaMboxes.MBOX0.MDH.word.LOW_WORD;//存放第三个字的内容
			iCANTempReg.iTempBox3 = ECanaMboxes.MBOX0.MDH.word.HI_WORD;//存放第四个字的内容
		break;
			
		default:
		break;
	}
}
/*************************************************************
文件名称：	vCanDataStore( )
功能描述：	将临时buffer中的数据类别进行判断并放到相应UPS数据存贮单元中
算法说明：	none

入口参数：	iCANTempReg
				MessageID_Low
				MessageID_High
				MessageID_Version
				VersionNumber
				


出口参数：	uMACIDFlag
			iParaRunFlag
			sParaData
	
			
局部变量：	iMsssagePRIID
			iMACID
			iMessageOldVersion
函数调用：	none

*************************************************************/
void SysCanParallelHandle::vCANDataStore( )
{
	UINT16 iMessagePRIID;
	UINT16 iMACID;
	UINT16 iMessageOldVersion;

	//提取ID段中的信息类别值
	iMessagePRIID = (iCANTempReg.iMessageID & 0x03C0)>>6;

	//提取ID段中的UPS ID值
	iMACID = (iCANTempReg.iMessageID & 0x003F);
	//提取整个ID值
	iMessageOldVersion = (iCANTempReg.iMessageID & 0x03FF);


	if ( 	iMACID <= 6 )
	{
	
		//低优先级信息帧处理
		if (iMessagePRIID == MessageID_Low)
		{
			sCanFlag.bReceived = 1;

			//判断UPS ID
			if (iMACID ==0)
			{
				uMACIDFlag.sMACIDFlagBits.bMACID0 = 1;  //判断旁路是否在线
			}
						
			if (iMACID ==1)
			{
				uMACIDFlag.sMACIDFlagBits.bMACID1 = 1;	//UPS在线标志
				iParaRunFlag |= 0x0001; //将相应的UPS开机位置1
			}
		
			if (iMACID ==2)
			{
				uMACIDFlag.sMACIDFlagBits.bMACID2 = 1;	//UPS在线标志
				iParaRunFlag |= 0x0002;	//将相应的UPS开机位置1
			}

			if (iMACID ==3)
			{
				uMACIDFlag.sMACIDFlagBits.bMACID3 = 1;	//UPS在线标志
				iParaRunFlag |= 0x0004;	//将相应的UPS开机位置1
			}

			if (iMACID ==4)
			{
				uMACIDFlag.sMACIDFlagBits.bMACID4 = 1;	//UPS在线标志
				iParaRunFlag |= 0x0008;	//将相应的UPS开机位置1
			}

			if (iMACID ==5)
			{
				uMACIDFlag.sMACIDFlagBits.bMACID5 = 1;	//UPS在线标志
				iParaRunFlag |= 0x0010;	//将相应的UPS开机位置1
			}

			if (iMACID ==6)
			{
				uMACIDFlag.sMACIDFlagBits.bMACID6 = 1;	//UPS在线标志
				iParaRunFlag |= 0x0020;	//将相应的UPS开机位置1
			}

			//数据存储
			//是否为低优先级数据第一帧
			if ((iCANTempReg.iTempBox0 & 0x00FF)==1)
			{
				sParaData[iMACID].m_i16Pouta = iCANTempReg.iTempBox1;
				sParaData[iMACID].m_i16Poutb = iCANTempReg.iTempBox2;
				sParaData[iMACID].m_i16Poutc = iCANTempReg.iTempBox3;
			}

			//是否为低优先级数据第三帧
			if ((iCANTempReg.iTempBox0 & 0x00FF)==2)
			{
				sParaData[iMACID].m_i16Souta = iCANTempReg.iTempBox1;
				sParaData[iMACID].m_i16Soutb = iCANTempReg.iTempBox2;
				sParaData[iMACID].m_i16Soutc = iCANTempReg.iTempBox3;
			}				 

			//是否为低优先级数据第二帧
			if ((iCANTempReg.iTempBox0 & 0x00FF)==3)
			{
				sParaData[iMACID].uSystemFlag = iCANTempReg.iTempBox1;
				sParaData[iMACID].m_i16ModelKVA = iCANTempReg.iTempBox2;
				sParaData[iMACID].m_i16wGrantKVA = iCANTempReg.iTempBox3;
			}
		}
		
		//高优先级信息帧处理
		if (iMessagePRIID == MessageID_High)
		{
			sCanFlag.bReceived = 1;
			sParaData[iMACID].uSystemFlag = iCANTempReg.iTempBox1;
		}			

		//版本信息帧处理
		if ( iMessagePRIID == MessageID_Version )
		{
		
			if  ( iCANTempReg.iTempBox0 != VersionNumber ) // 判断是否等于本机版本号
			{
				 sCanFlag.bVersionErr = 1;
			}
					
		}
	}

		//是否为老版本协议中的信息帧
	if ( iMessageOldVersion == 0x41)
	{
		sCanFlag.bVersionErr = 1;
	}
}

/* ***********************************************************
文件名称：	vCanCalculation( )
功能描述：	计算并机系统数据,包括并机系统的功率计算和工作状态标志判断
算法说明：	none

入口参数：	ParaNumCNST
			sParaData
			m_i16ModelKVA
			m_i16wSerialNumSet_0
			iParaWarnFlag
			objMonInterface.m_st_wUPSModeRun
			iNumUPS
			m_i16BasicNum
			m_i16RedunNum
出口参数：	
			iNumBp
			iNumInv
			iNumOn
			iNumUPS
			iNumBattWarn
			iNumBattInv
			iParaWarnFlag
			iCapacitySumBasic
			iCapacitySum_InvOn
			iCapacitySum_Invmod
			iMaxCapacityInv
			iMaxCapacityOn
			InvMasterNum_20
			iPoutTotala
			iPoutTotalb
			iPoutTotalc
			iSoutTotala
			iSoutTotalb
			iSoutTotalc
			iPoutTotalMax
			iSoutTotalMax
			iPSoutTotalMax
			sCanFlag

局部变量：	i
			j
			temp
			INT32temp
			uCanDataBuff1
函数调用：	none
*********************************************************** */
void SysCanParallelHandle::vCANCalculation( )
{
	INT16		i,j,temp;				//临时变量
	INT32 	INT32temp;
	UINT16 		uCanDataBuff1;


	j = 0;
	for(i = 0;i < ParaNumCNST-1;i++)
	{
		iCapacityOn[i] = 0;
	}
//cb_H 030403 增加旁路供电台数
	iNumBp = 0;
//cb_E 030403
	iNumInv = 0;
	iNumOn = 0;
	iNumUPS = 0;
	iNumBattWarn = 0;
	iNumBattInv = 0;
	iParaWarnFlag = 0;
	iCapacitySumBasic = 0;
	iCapacitySum_InvOn = 0;
	iCapacitySum_Invmod = 0;

//YWF_H20051029不同容量并联
	iMaxCapacityInv = 0;
	iMaxCapacityOn = 0;
	iInvMasterPriorINT16er = 0;
//YWF_E20051029

//wzh 20060802 电池管理
	iSumOfInvONCapacity = 0;
	iSumOfBattInvCapacity = 0;
	iSumOfBattWarCapacity = 0;
//wzh end
	
//zqf 060615--降额机型不同容量并联
	iKCapSum_Invmod = 0;		
		
    
//判断旁路柜信息
	sCanFlag.bAutoBp = (sParaData[0].uSystemFlag & SBIT7);//具体是哪一位TBD	
	
	/* 计算并机系统逆变总开机台数和总供电台数 */
	for(i=1;i<=ParaNumCNST-1;i++)
	{    
		if ( ( sParaData[i].uSystemFlag & SBIT0 ) != 0 )
		{
			iNumInv ++;					//逆变开机总台数
		}
		if ( ( sParaData[i].uSystemFlag & SBIT1 ) != 0 )
		{
			iNumOn ++;					//逆变供电总台数
		}
		if ( ( sParaData[i].uSystemFlag & SBIT2 ) != 0 )
		{
			iNumBattWarn ++;			//并机系统电池容量告警总台数
		}
		if ( ( sParaData[i].uSystemFlag & SBIT3 ) != 0 )
		{
			iNumBattInv ++;				//并机系统电池逆变供电总台数
		}
		if ( ( sParaData[i].uSystemFlag & SBIT9 ) != 0 )
		{
			iNumUPS ++;					//并机系统实际上电总台数
		}

//wzh 20060802 计算处在逆变供电状态下的各台机器的GrandKVA之和
		if((sParaData[i].uSystemFlag & SBIT1)!=0)
		{
			iSumOfInvONCapacity += sParaData[i].m_i16wGrantKVA;
		}
//wzh end

//wzh 20060802 计算处在电池逆变状态下的各台机器的GrandKVA之和;
		if((sParaData[i].uSystemFlag & SBIT3)!=0)
		{
			iSumOfBattInvCapacity += sParaData[i].m_i16wGrantKVA;
		}
//wzh end

//wzh 20060802 计算处在电池预告警状态下的各台机器的GrandKVA之和;
		if((sParaData[i].uSystemFlag & SBIT2)!=0)
		{
			iSumOfBattWarCapacity += sParaData[i].m_i16wGrantKVA;
		}
//wzh end

//wzh20060802 

//cb_H 030403 增加旁路上电台数
		if ( sParaData[i].uSystemFlag & SBIT8 )
		{
			iNumBp ++;					//并机系统旁路供电总台数
		}
//cb_E 030403				

		if ( ( sParaData[i].uSystemFlag & SBIT0 ) != 0 )
		{
			iCapacitySum_InvOn = iCapacitySum_InvOn + sParaData[i].m_i16wGrantKVA;

//YWF_H20051029不同容量并联
			if (iMaxCapacityInv < sParaData[i].m_i16wGrantKVA)
			{
				iMaxCapacityInv = sParaData[i].m_i16wGrantKVA;	
			}
//YWF_E20051029
		}

		if ( ( sParaData[i].uSystemFlag & SBIT1 ) != 0 )
		{
			iCapacitySum_Invmod = iCapacitySum_Invmod + sParaData[i].m_i16wGrantKVA;

//YWF_H20051031不同容量并联
			if (iMaxCapacityOn < sParaData[i].m_i16wGrantKVA)
			{
				iMaxCapacityOn = sParaData[i].m_i16wGrantKVA;	
			}
//YWF_E20051031
//zqf 060615--降额机型的不同容量并联
			iKCapSum_Invmod += (((INT32)sParaData[i].m_i16wGrantKVA)<<12)/(sParaData[i].m_i16ModelKVA);
//zqf 060615--降额机型的不同容量并联
		}

		if ( ( sParaData[i].uSystemFlag & SBIT9 ) != 0 ) //并机系统实际上电
		{
			if ((sParaData[i].m_i16wGrantKVA > objMonInterface.m_i16GrantKVA)
				||((sParaData[i].m_i16wGrantKVA == objMonInterface.m_i16GrantKVA) && (i < objMonInterface.m_i16wSerialNumSet_0)))
			{
				iInvMasterPriorINT16er++;
			}

			iCapacityOn[j] = sParaData[i].m_i16wGrantKVA;
			j++;
			
		}
						
		/* 合成并机系统的告警标志 */
		uCanDataBuff1 = ( sParaData[i].uSystemFlag >> 4 ) & 0xF;
		iParaWarnFlag = iParaWarnFlag | uCanDataBuff1;
	}

//zqf 060615--降额机型的不同容量并联
	if ((iKCapSum_Invmod !=0)&&(iNumOn>=2))
		iKCapAdj = (((INT32)objMonInterface.m_i16GrantKVA<<12) / objMonInterface.m_i16ModelKVA * iNumOn * 4096) / iKCapSum_Invmod;
	else
		iKCapAdj = 4096;
//zqf 060615--降额机型的不同容量并联

//YWF_H20051125不同容量冗余并联			
//			objInvAlg.m_i16wInvMasterPrior_0 = iInvMasterPriorINT16er;  //

	for (i = 0;i < iNumUPS-1;i++)
	{
		for (j = i+1;j < iNumUPS;j++)
		{
			if (iCapacityOn[i] > iCapacityOn[j])
			{
				temp = iCapacityOn[i];
				iCapacityOn[i] = iCapacityOn[j];
				iCapacityOn[j] = temp;
			}
		}
	}

	for (j = 0;j < objMonInterface.m_i16BasicNum;j++)
	{
		iCapacitySumBasic = iCapacitySumBasic + iCapacityOn[j];
	}
//YWF_E20051125

//wzh 20060606 计算总的旁路幅值范围标志

	objBypass.m_st_wBpAmpRangeFlag.bCANWide = 0;
	for (j = 0; j<iNumUPS;j++)
	{
		if( (sParaData[j+1].uSystemFlag & SBIT12 ) != 0)
		{
			objBypass.m_st_wBpAmpRangeFlag.bCANWide = 1;
			break;
		}
		
	}

//wzh end



	/* 计算并机系统各相总有功功率和视在功率 */
	if ( objMonInterface.m_st_wUPSModeRun.bCabinet != 2 )	//并机系统不走旁路柜
	{
		iPoutTotala = 0;
		iPoutTotalb = 0;
		iPoutTotalc = 0;
		iSoutTotala = 0;
		iSoutTotalb = 0;
		iSoutTotalc = 0;

		//计算并机系统各相实际功率
		for(i=1;i<=ParaNumCNST-1;i++)
		{
			INT32temp = ((INT32)sParaData[i].m_i16Pouta*sParaData[i].m_i16wGrantKVA*10)>>10;
			INT32temp = (INT32temp*K1div3Cnst)>>12;     // *1/3
			iPoutTotala = iPoutTotala + (INT16)(INT32temp); 
			
			INT32temp = ((INT32)sParaData[i].m_i16Poutb*sParaData[i].m_i16wGrantKVA*10)>>10;
			INT32temp = (INT32temp*K1div3Cnst)>>12;	// *1/3
			iPoutTotalb = iPoutTotalb + (INT16)(INT32temp);
			
			INT32temp = ((INT32)sParaData[i].m_i16Poutc*sParaData[i].m_i16wGrantKVA*10)>>10;
			INT32temp = (INT32temp*K1div3Cnst)>>12;	// *1/3
			iPoutTotalc = iPoutTotalc + (INT16)(INT32temp);
			
			INT32temp = ((INT32)sParaData[i].m_i16Souta*sParaData[i].m_i16wGrantKVA*10)>>10;
			INT32temp = (INT32temp*K1div3Cnst)>>12;	// *1/3
			iSoutTotala = iSoutTotala + (INT16)(INT32temp); 
			
			INT32temp = ((INT32)sParaData[i].m_i16Soutb*sParaData[i].m_i16wGrantKVA*10)>>10;
			INT32temp = (INT32temp*K1div3Cnst)>>12;	// *1/3
			iSoutTotalb = iSoutTotalb + (INT16)(INT32temp);
			
			INT32temp = ((INT32)sParaData[i].m_i16Soutc*sParaData[i].m_i16wGrantKVA*10)>>10;
			INT32temp = (INT32temp*K1div3Cnst)>>12;	// *1/3
			iSoutTotalc = iSoutTotalc + (INT16)(INT32temp);
	
		}

		//计算并机系统各相负载中的最大值
		iPoutTotalMax = MAX2(iPoutTotala,iPoutTotalb);
		iPoutTotalMax = MAX2(iPoutTotalMax,iPoutTotalc);

		iSoutTotalMax = MAX2(iSoutTotala,iSoutTotalb);
		iSoutTotalMax = MAX2(iSoutTotalMax,iSoutTotalc);

		iPSoutTotalMax = MAX2(iSoutTotalMax,iPoutTotalMax);

		//计算系统负载标幺值
		INT32temp = (INT32)iPoutTotala*objSystem.m_i161DivGrantKVA;			//6.10
		iPoutTotala = (INT16)((INT32temp*K1div10Cnst)>>12);			
	
		INT32temp = (INT32)iPoutTotalb*objSystem.m_i161DivGrantKVA;
		iPoutTotalb = (INT16)((INT32temp*K1div10Cnst)>>12);

		INT32temp = (INT32)iPoutTotalc*objSystem.m_i161DivGrantKVA;
		iPoutTotalc = (INT16)((INT32temp*K1div10Cnst)>>12);
	
		INT32temp = (INT32)iSoutTotala*objSystem.m_i161DivGrantKVA;
		iSoutTotala = (INT16)((INT32temp*K1div10Cnst)>>12);
	
		INT32temp = (INT32)iSoutTotalb*objSystem.m_i161DivGrantKVA;
		iSoutTotalb = (INT16)((INT32temp*K1div10Cnst)>>12);

		INT32temp = (INT32)iSoutTotalc*objSystem.m_i161DivGrantKVA;
		iSoutTotalc = (INT16)((INT32temp*K1div10Cnst)>>12);

	}       
}

/*************************************************************
文件名称：	vCANLocalData( )
功能描述：	将本机的数据放到相应UPS数据存贮单元中
算法说明：	none

入口参数：	
			objSwitches.m_st_FlagBreaker
			m_i16Pouta
			m_i16Poutb
			m_i16Poutc
			m_i16Souta
			m_i16Soutb
			m_i16Soutc
			m_i16ModelKVA
			sParaData
			
出口参数：	
			sParaData
			uInvRealInfo
			iLocalStatusChanged
			
局部变量：	
			uLocalStatus
			uCanDataBuff1
函数调用：	none
*************************************************************/		
void SysCanParallelHandle::vCANLocalData( )
{
	UINT16		uLocalStatus;
	UINT16 		uCanDataBuff1;

	if ( objSwitches.m_st_FlagBreaker.bQ5 == 1 )
	{
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Pouta = objOutput.m_i16Pouta;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Poutb = objOutput.m_i16Poutb;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Poutc = objOutput.m_i16Poutc;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Souta = objOutput.m_i16Souta;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Soutb = objOutput.m_i16Soutb;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Soutc = objOutput.m_i16Soutc;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16ModelKVA = objMonInterface.m_i16ModelKVA;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16wGrantKVA = objMonInterface.m_i16GrantKVA;			
	}
	else
	{
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Pouta = 0x0000;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Poutb = 0x0000;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Poutc = 0x0000;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Souta = 0x0000;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Soutb = 0x0000;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Soutc = 0x0000;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16ModelKVA = objMonInterface.m_i16ModelKVA;
		sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16wGrantKVA = objMonInterface.m_i16GrantKVA;
	}
	
	uLocalStatus = sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag;
	sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag = 0;
	if ( objSwitches.m_st_FlagBreaker.bInv == 1 )			//逆变开启
	{
		sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag |= SBIT0;
	}
				
	if ( objSwitches.m_st_FlagBreaker.bOn == 1 )			//逆变供电
	{
		sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag |= SBIT1;
	}
					
	//cb_H 030403  增加旁路供电判断
	if ( objSwitches.m_st_FlagBreaker.bBpOn == 1 )		//旁路供电
	{
		sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag |= SBIT8;
	}
	//cb_E 030403

	//wzh 20060606 增加旁路幅值范围标志
	if (objBypass.m_st_wBpAmpRangeFlag.bLocalWide == 1)
	{
		sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag |= SBIT12;	//本机在线标志	
	}
	// wzh end

	/* 合成本机告警信息 */
	uCanDataBuff1 = ( S2U(objMonitor.uInvRealInfo) << 2 ) & 0xFC;
	sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag |= uCanDataBuff1;

	sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag |= SBIT9;	//本机在线标志

	uCanDataBuff1 = (UINT16) objSystem.m_st_wFlagStatus.bVbp;
	sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag |= ( uCanDataBuff1 << 10 );
	
	if (sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag != uLocalStatus)
	{
		iLocalStatusChanged = 1;
	}
	//cb_H 030412 协议升级，清本机实时信息的故障信息
	objMonitor.uInvRealInfo.bSevereFail = 0;
	objMonitor.uInvRealInfo.bNormalFail = 0;
	objMonitor.uInvRealInfo.bSevereWarn = 0;
	objMonitor.uInvRealInfo.bNormalWarn = 0;				

}


/*************************************************************
文件名称：	vCANLow_Tx( )
功能描述：	将本机的低优先级数据在总线上广播
算法说明：	none

入口参数：	
			ECanaRegs
			EcanaMboxes
			MessageID_Low
			m_i16wSerialNumSet_0
			iFrameIDLow
			sParaData
出口参数:	
			ECanaRegs
			ECanaMboxes
局部变量:	temp0
			tempECanReg
			tempECanMboxReg
函数调用:	none

*************************************************************/
void SysCanParallelHandle::vCANLow_Tx( )
{
	INT16	temp0;
	struct	ECAN_REGS	tempECanReg;
	struct  	ECAN_MBOXES	tempECanMboxReg;

	//清除发送成功标志位
	tempECanReg.CANTA.all = ECanaRegs.CANTA.all;
	tempECanReg.CANTA.bit.TA10 = 1;
	ECanaRegs.CANTA.all = tempECanReg.CANTA.all;

	//清除发送被忽略标志位
	tempECanReg.CANAA.all = ECanaRegs.CANAA.all;
	tempECanReg.CANAA.bit.AA10 = 1;
	ECanaRegs.CANAA.all = tempECanReg.CANAA.all;

	//禁止邮箱10请求
	tempECanReg.CANME.all = ECanaRegs.CANME.all;
	tempECanReg.CANME.bit.ME10 = 0;
	ECanaRegs.CANME.all = tempECanReg.CANME.all;
	
	//邮箱10 ID 配置
	tempECanMboxReg.MBOX10.MSGID.all =  ECanaMboxes.MBOX10.MSGID.all;
	tempECanMboxReg.MBOX10.MSGID.bit.IDE = 0; //stardart identifier
	tempECanMboxReg.MBOX10.MSGID.bit.AME =0;//dont care
	tempECanMboxReg.MBOX10.MSGID.bit.AAM = 0;//normal transmit mode
	tempECanMboxReg.MBOX10.MSGID.bit.STDMSGID = (MessageID_Low<<6);//写信息类别邋ID
	temp0 = (objMonInterface.m_i16wSerialNumSet_0 & 0x07);
	tempECanMboxReg.MBOX10.MSGID.bit.STDMSGID |= temp0;	//写UPS ID
	tempECanMboxReg.MBOX10.MSGID.bit.EXTMSGID_H = 0;
	tempECanMboxReg.MBOX10.MSGID.bit.EXTMSGID_L = 0;
	ECanaMboxes.MBOX10.MSGID.all = tempECanMboxReg.MBOX10.MSGID.all;
	
	//ECanaMboxes.MBOX10.MSGCTRL.all = ECanaMboxes.MBOX10.MSGCTRL.all;
	ECanaMboxes.MBOX10.MSGCTRL.all = 0x00000008;//写数据长度
	
	if (iFrameIDLow == 1)
	{
		//写数据
		ECanaMboxes.MBOX10.MDL.word.LOW_WORD= iFrameIDLow;  
		ECanaMboxes.MBOX10.MDL.word.HI_WORD = sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Pouta;
		ECanaMboxes.MBOX10.MDH.word.LOW_WORD = sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Poutb;
		ECanaMboxes.MBOX10.MDH.word.HI_WORD = sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Poutc;
		//CAN_MDER = 0x00EF;

		//允许请求发送
		tempECanReg.CANME.all = ECanaRegs.CANME.all;
		tempECanReg.CANME.bit.ME10 = 1;
		ECanaRegs.CANME.all = tempECanReg.CANME.all;
		
		//启动发送
		tempECanReg.CANTRS.all = ECanaRegs.CANTRS.all;
		tempECanReg.CANTRS.bit.TRS10 = 1;
		ECanaRegs.CANTRS.all = tempECanReg.CANTRS.all;

		iFrameIDLow++;
		return;
	}
	
	if (iFrameIDLow == 2)
	{
		//写数据
		ECanaMboxes.MBOX10.MDL.word.LOW_WORD =iFrameIDLow;
		ECanaMboxes.MBOX10.MDL.word.HI_WORD = sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Souta;
		ECanaMboxes.MBOX10.MDH.word.LOW_WORD = sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Soutb;
		ECanaMboxes.MBOX10.MDH.word.HI_WORD = sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16Soutc;
		//CAN_MDER = 0x00EF;
		
		//允许请求发送
		tempECanReg.CANME.all = ECanaRegs.CANME.all;
		tempECanReg.CANME.bit.ME10 = 1;
		ECanaRegs.CANME.all = tempECanReg.CANME.all;
		
		//启动发送
		tempECanReg.CANTRS.all = ECanaRegs.CANTRS.all;
		tempECanReg.CANTRS.bit.TRS10 = 1;
		ECanaRegs.CANTRS.all = tempECanReg.CANTRS.all;
		
		iFrameIDLow++;
		return;
	}
																																																																																																																																																																																																						
	if (iFrameIDLow == 3)
	{
		//写数据
		ECanaMboxes.MBOX10.MDL.word.LOW_WORD =iFrameIDLow;
		ECanaMboxes.MBOX10.MDL.word.HI_WORD = sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag; 
		ECanaMboxes.MBOX10.MDH.word.LOW_WORD = sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16ModelKVA;
		ECanaMboxes.MBOX10.MDH.word.HI_WORD = sParaData[objMonInterface.m_i16wSerialNumSet_0].m_i16wGrantKVA;
		//CAN_MDER = 0x00EF;
		
		//允许请求发送
		tempECanReg.CANME.all = ECanaRegs.CANME.all;
		tempECanReg.CANME.bit.ME10 = 1;
		ECanaRegs.CANME.all = tempECanReg.CANME.all;
		
		//启动发送
		tempECanReg.CANTRS.all = ECanaRegs.CANTRS.all;
		tempECanReg.CANTRS.bit.TRS10 = 1;
		ECanaRegs.CANTRS.all = tempECanReg.CANTRS.all;
		
		iFrameIDLow = 0;
	}
		
}
/*************************************************************
文件名称：	vCANVersion_Tx( )
功能描述：	将本机的CAN 协议版本号在总线上广播
算法说明：	none

入口参数：
			ECanaRegs
			ECanaMboxes
			MessageID_Version
			m_i16wSerialNumSet_0
			VersionNumber
出口参数：	
			ECanaRegs
			ECanaMboxes
局部变量：	temp0
			tempECanReg
			tempECanMboxReg
函数调用：	none

*************************************************************/				
void SysCanParallelHandle::vCANVersion_Tx( )
{
	INT16 temp0;
	struct	ECAN_REGS	tempECanReg;
	struct  	ECAN_MBOXES	tempECanMboxReg;

	//清除发送成功标志位
	tempECanReg.CANTA.all = ECanaRegs.CANTA.all;
	tempECanReg.CANTA.bit.TA10 = 1;
	ECanaRegs.CANTA.all = tempECanReg.CANTA.all;

	//清除发送被忽略标志位
	tempECanReg.CANAA.all = ECanaRegs.CANAA.all;
	tempECanReg.CANAA.bit.AA10 = 1;
	ECanaRegs.CANAA.all = tempECanReg.CANAA.all;

	//禁止邮箱10请求
	tempECanReg.CANME.all = ECanaRegs.CANME.all;
	tempECanReg.CANME.bit.ME10 = 0;
	ECanaRegs.CANME.all = tempECanReg.CANME.all;

	//邮箱10 ID 配置
	tempECanMboxReg.MBOX10.MSGID.all =  ECanaMboxes.MBOX10.MSGID.all;
	tempECanMboxReg.MBOX10.MSGID.bit.IDE = 0; //stardart identifier
	tempECanMboxReg.MBOX10.MSGID.bit.AME =0;//dont care
	tempECanMboxReg.MBOX10.MSGID.bit.AAM = 0;//normal transmit mode
	tempECanMboxReg.MBOX10.MSGID.bit.STDMSGID = (MessageID_Version<<6);//写信息类别邋ID
	temp0 = (objMonInterface.m_i16wSerialNumSet_0 & 0x07);
	tempECanMboxReg.MBOX10.MSGID.bit.STDMSGID |= temp0;	//写UPS ID
	tempECanMboxReg.MBOX10.MSGID.bit.EXTMSGID_H = 0;
	tempECanMboxReg.MBOX10.MSGID.bit.EXTMSGID_L = 0;
	ECanaMboxes.MBOX10.MSGID.all = tempECanMboxReg.MBOX10.MSGID.all;
	
	ECanaMboxes.MBOX10.MSGCTRL.all = 0x00000002;//写数据长度

	//写数据	
	ECanaMboxes.MBOX10.MDL.word.LOW_WORD = VersionNumber;
	//CAN_MBX5B = sParaData[m_i16wSerialNumSet_0].uSystemFlag; 
	
	//允许请求发送
	tempECanReg.CANME.all = ECanaRegs.CANME.all;
	tempECanReg.CANME.bit.ME10 = 1;
	ECanaRegs.CANME.all = tempECanReg.CANME.all;
		
	//启动发送
	tempECanReg.CANTRS.all = ECanaRegs.CANTRS.all;
	tempECanReg.CANTRS.bit.TRS10 = 1;
	ECanaRegs.CANTRS.all = tempECanReg.CANTRS.all;
}																																																																																																																																																																																																
/*************************************************************
文件名称：	vCANHigh_Tx( )
功能描述：	将本机的高优先级数据在总线上广播
算法说明：	none

入口参数：
			ECanaRegs
			ECanaMboxes
			MessageID_High
			m_i16wSerialNumSet_0
			iFrameIDHigh
			
出口参数：	
			ECanaRegs
			ECanaMboxes
局部变量：	temp0
			tempECanReg
			tempECanMboxReg
函数调用：	none

*************************************************************/																																																																																																																																										
void SysCanParallelHandle::vCANHigh_Tx( )
{

	INT16 temp0;
	struct	ECAN_REGS	tempECanReg;
	struct  	ECAN_MBOXES	tempECanMboxReg;

	iFrameIDHigh = 1;

	//清除发送成功标志位
	tempECanReg.CANTA.all = ECanaRegs.CANTA.all;
	tempECanReg.CANTA.bit.TA10 = 1;
	ECanaRegs.CANTA.all = tempECanReg.CANTA.all;

	//清除发送被忽略标志位
	tempECanReg.CANAA.all = ECanaRegs.CANAA.all;
	tempECanReg.CANAA.bit.AA10 = 1;
	ECanaRegs.CANAA.all = tempECanReg.CANAA.all;

	//禁止邮箱10请求
	tempECanReg.CANME.all = ECanaRegs.CANME.all;
	tempECanReg.CANME.bit.ME10 = 0;
	ECanaRegs.CANME.all = tempECanReg.CANME.all;

	//邮箱10 ID 配置
	tempECanMboxReg.MBOX10.MSGID.all =  ECanaMboxes.MBOX10.MSGID.all;
	tempECanMboxReg.MBOX10.MSGID.bit.IDE = 0; //stardart identifier
	tempECanMboxReg.MBOX10.MSGID.bit.AME =0;//dont care
	tempECanMboxReg.MBOX10.MSGID.bit.AAM = 0;//normal transmit mode
	tempECanMboxReg.MBOX10.MSGID.bit.STDMSGID = (MessageID_High<<6);//写信息类别邋ID
	temp0 = (objMonInterface.m_i16wSerialNumSet_0 & 0x07);
	tempECanMboxReg.MBOX10.MSGID.bit.STDMSGID |= temp0;	//写UPS ID
	tempECanMboxReg.MBOX10.MSGID.bit.EXTMSGID_H = 0;
	tempECanMboxReg.MBOX10.MSGID.bit.EXTMSGID_L = 0;
	ECanaMboxes.MBOX10.MSGID.all = tempECanMboxReg.MBOX10.MSGID.all;
	
	ECanaMboxes.MBOX10.MSGCTRL.all = 0x00000004;//写数据长度

	//写数据
	ECanaMboxes.MBOX10.MDL.word.LOW_WORD = iFrameIDHigh;
	ECanaMboxes.MBOX10.MDL.word.HI_WORD = sParaData[objMonInterface.m_i16wSerialNumSet_0].uSystemFlag; 
	
	//允许请求发送
	tempECanReg.CANME.all = ECanaRegs.CANME.all;
	tempECanReg.CANME.bit.ME10 = 1;
	ECanaRegs.CANME.all = tempECanReg.CANME.all;
		
	//启动发送
	tempECanReg.CANTRS.all = ECanaRegs.CANTRS.all;
	tempECanReg.CANTRS.bit.TRS10 = 1;
	ECanaRegs.CANTRS.all = tempECanReg.CANTRS.all;
	
}

/*************************************************************
文件名称：	vCAN_Com_Err( )
功能描述：	并机系统通讯故障判断
算法说明：	none

入口参数：	
			uMACIDFlag
			m_i16wSerialNumSet_0
			sCanFlag
			m_i16BasicNum
			m_i16RedunNum
			objMonInterface.m_st_wUPSModeRun
			
出口参数:
			sCanFlag
			iSpanCanErr
			sParaData
			uMACIDFlag
局部变量：	i
				iUPSOnlineNum
函数调用：	none

*************************************************************/																																																																																																																																										
void SysCanParallelHandle::vCAN_Com_Err( )
{
	UINT16 iUPSOnlineNum; 
	UINT16 i;

	//是否总线上有与本机相同的ID号
	if ( uMACIDFlag.iMACIDFlagWord & (1<<(objMonInterface.m_i16wSerialNumSet_0)))
	{
		iUPSOnlineNum = 0;
	}
	else
	{
		iUPSOnlineNum = 1;
	}
	
	//计算总线上的在线台数
	if (uMACIDFlag.sMACIDFlagBits.bMACID1 == 1)
	{
		iUPSOnlineNum++;
	}
	
	if (uMACIDFlag.sMACIDFlagBits.bMACID2 == 1)
	{
		iUPSOnlineNum++;
	}
	
	if (uMACIDFlag.sMACIDFlagBits.bMACID3 == 1)
	{
		iUPSOnlineNum++;
	}
	
	if (uMACIDFlag.sMACIDFlagBits.bMACID4 == 1)
	{
		iUPSOnlineNum++;
	}
	
	if (uMACIDFlag.sMACIDFlagBits.bMACID5 == 1)
	{
		iUPSOnlineNum++;
	}
	
	if (uMACIDFlag.sMACIDFlagBits.bMACID6 == 1)
	{
		iUPSOnlineNum++;
	}

	//判断是否单机在线
	if (iUPSOnlineNum == 1 )
	{
		sCanFlag.bUPSSingleOline = 1;
	}
	else
	{
		sCanFlag.bUPSSingleOline = 0;
	}

	//当有新的UPS加入,版本信息帧发送标志置1
	if (iUPSOnlineNum >( objMonInterface.m_i16BasicNum + objMonInterface.m_i16RedunNum ) )
	{
		sCanFlag.bVersion = 1;
	}

	
	if (( iUPSOnlineNum != ( objMonInterface.m_i16BasicNum + objMonInterface.m_i16RedunNum ) )
		||( ( objMonInterface.m_st_wUPSModeRun.bCabinet == 2 ) && ( sCanFlag.bAutoBp == 0 ) ))
		//实际台数不等于设置台数，或设置成自动旁路柜但旁路柜未启动
	{
		iSpanCanErr ++;
		if (iSpanCanErr >=5)
		{
			iSpanCanErr = 5;
			sCanFlag.bErr = 1;
			for (i=1; i<=ParaNumCNST-1; i++)
			{
				//本机不需要检测20061008
				if(i == objMonInterface.m_i16wSerialNumSet_0)
				{
					sMACIDErrCounter[i] = 0;//对本机计数器清0
					continue;
				}
				
				//对掉电的UPS数据清零20061008
				if (uMACIDFlag.iMACIDFlagWord & ( 1 <<i ))
				{
					sMACIDErrCounter[i] = 0;
				}
				else if ((++sMACIDErrCounter[i]) >= 5 )
				{
					sParaData[i].m_i16Pouta = 0;
					sParaData[i].m_i16Poutb = 0;
					sParaData[i].m_i16Poutc = 0;
					sParaData[i].m_i16Souta = 0;
					sParaData[i].m_i16Soutb = 0;
					sParaData[i].m_i16Soutc = 0;
					//sParaData[i].m_i16ModelKVA = 0;
					//sParaData[i].m_i16wGrantKVA = 0;
					sParaData[i].uSystemFlag = 0;
					sMACIDErrCounter[i]=5;
				}
			
				/*if ( (  uMACIDFlag.iMACIDFlagWord & ( 1 <<i ) ) !=( ( uMACIDFlag.iMACIDFlagWord & ( 1 <<(i+6) ) ) >>6)) 
				{
					sParaData[i].m_i16Pouta = 0;
					sParaData[i].m_i16Poutb = 0;
					sParaData[i].m_i16Poutc = 0;
					sParaData[i].m_i16Souta = 0;
					sParaData[i].m_i16Soutb = 0;
					sParaData[i].m_i16Soutc = 0;
					//sParaData[i].m_i16ModelKVA = 0;
					//sParaData[i].m_i16wGrantKVA = 0;
					sParaData[i].uSystemFlag = 0;
				}
				*/
			}	
			
		}
	}
	else
	{
		iSpanCanErr = 0;
		sCanFlag.bErr = 0;

		//清各机错误计数器
		for (i=1; i<=ParaNumCNST-1; i++)
		{
			sMACIDErrCounter[i] = 0;//20061008
		}	

		//备份各UPS在线标志
		uMACIDFlag.sMACIDFlagBits.bMACID1Backup  = uMACIDFlag.sMACIDFlagBits.bMACID1; 
		uMACIDFlag.sMACIDFlagBits.bMACID2Backup  = uMACIDFlag.sMACIDFlagBits.bMACID2; 
		uMACIDFlag.sMACIDFlagBits.bMACID3Backup  = uMACIDFlag.sMACIDFlagBits.bMACID3; 
		uMACIDFlag.sMACIDFlagBits.bMACID4Backup  = uMACIDFlag.sMACIDFlagBits.bMACID4; 
		uMACIDFlag.sMACIDFlagBits.bMACID5Backup  = uMACIDFlag.sMACIDFlagBits.bMACID5; 
		uMACIDFlag.sMACIDFlagBits.bMACID6Backup  = uMACIDFlag.sMACIDFlagBits.bMACID6;
	}

	
	uMACIDFlag.iMACIDFlagWord  &= 0xFF80;	//清接收数据标志位
}	

	




