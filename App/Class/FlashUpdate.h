/*==========================================================*/
/* Title		:	FlashUpdate.h							*/
/* Description	: 	Class_FlashUpdate definition				*/
/* Date			:	Dec.2009								*/
/*==========================================================*/
#ifndef FLASHUPDATE_H

#define FLASHUPDATE_H

//status
#define		KERNEL_READY_FOR_ERASE				0x6c
#define		KERNEL_READY_FOR_DATA_RX			0x6e
#define		KERNEL_FLASH_NOT_ERASED			   	0x70
#define		KERNEL_PROGRAM_COMPLETE				5
#define		KERNEL_BLOCK_HEAD_OK					0
#define		KERNEL_BLOCK_SIZE_ERROR				10
#define		KERNEL_BLOCK_ADDR_ERROR				20
#define		KERNEL_DATA_CHKSUM_OK				0x82
#define		KERNEL_DATA_CHKSUM_ERROR			0x83

#define		KERNEL_ERASE_ALLFLASH				6


/*---------------------------------------------------------------------------
  Data buffer used for serial flash programming:  
---------------------------------------------------------------------------*/
#define  	WORDS_IN_FLASH_BUFFER 0x400               // 1024 Words

//Modify the follow keys for your specific firmware protection                      
#define 	PRG_Key0  	0xFFFF  		//PSWD bits 15-0
#define 	PRG_Key1  	0xFFFF  		//PSWD bits 31-16
#define 	PRG_Key2  	0xFFFF  		//PSWD bits 47-32
#define 	PRG_Key3  	0xFFFF  		//PSWD bits 63-48
#define 	PRG_Key4  	0xFFFF  		//PSWD bits 79-64
#define 	PRG_Key5  	0xFFFF 		//PSWD bits 95-80
#define 	PRG_Key6  	0xFFFF  		//PSWD bits 111-96 
#define 	PRG_Key7  	0xFFFF  		//PSWD bits 127-112

//信息帧类型
#define	FRAME_HANDSHAKE 0x01
#define	FRAME_CSMUNLOCKSTATUS 0x02
#define	FRAME_APIVERSIONSTATUS 0x03
#define	FRAME_ERASE 0x04
#define	FRAME_PROGRAM 0x05
#define	FRAME_BLOCKHEAD 0x06
#define	FRAME_BLOCKDATA 0x07
#define	FRAME_CHECKSUM 0x08
#define	FRAME_PROGRAMSTATUS 0x09
#define	FRAME_VERIFYSTATUS 0x0A
#define	FRAME_ERROR 0x0

//For destination address validation
#define 	FLASH_START_ADDR  0x3E8000
#define 	FLASH_END_ADDR    0x3F7FFF
#define	FLASH_PROGRAM_START	0x3E8000
#define	FLASH_PROGRAM_END	0x3F3FFF

// fort toggle test on GPIO34
//#define GPBTOGGLE      (volatile UINT32*)0x00006FCE		//GPIO B toggle
//#define GPIO34_MASK	  (UINT32)0x00000004				//GPIO34 mask



//监控ID
#define	MONITOR_ID 0x01
#define	BROADCAST_ID 0x3F

struct StructFlashUpdateFlag
{
	UINT16	bRecValid		:1;		// BIT0 数据接收有效
	UINT16	bModuleIDLock	:1;		// BIT1 1- moduleID获取成功
	UINT16	bFlashErased	:1;		// BIT2 FlashErase完成	
	UINT16	bReserved		:13;		// BIT3-BIT15 
};

struct StructFlashVariable
{	INT16	m_i16_FlashClockMissing;							//时钟丢失标志位
	UINT16	m_u16_RamCheckErr;							// RAM自检错误标志
	UINT16 	u16_FrameType;			// 帧类型
	UINT16	u16_EraseType;			// 擦除类型:6-全擦;5-擦BCD
	UINT16	u16_BlockSize;
	UINT16	u16_APIVersion;		
	UINT16	u16_BlockData[3];			// 每帧接收到的有效数据
	UINT16	u16_MailBoxDLC;			// 每帧数据的字数(除DestID,SeviceCode)
	UINT16	u16_ModuleID;			// 模块通讯的ID
	UINT16	u16_HandShakeData;		// CAN通讯握手数据
	UINT16	u16_BlockSeqNum;		// 完成Block的数量
	UINT32  	u32_BlockAddr;
	UINT32  	u32_CheckSumRec;		// 接收校验和
	UINT32  	u32_CheckSumDSP;		// DSP计算的校验和	
	union 	ExInBReg st_wSPIDataIn2_Flash;
	struct StructFlashUpdateFlag  st_FlashUpdateFlag;
	UINT16 *	pDestBuffer;
	UINT16* 	p_SourceAddr;
	UINT16* 	p_SourceEndAddr;
	UINT16* 	p_DestAddr;
	FLASH_ST 	FlashStatus;
	UINT16*  	p_Flash;
	UINT16  	FlashBuffer1[1];					// 写程序分支标志缓冲区
	FLASH_ST 	FlashStatus1;  				// program api函数参数
	UINT16 		u16_Status;
};

extern Uint16 Flash28_API_LoadStart;
extern Uint16 Flash28_API_LoadEnd;
extern Uint16 Flash28_API_RunStart;

// 全局变量定义
#pragma DATA_SECTION(".flashdata");
UINT16  	FlashBuffer[WORDS_IN_FLASH_BUFFER];


#pragma DATA_SECTION(".flashdata");
struct StructFlashVariable st_FlashVariable;

//============================================================
//	Class_FlashUpdate与外部的接口
//	1)
//	2)
//	3)调用
//	4)调用
//============================================================
class Class_FlashUpdate
{
	private:	
		void	InitPeripheralClock(void);
	public:
		
		void	Drv_InitPll(Uint16 clock_source, Uint16 imult, Uint16 fmult, Uint16 divsel);
		void	Drv_InitSysCtrl(void);
		void	Drv_InitPieCtrl(void);
		void	Drv_InitPieVectTable(void);

		void	Drv_SPIInit(void);
		void	Drv_GPIOInit(void);
		void	CPUSELConfig(void);
		void	Drv_PWMInit(void);
		void	Drv_eCANAInit(void);
		void	Drv_SystemInit(void);
		void	Drv_RamCheck(void);

		void	App_FlashAPIInit(void);
		void 	App_FlashUpdateMain(void);			//flashUpdate 主程序
		UINT16 	App_Flash_CsmUnlock(void);		// 解密函数

		void 	App_Delay1ms (void);
		void 	App_Delay20ms (void);
		void 	Dat_MBox_Read(void);		// 读取邮箱数据
		void  	Dat_MBox_Send(UINT16 u16_SerCode, UINT16 u16_Data);  //发送数据
		void	App_ModuleIDGet(void);		// 从SPI获取ModuleID
		void	App_DSPRestart(void);			// dsp重启		

		
		void 	MainSwitch(void);	//所有程序入口
		void	FlagProgram(void);
		
};
#endif
//============================================================
// End of file.
//===========================================
