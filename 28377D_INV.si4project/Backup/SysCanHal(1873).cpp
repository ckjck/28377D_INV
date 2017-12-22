/***************************************************************************************
*
* Filename: 		SysCanHal.cpp
* Contributor:
* Author: 		Wang Zhihua
* Date: 			1/21/2008
* Copyright 		2008 ENPC Corporation, All Rights Reserved
*
* Description: 	This module definite HAL(hardware abstraction layer) between CAN controller and
*			CAN protocol Layer.
*				HAL provide methods to receive CAN message and transmit CAN message. These
*			is achieved by interrupt mode
*
*
* Version			0
* Last rev by:		Wang Zhihua
* Last rev date:	1/21/2008
* Last rev desc:
****************************************************************************************/

//---------------------------------------------------------------
//Include header
#include "SysCanHal.h"

//--------------------------------------------------------------------------------



//-------------------------------------------------------------------------------
//Private macro and const variable defintion

	//the ptr definition about xmit and receive call back
	//SYS_CAN_RECV_FCB pSysCANRecvFcb;
	//SYS_CAN_XMIT_FCB pSysCanXmitFcb;

	CommandBase *pCanIsrRecvFcb;
	CommandBase *pCanIsrXmitFcb;

	//the buffer used to save received msg
	SYS_CAN_FRAME ReceiveFrame;
	CAN_PACKED_PROTOCOL_U TransmitFrame;
		
	tCANMsgObject sTXCANMessage;
	tCANMsgObject sRXCANMessage;


//--------------------------------------------------------------------------------
//public variable

//--------------------------------------------------------------------------------
//private variable

//--------------------------------------------------------------------------------
//private  function

//--------------------------------------------------------------------------------
//function definiton


/****************************************************************************************
SysCanFcbRegister----

Parameters:          		pRecvCallBack:CAN���յ���Ϣ����øú������д���
					pXmitCallBack:CAN�ɹ�����msg����øú���������һ��msg
Return Value:
Precondition:
Postcondition:
****************************************************************************************/
VOID SysCanFcbRegister(IN CommandBase *pRecvCallBack,
							IN CommandBase *pXmitCallBack)
{
	pCanIsrRecvFcb = pRecvCallBack;
	pCanIsrXmitFcb = pXmitCallBack;
}


/**************************************************************************************
CanMailBoxRead----��ָ���������ж�ȡһ֡msg

Parameters:          		pDesFrame:Ŀ��buffer��ַ
					pSourceMailBox:Դ�����ַ
Return Value:
Precondition:
Postcondition:
***************************************************************************************/
VOID CanMailBoxRead(OUT SYS_CAN_FRAME *pDesFrame, IN tCANMsgObject *pSourceMailBox)
{
	UINT16 * ChartoWords = (UINT16 *)(pSourceMailBox->pucMsgData);

	pDesFrame->CANId = (UINT16)pSourceMailBox->ui32MsgID;
	pDesFrame->CANDlc = (UINT16)pSourceMailBox->ui32MsgLen;

	pDesFrame->CANDataHead = *ChartoWords;
	pDesFrame->CANData1 = *(ChartoWords+1);
	pDesFrame->CANData2 = *(ChartoWords+2);
	pDesFrame->CANData3 = *(ChartoWords+3);
}

/************************************************************************************
CanMailBoxWrite----��������д��һ֡msg

Parameters:          		pDesMailBox:Ŀ�������ַ
					pSourceFrame:��д����Ϣ�ĵ�ַ
Return Value:
Precondition:
Postcondition:
*************************************************************************************/
VOID CanMailBoxWrite(IN const UINT32 &MailBoxID, IN SYS_CAN_FRAME *pSourceFrame)
{
	sTXCANMessage.ui32MsgID = (UINT32)(pSourceFrame->CANId);
	//The message identifier mask used when identifier filtering is enabled
	sTXCANMessage.ui32MsgIDMask = 0;
	sTXCANMessage.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
	sTXCANMessage.ui32MsgLen = (UINT32)(pSourceFrame->CANDlc);

	sTXCANMessage.pucMsgData[0] = (UCHAR)((pSourceFrame->CANDataHead)&0xFF);
	sTXCANMessage.pucMsgData[1] = (UCHAR)(((pSourceFrame->CANDataHead)>>8)&0xFF);
	sTXCANMessage.pucMsgData[2] = (UCHAR)((pSourceFrame->CANData1)&0xFF);
	sTXCANMessage.pucMsgData[3] = (UCHAR)(((pSourceFrame->CANData1)>>8)&0xFF);
	sTXCANMessage.pucMsgData[4] = (UCHAR)((pSourceFrame->CANData2)&0xFF);
	sTXCANMessage.pucMsgData[5] = (UCHAR)(((pSourceFrame->CANData2)>>8)&0xFF);
	sTXCANMessage.pucMsgData[6] = (UCHAR)((pSourceFrame->CANData3)&0xFF);
	sTXCANMessage.pucMsgData[7] = (UCHAR)(((pSourceFrame->CANData3)>>8)&0xFF);
/*
	*(sTXCANMessage.pucMsgData) = (UCHAR)((pSourceFrame->CANDataHead)&0xFF);
	*(sTXCANMessage.pucMsgData+1) = (UCHAR)(((pSourceFrame->CANDataHead)>>8)&0xFF);
	*(sTXCANMessage.pucMsgData+2) = (UCHAR)((pSourceFrame->CANData1)&0xFF);
	*(sTXCANMessage.pucMsgData+3) = (UCHAR)(((pSourceFrame->CANData1)>>8)&0xFF);
	*(sTXCANMessage.pucMsgData+4) = (UCHAR)((pSourceFrame->CANData2)&0xFF);
	*(sTXCANMessage.pucMsgData+5) = (UCHAR)(((pSourceFrame->CANData2)>>8)&0xFF);
	*(sTXCANMessage.pucMsgData+6) = (UCHAR)((pSourceFrame->CANData3)&0xFF);
	*(sTXCANMessage.pucMsgData+7) = (UCHAR)(((pSourceFrame->CANData3)>>8)&0xFF);
*/
	objCANDrv.CANMessageSet(CANA_BASE, MailBoxID, &sTXCANMessage, MSG_OBJ_TYPE_TX);
}

/**********************************************************************************
CloseSysCanInt----�ر�system CAN�ж�, ��ֹ�ж����룬�����������ж�
				Ƕ�ף����жϷ������ʼ������

Parameters:
Return Value:
Precondition:
Postcondition:
************************************************************************************/
VOID CloseSysCanInt(VOID)
{
	objCANDrv.CANGlobalIntDisable(CANA_BASE, CAN_GLB_INT_CANINT0);

}


/*********************************************************************************
OpenSysCanInt-----��system CAN�ж�, ���жϷ�����򷵻�ǰ����

Parameters:
Return Value:
Precondition:
Postcondition:
*********************************************************************************/
VOID OpenSysCanInt(VOID)
{
	objCANDrv.CANGlobalIntEnable(CANA_BASE, CAN_GLB_INT_CANINT0);
}

/********************************************************************************
SysCanSendData-----����һ֡��Ϣ��CAN����:��������ΪMBOX1

Parameters:          SendFrame:����msgָ��

Return Value:		undefinition

Precondition:
Postcondition:
*******************************************************************************/
INT16 SysCanSendData(IN SYS_CAN_FRAME *pSendFrame)
{
	CanMailBoxWrite(MAIL_BOX1, pSendFrame);

	return 0;
}


/********************************************************************************
ECAN0INTA_ISR-----system can ISR :ECAN-A
�жϷ�Ϊ����������жϣ������ж�����Ϊ�����ѷ��ͣ������Ϊ�գ������ж�Ϊ�����ѱ����գ��������
Parameters:
Return Value:
Precondition:
Postcondition:
********************************************************************************/
volatile UINT32 g_bErrFlag = 0;

interrupt void ECAN0INTA_ISR(VOID)
{

	UINT32	Mbox_ID;
	UINT32	ulStatus;

	CloseSysCanInt();
	asm(" NOP");
	//enable global interrupt, permit nested interrupt
	EINT;	// Clear INTM to enable interrupt
	//
	// Read the CAN interrupt status to find the cause of the interrupt(mailbox1~32)
	//
	Mbox_ID = objCANDrv.CANIntStatus(CANA_BASE, CAN_INT_STS_CAUSE);
	switch(Mbox_ID)
		{
			case MAIL_BOX1:	//INV message Transmit

				ProtocolObj.SysCanXmitFcb(NULL);
				objCANDrv.CANIntClear(CANA_BASE, MAIL_BOX1);
				break;
			case MAIL_BOX2:	//REC message transmit
				objDigitalIO.Dat_CPU2toCanBus(&TransmitFrame);
				CanMailBoxWrite(MAIL_BOX2, &(TransmitFrame.Frame));
				objCANDrv.CANIntClear(CANA_BASE, MAIL_BOX2);
			case MAIL_BOX5:

				//fetch message from mailbox.
				objCANDrv.CANMessageGet(CANA_BASE, MAIL_BOX5, &sRXCANMessage, true);

				CanMailBoxRead(&ReceiveFrame, &sRXCANMessage);
				ReceiveFrame.CANErrorStatus = CAN_STATUS_ERROR_ACTIVE;
				if (0 != ReceiveFrame.CANId)
				{
					ProtocolObj.SysCanRecvFcb(&ReceiveFrame);
				}
				objCANDrv.CANIntClear(CANA_BASE, MAIL_BOX5);
				break;

			case MAIL_BOX4:

				//fetch message from mailbox
				objCANDrv.CANMessageGet(CANA_BASE, MAIL_BOX4, &sRXCANMessage, true);

				CanMailBoxRead(&ReceiveFrame, &sRXCANMessage);
				ReceiveFrame.CANErrorStatus = CAN_STATUS_ERROR_ACTIVE;
				if (0 != ReceiveFrame.CANId)
				{
					ProtocolObj.SysCanRecvFcb(&ReceiveFrame);
				}
				objCANDrv.CANIntClear(CANA_BASE, MAIL_BOX4);
				break;

			//Process Error
			case CAN_INT_INT0ID_STATUS:
				//
        		// Read the controller status.  This will return a field of status
        		// error bits that can indicate various errors.  Error processing
        		// is not done in this example for simplicity.  Refer to the
        		// API documentation for details about the error status bits.
        		// The act of reading this status will clear the interrupt.  If the
        		// CAN peripheral is not connected to a CAN bus with other CAN devices
        		// present, then errors will occur and will be indicated in the
        		// controller status.
        		//
        		ulStatus = objCANDrv.CANStatusGet(CANA_BASE, CAN_STS_CONTROL);

        		//
        		//Check to see if an error occurred.
        		//
        		if(((ulStatus  & ~(CAN_ES_TXOK | CAN_ES_RXOK)) != 7) &&
           			((ulStatus  & ~(CAN_ES_TXOK | CAN_ES_RXOK)) != 0))
        		{
            		//
            		// Set a flag to indicate some errors may have occurred.
            		//
            		g_bErrFlag = 1;
					asm("   ESTOP0");
        		}
				break;
			default:
				asm("   ESTOP0");
				break;
		}//End switch case

	objCANDrv.CANGlobalIntClear(CANA_BASE, CAN_GLB_INT_CANINT0);
	PieCtrlRegs.PIEACK.all |= 0x100;  		// Issue PIE ack
	OpenSysCanInt();

}
