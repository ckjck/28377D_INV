/*==========================================================*/ 	
/* Title		:	IPC.cpp													*/ 	
/* Description	: 	Class_IPC definition										*/ 	
/* Date		:	Apr.2017														*/ 	
/*==========================================================*/
#include "IPC.h"
void Class_IPC::Drv_IPC_Communication(void)
{
	IPC_DataRead(IPC_RECEIVE_FLAG1, HWRE(ADDRESS_CPU1TOCPU2_A));
	IPC_DataWrite(IPC_TRANSMIT_FLAG1, HWRE(ADDRESS_CPU2TOCPU1_A));

}

//*****************************************************************************
//! ulFlag: IPC_flag bit,after write message in *ulAddress, set flag,flag = 1
//! ilAddress: MSGRAM Address 
//! return status of command (0=success, 1=error)
//
//*****************************************************************************

UINT16 Class_IPC::IPC_DataWrite(UINT32 &ulFlag, UINT32 *ulAddress)
{
	UINT16 returnStatus;

	//
	 // Return false ,if IPC Local to Remote flags are not clear
	//
	if (IpcRegs.IPCFLG.all & ulFlag)
	{
		returnStatus = STATUS_FAIL;
	}
	else
	{
		*ulAddress = m_un_CPU1toCPU2.all;
		//
		// Force IPC event on selected request task and enable status-checking
		//
		IpcRegs.IPCSET.all |= ulFlag;

		returnStatus = STATUS_PASS;
	}

	return returnStatus;
}

 //*****************************************************************************
 //! ulFlag: IPC_flag bit,after read message, clear flag,flag = 0
 //! ilAddress: MSGRAM Address 
 //! return status of command (0=success, 1=error)
 //
 //*****************************************************************************
 UINT16 Class_IPC::IPC_DataRead(UINT32 &ulFlag, UINT32 *ulAddress)
 {
	 uint16_t returnStatus;
 
	 //
	 // Check IPC Flag,if true,remote CPU already write message in *ulAddress
	 //
	 if (IpcRegs.IPCSTS.all & ulFlag)
	 {
		 m_un_CPU2toCPU1.all = *ulAddress;
		 //
		 // acknowledge IPC flag
		 //
		 IpcRegs.IPCACK.all |= ulFlag;
		 returnStatus = STATUS_PASS;
	 }
	 else
	 {

 
		 returnStatus = STATUS_FAIL;
	 }
 
	 return returnStatus;
 }


void	Class_IPC::Dat_CanBustoCPU2(CAN_PACKED_PROTOCOL_U *message)
{	if(IpcRegs.IPCFLG.bit.IPC0 == 0)
	{
		CanBustoCPU2 = *message;
		IpcRegs.IPCSET.bit.IPC0 = 1;
	}
}

void	Class_IPC::Dat_CPU2toCanBus(CAN_PACKED_PROTOCOL_U * message)
{
	if(IpcRegs.IPCSTS.bit.IPC0)
	{
		*message = CPU2toCanBus;
		IpcRegs.IPCACK.bit.IPC0 = 1;
	}
		
}

#ifdef 0
void	Class_IPC::Dat_CanBustoCPU2(CAN_PACKED_PROTOCOL_U *message)
{	if(IpcRegs.IPCSTS.bit.IPC0)
	{
		*message = CanBustoCPU2;
		IpcRegs.IPCACK.bit.IPC0 = 1;
	}
}
void	Class_IPC::Dat_CPU2toCanBus(CAN_PACKED_PROTOCOL_U * message)
{
	if(IpcRegs.IPCFLG.bit.IPC0 == 0)
	{
		CPU2toCanBus = *message;
		IpcRegs.IPCACK.bit.IPC0 = 1;
	}
		
}
#endif

//===========================================================================
// End of file.
//===========================================================================
