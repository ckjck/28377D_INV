/*==========================================================*/ 	
/* Title		:	PowCANProtocol.h								*/ 	
/* Description	: 	Class_PowCANProtocol definition				*/ 	
/* Date			:	Apr.2008								*/ 	
/*==========================================================*/

#ifndef		POWCANPROTOCOL_H 
             
#define     POWCANPROTOCOL_H
//By wxw20090714 ���Ӹ���֡��ʶ��ID
/*PowerCan��11λID��������:	Bit0~Bit5	������
							Bit6~Bit10	��Ϣ֡ID
									1:������Ϣ֡
									2:�����й�����֡
									3:�������ڹ���֡
									4:Ԥ��
									5:���õ������Ϣ֡
*/
#define	MESSAGEID_PLOAD		0x0002	//�й���Ϣ֡ID
#define	MESSAGEID_SLOAD		0x0003	//���ڹ�����Ϣ֡ID
#define	MESSAGEID_STATE		0x0005	//��ؼ�Inv״̬֡
#define	MESSAGEID_INVALID_CURRENT	0x0006	//��ЧDQ����
#define	MESSAGEID_ILFILTER			0x0007
#define	MESSAGEID_IINVADQCURRENT			0x0008
#define	MESSAGEID_IINVBDQCURRENT			0x0009
#define	MESSAGEID_IINVCDQCURRENT			0x000A


#define	MESSAGEID_MASK		0x07c0	//��Ϣ֡ID������ID�е�λ��
 
const	UINT16	UPSID_MASK = 0x003f;			//ups������ID�е�λ��
const	UINT16	MessageID_Power = 0x0001;		//������Ϣ֡ID
	
	class Class_PowCANProtocol
	{
		public:
			
		private:
								
			UINT16 		m_u16UpsIdx;  
			
		public:
			void 	Dat_Int_MBox_Read(volatile struct MBOX *pMBox, UINT32 u32MBoxIdx);
			
	}; 
	
#endif

//===========================================================================
// End of file.
//===========================================================================

