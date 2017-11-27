/*==========================================================*/
/* Title		:	I2CDrv.cpp								*/
/* Description	: 	Class_ADCDrv realization 				*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#include "I2CDrv.h"
/****************************************************************************
�������ƣ�Drv_I2CInit()
���������� I2C��ʼ��
�������ã���
****************************************************************************/
void	Class_I2CDrv::Drv_I2CInit(void)
{
	INT16 i16Temp; 
	
	I2caRegs.I2CSAR.bit.SAR = 0x50;									// ����EEPROM Ƭ��ַ
	
	// I2Cʱ�ӷ�Ƶ����ǰʱ��Ϊ400kHz
	I2caRegs.I2CPSC.all = 9;								
	I2caRegs.I2CCLKL = 10;			
	I2caRegs.I2CCLKH = 5;			
	
	I2caRegs.I2CIER.all = 0x0000;								// ���ж�
	I2caRegs.I2CMDR.all = 0x0020;							//ʹ��I2Cģ��
	I2caRegs.I2CFFTX.all = 0x6000;							// ʹ�� FIFO �� TXFIFO
	I2caRegs.I2CFFRX.all = 0x2040;							// ʹ�� RXFIFO,�� RXFFINT, 

	m_i16Pagewords = 32;
  
	for (i16Temp = 0;i16Temp < 10000;i16Temp++)			//�ȴ�10ms����ȷ�ж�����״̬
  	{
		asm( " rpt #99||nop");
  	}	
  	
  //	objSysDrv.Drv_InterruptEnable();		 
}
/****************************************************************************
�������ƣ�Drv_I2CRead()
���������� EEPROM���ݶ�ȡ����
//����ָ���׵�ַu16_Addr �е�i16_Num �� 16λ���ݴ���*pDestָ��Ļ�����
�������ã���
****************************************************************************/
	
void	Class_I2CDrv::Drv_I2CRead(UINT16 u16Addr, INT16 i16Num, UINT16 *pDest)
{
	INT16	i16Temp0,i16Temp1,i16Temp2,i16DataCnt;
	UINT16	 u16DataL,u16DataH;		//��Ŷ��������ݵ���ʱ����
	i16Temp1 = 0;
	i16Temp2 = 0;
		
	i16DataCnt = i16Num << 1;
	
	I2caRegs.I2CCNT = 2;									//����2���ֽڵĵ�ַ	
    I2caRegs.I2CDXR.bit.DATA = u16Addr >> 8;
   	I2caRegs.I2CDXR.bit.DATA = u16Addr & 0x00FF;
   	I2caRegs.I2CMDR.all = 0x6620;							//I2Cģ������Ϊ��master,��ֹͣλ�ķ���	 
   	
    while (I2caRegs.I2CSTR.bit.ARDY != 1) 					//�Ĵ������ɶ�
    { 
    	i16Temp1 ++;    	
		if (i16Temp1 > 5000) 
    	{	
    		return;										//���ش���				
    	}
    }
	     
	I2caRegs.I2CCNT = i16DataCnt;							//����2���ֽڵ�����	    
	I2caRegs.I2CMDR.all = 0x6C20;							//I2Cģ������Ϊ��master,��ֹͣλ�Ľ���
								        		
	while(I2caRegs.I2CMDR.bit.STP == 1)					//���ݽ���û�н���
   	{ 
    	i16Temp2 ++;   		
		if (i16Temp2 > 5000) 
		{
			return;
		}
   	}
   	   		
	for (i16Temp0 = 0;i16Temp0 < i16Num;i16Temp0 ++)
	{
    	u16DataH = I2caRegs.I2CDRR.bit.DATA;							//�������ݵĸ�8λ
   		u16DataL = I2caRegs.I2CDRR.bit.DATA;							//�������ݵĵ�8λ
    
    	*(pDest + i16Temp0) = (u16DataH << 8) | u16DataL;			//���ص�16λ����
	}
    
	I2caRegs.I2CSTR.bit.ARDY = 1;							//��Ĵ����ɶ���־λ			
}

/****************************************************************************
�������ƣ�Drv_I2CRead()
���������� EEPROM������д�뺯��
//д��16λ����u16_Data ��ָ����ַu16_Addr
�������ã���
****************************************************************************/
	
void	Class_I2CDrv::Drv_I2CWordWrite(UINT16 u16Addr, UINT16 u16Data)
{	
	I2caRegs.I2CCNT = 4;									//����4���ֽ�		
	I2caRegs.I2CDXR.bit.DATA = u16Addr >> 8;						//����16λ�ĵ�ַ�����ݣ��ȷ���8λ���󷢵�8λ
	I2caRegs.I2CDXR.bit.DATA = u16Addr & 0x00FF;
	I2caRegs.I2CDXR.bit.DATA = u16Data >> 8;
	I2caRegs.I2CDXR.bit.DATA = u16Data & 0x00FF;
		
	I2caRegs.I2CMDR.all = 0x6E20;							//I2Cģ������Ϊ��master,��ֹͣλ�ķ���
}

/****************************************************************************
�������ƣ�Drv_I2CRead()
���������� EEPROMҳ����д�뺯��
			��*pSourceָ��Ļ�������i16_Num(<=7)�����ݴ�����u16_Addr Ϊ�׵�ַ��EEPROM�ռ�
�������ã���
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
�������ƣ�Drv_I2CRead()
���������� EEPROMҳ����д�뺯��
		��*pSourceָ��Ļ�������*pNum(����)�����ݴ�����*pStartAddr Ϊ�׵�ַ��EEPROM�ռ�,�����ظı���pSource
�������ã���
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

