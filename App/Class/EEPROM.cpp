/*==========================================================*/
/* Title		:	EEPROM.cpp								*/
/* Description	: 	Class_EEPROM definition					*/
/* Date			:	Apr.2008								*/
/*==========================================================*/

#include "EEPROM.h"
/************************************************************************************
函数名：	Dat_EEPROMInit()	
功能描述：	类变量初始化		
函数调用：	
***********************************************************************************/

void	Class_EEPROM::Dat_EEPROMInit(void)
{	
	UINT16 u16Temp0;

	HistoryBuffW = objMonInterface.m_u16HistoryRecord;
	HistoryBuffR = objMonInterface.m_u16HistoryRecord;

	u16Temp0 = objEEPROM.App_ParamRead(HistoryRecordAddr);
	if(u16Temp0 < 512)
	{
		m_u16HistoryNextAddrW = u16Temp0;
	}
	else
	{
		m_u16HistoryNextAddrW = 0;
	}

	u16Temp0 = objEEPROM.App_ParamRead(HistoryRecordCnt);
	if(u16Temp0 <= 512)
	{
		m_u16HistoryCurrCnt = u16Temp0;
		//m_u16_HistoryCnt = 0;	
	}
	else
	{
		m_u16HistoryCurrCnt = 0;
	}

	u16Temp0 = objEEPROM.App_ParamRead(FaultAddr);
	if((u16Temp0 >= FaultRecordAddr)&&(u16Temp0 < 0x4000))
	{
		m_u16FaultNextAddrW = u16Temp0 & 0x0FF00;
		m_u16FaultAddrNext = m_u16FaultNextAddrW;
	}
	else
	{
		m_u16FaultNextAddrW = FaultRecordAddr;
		m_u16FaultAddrNext = FaultRecordAddr;
	}

	u16Temp0 = App_ParamRead(FaultRecordCnt);
	if(u16Temp0 <= 40)
	{
		m_u16FaultCurrCnt = u16Temp0;
	}
	else
	{
		m_u16FaultCurrCnt = 0;
	}

	FaultAddrW = &m_u16FaultNextAddrW;
	FaultWriteCnt = &m_i16FaultWriteCnt;
	FaultBuffR = uFaultRecord;	
}
/************************************************************************************
函数名：	App_ParamEEPROMW()	
功能描述：	将缓冲区中的校正数据按字写入EEPROM中		
函数调用：	objI2CDrv.Drv_I2CWordWrite();
***********************************************************************************/

void	Class_EEPROM::App_ParamEEPROMW(void)
{
	
		if((objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0x0001) == 0x0001)
		{
			objI2CDrv.Drv_I2CWordWrite(VoutARevAddr, objMonInterface.m_i16KsVoutaSet);
			objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh
				= (objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0xFFFE);
		}
		
		else if((objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0x0002) == 0x0002)
		{
			objI2CDrv.Drv_I2CWordWrite(VoutBRevAddr, objMonInterface.m_i16KsVoutbSet);
			objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh 
				= (objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0xFFFD);
		}
		
		else if((objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0x0004) == 0x0004)
		{
			objI2CDrv.Drv_I2CWordWrite(VoutCRevAddr, objMonInterface.m_i16KsVoutcSet);
			objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh
				= (objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0xFFFB);
		}
		
		else if((objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0x0008) == 0x0008)
		{
			objI2CDrv.Drv_I2CWordWrite(IoutARevAddr, objMonInterface.m_i16KsIoutaSet);
			objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh
				= (objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0xFFF7);
		}
		
		else if((objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0x0010) == 0x0010)
		{
			objI2CDrv.Drv_I2CWordWrite(IoutBRevAddr, objMonInterface.m_i16KsIoutbSet);
			objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh
				= (objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0xFFEF);
		}
	
		else if((objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0x0020) == 0x0020)
		{
			objI2CDrv.Drv_I2CWordWrite(IoutCRevAddr, objMonInterface.m_i16KsIoutcSet);
			objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh
				= (objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0xFFDF);
		}
		
		else if((objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0x0040) == 0x0040)
		{
			objI2CDrv.Drv_I2CWordWrite(VinvARevAddr, objMonInterface.m_i16KsVinvaSet);
			objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh 
				= (objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0xFFBF);
		}
		
		else if((objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0x0080) == 0x0080)
		{
			objI2CDrv.Drv_I2CWordWrite(VinvBRevAddr, objMonInterface.m_i16KsVinvbSet);
			objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh
				= (objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0xFF7F);
		}
		
		else if((objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0x0100) == 0x0100)
		{
			objI2CDrv.Drv_I2CWordWrite(VinvCRevAddr, objMonInterface.m_i16KsVinvcSet);
			objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh 
				= (objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0xFEFF);
		}
		else if((objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0x0200) == 0x0200)
		{
			objI2CDrv.Drv_I2CWordWrite(IinvARevAddr, objMonInterface.m_i16KsIinvaSet);
			objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh 
				= (objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0xFDFF);
		}
		else if((objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0x0400) == 0x0400)
		{
			objI2CDrv.Drv_I2CWordWrite(IinvBRevAddr, objMonInterface.m_i16KsIinvbSet);
			objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh 
				= (objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0xFBFF);
		}
		
		else if((objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0x0800) == 0x0800)
		{
			objI2CDrv.Drv_I2CWordWrite(IinvCRevAddr, objMonInterface.m_i16KsIinvcSet);
			objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh
				= (objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0xF7FF);
		}
		
		else if((objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0x1000) == 0x1000)
		{
			objI2CDrv.Drv_I2CWordWrite(VbpARevAddr, objMonInterface.m_i16KsVbpaSet);
			objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh 
				= (objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0xEFFF);
		}
		
		else if((objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0x2000) == 0x2000)
		{
			objI2CDrv.Drv_I2CWordWrite(VbpBRevAddr, objMonInterface.m_i16KsVbpbSet);
			objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh
				= (objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0xDFFF);
		}
		
		else if((objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0x4000) == 0x4000)
		{
			objI2CDrv.Drv_I2CWordWrite(VbpCRevAddr, objMonInterface.m_i16KsVbpcSet);
			objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh 
				= (objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh & 0xBFFF);
		}				
			
		
}

/************************************************************************************
函数名：	App_ParamRead()	
功能描述：	读出以u16_Addr为首地址的1个校正参数		
函数调用：	objI2CDrv.Drv_I2CRead();
***********************************************************************************/
inline	UINT16 	Class_EEPROM::App_ParamRead(UINT16 u16Addr)
{
	UINT16 u16ParamTemp;
	
	objI2CDrv.Drv_I2CRead(u16Addr, 1, &u16ParamTemp);
	
	return(u16ParamTemp);						
}

/************************************************************************************
函数名：	App_HistoryRecordW()	
功能描述：	把*pSource指向区域的4字历史记录数据写入到缓冲区中		
函数调用：	objI2CDrv.Drv_I2CRead();
***********************************************************************************/

void 	Class_EEPROM::App_HistoryRecordW(UINT16 *pSource)
{
	INT16 i16Temp;
	
	for(i16Temp = 0;i16Temp < HistoryWords;i16Temp++)
	{
		*(HistoryBuffW + i16Temp) = *(pSource + i16Temp);
	}

	m_i16HistoryWriteCnt = m_i16HistoryWriteCnt + HistoryWords;
	
	if(HistoryBuffW == &objMonInterface.m_u16HistoryRecord[252])
	{
		HistoryBuffW = objMonInterface.m_u16HistoryRecord;
	}
	else
	{
		HistoryBuffW = HistoryBuffW + HistoryWords;
	}
	
}

/************************************************************************************
函数名：	App_HistoryRecordW()	
功能描述：	把历史记录缓冲区中的数据写入到EEPROM中(每次写4字,保证记录的完整性)		
函数调用：	objI2CDrv.Drv_I2CPageWrite();
			objI2CDrv.Drv_I2CWordWrite()
***********************************************************************************/

void		Class_EEPROM::App_HistoryEEPROMW(void)
{
	UINT16	u16HistoryAddrW;
	UINT16	u16Temp[2];
	UINT16	*pSource;
	
	if(m_i16HistEEPROMFrashCnt !=10)
	{
		u16HistoryAddrW = HistoryAddr + (m_u16HistoryNextAddrW << 3);
			
		objI2CDrv.Drv_I2CPageWrite(u16HistoryAddrW, HistoryWords, HistoryBuffR);

		if(HistoryBuffR == &objMonInterface.m_u16HistoryRecord[252])
		{
			HistoryBuffR = objMonInterface.m_u16HistoryRecord;
		}
		else
		{
			HistoryBuffR = HistoryBuffR + HistoryWords;
		}

		m_i16HistoryWriteCnt = m_i16HistoryWriteCnt - HistoryWords;	
		m_u16HistoryCurrCnt++;
		if(m_u16HistoryCurrCnt >= 513)
		{
			m_u16HistoryCurrCnt = 512;
		}
		
		m_u16HistoryNextAddrW++;
		if(m_u16HistoryNextAddrW >= 512)
		{
			m_u16HistoryNextAddrW = 0;
		}
		m_i16HistEEPROMFrashCnt++;	
	}
	else															//每记录10条历史记录更新一次历史记录保存的条数和地址
	{
		m_i16HistEEPROMFrashCnt = 0;
		u16Temp[0] = m_u16HistoryNextAddrW;
		u16Temp[1] = m_u16HistoryCurrCnt;
		pSource = u16Temp;
				
		objI2CDrv.Drv_I2CPageWrite(HistoryRecordAddr, 2, pSource);	
	}						
}
	
/************************************************************************************
函数名：	App_HistoryRead()	
功能描述：	读出以0x0800为首地址的Num条历史记录,Num<=128	
函数调用：	objI2CDrv.Drv_I2CRead			
***********************************************************************************/
void		Class_EEPROM::App_HistoryRead(void)
{
	UINT16 	u16HistoryAddr;
	UINT16	*HistoryBuffWrite;				//写历史记录缓冲区指针(用于从EEPROM读取数据写入缓冲区)
	INT16	i16Num;
	INT16	i16Temp, i16Temp1;
	
	//while((m_i16HistoryReadComand == 1)&&(objDigitalIO.Dat_GetIOFlag().bModuleReady == 0))
	//{
		if(m_i16HistoryReady == 0)
		{	
			
			if(m_u16HistoryCurrCnt >= 512)
			{
				if(m_i16HistoryReadCnt <=8)    //历史记录存满512条后，每次将读取512条记录，一次读取64条，读8次
				{
					HistoryBuffWrite = objMonInterface.m_u16HistoryRecord;
					i16Num = 256;
					
					if(m_i16HistoryReadCnt == 0)
					{
						m_u16HistoryNextAddrR = objEEPROM.App_ParamRead(HistoryRecordAddr);
            	    	
						if(m_u16HistoryNextAddrR == 0)
						{
							m_u16HistoryNextAddrR = 512;				//历史记录从最新的开始读取
						}
					}
					while(i16Num > 0)
					{					
						u16HistoryAddr = HistoryAddr + ((m_u16HistoryNextAddrR - 1) << 3);		//历史记录由最新的到着读取到最老的记录		
						
						objI2CDrv.Drv_I2CRead(u16HistoryAddr, HistoryWords, HistoryBuffWrite);
            	 		
						m_u16HistoryNextAddrR -- ;
    								
						if(m_u16HistoryNextAddrR == 0)
						{
							m_u16HistoryNextAddrR = 512;
						}						    	    	    	
						i16Num = i16Num - HistoryWords;	
							
						HistoryBuffWrite = HistoryBuffWrite + HistoryWords;
										
					}
					m_i16HistoryReadCnt++;
					m_i16HistoryReady = 1;					//64条历史记录准备好					
				}
				else
				{
					m_i16HistoryReadCnt = 0;
					m_i16HistoryReadComand = 0;
				}				
			}
			else													//历史记录未记满512条记录
			{
				//计算共需读取多少次
				i16Temp = m_u16HistoryCurrCnt/64;
				i16Temp1 = m_u16HistoryCurrCnt % 64;
				if(i16Temp1 != 0)
				{
					i16Temp = i16Temp + 1;
				}			
				
				if(m_i16HistoryReadCnt < i16Temp)
				{
					HistoryBuffWrite = objMonInterface.m_u16HistoryRecord;
            	
					//计算每次需读取的字数
					if((m_i16HistoryReadCnt == (i16Temp -1)) && (i16Temp1 != 0))
					{
						i16Num = i16Temp1<<2;
					}
					else
					{
						i16Num = 256;	
					}	
            	
					if(m_i16HistoryReadCnt == 0)
					{
						m_u16HistoryNextAddrR = objEEPROM.App_ParamRead(HistoryRecordAddr);
            		
						if(m_u16HistoryNextAddrR == 0)
						{
							m_u16HistoryNextAddrR = 512;
						}
					}
    	    		
					while(i16Num > 0)
					{					
						u16HistoryAddr = HistoryAddr + ((m_u16HistoryNextAddrR - 1) << 3);				
						
						objI2CDrv.Drv_I2CRead(u16HistoryAddr, HistoryWords, HistoryBuffWrite);
            		 	
						m_u16HistoryNextAddrR -- ;
    								
						if(m_u16HistoryNextAddrR == 0)
						{
							m_u16HistoryNextAddrR = 512;
						}						
    	    		
						i16Num = i16Num - HistoryWords;	
							
						HistoryBuffWrite = HistoryBuffWrite + HistoryWords;
											
					}    	    		
					m_i16HistoryReady = 1;			
					m_i16HistoryReadCnt ++;
				}
				else
				{
					m_i16HistoryReadCnt = 0;
					m_i16HistoryReadComand  = 0;
				}
			}		
				
		}							    	    									
}
/************************************************************************************
函数名：	App_FaultEEPROMW()	
功能描述：	把故障记录缓冲区中的数据写入到EEPROM中	
函数调用：	objI2CDrv.Drv_I2CPageWrite();
			objI2CDrv.Drv_I2CWordWrite()
			objI2CDrv.Drv_I2CWriteAll()
***********************************************************************************/
void 	Class_EEPROM::App_FaultEEPROMW(void)
{
	UINT16 *pTemp;
	UINT16  u16Temp0[2];
	UINT16 *pSource;
	
	if(m_i16FaultWriteCnt > 0)
	{
		pTemp = objI2CDrv.Drv_I2CWriteAll(FaultAddrW, FaultWriteCnt, FaultBuffR);

		FaultBuffR = pTemp;
		
		if(m_u16FaultNextAddrW == 0x4000)
		{
			m_u16FaultNextAddrW = FaultRecordAddr;
		}
	}
	
	else
	{
		FaultBuffR = uFaultRecord;			//写指针重新指回写缓冲区的首地址
		
		m_u16FaultAddrNext = m_u16FaultNextAddrW;			//保存当前故障记录的保存地址
		
		m_u16FaultCurrCnt++;				
		if(m_u16FaultCurrCnt <= 40)							//故障记录未记满40条记录时保存记录的地址和记录条数
		{
			u16Temp0[0] = m_u16FaultAddrNext;
			u16Temp0[1] = m_u16FaultCurrCnt;
			pSource = u16Temp0;
				
			objI2CDrv.Drv_I2CPageWrite(FaultAddr, 2, pSource);
		}
		else
		{
			objI2CDrv.Drv_I2CWordWrite(FaultAddr, m_u16FaultAddrNext);
			m_u16FaultCurrCnt = 40;
		}
		
		objSystem.m_st_wFlagSystem.bRecordSaveRequest = 0;
	}
}
/************************************************************************************
函数名：	App_FaultReadTaskInit()	
功能描述：	读取故障记录任务初始化(由监控设置命令时调用)	
函数调用：	
***********************************************************************************/	
void		Class_EEPROM::App_FaultReadTaskInit(INT16 i16FaultNum)					
{
	UINT16	u16_NumPointerReset;	

	u16_NumPointerReset = (m_u16FaultAddrNext-FaultRecordAddr)/FaultBytesPer;		//距离指针循环边界的故障记录条数
	if(i16FaultNum > u16_NumPointerReset)
	{
		m_u16FaultAddrR = 0x4000 - ((i16FaultNum- u16_NumPointerReset)<< 8);
	}
	else
	{	
		m_u16FaultAddrR = m_u16FaultAddrNext - (i16FaultNum << 8);			//读取最新的第i16_FaultNum 条故障记录
	}
		
	FaultBuffW = u16FaultRecordRead;
	m_i16WordsCnt = 128;
}

/************************************************************************************
函数名：	App_FaultRead()	
功能描述：	从EEPROM读出指定的1条故障记录,存入缓冲区	
函数调用：	objI2CDrv.Drv_I2CRead
***********************************************************************************/

void		Class_EEPROM::App_FaultRead(void)
{
	if(m_i16FaultReadReady == 0)
	{	
		if(m_i16WordsCnt > 0)
		{
			if(m_i16WordsCnt >= MaxWordsRead)
			{
				objI2CDrv.Drv_I2CRead(m_u16FaultAddrR, MaxWordsRead, FaultBuffW);

				m_u16FaultAddrR = m_u16FaultAddrR + (MaxWordsRead << 1);
				FaultBuffW = FaultBuffW + MaxWordsRead;

				m_i16WordsCnt = m_i16WordsCnt - MaxWordsRead;
		
			}
			else 
			{
				objI2CDrv.Drv_I2CRead(m_u16FaultAddrR, m_i16WordsCnt, FaultBuffW);
				m_i16WordsCnt = 0;
			}
		}
		else if(m_i16WordsCnt == 0)
		{
			m_i16FaultReadReady = 1;			
			m_i16FaultReadComand = 0;
		}
		
	}	
}
/************************************************************************************
函数名：	App_EEPROMAction()	
功能描述：	EEPROM读写操作	
函数调用：	objI2CDrv.Drv_I2CPageWrite()
***********************************************************************************/
void	 Class_EEPROM::App_EEPROMAction(void)
{
	UINT16	u16AddrTemp[4];
	UINT16	*pAddrSource;
	
	if(objSystem.m_st_wFlagSystem.bTimer1ms == 1)
	{
		m_i16t1msCnt++;
		
		if(m_i16t1msCnt>5)
		{
			m_i16t1msCnt = 0;
			
			
			if(objSystem.m_st_wFlagFault.bAuxPS2Fault  == 1)
			//if(m_i16_FaultWNotOver == 0)
			
			{
				u16AddrTemp[0] = m_u16HistoryNextAddrW;
				u16AddrTemp[1] = m_u16HistoryCurrCnt;
				u16AddrTemp[2] = m_u16FaultAddrNext;
				u16AddrTemp[3] = m_u16FaultCurrCnt;
				pAddrSource = u16AddrTemp;

				objI2CDrv.Drv_I2CPageWrite(0x0200, 4, pAddrSource);				
			}

			else if(objMonInterface.m_st_wInvReceiveMonitor.m_st_InvSetInfo.u16SetParamFresh != 0)								//等待写的数据个数大于0
			{
				objEEPROM.App_ParamEEPROMW();
			}

			else if(m_i16HistoryWriteCnt > 0)
			{
				objEEPROM.App_HistoryEEPROMW();
			}

			else if(objSystem.m_st_wFlagSystem.bRecordSaveRequest == 1)
			{
				objEEPROM.App_FaultEEPROMW();
			}

			else if(m_i16FaultReadComand == 1)
			{
				objEEPROM.App_FaultRead();
			}
		}


	}

}
//============================================================
// End of file.
//============================================================
