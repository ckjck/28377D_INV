/*==========================================================*/ 	
/* Title		:	PowCANProtocol.h								*/ 	
/* Description	: 	Class_PowCANProtocol definition				*/ 	
/* Date			:	Apr.2008								*/ 	
/*==========================================================*/

#ifndef		POWCANPROTOCOL_H 
             
#define     POWCANPROTOCOL_H
//By wxw20090714 增加负载帧的识别ID
/*PowerCan的11位ID定义如下:	Bit0~Bit5	机器号
							Bit6~Bit10	信息帧ID
									1:均流信息帧
									2:本机有功功率帧
									3:本机视在功率帧
									4:预留
									5:公用电池组信息帧
*/
#define	MESSAGEID_PLOAD		0x0002	//有功信息帧ID
#define	MESSAGEID_SLOAD		0x0003	//视在功率信息帧ID
#define	MESSAGEID_STATE		0x0005	//电池及Inv状态帧
#define	MESSAGEID_INVALID_CURRENT	0x0006	//无效DQ电流
#define	MESSAGEID_ILFILTER			0x0007
#define	MESSAGEID_IINVADQCURRENT			0x0008
#define	MESSAGEID_IINVBDQCURRENT			0x0009
#define	MESSAGEID_IINVCDQCURRENT			0x000A


#define	MESSAGEID_MASK		0x07c0	//信息帧ID在整个ID中的位置
 
const	UINT16	UPSID_MASK = 0x003f;			//ups机号在ID中的位置
const	UINT16	MessageID_Power = 0x0001;		//均流信息帧ID
	
	class Class_PowCANProtocol
	{
		public:
			
		private:
								
			UINT16 		m_u16UpsIdx;  
			
		public:
			void 	Dat_Int_MBox_Read(volatile struct MBOX *pMBox, UINT32 u32MBoxIdx);
			
	}; 
	
#endif

//===========================================================================
// End of file.
//===========================================================================

