/*
 * IPC.h
 *
 *  Created on: 2017/11/30
 *      Author: mnlife
 */

#ifndef IPC_H_
#define IPC_H_
//
// The following are values successfully sent or not.
//
#define STATUS_FAIL                 0x0001
#define STATUS_PASS                 0x0000

typedef	UINT32	(*IpcFuncCall)(UINT32 &IpcSendData);
//
// define MSGRAM, communication CPU1 with CPU2
//
const UINT32 DATA_CPU1TOCPU2_A = 0x03FC07;
const UINT32 DATA_CPU1TOCPU2_B = 0x03FC08;
const UINT32 DATA_CPU1TOCPU2_C = 0x03FC09;

const UINT32 VIN_A_CPU1TOCPU2  = 0x03FC10;
const UINT32 VIN_B_CPU1TOCPU2  = 0x03FC11;
const UINT32 VIN_C_CPU1TOCPU2  = 0x03FC12;

const UINT32 DATA_CPU2TOCPU1_A = 0x03F807;
const UINT32 DATA_CPU2TOCPU1_B = 0x03F808;
const UINT32 DATA_CPU2TOCPU1_C = 0x03F809;

const UINT32 VBUS_P_CPU2TOCPU1 = 0x03F810;
const UINT32 VBUS_N_CPU2TOCPU1 = 0x03F811;


// defien IPC receive flag, use it to receive message
const UINT32 IPC_STS_IPC1 = 0x00000001;
const UINT32 IPC_STS_IPC2 = 0x00000002;
const UINT32 IPC_STS_IPC3 = 0x00000003;
const UINT32 IPC_STS_IPC4 = 0x00000004;
const UINT32 IPC_STS_IPC5 = 0x00000005;
const UINT32 IPC_STS_IPC6 = 0x00000006;


// define IPC transmit flag,when tranmit dowm, set this flag
const UINT32 IPC_FLAG_IPC1 = 0x00000001;
const UINT32 IPC_FLAG_IPC2 = 0x00000002;
const UINT32 IPC_FLAG_IPC3 = 0x00000003;
const UINT32 IPC_FLAG_IPC4 = 0x00000004;
const UINT32 IPC_FLAG_IPC5 = 0x00000005;
const UINT32 IPC_FLAG_IPC6 = 0x00000006;

// CPU2 transmit message to CPU1, those are "COMMAND", "ADDRESS", "DATA",
// "ADDRESS" is the message CPU2 transmit to CPU1;is define in ADDRESS MAP of
// MAGRAM
const UINT32 IPCSENDCOM_CANDATA = 0x00001234;
const UINT32 IPCSENDADDR_CANDATA = *(UINT32 *)(0x03F812);
const UINT32 IPCSENDDATA_CANDATA = 0x00000000;


union IPC_Data
	{
		UINT16		all;
		struct
			{
			UINT16	rsv;
			}		bit;
	};

// Class_IPC define "Inter process Communication" protocol,
// and define some DATA to read or write from MSGRAM
class	Class_IPC
	{
	private:
		UINT16		IPC_DataWrite(const UINT32 &ulFlag, UINT16 * const ulAddress, const UINT16 &ulData);
		UINT16		IPC_DataRead(const UINT32 &ulFlag, UINT16 * const ulAddress, UINT16 &mutable_ulData);

 		// IPCData of status message
		union		IPC_Data		m_un_CPU1toCPU2;
	 	union		IPC_Data		m_un_CPU2toCPU1;

		//static		Class_IPC		*m_pClass_IPC;
		//
    	CAN_PACKED_PROTOCOL_U   * const CPU2toCanBus_data;
    	CAN_PACKED_PROTOCOL_U   * const CanBustoCPU2_data;

	public:

		// constructed function to initialize *CanBustoCPU2 and *CPU2toCanBus;
	 	explicit 	Class_IPC(CAN_PACKED_PROTOCOL_U * const p1, CAN_PACKED_PROTOCOL_U * const p2)
						: CPU2toCanBus_data(p1), CanBustoCPU2_data(p2){}
		//static		Class_IPC	*GetClass_IPCPtr(void);
		// transmit and receive some ADsample DATA
		void		Drv_IPC_ADSample_Transmit(const UINT16 & Vina_0, const UINT16 & Vinb_0, const UINT16 & Vinc_0);
		void		Drv_IPC_ADSample_Receive(UINT16 & mutable_VbusP_0, UINT16 & mutable_VbusN_0);

		void		Drv_IPC_StateMessage(void);

		void		Dat_CanBustoCPU2(const CAN_PACKED_PROTOCOL_U * const mutable_message);
		void		Dat_CPU2toCanBus(void);

		//virtual		~Class_IPC(void);
};




#endif /* IPC_H_ */
