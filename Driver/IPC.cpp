/*==========================================================*/
/* Title		:	IPC.cpp													*/
/* Description	: 	Class_IPC definition										*/
/* Date		:	Apr.2017														*/
/*==========================================================*/
#include "IPC.h"
/*
Class_IPC*	Class_IPC::m_pClass_IPC = NULL;

Class_IPC*	Class_IPC::GetClass_IPCPtr(void)
{
	if(m_pClass_IPC == NULL)
	{
		m_pClass_IPC = new	Class_IPC((CAN_PACKED_PROTOCOL_U*)0x03F800, (CAN_PACKED_PROTOCOL_U*)0x03FC00);

	}
	return	m_pClass_IPC;
}

Class_IPC::~Class_IPC(void)
{
	delete	m_pClass_IPC;
}
*/
void    Class_IPC::Drv_IPC_ADSample_Receive(UINT16 & mutable_VbusP_0, UINT16 & mutable_VbusN_0)
{

	//receive Vbus_p Vbus_n from REC
    this->IPC_DataRead(IPC_STS_IPC4, HWRE(VBUS_P_CPU2TOCPU1), mutable_VbusP_0);
	this->IPC_DataRead(IPC_STS_IPC5, HWRE(VBUS_N_CPU2TOCPU1), mutable_VbusN_0);

}

//
// transmit Vin_a Vin_b Vin_c of sample data to REC
//
void	Class_IPC::Drv_IPC_ADSample_Transmit(const UINT16 & Vina_0, const UINT16 & Vinb_0, const UINT16 & Vinc_0)
{

    this->IPC_DataWrite(IPC_FLAG_IPC3, HWRE(VIN_A_CPU1TOCPU2), Vina_0);
    this->IPC_DataWrite(IPC_FLAG_IPC4, HWRE(VIN_B_CPU1TOCPU2), Vinb_0);
    this->IPC_DataWrite(IPC_FLAG_IPC5, HWRE(VIN_C_CPU1TOCPU2), Vinc_0);

}

//
// refresh state message of read from remote CPU or write some message wait remote CPU to Read
//
void Class_IPC::Drv_IPC_StateMessage(void)
{
	this->IPC_DataRead(IPC_STS_IPC3, HWRE(DATA_CPU1TOCPU2_A), m_un_CPU2toCPU1.all);
	this->IPC_DataWrite(IPC_FLAG_IPC2, HWRE(DATA_CPU2TOCPU1_A), m_un_CPU2toCPU1.all);

}
//*****************************************************************************
//! ulFlag: IPC_flag bit,after write message in *ulAddress, set flag,flag = 1
//! ilAddress: MSGRAM Address;
//! ilData: The message wait for transmit;
//! return status of command (0=success, 1=error)
//
//*****************************************************************************

UINT16 Class_IPC::IPC_DataWrite(const UINT32 & ulFlag, UINT16 * const ulAddress, const UINT16 & ulData)
{
    //
    // Return false ,if IPC Local to Remote flags are not clear,and not refresh MSGRAM's data
    //
    if (IpcRegs.IPCFLG.all & ulFlag)
    {
        return STATUS_FAIL;
    }
    else
    {
        //write data to MSGRAM,wait for remote CPU to read
        *ulAddress = ulData;
        //
        // Force IPC event on selected request task and enable status-checking
        //
        IpcRegs.IPCSET.all |= ulFlag;

        return STATUS_PASS;
    }

}

 //*****************************************************************************
 //! ulFlag: IPC_flag bit,after read message, clear flag,flag = 0;
 //! ilAddress: MSGRAM Address ;
 //! ilData: the data of wait for refresh;
 //! return status of command (0=success, 1=error)
 //
 //*****************************************************************************
 UINT16 Class_IPC::IPC_DataRead(const UINT32 & ulFlag, UINT16 * const ulAddress, UINT16 & mutable_ulData)
     //
     // Check IPC Flag,if true,remote CPU already write message in *ulAddress
     //
 {
     if (IpcRegs.IPCSTS.all & ulFlag)
     {
        //refresh data from remote CPU message
         mutable_ulData = *ulAddress;
         //
         // acknowledge IPC flag
         //
         IpcRegs.IPCACK.all |= ulFlag;
         return STATUS_PASS;
     }
     else
     {


         return STATUS_FAIL;
     }

 }


//Receive message from CANBUS,Transmit to Rec
void	Class_IPC::Dat_CanBustoCPU2(const CAN_PACKED_PROTOCOL_U * const mutable_message)
{
    if(IpcRegs.IPCFLG.bit.IPC0 == 0)
	{
		*CanBustoCPU2_data = *mutable_message;
		IpcRegs.IPCSET.bit.IPC0 = 1;
	}
}

//Receive message from Rec,and transmit to CAN bus
void Class_IPC::Dat_CPU2toCanBus(void)
{
	//TBD CanaRegs.CAN_NDAT_21 & 0X0002 == 0X0002
	if(IpcRegs.IPCSTS.bit.IPC0)
	{

		CanMailBoxWrite(MAIL_BOX2, &(CPU2toCanBus_data->Frame));
		IpcRegs.IPCACK.bit.IPC0 = 1;
	}

}

#if (0)
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
