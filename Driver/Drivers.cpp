/*==========================================================*/
/* Title		:	Drivers.cpp													*/
/* Description	: 	ControlChip definition											*/
/* Date		:	Apr.2008														*/
/*==========================================================*/

#include "Drivers.h"

 Class_ADDrv objADDrv;
 Class_CANDrv objCANDrv;
 Class_I2CDrv objI2CDrv;

 Class_CAPDrv objCAPDrv;
 Class_FlashDrv objFlashDrv;

 Class_GPIODrv objGPIODrv;
 


 Class_PWMDrv objPWMDrv;
 Class_RAMDrv objRAMDrv;


 Class_SCIDrv objSCIDrv;
 Class_SPIDrv objSPIDrv;


 Class_SysDrv objSysDrv;
 Class_TimerDrv objTimerDrv;

 Class_WatchDogDrv objWatchDogDrv;
 Class_IPC		objIPC((CAN_PACKED_PROTOCOL_U*)0x03F800, (CAN_PACKED_PROTOCOL_U*)0x03FC00);

//============================================================
// End of file.
//============================================================
