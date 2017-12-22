/*
 * IPC_rec.cpp
 *
 *  Created on: 2017Äê12ÔÂ13ÈÕ
 *      Author: Ratio
 */
#include "IPC.h"

 void    Class_IPC::Drv_IPC_ADSample_Transmit(UINT16 &VbusP_0, UINT16 &VbusN_0)
 {

 	//receive Vbus_p Vbus_n from REC
   this->IPC_DataWrite(IPC_STS_IPC4, HWRE(VBUS_P_CPU2TOCPU1), VbusP_0);
 	this->IPC_DataWrite(IPC_STS_IPC5, HWRE(VBUS_N_CPU2TOCPU1), VbusN_0);

 }

 //
 // transmit Vin_a Vin_b Vin_c of sample data to REC
 //
 void	Class_IPC::Drv_IPC_ADSample_Receive(UINT16 &Vina_0, UINT16 &Vinb_0, UINT16 &Vinc_0)
 {

     this->IPC_DataRead(IPC_FLAG_IPC3, HWRE(VIN_A_CPU1TOCPU2), Vina_0);
     this->IPC_DataRead(IPC_FLAG_IPC4, HWRE(VIN_B_CPU1TOCPU2), Vinb_0);
     this->IPC_DataRead(IPC_FLAG_IPC5, HWRE(VIN_C_CPU1TOCPU2), Vinc_0);

 }

 //
 // refresh state message of read from remote CPU or write some message wait remote CPU to Read
 //
 void Class_IPC::Drv_IPC_StateMessage(void)
 {
 	this->IPC_DataWrite(IPC_STS_IPC3, HWRE(DATA_CPU1TOCPU2_A), m_un_CPU2toCPU1.all);
 	this->IPC_DataRead(IPC_FLAG_IPC2, HWRE(DATA_CPU2TOCPU1_A), m_un_CPU2toCPU1.all);

 }
 //*****************************************************************************
 //! ulFlag: IPC_flag bit,after write message in *ulAddress, set flag,flag = 1
 //! ilAddress: MSGRAM Address;
 //! ilData: The message wait for transmit;
 //! return status of command (0=success, 1=error)
 //
 //*****************************************************************************

 UINT16 Class_IPC::IPC_DataWrite(const UINT32 &ulFlag, UINT16 * const ulAddress, UINT16 &ulData)
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
  UINT16 Class_IPC::IPC_DataRead(const UINT32 &ulFlag, UINT16 * const ulAddress, UINT16 &ulData)
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
     if(IpcRegs.IPCSTS.bit.IPC0 == 0)
 	{
 		*message = *CanBustoCPU2;
 		IpcRegs.IPCACK.bit.IPC0 = 1;
 	}
 }

 //Receive message from Rec,and transmit to CAN bus
 void Class_IPC::Dat_CPU2toCanBus(const UINT32 & IpcFlagStatus, const UINT32 & IpcFlag)

 {
 	IpcRegs.IPCSTS.all |= IpcFlagStatus;
 	if(IpcRegs.IPCSENDCOM == IPCSENDCOM_CANDATA)
 	{
 		IpcRegs.IPCSENDADDR = IpcSendAddr;
 		IpcRegs.IPCSENDDATA = IpcSendData;

 		if(IpcRegs.IPCSTS.all && IpcFlag)
 		{

 			CanMailBoxWrite(MAIL_BOX2, &(CPU2toCanBus->Frame));
 			IpcRegs.IPCACK.all |= IpcFlag;
 		}
 	}

 }
