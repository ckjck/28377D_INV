/*==========================================================*/
/* Title		:	I2CDrv.h								*/
/* Description	: 	Class_RAMDrv definition					*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#ifndef		I2CDRV_H 
             
#define     I2CDRV_H

#define	MaxPageBytes	64				//ÿҳ�����д�ֽ���
#define	MaxWordsWrite	7				//ÿ��д���������д����������

class Class_I2CDrv
{
	private:
		INT16	 m_i16Pagewords;							//ÿҳEEPROM��д������
				
	public:
		void	Drv_I2CInit(void);								//��ʼ��
		
		//����ָ���׵�ַu16_Addr �е�i16_Num (i16_Num<=8)�� 16λ���ݴ���pDestָ��Ļ�����	
		void		Drv_I2CRead(UINT16 u16Addr, INT16 i16Num, UINT16 *pDest);
		
		//д��16λ����u16_Data ��ָ����ַu16_Addr				
		void		Drv_I2CWordWrite(UINT16 u16Addr, UINT16 u16Data);	

		//��*pSourceָ��Ļ�������i16_Num(<=7)�����ݴ�����u16_Addr Ϊ�׵�ַ��EEPROM�ռ�
		void		Drv_I2CPageWrite(UINT16 u16Addr, INT16 i16Num, UINT16 *pSource);	
		
		//��*pSourceָ��Ļ�������*pNum(����)�����ݴ�����*pStartAddr Ϊ�׵�ַ��EEPROM�ռ�,�����ظı���pSource
		UINT16  *Drv_I2CWriteAll(UINT16 *pStartAddr, INT16 *pNum, UINT16 *pSource);		
};
#endif



