/*==========================================================*/
/* Title		:	types.h									*/
/* Description	: 	Data types definition					*/
/* Date			:	Dec.2005								*/
/*==========================================================*/
#ifndef TYPES_H_
#define TYPES_H_
//  480V�����F2808ƽ̨��ֲ��һ�μ��ӻ��飨051202��������
//  ����������Զ���ı�����������ͳһ����

//	�ṹ������ǰ׺struct���ṹ���������ǰ׺s����structFlagSystem��sFlagSystem
//	typedef int    			int;			//��������ǰ׺i
//	typedef long    		long;			//��������ǰ׺l

	typedef		signed int		INT16;							//��������ǰ׺i16_
	typedef		unsigned int	UINT16;							//��������ǰ׺u16_
	typedef		signed long		INT32;							//��������ǰ׺i32_
	typedef		unsigned long	UINT32;							//��������ǰ׺u32_

	typedef union
	{
		INT32		dword;
		struct
		{
			UINT16 	lword;
			INT16 	hword;
		}			half;
	}UNLONG;											//��������ǰ׺un_

    #define T_MAX 10
	typedef struct
	{
		int num ;
		int half_gap ;
		int range[T_MAX+1];
		int flags[T_MAX];
	} FLAG_TABLE;

#endif
