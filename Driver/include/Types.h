/*==========================================================*/
/* Title		:	types.h									*/
/* Description	: 	Data types definition					*/
/* Date			:	Dec.2005								*/
/*==========================================================*/
#ifndef TYPES_H_
#define TYPES_H_
//  480V代码的F2808平台移植第一次检视会议（051202）决定：
//  整流、逆变自定义的变量采用以下统一类型

//	结构体名加前缀struct，结构体变量名加前缀s，如structFlagSystem，sFlagSystem
//	typedef int    			int;			//变量名加前缀i
//	typedef long    		long;			//变量名加前缀l

	typedef		signed int		INT16;							//变量名加前缀i16_
	typedef		unsigned int	UINT16;							//变量名加前缀u16_
	typedef		signed long		INT32;							//变量名加前缀i32_
	typedef		unsigned long	UINT32;							//变量名加前缀u32_

	typedef union
	{
		INT32		dword;
		struct
		{
			UINT16 	lword;
			INT16 	hword;
		}			half;
	}UNLONG;											//变量名加前缀un_

    #define T_MAX 10
	typedef struct
	{
		int num ;
		int half_gap ;
		int range[T_MAX+1];
		int flags[T_MAX];
	} FLAG_TABLE;

#endif
