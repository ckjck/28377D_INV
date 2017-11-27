/***************************************************************************************
* 
* Filename: 		SysCanHal.h
* Contributor:
* Author: 		Wang Zhihua	
* Date: 			1/21/2008
* Copyright 		2008 ENPC Corporation, All Rights Reserved
*
* Description: 	
* 
* 
* Version
* Last rev by:		Wang Zhihua
* Last rev date:	1/21/2008
* Last rev desc:	
****************************************************************************************/

#ifndef SYS_CAN_HAL_H
#define SYS_CAN_HAL_H

//Private Enumerated and Structure Definitions
#define MAIL_BOX4 4
#define MAIL_BOX5 5
#define MAIL_BOX1 1

//-----------------------------------------------------------------------------
//Macro definition and enum
	enum SET_PARAR_TYPE
	{
		SYS_BASIC_SET_PARA = 0,
		SYS_MON_SET_PARA,
		SYS_BATT_SET_PARA,
		SYS_DSP_SET_PARA,
		REC_SET_PARA,
		INV_SET_PARA,
		SET_PARA_EOL
	};

	//CANMC definitions.
	enum 
	{
		CANMC_SCB_SCC_MODE = 0, 
		CANMC_SCB_ECAN_MODE
	};
	enum 
	{
		CANMC_DBO_BIG_ENDIAN = 0, 
		CANMC_DBO_LITTLE_ENDIAN
	};
	enum 
	{
		CANMC_SUSP_SOFT_MODE = 0, 
		CANMC_SUSP_FREE_MODE
	};
	
	//CANBTC definitions.
	enum 
	{
		CANBTC_SAM_ONCE_SAMPLE = 0, 
		CANBTC_SAM_THREE_SAMPLES
	};
	
	//CANMD defintions.  (Mailbox Direction)
	enum 
	{
		CANMD_XMIT = 0, 
		CANMD_RECV
	};
	
	//CANME defintions.  (Mailbox Enable)
	enum 
	{
		CANME_DISABLE = 0, 
		CANME_ENABLE
	};
	
	//CANMIM defintions. (Mailbox Interrupt Mask).
	enum 
	{
		CANMIM_DISABLED = 0, 
		CANMIM_ENABLED
	};
	
	//CAN Error States
	enum 
	{
		CAN_STATUS_ERROR_ACTIVE = 0,
		CAN_STATUS_ERROR_PASSIVE, 
		CAN_STATUS_ERROR_BUS_OFF
	};

	//CAN frame package
#if (0)
	union Can_ID
	{
		UINT16 CANId;
		struct
		{
			UINT16 bCanSourceID:6;
			UINT16 bCanMsgID:4;
			UINT16 bReserved:6;
		}
	};

	union Can_Data_Head
	{
		UINT16 CANDataHead;
		struct
		{
			UINT16 bDestinationID:6;
			UINT16 bReserved:1;
			UINT16 bFragFlag:1;
			UINT16 bServercode:7;
			UINT16 bRsRqFlag:1;
		}
	};

	typedef struct Can_Frame
	{
	    UINT16 CANErrorStatus;//Error status register
	    union Can_ID CANId;         //CAN ID field
	    UINT16 CANDlc;        //Data length code
	    union Can_Data_Head CANFrameDataHead;
	    ULONG CANData32_L;  //Data fields (little endian format)
	    ULONG CANData32_H;
	} SYS_CAN_FRAME;
#endif 

#if (0)	
//	#pragma pack(2)
	typedef struct Can_Frame
	{
	    UINT16 CANErrorStatus;//Error status register
	    UINT16 CANId;         //CAN ID field
	    UINT16 CANDlc;        //Data length code
	    ULONG CANData32_L;  //Data fields (little endian format)
	    ULONG CANData32_H;
	} SYS_CAN_FRAME;
//	#pragma pack()
#endif

typedef struct Can_Frame
{
	UINT16 CANErrorStatus;//Error status register
	UINT16 CANId;         //CAN ID field
	UINT16 CANDlc;        //Data length code
		
	UINT16 CANDataHead;//the data head of Data fields
	UINT16 CANData1;
	UINT16 CANData2;
	UINT16 CANData3;

	//ULONG CANData32_L;  //Data fields (little endian format)
	//ULONG CANData32_H;
} SYS_CAN_FRAME;


/* --------------------------------------------------- */
/* eCAN Mailbox Registers                               */
/* ----------------------------------------------------*/

/* eCAN Message Data Register low (MDR_L) word definitions */
struct  CANMDL_WORDS {      // bits  description
   Uint16      LOW_WORD:16; // 0:15  
   Uint16      HI_WORD:16;  // 31:16  
};

/* eCAN Message Data Register low (MDR_L) byte definitions */
struct  CANMDL_BYTES {      // bits   description
   Uint16      BYTE3:8;     // 31:24
   Uint16      BYTE2:8;     // 23:16
   Uint16      BYTE1:8;     // 15:8
   Uint16      BYTE0:8;     // 7:0             
};


/* Allow access to the bit fields or entire register */ 

union CANMDL_REG {
   Uint32                all;
   struct CANMDL_WORDS   word;
   struct CANMDL_BYTES   byte;      
};



/* eCAN Message Data Register high  (MDR_H) word definitions */
struct  CANMDH_WORDS {         // bits  description
   Uint16      LOW_WORD:16;    // 0:15  
   Uint16      HI_WORD:16;     // 31:16  
};

/* eCAN Message Data Register low (MDR_H) byte definitions */
struct  CANMDH_BYTES {      // bits   description
   Uint16      BYTE7:8;     // 63:56 
   Uint16      BYTE6:8;     // 55:48
   Uint16      BYTE5:8;     // 47:40    
   Uint16      BYTE4:8;     // 39:32    
};

/* Allow access to the bit fields or entire register */ 
union CANMDH_REG {
   Uint32                  all;
   struct CANMDH_WORDS     word;
   struct CANMDH_BYTES     byte;
};

struct DATA{
	union CANMDL_REG       MDL;
   	union CANMDH_REG       MDH;
};
struct MBOX {
	//! The CAN message identifier used for 11 or 29 bit identifiers.
	uint32_t ui32MsgID;
	
	//! The message identifier mask used when identifier filtering is enabled.
	uint32_t ui32MsgIDMask;
	
	//! This value holds various status flags and settings specified by
	//! tCANObjFlags.
	uint32_t ui32Flags;
	
	//! This value is the number of bytes of data in the message object.
	uint32_t ui32MsgLen;

	struct	DATA	DATA;
};


//------------------------------------------------------------------------------
//Public variable definition
	typedef VOID (*SYS_CAN_XMIT_FCB)(VOID);
	typedef VOID (*SYS_CAN_RECV_FCB)(SYS_CAN_FRAME *pFrame);

//-------------------------------------------------------------------------------
//func prototype definiton

//-------------------------------------------------------------------------
//Class prototype
extern interrupt void ECAN0INTA_ISR(VOID);





#endif









