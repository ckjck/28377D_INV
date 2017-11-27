/*==========================================================*/
/* Title		:	I2CDrv.h								*/
/* Description	: 	Class_RAMDrv definition					*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#ifndef		I2CDRV_H 
             
#define     I2CDRV_H

#define	MaxPageBytes	64				//每页最大能写字节数
#define	MaxWordsWrite	7				//每次写操作最多能写的数据字数

class Class_I2CDrv
{
	private:
		INT16	 m_i16Pagewords;							//每页EEPROM能写的字数
				
	public:
		void	Drv_I2CInit(void);								//初始化
		
		//读出指定首地址u16_Addr 中的i16_Num (i16_Num<=8)个 16位数据存入pDest指向的缓冲区	
		void		Drv_I2CRead(UINT16 u16Addr, INT16 i16Num, UINT16 *pDest);
		
		//写入16位数据u16_Data 到指定地址u16_Addr				
		void		Drv_I2CWordWrite(UINT16 u16Addr, UINT16 u16Data);	

		//把*pSource指向的缓冲区的i16_Num(<=7)个数据存入以u16_Addr 为首地址的EEPROM空间
		void		Drv_I2CPageWrite(UINT16 u16Addr, INT16 i16Num, UINT16 *pSource);	
		
		//把*pSource指向的缓冲区的*pNum(任意)个数据存入以*pStartAddr 为首地址的EEPROM空间,并返回改变后的pSource
		UINT16  *Drv_I2CWriteAll(UINT16 *pStartAddr, INT16 *pNum, UINT16 *pSource);		
};
#endif



