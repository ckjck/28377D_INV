/*
 * IPC.h
 *
 *  Created on: 2017Äê11ÔÂ30ÈÕ
 *      Author: mnlife
 */

#ifndef IPC_H_
#define IPC_H_

//
// The following are values successfully sent or not.
//
#define STATUS_FAIL                 0x0000
#define STATUS_PASS                 0x0001
//
// define MSGRAM, communication CPU1 with CPU2
//
#define	ADDRESS_CPU1TOCPU2_A			0x03FC07
#define	ADDRESS_CPU1TOCPU2_B			0x03FC09

#define	ADDRESS_CPU2TOCPU1_A			0x03F807
#define	ADDRESS_CPU2TOCPU1_B			0x03F809

const UINT32 IPC_RECEIVE_FLAG1 = 0x00000001;
const UINT32 IPC_RECEIVE_FLAG2 = 0x00000002;

const UINT32 IPC_TRANSMIT_FLAG1 = 0x00000001;
const UINT32 IPC_TRANSMIT_FLAG2 = 0x00000002;


union IPC_CPU1toCPU2
	{
		UINT32		all;
		struct
			{
			UINT32	rsv;
			}		bit;
	};
union IPC_CPU2toCPU1
	{
		UINT32		all;
		struct
			{
			UINT32	rsv;
			}		bit;
	};


class	Class_IPC
	{
	private:
		UINT16		IPC_DataWrite(UINT32 ulFlag, UINT32 * ulAddress);
		UINT16		IPC_DataRead(UINT32 ulFlag, UINT32 * ulAddress);
		
		union		IPC_CPU1toCPU2		m_un_CPU1toCPU2;
		union		IPC_CPU2toCPU1		m_un_CPU2toCPU1;
		
	public:
		void		Drv_IPC_Communication(void);
		void		Dat_CanBustoCPU2(CAN_PACKED_PROTOCOL_U *message);
		void		Dat_CPU2toCanBus(CAN_PACKED_PROTOCOL_U *message);

		#pragma CODE_SECTION(".CanMsgInvtoRec")
		CAN_PACKED_PROTOCOL_U	CanBustoCPU2;
		#pragma CODE_SECTION(".CanMsgRectoInv")
		CAN_PACKED_PROTOCOL_U	CPU2toCanBus;
};



#endif /* IPC_H_ */
