/*==========================================================*/ 	
/* Title		:	FaultRecord.h													*/ 	
/* Description	: 	Class_FaultRecord definition										*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#ifndef      FAULTRECORD_H
        
#define      FAULTRECORD_H

//故障记录
#define 	FLAG_FAULT_MASK		0x0E51	//Bit0  逆变器故障*/
										//Bit4 	母线过压*/
										//Bit6	旁路过流*/
										//Bit9	输出过载*/
										//Bit10	逆变过载延时到*/
										//Bit11	IGBT过流保护*/

#define 	FLAG_PROMPT_MASK	0xB01D	//Bit0  母线电压异常关机*/
										//Bit2  本机逆变冲击转旁路*/
										//Bit3  系统请求冲击转旁路*/
										//Bit4  并机均流故障*/
										//Bit12 逆变输出短路*/
										//Bit13 输出电压异常告警*/
										//Bit15 POW_OK故障*/


#define		FLAG_BREAKER_MASK	0xF080	//Bit7    旁路反灌保护*/
										//Bit12   旁路晶闸管断路*/
										//Bit13   旁路晶闸管短路*/
										//Bit14   逆变晶闸管断路*/
										//Bit15   逆变晶闸管短路*/
										
#define		FLAG_STATUS_MASK	0x0900	//Bit8   EPO*/
										//Bit11  输出禁止*/


	class Class_FaultRecord
	{
	public:
		UINT16			m_u16wClkCntPer14ms;
		
		 INT16			m_i16CurrRecordGrp;				//当前记录号:32×4  0 1 2 3 
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
