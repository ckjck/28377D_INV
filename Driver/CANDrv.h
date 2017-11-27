/*==========================================================*/
/* Title		:	CANDrv.h								*/
/* Description	: 	Class_CANDrv definition					*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
 #ifndef		CANDRV_H
            
 #define    	CANDRV_H


 
 //*****************************************************************************
 // This is the maximum number that can be stored as an 11bit Message
 // identifier.
 //*****************************************************************************
#define CAN_MAX_11BIT_MSG_ID    (0x7ff)
 
 //*****************************************************************************
 // This is used as the loop delay for accessing the CAN controller registers.
 //*****************************************************************************
 
 // The maximum CAN bit timing divisor is 13.
#define CAN_MAX_BIT_DIVISOR     (13)
 
 // The minimum CAN bit timing divisor is 5.
#define CAN_MIN_BIT_DIVISOR     (5)
 
 // The maximum CAN pre-divisor is 1024.
#define CAN_MAX_PRE_DIVISOR     (1024)
 
 // The minimum CAN pre-divisor is 1.
#define CAN_MIN_PRE_DIVISOR     (1)
 
 //*****************************************************************************
 // This table is used by the CANBitRateSet() API as the register defaults for
 // the bit timing values.
 //*****************************************************************************
 
 static const uint16_t g_ui16CANBitValues[] =
 {
	 0x1100, // TSEG2 2, TSEG1 2, SJW 1, Divide 5
	 0x1200, // TSEG2 2, TSEG1 3, SJW 1, Divide 6
	 0x2240, // TSEG2 3, TSEG1 3, SJW 2, Divide 7
	 0x2340, // TSEG2 3, TSEG1 4, SJW 2, Divide 8
	 0x3340, // TSEG2 4, TSEG1 4, SJW 2, Divide 9
	 0x3440, // TSEG2 4, TSEG1 5, SJW 2, Divide 10
	 0x3540, // TSEG2 4, TSEG1 6, SJW 2, Divide 11
	 0x3640, // TSEG2 4, TSEG1 7, SJW 2, Divide 12
	 0x3740  // TSEG2 4, TSEG1 8, SJW 2, Divide 13
 };
 

 
 
	class Class_CANDrv
	{
	private:
		bool		CANBaseValid(uint32_t ui32base);
		int32_t 	CANIntNumberGet(uint32_t ui32Base, unsigned char ucNumber);
		void 		CANDataRegWrite(unsigned char *pucData, uint32_t *pui32Register, int16_t iSize);
 		void 		CANDataRegRead(unsigned char *pucData, uint32_t *pui32Register, int16_t iSize);
	
	public:
		void 		Drv_CANInit(void);

		void		CANInit(uint32_t ui32Base);
		void		CANEnable(uint32_t ui32Base);
		void		CANDisable(uint32_t ui32Base);
		void		CANClkSourceSelect(uint32_t ui32Base, uint16_t ui16Source);
		void		CANBitTimingGet(uint32_t ui32Base, tCANBitClkParms * pClkParms);
		uint32_t	CANBitRateSet(uint32_t ui32Base, uint32_t ui32SourceClock, uint32_t ui32BitRate);
		void		CANBitTimingSet(uint32_t ui32Base, tCANBitClkParms * pClkParms);
		void		CANIntRegister(uint32_t ui32Base, unsigned char ucIntNumber, void(* pfnHandler)(void));
		void		CANIntUnregister(uint32_t ui32Base, unsigned char ucIntNumber);
		void		CANIntEnable(uint32_t ui32Base, uint32_t ui32IntFlags);
		void		CANIntDisable(uint32_t ui32Base, uint32_t ui32IntFlags);
		uint32_t	CANIntStatus(uint32_t ui32Base, tCANIntStsReg eIntStsReg);
		void		CANIntClear(uint32_t ui32Base, uint32_t ui32IntClr);
		void		CANRetrySet(uint32_t ui32Base, bool bAutoRetry);
		bool		CANRetryGet(uint32_t ui32Base);
		uint32_t	CANStatusGet(uint32_t ui32Base, tCANStsReg eStatusReg);
		bool		CANErrCntrGet(uint32_t ui32Base, uint32_t * pui32RxCount, uint32_t * pui32TxCount);
		void		CANMessageSet(uint32_t ui32Base, uint32_t ui32ObjID, tCANMsgObject * pMsgObject, tMsgObjType eMsgType);
		void		CANMessageGet(uint32_t ui32Base, uint32_t ui32ObjID, tCANMsgObject * pMsgObject, bool bClrPendingInt);
		void		CANMessageClear(uint32_t ui32Base, uint32_t ui32ObjID);
		void		CANGlobalIntEnable(uint32_t ui32Base, uint32_t ui32IntFlags);
		void		CANGlobalIntDisable(uint32_t ui32Base, uint32_t ui32IntFlags);
		void		CANGlobalIntClear(uint32_t ui32Base, uint32_t ui32IntFlags);
		bool		CANGlobalIntstatusGet(uint32_t ui32Base, uint32_t ui32IntFlags);
				
				
	};

#endif	
	
//===========================================================================
// End of file.
//===========================================================================
