/*==========================================================*/ 	
/* Title		:	FaultRecord.h													*/ 	
/* Description	: 	Class_FaultRecord definition										*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#ifndef      FAULTRECORD_H
        
#define      FAULTRECORD_H

//���ϼ�¼
#define 	FLAG_FAULT_MASK		0x0E51	//Bit0  ���������*/
										//Bit4 	ĸ�߹�ѹ*/
										//Bit6	��·����*/
										//Bit9	�������*/
										//Bit10	��������ʱ��*/
										//Bit11	IGBT��������*/

#define 	FLAG_PROMPT_MASK	0xB01D	//Bit0  ĸ�ߵ�ѹ�쳣�ػ�*/
										//Bit2  ���������ת��·*/
										//Bit3  ϵͳ������ת��·*/
										//Bit4  ������������*/
										//Bit12 ��������·*/
										//Bit13 �����ѹ�쳣�澯*/
										//Bit15 POW_OK����*/


#define		FLAG_BREAKER_MASK	0xF080	//Bit7    ��·���ౣ��*/
										//Bit12   ��·��բ�ܶ�·*/
										//Bit13   ��·��բ�ܶ�·*/
										//Bit14   ��侧բ�ܶ�·*/
										//Bit15   ��侧բ�ܶ�·*/
										
#define		FLAG_STATUS_MASK	0x0900	//Bit8   EPO*/
										//Bit11  �����ֹ*/


	class Class_FaultRecord
	{
	public:
		UINT16			m_u16wClkCntPer14ms;
		
		 INT16			m_i16CurrRecordGrp;				//��ǰ��¼��:32��4  0 1 2 3 
	private:
		 
		 
		
		INT16			m_i16TotalRecordGrp;      	
		INT16			m_i16CurrRecordGrpBuff;
		INT16			m_i16FaultAriseinTx;
		
		void			App_FaultRecordTaxsis(void);
	public:
		inline	void			App_FaultRecordVarInit(void);
		void			App_FaultRecord(void);
	
	};

	
	#pragma DATA_SECTION(".FaultRecord") 
	UINT16	uFaultRecord[128];
		
	#pragma DATA_SECTION(".FaultRecord") 
	UINT16	uFaultRecordBuff[128];

#endif
//===========================================================================
// End of file.
//===========================================================================
