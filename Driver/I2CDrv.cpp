/*==========================================================*/
/* Title		:	I2CDrv.cpp								*/
/* Description	: 	Class_ADCDrv realization 				*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#include "I2CDrv.h"
/****************************************************************************
函数名称：Drv_I2CInit()
功能描述： I2C初始化
函数调用：无
****************************************************************************/
void	Class_I2CDrv::Drv_I2CInit(void)
{
	INT16 i16Temp; 
	
	I2caRegs.I2CSAR.bit.SAR = 0x50;									// 配置EEPROM 片地址
	
	// I2C时钟分频，当前时钟为400kHz
	I2caRegs.I2CPSC.all = 9;								
	I2caRegs.I2CCLKL = 10;			
	I2caRegs.I2CCLKH = 5;			
	
	I2caRegs.I2CIER.all = 0x0000;								// 关中断
	I2caRegs.I2CMDR.all = 0x0020;							//使能I2C模块
	I2caRegs.I2CFFTX.all = 0x6000;							// 使能 FIFO 和 TXFIFO
	I2caRegs.I2CFFRX.all = 0x2040;							// 使能 RXFIFO,清 RXFFINT, 

	m_i16Pagewords = 32;
  
	for (i16Temp = 0;i16Temp < 10000;i16Temp++)			//等待10ms以正确判断总线状态
  	{
		asm( " rpt #99||nop");
  	}	
  	
  //	objSysDrv.Drv_InterruptEnable();		 
}
/****************************************************************************
函数名称：Drv_I2CRead()
功能描述： EEPROM数据读取函数
//读出指定首地址u16_Addr 中的i16_Num 个 16位数据存入*pDest指向的缓冲区
函数调用：无
****************************************************************************/
	
void	Class_I2CDrv::Drv_I2CRead(UINT16 u16Addr, INT16 i16Num, UINT16 *pDest)
{
	INT16	i16Temp0,i16Temp1,i16Temp2,i16DataCnt;
	UINT16	 u16DataL,u16DataH;		//存放读出的数据的临时变量
	i16Temp1 = 0;
	i16Temp2 = 0;
		
	i16DataCnt = i16Num << 1;
	
	I2caRegs.I2CCNT = 2;									//发送2个字节的地址	
    I2caRegs.I2CDXR.bit.DATA = u16Addr >> 8;
   	I2caRegs.I2CDXR.bit.DATA = u16Addr & 0x00FF;
   	I2caRegs.I2CMDR.all = 0x6620;							//I2C模块配置为：master,无停止位的发送	 
   	
    while (I2caRegs.I2CSTR.bit.ARDY != 1) 					//寄存器不可读
    { 
    	i16Temp1 ++;    	
		if (i16Temp1 > 5000) 
    	{	
    		return;										//返回错误				
    	}
    }
	     
	I2caRegs.I2CCNT = i16DataCnt;							//读回2个字节的数据	    
	I2caRegs.I2CMDR.all = 0x6C20;							//I2C模块配置为：master,有停止位的接收
								        		
	while(I2caRegs.I2CMDR.bit.STP == 1)					//数据接收没有结束
   	{ 
    	i16Temp2 ++;   		
		if (i16Temp2 > 5000) 
		{
			return;
		}
   	}
   	   		
	for (i16Temp0 = 0;i16Temp0 < i16Num;i16Temp0 ++)
	{
    	u16DataH = I2caRegs.I2CDRR.bit.DATA;							//读回数据的高8位
   		u16DataL = I2caRegs.I2CDRR.bit.DATA;							//读回数据的低8位
    
    	*(pDest + i16Temp0) = (u16DataH << 8) | u16DataL;			//读回的16位数据
	}
    
	I2caRegs.I2CSTR.bit.ARDY = 1;							//清寄存器可读标志位			
}

/****************************************************************************
函数名称：Drv_I2CRead()
功能描述： EEPROM字数据写入函数
//写入16位数据u16_Data 到指定地址u16_Addr
函数调用：无
****************************************************************************/
	
void	Class_I2CDrv::Drv_I2CWordWrite(UINT16 u16Addr, UINT16 u16Data)
{	
	I2caRegs.I2CCNT = 4;									//发送4个字节		
	I2caRegs.I2CDXR.bit.DATA = u16Addr >> 8;						//发送16位的地址和数据，先发高8位，后发低8位
	I2caRegs.I2CDXR.bit.DATA = u16Addr & 0x00FF;
	I2caRegs.I2CDXR.bit.DATA = u16Data >> 8;
	I2caRegs.I2CDXR.bit.DATA = u16Data & 0x00FF;
		
	I2caRegs.I2CMDR.all = 0x6E20;							//I2C模块配置为：master,有停止位的发送
}

/****************************************************************************
函数名称：Drv_I2CRead()
功能描述： EEPROM页数据写入函数
			把*pSource指向的缓冲区的i16_Num(<=7)个数据存入以u16_Addr 为首地址的EEPROM空间
函数调用：无
****************************************************************************/

void	Class_I2CDrv::Drv_I2CPageWrite(UINT16 u16Addr, INT16 i16Num, UINT16 *pSource)
{
	INT16 i16DataCnt,i16Temp;

	i16DataCnt = i16Num << 1;
	
	I2caRegs.I2CCNT = i16DataCnt + 2;
	I2caRegs.I2CDXR.bit.DATA = u16Addr >> 8;
	I2caRegs.I2CDXR.bit.DATA = u16Addr & 0x00FF;

	for(i16Temp = 0;i16Temp < i16Num;i16Temp ++)
	{
		I2caRegs.I2CDXR.bit.DATA = *(pSource + i16Temp) >> 8;
		I2caRegs.I2CDXR.bit.DATA = *(pSource + i16Temp) & 0x00FF;
	}
	I2caRegs.I2CMDR.all = 0x6E20;	
}

/****************************************************************************
函数名称：Drv_I2CRead()
功能描述： EEPROM页数据写入函数
		把*pSource指向的缓冲区的*pNum(任意)个数据存入以*pStartAddr 为首地址的EEPROM空间,并返回改变后的pSource
函数调用：无
****************************************************************************/

UINT16  *Class_I2CDrv::Drv_I2CWriteAll(UINT16 *pStartAddr, INT16 *pNum, UINT16 *pSource)
{
	INT16  i16PageWords;
	UINT16	*pSourceTemp;	

	i16PageWords = (MaxPageBytes - (*pStartAddr % MaxPageBytes)) >> 1;
	pSourceTemp = pSource;
	
	if(*pNum >= MaxWordsWrite)
	{
			if(i16PageWords > MaxWordsWrite)
			{
				objI2CDrv.Drv_I2CPageWrite(*pStartAddr, MaxWordsWrite, pSourceTemp);

				pSourceTemp = pSourceTemp + MaxWordsWrite;
				*pStartAddr = *pStartAddr + (MaxWordsWrite << 1);
				*pNum = *pNum -MaxWordsWrite;
										
			}
			else
			{
				objI2CDrv.Drv_I2CPageWrite(*pStartAddr, i16PageWords, pSourceTemp);

				pSourceTemp = pSourceTemp + i16PageWords;			
				*pStartAddr = *pStartAddr + (i16PageWords << 1);				
				*pNum = *pNum - i16PageWords;
						
			}
		}
		else
		{
			if(*pNum < i16PageWords)
			{
				objI2CDrv.Drv_I2CPageWrite(*pStartAddr, *pNum, pSourceTemp);

				pSourceTemp = pSourceTemp + *pNum;			
				*pStartAddr = *pStartAddr + (*pNum << 1);						
				*pNum = 0;					
			}

			else
			{
				objI2CDrv.Drv_I2CPageWrite(*pStartAddr, i16PageWords, pSourceTemp);

				pSourceTemp = pSourceTemp + i16PageWords;
				*pStartAddr = *pStartAddr + (i16PageWords << 1);
				*pNum = *pNum -i16PageWords;
			}
		}

		return(pSourceTemp);
}


//===========================================================================
// End of file.
//===========================================================================

