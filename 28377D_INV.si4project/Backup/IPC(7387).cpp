/*==========================================================*/ 	
/* Title		:	IPC.cpp													*/ 	
/* Description	: 	Class_IPC definition										*/ 	
/* Date		:	Apr.2017														*/ 	
/*==========================================================*/
#include "IPC.h"



void    Class_IPC::Drv_IPC_ADSample_Receive(void)
{
	//receive Vbus_p Vbus_n from REC
  //  this->IPC_DataRead(IPC_RECEIVE_FLAG2, HWRE(VBUS_PfromREC), (UINT16)(objADDrv.m_i16ADC_VbusP_0));
//	this->IPC_DataRead(IPC_RECEIVE_FLAG3, HWRE(VBUS_NfromREC), (UINT16)(objADDrv.m_i16ADC_VbusN_0));

}

//
// transmit Vin_a Vin_b Vin_c of sample data to REC
//
void	Class_IPC::Drv_IPC_ADSample_Transmit(void)
{
	
  //  this->IPC_DataWrite(IPC_TRANSMIT_FLAG2, HWRE(VIN_AtoREC), (UINT16)(objADDrv.m_i16ADC_Vina_0));
  //  this->IPC_DataWrite(IPC_TRANSMIT_FLAG3, HWRE(VIN_BtoREC), (UINT16)(objADDrv.m_i16ADC_Vinb_0));
  //  this->IPC_DataWrite(IPC_TRANSMIT_FLAG4, HWRE(VIN_CtoREC), (UINT16)(objADDrv.m_i16ADC_Vinc_0));

}

//
// refresh state message of read from remote CPU or write some message wait remote CPU to Read
//
void Class_IPC::Drv_IPC_StateMessage(void)
{
	this->IPC_DataRead(IPC_RECEIVE_FLAG1, HWRE(ADDRESS_CPU1TOCPU2_A), m_un_CPU2toCPU1.all);
	this->IPC_DataWrite(IPC_TRANSMIT_FLAG1, HWRE(ADDRESS_CPU2TOCPU1_A), m_un_CPU2toCPU1.all);

}
//*****************************************************************************
//! ulFlag: IPC_flag bit,after write message in *ulAddress, set flag,flag = 1
//! ilAddress: MSGRAM Address;
//! ilData: The message wait for transmit;
//! return status of command (0=success, 1=error)
//
//*****************************************************************************

inline  UINT16 Class_IPC::IPC_DataWrite(const UINT32 &ulFlag, UINT16 * const ulAddress, UINT16 &ulData)
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
 inline UINT16 Class_IPC::IPC_DataRead(const UINT32 &ulFlag, UINT16 * const ulAddress, UINT16 &ulData)
 {
     //
     // Check IPC Flag,if true,remote CPU already write message in *ulAddress
     //
     if (IpcRegs.IPCSTS.all & ulFlag)
     {
        //refresh data from remote CPU message
         ulData = *ulAddress;
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
void	Class_IPC::Dat_CanBustoCPU2(CAN_PACKED_PROTOCOL_U *message)
{
    if(IpcRegs.IPCFLG.bit.IPC0 == 0)
	{
		CanBustoCPU2 = *message;
		IpcRegs.IPCSET.bit.IPC0 = 1;
	}
}

//Receive message from Rec,and transmit to CAN bus
void	Class_IPC::Dat_CPU2toCanBus(CAN_PACKED_PROTOCOL_U *message)
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
