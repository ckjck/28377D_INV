/*==========================================================*/
/* Title		:	EEPROM.h								*/
/* Description	: 	Class_EEPROM definition					*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#ifndef		EEPROM_H

#define		EEPROM_H


#define 	MaxWordsRead 		4				//每次读操作最多能读出的字数,实际最多为8,考虑对主循环的影响,设为4

#define 	MaxPageWords		32				//每页最多能存储的字数
#define		HistoryWords		4				//每条历史记录信息为4字

#define		ParamAddrBase		0x0400			//EEPROM中校正参数的首地址
#define		HistoryAddr			0x0800				//EEPROM中历史记录的首地址
#define		FaultRecordAddr		0x01800				//EEPROM中故障记录的首地址

#define	VoutARevAddr		ParamAddrBase				//EEPROM中A相输出电压校正参数的地址
#define	VoutBRevAddr		(ParamAddrBase+2)			//EEPROM中B相输出电压校正参数的地址
#define	VoutCRevAddr		(ParamAddrBase+4)			//EEPROM中C相输出电压校正参数的地址
#define	IoutARevAddr		(ParamAddrBase+6)			//EEPROM中A相输出电流校正参数的地址
#define	IoutBRevAddr		(ParamAddrBase+8)			//EEPROM中B相输出电流校正参数的地址
#define	IoutCRevAddr		(ParamAddrBase+10)			//EEPROM中C相输出电流校正参数的地址
#define	VinvARevAddr		(ParamAddrBase+12)			//EEPROM中A相逆变电压校正参数的地址
#define	VinvBRevAddr		(ParamAddrBase+14)			//EEPROM中B相逆变电压校正参数的地址
#define	VinvCRevAddr		(ParamAddrBase+16)			//EEPROM中C相逆变电压校正参数的地址
#define	IinvARevAddr		(ParamAddrBase+18)			//EEPROM中A相逆变电流校正参数的地址
#define	IinvBRevAddr		(ParamAddrBase+20)			//EEPROM中B相逆变电流校正参数的地址
#define	IinvCRevAddr		(ParamAddrBase+22)			//EEPROM中C相逆变电流校正参数的地址
#define	VbpARevAddr			(ParamAddrBase+24)			//EEPROM中A相旁路电压校正参数的地址
#define	VbpBRevAddr			(ParamAddrBase+26)			//EEPROM中B相旁路电压校正参数的地址
#define	VbpCRevAddr			(ParamAddrBase+28)			//EEPROM中C相旁路电压校正参数的地址

#define	HistoryRecordAddr		0x0200				//EEPROM中用于保存下一次写历史记录的地址序号
#define	HistoryRecordCnt		0x0202				//EEPROM中用于保存EEPROM中现有的历史记录条数
#define	FaultAddr				0x0204				//EEPROM中用于保存故障记录下一次写地址
#define	FaultRecordCnt			0x0206				//EEPROM中用于保存EEPROM中现有的故障记录条数
#define	FaultBytesPer			0x100					// 每条故障记录数据占据256字节


//============================================================
//	Class_EEPROM与外部的接口
//	1)调用objTimerDrv.Drv_10msCome
//============================================================
class Class_EEPROM
{
	private:		
		
		UINT16	*HistoryBuffW;
		UINT16	*HistoryBuffR;		
		INT16	m_i16HistoryWriteCnt;			//历史记录待写个数
		INT16	m_i16HistoryWriteFlag;				//写1条历史记录完成标志
		INT16	m_i16HistoryReadCnt;				//已进行读历史记录的次数(每次读取128条)
		UINT16	m_u16HistoryNextAddrW;					//下一次写历史记录的地址序号(用于计算下一次历史记录的写地址)，< 512
		UINT16	m_u16HistoryNum10Cnt;
		UINT16	m_u16HistoryNextAddrR;					//读历史记录的地址序号(用于计算下一次读历史记录的地址)，< 512
					
		INT16	m_i16FaultPagewords;			//故障记录EEPROM每页剩余可写字数
		UINT16	m_u16FaultNextAddrW;				//故障记录EEPROM 写地址
		UINT16	*FaultAddrW;					//指向故障记录EEPROM 写地址的指针
		INT16	*FaultWriteCnt;					//指向故障记录待写字数的指针
		UINT16	m_u16FaultAddrNext;			//故障记录EEPROM 下一次写地址(保存1条完整的故障记录之后)
		UINT16	*FaultBuffR;						//故障记录缓冲区读取指针(用于从FaultBuffer中读取数据写入EEPROM中)
						
		INT16     m_i16t1msCnt;			
		
	public:

		INT16	m_i16HistoryReadComand;		//  1: 监控命令读取历史记录(监控置1)
		INT16	m_i16HistoryReady;			// 1: 128条历史记录读取完成，等待监控读取(监控读取后清零)
		UINT16	m_u16HistoryCurrCnt;					//EEPROM中现有的历史记录条数
				
		INT16	m_i16FaultWriteCnt;			//故障记录待写字数	(Class_Debug)
//		INT16	m_i16FaultWNotOver;				//故障记录缓冲区数据及当前地址写未完成标志:0-写完成,可更新缓冲区(Class_Debug)
		INT16	m_i16FaultReadComand;		//  1: 监控命令读取故障记录(监控置1)
		INT16	m_i16FaultReadReady;			//  1:1条故障记录读取完成，等待监控读取(监控读取后清零)
		UINT16	m_u16FaultCurrCnt;					//EEPROM中现有的故障记录条数
	
		INT16	m_i16HistEEPROMFrashCnt;	//EEPROM中历史记录条数及记录地址更新计数器，每10次更新一次
		INT16	m_i16WordsCnt;				//故障记录读取的字数
		UINT16	*FaultBuffW;						//故障记录缓冲区写指针(用于从EEPROM中读取数据存入缓冲区)
		UINT16	m_u16FaultAddrR;				//故障记录EEPROM读地址
		
	public:			
		void		Dat_EEPROMInit(void);						//初始化

		void		App_ParamEEPROMW(void);					//将缓冲区中的数据写入I2C协议的EEPROM中
		inline	UINT16 	App_ParamRead(UINT16 u16Addr);		//读出以u16_Addr为首地址的1个校正参数,并返回


		void 	App_HistoryRecordW(UINT16 *pSource);		//把*pSource指向区域的4字历史记录数据写入到缓冲区中
		void		App_HistoryEEPROMW(void);				//把历史记录缓冲区中的数据写入到EEPROM中
		void		App_HistoryRead(void);					//读出以0x0800为首地址的Num条历史记录,存入缓冲区，Num<=128

		
		void		App_FaultEEPROMW(void);							//把故障记录缓冲区中的数据写入到EEPROM中
		void		App_FaultRead(void);							//从EEPROM读出以0x1000为首地址的Num个故障记录,存入缓冲区Num<=128
		void		App_FaultReadTaskInit(INT16 i16FaultNum);					//读取故障记录任务初始化(由监控设置命令时调用),指定读取记录的编号	

		void		App_EEPROMAction(void);							//EEPROM读写操作

};
#endif

//============================================================
// End of file.
//============================================================
