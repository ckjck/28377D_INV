/*
 * IPC.h
 *
 *  Created on: 2017Äê11ÔÂ30ÈÕ
 *      Author: mnlife
 */

#ifndef IPC_H_
#define IPC_H_
#include	"include\Types.h"
//
// The following are values successfully sent or not.
//
#define STATUS_FAIL                 0x0001
#define STATUS_PASS                 0x0000
//
// define MSGRAM, communication CPU1 with CPU2
//
#define	ADDRESS_CPU1TOCPU2_A	0x03FC07
#define	ADDRESS_CPU1TOCPU2_B	0x03FC08
#define ADDRESS_CPU1TOCPU2_C	0x03FC09

#define VIN_AtoREC				0x03FC10
#define VIN_BtoREC				0x03FC11
#define	VIN_CtoREC				0x03FC12

#define	ADDRESS_CPU2TOCPU1_A	0x03F807
#define	ADDRESS_CPU2TOCPU1_B	0x03F808
#define ADDRESS_CPU2TOCPU1_C	0x03F809

#define VBUS_PfromREC			0x03FC10
#define	VBUS_NfromREC			0x03FC11



const UINT32 IPC_RECEIVE_FLAG1 = 0x00000001;
const UINT32 IPC_RECEIVE_FLAG2 = 0x00000002;
const UINT32 IPC_RECEIVE_FLAG3 = 0x00000003;
const UINT32 IPC_RECEIVE_FLAG4 = 0x00000004;
const UINT32 IPC_RECEIVE_FLAG5 = 0x00000005;


const UINT32 IPC_TRANSMIT_FLAG1 = 0x00000001;
const UINT32 IPC_TRANSMIT_FLAG2 = 0x00000002;
const UINT32 IPC_TRANSMIT_FLAG3 = 0x00000003;
const UINT32 IPC_TRANSMIT_FLAG4 = 0x00000004;
const UINT32 IPC_TRANSMIT_FLAG5 = 0x00000005;
const UINT32 IPC_TRANSMIT_FLAG6 = 0x00000006;



union IPC_Data
	{
		UINT16		all;
		struct
			{
			UINT16	rsv;
			}		bit;
	};

class	Class_IPC
	{
	private:
		UINT16		IPC_DataWrite(const UINT32 &ulFlag, UINT16 * const ulAddress, UINT16 &ulData);
		UINT16		IPC_DataRead(const UINT32 &ulFlag, UINT16 * const ulAddress, UINT16 &ulData);
 		
		union		IPC_Data		m_un_CPU1toCPU2;
	 	union		IPC_Data		m_un_CPU2toCPU1;
		
	public:
		void		Drv_IPC_ADSample_Transmit(void);
		void		Drv_IPC_ADSample_Receive(void);
		void		Drv_IPC_StateMessage(void);
		void		Dat_CanBustoCPU2(CAN_PACKED_PROTOCOL_U *message);
		void		Dat_CPU2toCanBus(CAN_PACKED_PROTOCOL_U *message);

		#pragma CODE_SECTION(".CanMsgInvtoRec")
		CAN_PACKED_PROTOCOL_U	CanBustoCPU2;
		#pragma CODE_SECTION(".CanMsgRectoInv")
		CAN_PACKED_PROTOCOL_U	CPU2toCanBus;
};




#endif /* IPC_H_ */
