/*==========================================================*/
/* Title		:	EEPROM.h								*/
/* Description	: 	Class_EEPROM definition					*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#ifndef		EEPROM_H

#define		EEPROM_H


#define 	MaxWordsRead 		4				//ÿ�ζ���������ܶ���������,ʵ�����Ϊ8,���Ƕ���ѭ����Ӱ��,��Ϊ4

#define 	MaxPageWords		32				//ÿҳ����ܴ洢������
#define		HistoryWords		4				//ÿ����ʷ��¼��ϢΪ4��

#define		ParamAddrBase		0x0400			//EEPROM��У���������׵�ַ
#define		HistoryAddr			0x0800				//EEPROM����ʷ��¼���׵�ַ
#define		FaultRecordAddr		0x01800				//EEPROM�й��ϼ�¼���׵�ַ

#define	VoutARevAddr		ParamAddrBase				//EEPROM��A�������ѹУ�������ĵ�ַ
#define	VoutBRevAddr		(ParamAddrBase+2)			//EEPROM��B�������ѹУ�������ĵ�ַ
#define	VoutCRevAddr		(ParamAddrBase+4)			//EEPROM��C�������ѹУ�������ĵ�ַ
#define	IoutARevAddr		(ParamAddrBase+6)			//EEPROM��A���������У�������ĵ�ַ
#define	IoutBRevAddr		(ParamAddrBase+8)			//EEPROM��B���������У�������ĵ�ַ
#define	IoutCRevAddr		(ParamAddrBase+10)			//EEPROM��C���������У�������ĵ�ַ
#define	VinvARevAddr		(ParamAddrBase+12)			//EEPROM��A������ѹУ�������ĵ�ַ
#define	VinvBRevAddr		(ParamAddrBase+14)			//EEPROM��B������ѹУ�������ĵ�ַ
#define	VinvCRevAddr		(ParamAddrBase+16)			//EEPROM��C������ѹУ�������ĵ�ַ
#define	IinvARevAddr		(ParamAddrBase+18)			//EEPROM��A��������У�������ĵ�ַ
#define	IinvBRevAddr		(ParamAddrBase+20)			//EEPROM��B��������У�������ĵ�ַ
#define	IinvCRevAddr		(ParamAddrBase+22)			//EEPROM��C��������У�������ĵ�ַ
#define	VbpARevAddr			(ParamAddrBase+24)			//EEPROM��A����·��ѹУ�������ĵ�ַ
#define	VbpBRevAddr			(ParamAddrBase+26)			//EEPROM��B����·��ѹУ�������ĵ�ַ
#define	VbpCRevAddr			(ParamAddrBase+28)			//EEPROM��C����·��ѹУ�������ĵ�ַ

#define	HistoryRecordAddr		0x0200				//EEPROM�����ڱ�����һ��д��ʷ��¼�ĵ�ַ���
#define	HistoryRecordCnt		0x0202				//EEPROM�����ڱ���EEPROM�����е���ʷ��¼����
#define	FaultAddr				0x0204				//EEPROM�����ڱ�����ϼ�¼��һ��д��ַ
#define	FaultRecordCnt			0x0206				//EEPROM�����ڱ���EEPROM�����еĹ��ϼ�¼����
#define	FaultBytesPer			0x100					// ÿ�����ϼ�¼����ռ��256�ֽ�


//============================================================
//	Class_EEPROM���ⲿ�Ľӿ�
//	1)����objTimerDrv.Drv_10msCome
//============================================================
class Class_EEPROM
{
	private:		
		
		UINT16	*HistoryBuffW;
		UINT16	*HistoryBuffR;		
		INT16	m_i16HistoryWriteCnt;			//��ʷ��¼��д����
		INT16	m_i16HistoryWriteFlag;				//д1����ʷ��¼��ɱ�־
		INT16	m_i16HistoryReadCnt;				//�ѽ��ж���ʷ��¼�Ĵ���(ÿ�ζ�ȡ128��)
		UINT16	m_u16HistoryNextAddrW;					//��һ��д��ʷ��¼�ĵ�ַ���(���ڼ�����һ����ʷ��¼��д��ַ)��< 512
		UINT16	m_u16HistoryNum10Cnt;
		UINT16	m_u16HistoryNextAddrR;					//����ʷ��¼�ĵ�ַ���(���ڼ�����һ�ζ���ʷ��¼�ĵ�ַ)��< 512
					
		INT16	m_i16FaultPagewords;			//���ϼ�¼EEPROMÿҳʣ���д����
		UINT16	m_u16FaultNextAddrW;				//���ϼ�¼EEPROM д��ַ
		UINT16	*FaultAddrW;					//ָ����ϼ�¼EEPROM д��ַ��ָ��
		INT16	*FaultWriteCnt;					//ָ����ϼ�¼��д������ָ��
		UINT16	m_u16FaultAddrNext;			//���ϼ�¼EEPROM ��һ��д��ַ(����1�������Ĺ��ϼ�¼֮��)
		UINT16	*FaultBuffR;						//���ϼ�¼��������ȡָ��(���ڴ�FaultBuffer�ж�ȡ����д��EEPROM��)
						
		INT16     m_i16t1msCnt;			
		
	public:

		INT16	m_i16HistoryReadComand;		//  1: ��������ȡ��ʷ��¼(�����1)
		INT16	m_i16HistoryReady;			// 1: 128����ʷ��¼��ȡ��ɣ��ȴ���ض�ȡ(��ض�ȡ������)
		UINT16	m_u16HistoryCurrCnt;					//EEPROM�����е���ʷ��¼����
				
		INT16	m_i16FaultWriteCnt;			//���ϼ�¼��д����	(Class_Debug)
//		INT16	m_i16FaultWNotOver;				//���ϼ�¼���������ݼ���ǰ��ַдδ��ɱ�־:0-д���,�ɸ��»�����(Class_Debug)
		INT16	m_i16FaultReadComand;		//  1: ��������ȡ���ϼ�¼(�����1)
		INT16	m_i16FaultReadReady;			//  1:1�����ϼ�¼��ȡ��ɣ��ȴ���ض�ȡ(��ض�ȡ������)
		UINT16	m_u16FaultCurrCnt;					//EEPROM�����еĹ��ϼ�¼����
	
		INT16	m_i16HistEEPROMFrashCnt;	//EEPROM����ʷ��¼��������¼��ַ���¼�������ÿ10�θ���һ��
		INT16	m_i16WordsCnt;				//���ϼ�¼��ȡ������
		UINT16	*FaultBuffW;						//���ϼ�¼������дָ��(���ڴ�EEPROM�ж�ȡ���ݴ��뻺����)
		UINT16	m_u16FaultAddrR;				//���ϼ�¼EEPROM����ַ
		
	public:			
		void		Dat_EEPROMInit(void);						//��ʼ��

		void		App_ParamEEPROMW(void);					//���������е�����д��I2CЭ���EEPROM��
		inline	UINT16 	App_ParamRead(UINT16 u16Addr);		//������u16_AddrΪ�׵�ַ��1��У������,������


		void 	App_HistoryRecordW(UINT16 *pSource);		//��*pSourceָ�������4����ʷ��¼����д�뵽��������
		void		App_HistoryEEPROMW(void);				//����ʷ��¼�������е�����д�뵽EEPROM��
		void		App_HistoryRead(void);					//������0x0800Ϊ�׵�ַ��Num����ʷ��¼,���뻺������Num<=128

		
		void		App_FaultEEPROMW(void);							//�ѹ��ϼ�¼�������е�����д�뵽EEPROM��
		void		App_FaultRead(void);							//��EEPROM������0x1000Ϊ�׵�ַ��Num�����ϼ�¼,���뻺����Num<=128
		void		App_FaultReadTaskInit(INT16 i16FaultNum);					//��ȡ���ϼ�¼�����ʼ��(�ɼ����������ʱ����),ָ����ȡ��¼�ı��	

		void		App_EEPROMAction(void);							//EEPROM��д����

};
#endif

//============================================================
// End of file.
//============================================================
