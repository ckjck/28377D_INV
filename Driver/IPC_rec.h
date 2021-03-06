/*
 * IPC_rec.h
 *
 *  Created on: 2017��12��13��
 *      Author: Ratio
 */

#ifndef DRIVER_IPC_REC_H_
#define DRIVER_IPC_REC_H_

//
// The following are value successfully send or not
//
#define STSTUS_FAIL     0x0001
#define STATUS_PASS     0x0000

typedef UINT32  (*IpcFuncCall)(UINT32 &IpcSendData)

//
//  define MSGRAM, communictaion CPU1 with CPU2
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
		UINT16		IPC_DataWrite(const UINT32 &ulFlag, UINT16 * const ulAddress, UINT16 &ulData);
		UINT16		IPC_DataRead(const UINT32 &ulFlag, UINT16 * const ulAddress, UINT16 &ulData);

 		// IPCData of status message
		union		IPC_Data		m_un_CPU1toCPU2;
	 	union		IPC_Data		m_un_CPU2toCPU1;

		//
    CAN_PACKED_PROTOCOL_U   * const CPU2toCanBus;
    CAN_PACKED_PROTOCOL_U   * const CanBustoCPU2;

	public:

		// constructed function to initialize *CanBustoCPU2 and *CPU2toCanBus;
	 	Class_IPC(CAN_PACKED_PROTOCOL_U * const p1, CAN_PACKED_PROTOCOL_U * const p2)
						: CPU2toCanBus(p1), CanBustoCPU2(p2){}

		// transmit and receive some ADsample DATA
		void		Drv_IPC_ADSample_Receive(UINT16 & Vina_0, UINT16 & Vinb_0, UINT16 & Vinc_0);
		void		Drv_IPC_ADSample_Transmit(UINT16 & VbusP_0, UINT16 & VbusN_0);

		void		Drv_IPC_StateMessage(void);

		void		Dat_CanBustoCPU2(CAN_PACKED_PROTOCOL_U *message);
		void		Dat_CPU2toCanBus(const UINT32 & IpcFlagStatus, const UINT32 & IpcFlag);
};

#endif /* DRIVER_IPC_REC_H_ */
