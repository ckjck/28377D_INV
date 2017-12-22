/*==========================================================*/
/* Title		:	PowCANScheduler.cpp											*/
/* Description	: 	Class_PowCANScheduler realization 								*/
/* Date		:	Apr.2008														*/
/*==========================================================*/
#include "PowCANScheduler.h"


/*************************************************************
函数名称：  void App_PowCANInit()
功能描述：  初始化相关变量
算法说明：
入口参数：
出口参数：
初稿日期：	070919
修改日期：	070919
*************************************************************/
void	Class_PowCANScheduler::App_PowCANInit(void)
{
	INT16	i;

	//power can 函数调用表初始化
		PrdSchdlrTbl[0].m_u16SetTickCnt = K166usCnst;    //模块数增加，接收的模块数据增多，每1个中断检查一次邮箱并接收数据
		PrdSchdlrTbl[0].m_u16TickCntr = 0;
		PrdSchdlrTbl[0].pCbFunc = &Class_PowCANScheduler::App_Int_Can_Recv;

		PrdSchdlrTbl[1].m_u16SetTickCnt = K2msCnst;         //模块数增加至12，为避免CAN总线负载率过大而出现有的模块没有发送，改为2ms发送一帧数据
		PrdSchdlrTbl[1].m_u16TickCntr = 0;
		PrdSchdlrTbl[1].pCbFunc =&Class_PowCANScheduler::Dat_Int_Can_Send;

		PrdSchdlrTbl[2].m_u16SetTickCnt = (K1msCnst<<1);    //判断UPS在线的时间维持不变
		PrdSchdlrTbl[2].m_u16TickCntr = 0;
		PrdSchdlrTbl[2].pCbFunc = &Class_PowCANScheduler::App_Int_Can_ComErr;

		//Power CAN 有无功数据结构初始化
		for (i = 0; i < ParaNumCNST; i++)
		{
			PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVAP = 0;
			PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVAQ = 0;
			PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVBP = 0;
			PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVBQ = 0;
			PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVCP= 0;
			PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVCQ = 0;
			PowCanParaInfoEntry.UpsOnlineInfo.m_u16NotOnlineCnt[i] = 0;
		}

		PowCanParaInfoEntry.MsgRecvFalg.m_u16All = 0;
		PowCanParaInfoEntry.UpsOnlineInfo.uOnlineFlag.m_u16All = 0;

		//By wxw20090730 PowerCan的Load数据表初始化
		for (i = 0; i < ParaNumCNST; i++)
		{
			PowerCANParaLoadEntry.ParaLoadData[i].m_i16Pouta= 0;
			PowerCANParaLoadEntry.ParaLoadData[i].m_i16Poutb= 0;
			PowerCANParaLoadEntry.ParaLoadData[i].m_i16Poutc= 0;
			PowerCANParaLoadEntry.ParaLoadData[i].m_i16Souta= 0;
			PowerCANParaLoadEntry.ParaLoadData[i].m_i16Soutb= 0;
			PowerCANParaLoadEntry.ParaLoadData[i].m_i16Soutc= 0;
		}

		i16SendScheduleCnt = 0;
		m_i16InvState = 0;
		m_i16BattState = 0;
		PowerCANParaLoadEntry.DqCurrentValid.m_u16All = 0;
}
/* ***********************************************************
函数名称：	void App_Int_PrdTimer_Route( )
功能描述：	接收其他ups的输出电流值
算法说明：
入口参数：
出口参数：
初稿日期:	070919
修改日期:	070919
*********************************************************** */
#pragma CODE_SECTION(".Epwm1Funcs")

void	Class_PowCANScheduler::App_Int_PrdTimer_Route(void)
{
	INT16	 i;


	//单机或 并机信号隔离态 或UPSTestMode
//	if ( sUPSModeRun.bConfig != 3 || sParaMaint.bParaSignalMask == 1 || sFlagSystem.bUPSTestMode == 1 )
//	{
//		return;
//	}

//  ECanbRegs.CANES.bit.BO == 1,表明CAN总线上存在一个异常错误率(当CANTEC达到256个限值时)，此时只能通过清除CANMC寄存器中的CCR位或者ABO位(Auto Bus Open)被设定，
//  在收到128*11个接收位后退出，离开总线关闭状态，并且错误计数器(CANTEC)位被清除

	//总线busoff故障恢复
	if ( ECanbRegs.CANES.bit.BO != 0 )
	{
		EALLOW;
		ECanbRegs.CANMC.bit.ABO = 1;
		EDIS;
	}


	/*
		By wxw20090714 PowerCan模块每2ms调用一次发送，
		而负载大小约一个工频周期更新一次，因此对于50Hz系统每40个发送周期发一次负载大小，即40*2 = 80ms
    */
	for (i = 0; i < POW_CAN_PRD_TABL_SIZE; i++)
	{
		if (++PrdSchdlrTbl[i].m_u16TickCntr >= PrdSchdlrTbl[i].m_u16SetTickCnt)
		{
			PrdSchdlrTbl[i].m_u16TickCntr = 0;
			switch (i)
			{
				case 0:
					App_Int_Can_Recv();
					break;
				case 1:
					//By wxw20090724 30K模块PowerCan只传递逆变电流，因此系统处于逆变侧才调用
					//20KPowerCan还需传递负载大小，因此需一直调用
					if((objMonInterface.m_st_wUPSModeRun.bConfig == 3) && (objParallel.m_st_wParaMaint.bParaSignalMask != 1))
					{
						//By wxw20090714 先通过发送调度函数App_Int_Can_SendSchedule
						//得到当前需发送的帧类型,再发数据
						i16ScheduleID = App_Int_Can_SendSchedule();
						Dat_Int_Can_Send();
					}
					break;
				case 2:
					if((objMonInterface.m_st_wUPSModeRun.bConfig == 3)
					&& (objParallel.m_st_wParaMaint.bParaSignalMask != 1)
					&& (objSystem.m_st_wFlagSystem.bAfterParamDly == 1)
					&& (objDigitalIO.m_st_HardwareSignal.bRecOff == 0))
					{
						App_Int_Can_ComErr();
					}
					else
					{
						PowCanParaInfoEntry.MsgRecvFalg.m_u16All = 0;
						PowCanParaInfoEntry.UpsOnlineInfo.uOnlineFlag.m_u16All = 0;
					}
					break;
				default:
					break;

			}
		}
	}
}

/* ***********************************************************
函数名称：	void Int_Can_Recv( )
功能描述：	接收其他ups的输出有无功电流值
算法说明：
入口参数：
出口参数：
初稿日期:	070919
修改日期:	070919
*********************************************************** */
#pragma CODE_SECTION(".Epwm1Funcs")

void Class_PowCANScheduler::App_Int_Can_Recv(void)
{
	volatile 	struct MBOX *pMBox;
	volatile	struct	ECAN_REGS	*pECanReg;

	pECanReg = &ECanbRegs;

	//读取邮箱9内容
	pMBox = &(ECanbMboxes.MBOX9);
	objPowCANPro.Dat_Int_MBox_Read(pMBox, BIT9);

	//读取邮箱8内容
	pMBox = &(ECanbMboxes.MBOX8);
	objPowCANPro.Dat_Int_MBox_Read(pMBox, BIT8);

	//读取邮箱7内容
	pMBox = &(ECanbMboxes.MBOX7);
	objPowCANPro.Dat_Int_MBox_Read(pMBox, BIT7);

	//读取邮箱6内容
	pMBox = &(ECanbMboxes.MBOX6);
	objPowCANPro.Dat_Int_MBox_Read(pMBox, BIT6);

	//本机发送成功，更新本机数据
	//*****当台数为1时，D，Q，Z，输出电流信息有误，均流调节已作台数限制，其他使用时注意。
	if (pECanReg->CANTRS.bit.TRS11 == 0)
	{
		PowCanParaInfoEntry.ParaDQCurrEntry[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IINVAP = m_i16InvaPSend;
		PowCanParaInfoEntry.ParaDQCurrEntry[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IINVAQ = m_i16InvaQSend;
		PowCanParaInfoEntry.ParaDQCurrEntry[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IINVBP = m_i16InvbPSend;
		PowCanParaInfoEntry.ParaDQCurrEntry[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IINVBQ = m_i16InvbQSend;
		PowCanParaInfoEntry.ParaDQCurrEntry[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IINVCP = m_i16InvcPSend;
		PowCanParaInfoEntry.ParaDQCurrEntry[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IINVCQ = m_i16InvcQSend;

		PowCanParaInfoEntry.ParaDQCurrEntry[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16DCurr = m_i16DCurrSend;

	}
}
/* ***********************************************************
函数名称：	void Int_Can_Send( )
功能描述：	广播本ups的输出电流值
算法说明：
入口参数：
出口参数：
初稿日期:	070919
修改日期:	070919
*********************************************************** */
#pragma CODE_SECTION(".Epwm1Funcs")

void Class_PowCANScheduler::Dat_Int_Can_Send(void)
{
	UINT16	i16TempID;

	pECanReg = &ECanbRegs;
	pECanMboxes = &ECanbMboxes;
	i16TempID = objDigitalIO.m_st_wHardwareNewSignal.bInvID;

	PowCanParaInfoEntry.MsgRecvFalg.m_u16All |= ( 1 << objDigitalIO.m_st_wHardwareNewSignal.bInvID);
	//发送成功
	if (pECanReg->CANTRS.bit.TRS11 == 0)
	{
		//清除发送成功标志位
		tempECanReg.CANTA.all = pECanReg->CANTA.all;
		tempECanReg.CANTA.bit.TA11 = 1;
		pECanReg->CANTA.all = tempECanReg.CANTA.all;

		//清除发送被忽略标志位
		tempECanReg.CANAA.all = pECanReg->CANAA.all;
		tempECanReg.CANAA.bit.AA11 = 1;
		pECanReg->CANAA.all = tempECanReg.CANAA.all;

		//禁止邮箱11请求
		tempECanReg.CANME.all = pECanReg->CANME.all;
		tempECanReg.CANME.bit.ME11 = 0;
		pECanReg->CANME.all = tempECanReg.CANME.all;

		//邮箱11 ID 配置
		tempMbox.MSGID.all =  pECanMboxes->MBOX11.MSGID.all;
		tempMbox.MSGID.bit.IDE = 0; 				//stardart identifier
		tempMbox.MSGID.bit.AME = 0;					//dont care
		tempMbox.MSGID.bit.AAM = 0;					//normal transmit mode

		switch(i16ScheduleID)
		{
		 	case SEND_IINVADQCURRENT:
				if(objXfer.m_st_XferResult.bInvKM == 1)
				{
					tempMbox.MSGID.bit.STDMSGID = (SEND_IINVADQCURRENT << 6);//写信息类别邋ID
					PowerCANParaLoadEntry.DqCurrentValid.m_u16All |=( 1 << (i16TempID));
				}
				else
				{
					tempMbox.MSGID.bit.STDMSGID = (MESSAGEID_INVALID_CURRENT<< 6);//写无效电流信息类别邋ID
					//PowerCANParaLoadEntry.DqCurrentValid.m_u16All = 0;
					PowerCANParaLoadEntry.DqCurrentValid.m_u16All &= (~(1 << i16TempID));
				}
				//m_u16UpsIdxTemp = iSerialNumSet_0 & 0x07;//写UPS ID
				tempMbox.MSGID.bit.STDMSGID |=i16TempID;// objDigitalIO.m_st_wHardwareNewSignal.bInvID;
				tempMbox.MSGID.bit.EXTMSGID_H = 0;//扩展ID
				tempMbox.MSGID.bit.EXTMSGID_L = 0;//扩展ID

				pECanMboxes->MBOX11.MSGID.all = tempMbox.MSGID.all;
				pECanMboxes->MBOX11.MSGCTRL.all = 0x00000006;//写数据长度



			//写数据

					pECanMboxes->MBOX11.MDL.word.LOW_WORD= objInverter.m_i16IinvaP_0;
					pECanMboxes->MBOX11.MDL.word.HI_WORD = objInverter.m_i16IinvaQ_0;
					pECanMboxes->MBOX11.MDH.word.LOW_WORD = objInverter.m_unIinvMdFilter_0.half.hword;

					m_i16InvaPSend = objInverter.m_i16IinvaP_0;
					m_i16InvaQSend = objInverter.m_i16IinvaQ_0;
					m_i16DCurrSend = objInverter.m_unIinvMdFilter_0.half.hword;

				break;


			case SEND_IINVBDQCURRENT:
				if(objXfer.m_st_XferResult.bInvKM == 1)
				{
					tempMbox.MSGID.bit.STDMSGID = (SEND_IINVBDQCURRENT << 6);//写信息类别邋ID
					PowerCANParaLoadEntry.DqCurrentValid.m_u16All |=( 1 << (i16TempID));
				}
				else
				{
					tempMbox.MSGID.bit.STDMSGID = (MESSAGEID_INVALID_CURRENT<< 6);//写无效电流信息类别邋ID
					//PowerCANParaLoadEntry.DqCurrentValid.m_u16All = 0;
					PowerCANParaLoadEntry.DqCurrentValid.m_u16All &= (~(1 << i16TempID));
				}
				//m_u16UpsIdxTemp = iSerialNumSet_0 & 0x07;//写UPS ID
				tempMbox.MSGID.bit.STDMSGID |=i16TempID;// objDigitalIO.m_st_wHardwareNewSignal.bInvID;
				tempMbox.MSGID.bit.EXTMSGID_H = 0;//扩展ID
				tempMbox.MSGID.bit.EXTMSGID_L = 0;//扩展ID

				pECanMboxes->MBOX11.MSGID.all = tempMbox.MSGID.all;
				pECanMboxes->MBOX11.MSGCTRL.all = 0x00000006;//写数据长度



			//写数据

					pECanMboxes->MBOX11.MDL.word.LOW_WORD= objInverter.m_i16IinvbP_0;
					pECanMboxes->MBOX11.MDL.word.HI_WORD = objInverter.m_i16IinvbQ_0;
					pECanMboxes->MBOX11.MDH.word.LOW_WORD = objInverter.m_unIinvMdFilter_0.half.hword;

					objPowCANSched.m_i16InvbPSend = objInverter.m_i16IinvbP_0;
					objPowCANSched.m_i16InvbQSend = objInverter.m_i16IinvbQ_0;
					objPowCANSched.m_i16DCurrSend = objInverter.m_unIinvMdFilter_0.half.hword;

				break;


			case SEND_IINVCDQCURRENT:
				if(objXfer.m_st_XferResult.bInvKM == 1)
				{
					tempMbox.MSGID.bit.STDMSGID = (SEND_IINVCDQCURRENT << 6);//写信息类别邋ID
					PowerCANParaLoadEntry.DqCurrentValid.m_u16All |=( 1 << (i16TempID));
				}
				else
				{
					tempMbox.MSGID.bit.STDMSGID = (MESSAGEID_INVALID_CURRENT<< 6);//写无效电流信息类别邋ID
					//PowerCANParaLoadEntry.DqCurrentValid.m_u16All = 0;
					PowerCANParaLoadEntry.DqCurrentValid.m_u16All &= (~(1 << i16TempID));
				}
				//m_u16UpsIdxTemp = iSerialNumSet_0 & 0x07;//写UPS ID
				tempMbox.MSGID.bit.STDMSGID |=i16TempID;// objDigitalIO.m_st_wHardwareNewSignal.bInvID;
				tempMbox.MSGID.bit.EXTMSGID_H = 0;//扩展ID
				tempMbox.MSGID.bit.EXTMSGID_L = 0;//扩展ID

				pECanMboxes->MBOX11.MSGID.all = tempMbox.MSGID.all;
				pECanMboxes->MBOX11.MSGCTRL.all = 0x00000006;//写数据长度



			//写数据

					pECanMboxes->MBOX11.MDL.word.LOW_WORD= objInverter.m_i16IinvcP_0;
					pECanMboxes->MBOX11.MDL.word.HI_WORD = objInverter.m_i16IinvcQ_0;
					pECanMboxes->MBOX11.MDH.word.LOW_WORD = objInverter.m_unIinvMdFilter_0.half.hword;

					objPowCANSched.m_i16InvcPSend = objInverter.m_i16IinvcP_0;
					objPowCANSched.m_i16InvcQSend = objInverter.m_i16IinvcQ_0;
					objPowCANSched.m_i16DCurrSend = objInverter.m_unIinvMdFilter_0.half.hword;

				break;


			case SEND_PLOAD:
				tempMbox.MSGID.bit.STDMSGID = (MESSAGEID_PLOAD << 6);//写信息类别邋ID
				//m_u16UpsIdxTemp = iSerialNumSet_0 & 0x07;//写UPS ID
				tempMbox.MSGID.bit.STDMSGID |=i16TempID;// objDigitalIO.m_st_wHardwareNewSignal.bInvID;
				tempMbox.MSGID.bit.EXTMSGID_H = 0;//扩展ID
				tempMbox.MSGID.bit.EXTMSGID_L = 0;//扩展ID

				pECanMboxes->MBOX11.MSGID.all = tempMbox.MSGID.all;
				pECanMboxes->MBOX11.MSGCTRL.all = 0x00000006;//写数据长度

				pECanMboxes->MBOX11.MDL.word.LOW_WORD = m_i16PoutaSend;
				pECanMboxes->MBOX11.MDL.word.HI_WORD = m_i16PoutbSend;
				pECanMboxes->MBOX11.MDH.word.LOW_WORD = m_i16PoutcSend;
				break;


			case SEND_SLOAD:
				tempMbox.MSGID.bit.STDMSGID = (MESSAGEID_SLOAD << 6);//写信息类别邋ID
				//m_u16UpsIdxTemp = iSerialNumSet_0 & 0x07;//写UPS ID
				tempMbox.MSGID.bit.STDMSGID |=i16TempID;// objDigitalIO.m_st_wHardwareNewSignal.bInvID;
				tempMbox.MSGID.bit.EXTMSGID_H = 0;//扩展ID
				tempMbox.MSGID.bit.EXTMSGID_L = 0;//扩展ID

				pECanMboxes->MBOX11.MSGID.all = tempMbox.MSGID.all;
				pECanMboxes->MBOX11.MSGCTRL.all = 0x00000006;//写数据长度


			//写数据
				pECanMboxes->MBOX11.MDL.word.LOW_WORD = m_i16SoutaSend;
				pECanMboxes->MBOX11.MDL.word.HI_WORD = m_i16SoutbSend;
				pECanMboxes->MBOX11.MDH.word.LOW_WORD = m_i16SoutcSend;
				break;

			case SEND_SYSTEMSTATE:
				tempMbox.MSGID.bit.STDMSGID = (MESSAGEID_STATE << 6);//写信息类别邋ID
				//m_u16UpsIdxTemp = iSerialNumSet_0 & 0x07;//写UPS ID
				tempMbox.MSGID.bit.STDMSGID |=i16TempID;// objDigitalIO.m_st_wHardwareNewSignal.bInvID;
				tempMbox.MSGID.bit.EXTMSGID_H = 0;//扩展ID
				tempMbox.MSGID.bit.EXTMSGID_L = 0;//扩展ID

				pECanMboxes->MBOX11.MSGID.all = tempMbox.MSGID.all;
				pECanMboxes->MBOX11.MSGCTRL.all = 0x00000004;//写数据长度

				pECanMboxes->MBOX11.MDL.word.LOW_WORD = m_i16InvState;
				pECanMboxes->MBOX11.MDL.word.HI_WORD =  m_i16BattState;
				break;


			case SEND_ILFILTER:
				if(objXfer.m_st_XferResult.bInvKM == 1)
				{
					tempMbox.MSGID.bit.STDMSGID = (MESSAGEID_ILFILTER << 6);//写信息类别邋ID
					PowerCANParaLoadEntry.DqCurrentValid.m_u16All |=( 1 << (i16TempID));
				}
				else
				{
					tempMbox.MSGID.bit.STDMSGID = (MESSAGEID_INVALID_CURRENT<< 6);//写无效电流信息类别邋ID
					//PowerCANParaLoadEntry.DqCurrentValid.m_u16All = 0;
					PowerCANParaLoadEntry.DqCurrentValid.m_u16All &= (~(1 << i16TempID));
				}
				//m_u16UpsIdxTemp = iSerialNumSet_0 & 0x07;//写UPS ID
				tempMbox.MSGID.bit.STDMSGID |=i16TempID;// objDigitalIO.m_st_wHardwareNewSignal.bInvID;
				tempMbox.MSGID.bit.EXTMSGID_H = 0;//扩展ID
				tempMbox.MSGID.bit.EXTMSGID_L = 0;//扩展ID

				pECanMboxes->MBOX11.MSGID.all = tempMbox.MSGID.all;
				pECanMboxes->MBOX11.MSGCTRL.all = 0x00000006;//写数据长度

				pECanMboxes->MBOX11.MDL.word.LOW_WORD = objInverter.m_i16IlaFilter2_0;
				pECanMboxes->MBOX11.MDL.word.HI_WORD =  objInverter.m_i16IlbFilter2_0;
				pECanMboxes->MBOX11.MDH.word.LOW_WORD = objInverter.m_i16IlcFilter2_0;
				PowCanParaInfoEntry.IlFilter[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IlFiltera = objInverter.m_i16IlaFilter2_0;
				PowCanParaInfoEntry.IlFilter[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IlFilterb = objInverter.m_i16IlbFilter2_0;
				PowCanParaInfoEntry.IlFilter[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IlFilterc = objInverter.m_i16IlcFilter2_0;
				break;

			default:
				break;
		}

		//允许请求发送
		tempECanReg.CANME.all = pECanReg->CANME.all;
		tempECanReg.CANME.bit.ME11 = 1;
		pECanReg->CANME.all = tempECanReg.CANME.all;

		//启动发送
		tempECanReg.CANTRS.all = pECanReg->CANTRS.all;
		tempECanReg.CANTRS.bit.TRS11 = 1;
		pECanReg->CANTRS.all = tempECanReg.CANTRS.all;
	}
}

/* ***********************************************************
函数名称：	void Int_Can_ComErr( )
功能描述：	power 并机通信故障判断，与低优先级并机can
				共同维护并机通信故障标志
算法说明：
入口参数：
出口参数：
初稿日期:	070919
修改日期:	070919
*********************************************************** */
#pragma CODE_SECTION(".Epwm1Funcs")

void Class_PowCANScheduler::App_Int_Can_ComErr(void)
{
	UINT16	 i;

	INT16 i16UpsOnlineNum = 0;
	static UINT16 s_u16ComErrCnt2ms = 0;

	for (i = 0;  i < ParaNumCNST;  i++)
	{
		//ups在线判断
		if ((PowCanParaInfoEntry.MsgRecvFalg.m_u16All & (1 << i))  !=  0)
		{
			PowCanParaInfoEntry.UpsOnlineInfo.uOnlineFlag.m_u16All |= (1 << i);
			PowCanParaInfoEntry.UpsOnlineInfo.m_u16NotOnlineCnt[i] = 0;
			i16UpsOnlineNum++;
		}

		//ups离线判断，连续两次没有收到某ups的信息，则置该ups状态为离线
		else
		{
			if (++PowCanParaInfoEntry.UpsOnlineInfo.m_u16NotOnlineCnt[i]  >=  UPS_NOT_ONLINE_MAX_CNT)
			{
				PowCanParaInfoEntry.UpsOnlineInfo.uOnlineFlag.m_u16All &= (~(1 << i));
				PowCanParaInfoEntry.UpsOnlineInfo.m_u16NotOnlineCnt[i] = UPS_NOT_ONLINE_MAX_CNT;

			}
		}
	}

	//清本轮所有接收标志
	PowCanParaInfoEntry.MsgRecvFalg.m_u16All = 0;

	//实际台数不等于设置台数，或设置成自动旁路柜但旁路柜未启动
//	if (( i16UpsOnlineNum != (objMonInterface.m_st_wParallelData.iNumOn + 1) )

	//新判据: 收不到旁路模块的信息即认为通讯故障
	//By wxw20090715 Adapter15K~20K如果实际并机台数小于基本台数，报通讯故障
	if(i16UpsOnlineNum < (objMonInterface.m_i16BasicNum + objMonInterface.m_i16RedunNum))
	{
		//连续120ms不等于设置台数,则报通讯故障
		if (++s_u16ComErrCnt2ms >= UPS_NOT_ONLINE_MAX_CNT_0)
		{
			s_u16ComErrCnt2ms = UPS_NOT_ONLINE_MAX_CNT_0;
			PowCanParaInfoEntry.UpsOnlineInfo.uOnlineFlag.tBits.bSysCommerr = 1;
		}
	}
	else
	{
		PowCanParaInfoEntry.UpsOnlineInfo.uOnlineFlag.tBits.bSysCommerr = 0;
		s_u16ComErrCnt2ms = 0;
	}
}

/* ***********************************************************
函数名称：	UINT16 App_Int_Can_SendSchedule( void)
功能描述：	power 并机发送调度函数
算法说明：
入口参数：
出口参数：	SEND_DQCURRENT:当前需传DQ电流帧
			SEND_PLOAD	  :当前需传有功功率信息帧
			SEND_SLOAD	  :当前需传视在功率信息帧
初稿日期:	090714
修改日期:	090714
*********************************************************** */
#pragma CODE_SECTION(".Epwm1Funcs")
UINT16 Class_PowCANScheduler::App_Int_Can_SendSchedule(void)
{
	UINT16 i16SendId;

	/*
		By wxw20090714 PowerCan模块每1ms调用一次发送，
		而负载大小约一个工频周期更新一次，因此对于50Hz系统每20个发送周期发一次负载大小
		对于60Hz系统16.7个发送周期发送一次负载大小，取两者间的大值
	*/
	switch(i16SendScheduleCnt)
	{
		case 5:
				i16SendId = SEND_SYSTEMSTATE;
				break;
		case 15:
				i16SendId = SEND_PLOAD;
				break;
		case 25:
				i16SendId = SEND_SLOAD;
				break;
		case 35:
		        i16SendId = SEND_ILFILTER;
		        break;
		default:
				m_i16IabcShedule++;
				if(m_i16IabcShedule>2)
				{
					m_i16IabcShedule = 0;
				}
				if(m_i16IabcShedule == 0)
				{
					i16SendId = SEND_IINVADQCURRENT;
				}
				else if(m_i16IabcShedule == 1)
				{
					i16SendId = SEND_IINVBDQCURRENT;
				}
				else if(m_i16IabcShedule == 2)
				{
					i16SendId = SEND_IINVCDQCURRENT;
				}
				break;
	}

	if( i16SendScheduleCnt <39)
	{
		i16SendScheduleCnt++;
	}
	else
	{
		i16SendScheduleCnt = 0;
	}
	return (i16SendId);
	//return(SEND_PLOAD);
}

/* ***********************************************************
函数名称：	void Dat_Can_LoadCalculation( void)
功能描述：	power 并机负载计算
算法说明：	Adaptor15k~20K增加计算并机负载大小
入口参数：
出口参数：
初稿日期:	090714
修改日期:	090714
*********************************************************** */
void Class_PowCANScheduler::Dat_Can_LoadCalculation(void)
{
	INT16 i;
	INT16 PoutaTotal = 0;		//A相功率和
	INT16 SoutaTotal = 0;		//A相视在功率和
	INT16 PoutbTotal = 0;		//B相功率和
	INT16 SoutbTotal = 0;		//B相视在功率和
	INT16 PoutcTotal = 0;		//C相功率和
	INT16 SoutcTotal = 0;		//C相视在功率和

	INT16 InvNum = 0;				//Inv开启台数
	INT16 OnNum  = 0;				//Inv供电台数

	INT16 ChgOnNum  = 0;			//Charger开启台数
	INT16 ChgPBoostNum  = 0;		//正Charger均充台数
	INT16 BattDischgNum  = 0;		//电池放电预告警台数
	INT16 BattPreWarnNum  = 0;		//电池放电预告警台数
	INT16 BattSelfTestNum  = 0;		//电池放电预告警台数

	INT16 ChgOnMinID  = 0;

	//均不供电时调整发送的输出功率
	if(( objXfer.m_st_wXferCmd.bSwitchStatus == 1 )||(objXfer.m_st_XferResult.bInvKM == 1))
	{
		m_i16PoutaSend = objInverter.m_i16Pinva;
		m_i16PoutbSend = objInverter.m_i16Pinvb;
		m_i16PoutcSend = objInverter.m_i16Pinvc;

		m_i16SoutaSend = objInverter.m_i16Sinva;
		m_i16SoutbSend = objInverter.m_i16Sinvb;
		m_i16SoutcSend = objInverter.m_i16Sinvc;
	}
	else if(( objXfer.m_st_wXferCmd.bSwitchStatus == 0 )&&(objXfer.m_st_XferResult.bBpSTS == 1))
	{
		m_i16PoutaSend = objOutput.m_i16Pouta;
		m_i16PoutbSend = objOutput.m_i16Poutb;
		m_i16PoutcSend = objOutput.m_i16Poutc;

		m_i16SoutaSend = objOutput.m_i16Souta;
		m_i16SoutbSend = objOutput.m_i16Soutb;
		m_i16SoutcSend = objOutput.m_i16Soutc;
	}
	else
	{
		m_i16PoutaSend = 1;
		m_i16PoutbSend = 1;
		m_i16PoutcSend = 1;
		m_i16SoutaSend = 1;
		m_i16SoutbSend = 1;
		m_i16SoutcSend = 1;
	}
	//加工状态标志
	m_i16InvState = (objSwitches.m_st_FlagBreaker.bInv + (objSwitches.m_st_FlagBreaker.bOn<<1));
	m_i16BattState = S2U(m_st_BatteryStatus);

	//更新本机数据
	PowerCANParaLoadEntry.ParaLoadData[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16Pouta = m_i16PoutaSend;
	PowerCANParaLoadEntry.ParaLoadData[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16Poutb = m_i16PoutbSend;;
	PowerCANParaLoadEntry.ParaLoadData[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16Poutc = m_i16PoutcSend;;


	PowerCANParaLoadEntry.ParaLoadData[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16Souta = m_i16SoutaSend;
	PowerCANParaLoadEntry.ParaLoadData[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16Soutb = m_i16SoutbSend;
	PowerCANParaLoadEntry.ParaLoadData[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16Soutc = m_i16SoutcSend;
	PowerCANParaLoadEntry.SysFlag[objDigitalIO.m_st_wHardwareNewSignal.bInvID].InvFlag.m_u16all	= m_i16InvState;
	PowerCANParaLoadEntry.SysFlag[objDigitalIO.m_st_wHardwareNewSignal.bInvID].BattFlag.m_u16all= m_i16BattState;

	for (i = 1; i < ParaNumCNST; i++)
	{
		if ((PowCanParaInfoEntry.UpsOnlineInfo.uOnlineFlag.m_u16All & (1 << i)) != 0)
		{
			//三相总有功功率
			PoutaTotal += PowerCANParaLoadEntry.ParaLoadData[i].m_i16Pouta;
			PoutbTotal += PowerCANParaLoadEntry.ParaLoadData[i].m_i16Poutb;
			PoutcTotal += PowerCANParaLoadEntry.ParaLoadData[i].m_i16Poutc;

			//三相总视在功率
			SoutaTotal += PowerCANParaLoadEntry.ParaLoadData[i].m_i16Souta;
			SoutbTotal += PowerCANParaLoadEntry.ParaLoadData[i].m_i16Soutb;
			SoutcTotal += PowerCANParaLoadEntry.ParaLoadData[i].m_i16Soutc;
			if(PowerCANParaLoadEntry.SysFlag[i].InvFlag.tbits.bInv == 1)
			{
				InvNum++;
			}
			if(PowerCANParaLoadEntry.SysFlag[i].InvFlag.tbits.bOn == 1)
			{
				OnNum++;
			}


			//共用电池组相关状态加工
			if((PowerCANParaLoadEntry.SysFlag[i].BattFlag.tbits.bChargerWorkP == 1)
			||(PowerCANParaLoadEntry.SysFlag[i].BattFlag.tbits.bChargerWorkN == 1))
			{
				ChgOnNum++;

				if(ChgOnNum == 1)
				{
					ChgOnMinID = i;
				}
			}

			if(PowerCANParaLoadEntry.SysFlag[i].BattFlag.tbits.bChgBostP == 1)
			{
				ChgPBoostNum++;
			}

			if(PowerCANParaLoadEntry.SysFlag[i].BattFlag.tbits.bBattDischg == 1)
			{
				BattDischgNum++;
			}

			if(PowerCANParaLoadEntry.SysFlag[i].BattFlag.tbits.bBattPreWarn == 1)
			{
				BattPreWarnNum++;
			}

			if(PowerCANParaLoadEntry.SysFlag[i].BattFlag.tbits.bBattSelfTest == 1)
			{
				BattSelfTestNum++;
			}
		}
	}
	m_i16PoutaTotal = PoutaTotal;
	m_i16PoutbTotal = PoutbTotal;
	m_i16PoutcTotal = PoutcTotal;

	m_i16SoutaTotal = SoutaTotal;
	m_i16SoutbTotal = SoutbTotal;
	m_i16SoutcTotal = SoutcTotal;

	m_i16PoutTotalMax = MAX2(PoutaTotal,PoutbTotal);
	m_i16PoutTotalMax = MAX2(m_i16PoutTotalMax,PoutcTotal);

	m_i16SoutTotalMax = MAX2(SoutaTotal,SoutbTotal);
	m_i16SoutTotalMax = MAX2(m_i16SoutTotalMax,SoutcTotal);


	m_i16InvTotalNum = InvNum;
	m_i16OnTotalNum = OnNum;

	if(objMonInterface.m_st_wUPSModeRun2.bBattShare == 1)
	{
			m_i16ChgOnTotalNum = ChgOnNum;
			m_i16ChgPBoostTotalNum = ChgPBoostNum;
			m_i16BattDischgTotalNum = BattDischgNum;
			m_i16BattPreWarnTotalNum = BattPreWarnNum;
			m_i16BattSelfTestTotalNum = BattSelfTestNum;
			m_i16ChgOnMinID = ChgOnMinID;

			if(m_i16ChgPBoostTotalNum != 0)
			{
				m_i16ChgPBoostTotal = 1;
			}
			else
			{
				m_i16ChgPBoostTotal = 0;
			}

			if(m_i16BattDischgTotalNum != 0)
			{
				m_i16ChgOnTotalNum = 0;
				m_i16ChgPBoostTotal = 0;
				m_i16ChgOnMinID = 0;

				if(m_i16BattDischgTotalNum == m_i16BattPreWarnTotalNum)
				{
					m_i16BattPreWarnTotal = 1;
				}
				else
				{
					m_i16BattPreWarnTotal = 0;
				}
			}
			else
			{
				m_i16BattPreWarnTotal = 0;
			}

			if(m_i16BattSelfTestTotalNum != 0)
			{
					m_i16BattSelfTestTotal = 1;
			}
			else
			{
					m_i16BattSelfTestTotal = 0;
			}
	}
	else
	{
			m_i16ChgOnTotalNum = 0;
			m_i16ChgPBoostTotalNum = 0;
			m_i16BattDischgTotalNum = 0;
			m_i16BattPreWarnTotalNum = 0;
			m_i16ChgOnMinID = 0;
			m_i16ChgPBoostTotal = 0;
			m_i16BattPreWarnTotal = 0;
			m_i16BattSelfTestTotalNum = 0;
			m_i16BattSelfTestTotal = 0;

	}
}

/* ***********************************************************
函数名称：	void App_Can_ILFilter( void)
功能描述：
算法说明：
入口参数：
出口参数：
初稿日期:	090714
修改日期:	090714
*********************************************************** */
void Class_PowCANScheduler::App_Can_ILFilter(void)
{
	INT16 i;
	INT16	i16DqValidNum = 0;
	INT32	i32ILaFilterSum = 0;
	INT32	i32ILbFilterSum = 0;
	INT32	i32ILcFilterSum = 0;

	for (i = 1; i < ParaNumCNST; i++)
	{
		if ((PowCanParaInfoEntry.UpsOnlineInfo.uOnlineFlag.m_u16All & (1 << i)) != 0)
		{
			if((PowerCANParaLoadEntry.DqCurrentValid.m_u16All& (1 << i)) != 0)
			{

				i32ILaFilterSum += PowCanParaInfoEntry.IlFilter[i].m_i16IlFiltera;
				i32ILbFilterSum += PowCanParaInfoEntry.IlFilter[i].m_i16IlFilterb;
				i32ILcFilterSum += PowCanParaInfoEntry.IlFilter[i].m_i16IlFilterc;

				i16DqValidNum++;
			}
		}
	}
	if(i16DqValidNum != 0)
	{
		objInvAlg.m_i16IlaFilterAvg_0 = i32ILaFilterSum / i16DqValidNum;
		objInvAlg.m_i16IlbFilterAvg_0 = i32ILbFilterSum / i16DqValidNum;
		objInvAlg.m_i16IlcFilterAvg_0 = i32ILcFilterSum / i16DqValidNum;
	}
	else
	{
		objInvAlg.m_i16IlaFilterAvg_0 = 0;
		objInvAlg.m_i16IlbFilterAvg_0 = 0;
		objInvAlg.m_i16IlcFilterAvg_0 = 0;
	}
	objInvAlg.m_i16IlaFilterShare_0 = PowCanParaInfoEntry.IlFilter[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IlFiltera;
	objInvAlg.m_i16IlbFilterShare_0 = PowCanParaInfoEntry.IlFilter[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IlFilterb;
	objInvAlg.m_i16IlcFilterShare_0 = PowCanParaInfoEntry.IlFilter[objDigitalIO.m_st_wHardwareNewSignal.bInvID].m_i16IlFilterc;
}

/* ***********************************************************
函数名称：	void App_Can_IInvDQCal( void)
功能描述：
算法说明：
入口参数：
出口参数：
初稿日期:	090714
修改日期:	090714
*********************************************************** */
void Class_PowCANScheduler::App_Can_IInvDQCal(void)
{
	INT16 i;
	INT16	i16DqValidNum = 0;
	INT32	i32IaPSum = 0;
	INT32	i32IaQSum = 0;
	INT32	i32IbPSum = 0;
	INT32	i32IbQSum = 0;
	INT32	i32IcPSum = 0;
	INT32	i32IcQSum = 0;
	INT32	i32IdSum=0;
	INT16	i16UpsOnlineNum = 0;


	for (i = 1; i < ParaNumCNST; i++)
	{
		if ((PowCanParaInfoEntry.UpsOnlineInfo.uOnlineFlag.m_u16All & (1 << i)) != 0)
		{
			if((PowerCANParaLoadEntry.DqCurrentValid.m_u16All& (1 << i)) != 0)
			{

				i32IaPSum += m_i16PowerCanIaPM_0[i];
				i32IaQSum += m_i16PowerCanIaQM_0[i];
				i32IbPSum += m_i16PowerCanIbPM_0[i];
				i32IbQSum += m_i16PowerCanIbQM_0[i];
				i32IcPSum += m_i16PowerCanIcPM_0[i];
				i32IcQSum += m_i16PowerCanIcQM_0[i];

				i32IdSum  += m_i16PowerCanDcurr_0[i];

				i16DqValidNum++;
			}
			i16UpsOnlineNum++;
		}
	}


	m_i16PowerCanOnlineNum = i16UpsOnlineNum;
	m_i16PowerCanTotalNum = i16DqValidNum;

	if(i16DqValidNum != 0)
	{
		objInvAlg.m_i16IlaPAvg_0 = i32IaPSum / i16DqValidNum;
		objInvAlg.m_i16IlaQAvg_0 = i32IaQSum / i16DqValidNum;
		objInvAlg.m_i16IlbPAvg_0 = i32IbPSum / i16DqValidNum;
		objInvAlg.m_i16IlbQAvg_0 = i32IbQSum / i16DqValidNum;
		objInvAlg.m_i16IlcPAvg_0 = i32IcPSum / i16DqValidNum;
		objInvAlg.m_i16IlcQAvg_0 = i32IcQSum / i16DqValidNum;
		objInvAlg.m_i16IdAvg_0	 = i32IdSum / i16DqValidNum;
	}
	else
	{
		objInvAlg.m_i16IlaPAvg_0 = 0;
		objInvAlg.m_i16IlaQAvg_0 = 0;
		objInvAlg.m_i16IlbPAvg_0 = 0;
		objInvAlg.m_i16IlbQAvg_0 = 0;
		objInvAlg.m_i16IlcPAvg_0 = 0;
		objInvAlg.m_i16IlcQAvg_0 = 0;

		objInvAlg.m_i16IdAvg_0 = 0;
	}
	objInvAlg.m_i16IlaPShare_0 = m_i16PowerCanIaPM_0[objDigitalIO.m_st_wHardwareNewSignal.bInvID];
	objInvAlg.m_i16IlaQShare_0 = m_i16PowerCanIaQM_0[objDigitalIO.m_st_wHardwareNewSignal.bInvID];
	objInvAlg.m_i16IlbPShare_0 = m_i16PowerCanIbPM_0[objDigitalIO.m_st_wHardwareNewSignal.bInvID];
	objInvAlg.m_i16IlbQShare_0 = m_i16PowerCanIbQM_0[objDigitalIO.m_st_wHardwareNewSignal.bInvID];
	objInvAlg.m_i16IlcPShare_0 = m_i16PowerCanIcPM_0[objDigitalIO.m_st_wHardwareNewSignal.bInvID];
	objInvAlg.m_i16IlcQShare_0 = m_i16PowerCanIcQM_0[objDigitalIO.m_st_wHardwareNewSignal.bInvID];
}

/* ***********************************************************
函数名称：	void Alg_Int_MainInterfaceVar( void)
功能描述：
算法说明：
入口参数：
出口参数：
初稿日期:	090714
修改日期:	090714
*********************************************************** */
#pragma CODE_SECTION(".Epwm1Funcs")

void Class_PowCANScheduler::Alg_Int_MainInterfaceVar(void)
{
	INT16	i;

	for (i = 1; i < ParaNumCNST; i++)
	{

		m_i16PowerCanIaPM_0[i] = PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVAP;
		m_i16PowerCanIaQM_0[i] = PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVAQ;
		m_i16PowerCanIbPM_0[i] = PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVBP;
		m_i16PowerCanIbQM_0[i] = PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVBQ;
		m_i16PowerCanIcPM_0[i] = PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVCP;
		m_i16PowerCanIcQM_0[i] = PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16IINVCQ;

		m_i16PowerCanDcurr_0[i] = PowCanParaInfoEntry.ParaDQCurrEntry[i].m_i16DCurr;
	}
}
