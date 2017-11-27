/******************************************************************************
Copyright (c)	      2007 ENPC Corporation, All Rights Reserved
Original Author:    

Latest Revision:
    Author: 
    Date: 2007/08/09
    Change: 
******************************************************************************/
#ifndef 	__DEFINE_H
#define	__DEFINE_H

//-------------------------------------------------------------------
//Head files
//------------------------------------------------------------------------------
//内存检测宏开关


#ifdef MEMWATCH	
//#define __cplusplus	
#define	MW_PTHREADS

#include	"memwatch.h"

#define	new mwNew
#define	delete mwDelete
#endif


//-----------------------------------------------------------------------------
// Public Constants and Macro Definitions

//----------------------------------------------------------------
//变量描述符定义 
#ifndef NULL
#define NULL	0
#endif  
#if (0)
#ifndef NULL
    #define NULL ((void * )0)
#endif
#endif

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif


#ifndef Afx_msg
#define Afx_msg		   static
#endif

typedef char*		   LPSTR;
typedef const char*	   LPCSTR;

typedef unsigned long  DWORD;
typedef unsigned char  BOOL;
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef int            INT16;
//typedef unsigned int   UINT;
typedef unsigned int   UINT16;
typedef unsigned int   UINT;
typedef long           LONG;
//typedef short		   INT16;
//typedef unsigned short UINT16;

typedef UINT           WPARAM;
typedef LONG           LPARAM;
typedef LONG           LRESULT;
typedef int            HWND;
typedef int            HINSTANCE;
typedef	unsigned char  UCHAR;

typedef unsigned char* ICON;


#define	ULONG	unsigned long
#define	LONG	long

#define	UINT16	unsigned int
#define 	INT16	int

//#define	UINT16	unsigned short
//#define   INT16	short

#define	UCHAR   unsigned char
#define	CHAR 	char

#define	DOUBLE	double
#define	FLOAT	float

#define	VOID	void

#define	BOOL	bool

//----------------------------------------------------------
//数据存储类型
#define 	EXTERN		extern
#define		LOCAL		static		// static
#define 	VOLATILE	volatile
#define 	INLINE		inline		// inline api		


#define 	CONST		const
#define 	IN						// input arg
#define 	OUT						// output arg
#define		INOUT


//-----------------------------------------------------------------
//FlashUpdate 标志
#define	REC_FIRMWARE_UPDATE	1	//整流固件升级
#define	INV_FIRMWARE_UPDATE	2	//逆变固件升级
//#define	MON_FIRMWARE_UPDATE	3	//监控软件升级
#define	NONE_FIRMWARE_UPDATE	0	//无固件升级

//-----------------------------------------------------------------
//整流或逆变对监控量的支持情况宏定义
#define 	SUPPORT_OK     0x00		   /*支持*/
#define 	SUPPORT_NG     0x55		   /*不支持*/

//----------------------------------------------------------------
//整流侧、逆变侧共用部分的DataFlag宏定义									
#define		WARN_FLAG		0x01	/*bit0:告警位*/
#define		FAULT_FLAG		0x02	/*bit1:故障请求*/
#define		SWITCH_FLAG	0x10	/*bit4:开关位*/
#define		RESET_FLAG		0x20	/*bit5:设置刷新请求位*/
#define		MASTER_FLAG	0x40	/*bit6:主机位*/

//整流侧DataFlag宏定义
#define		REC_PARA_NX_FLAG  	0x3cc	/*bit9,8,bit7,6, bit3,2*/
#define		REC_PARA_NXA		0x00	/*bit7,6--00  bit3,2--00*/
#define		REC_PARA_NXB		0x80	/*bit7,6--10  bit3,2--00*/
#define		REC_PARA_NXE		0xc0	/*bit7,6--11  bit3,2--00*/
#define		REC_PARA_NXAP		0x04	/*bit7,6--00  bit3,2--01*/
#define		REC_PARA_NXBP		0x84	/*bit7,6--10  bit3,2--01*/
#define 		REC_PARA_NXF 		0x40	/*bit7,6--01  bit3,2--00*/
#define		REC_PARA_NXRM		0x100	//bit8 

//逆变侧Dataflag宏定义
#define		INV_PARA_NX_FLAG  	0x38c	/*bit9,8,bit7, bit3,2*/
#define		INV_PARA_NXA		0x00	/*bit7--0  bit3,2--00*/
#define		INV_PARA_NXB		0x08	/*bit7--0  bit3,2--10*/
#define		INV_PARA_NXE		0x0C	/*bit7--0  bit3,2--11*/
#define		INV_PARA_NXAP		0x80	/*bit7--1  bit3,2--00*/
#define		INV_PARA_NXBP		0x88	/*bit7--1  bit3,2--10*/
#define 		INV_PARA_NXF 		0x04	/*bit7--0  bit3,2--01*/
#define		AC_ENABLE			0x400	/*bit10--1*/
#define		INV_PARA_NXRM		0x100	//bit8
//--------------------------------------------------------------------
//Flash Update Flag
#define		FLASH_UPDATE_OVER	0	//升级结束或无升级请求
#define		FLASH_UPDATE_REC		1	//整流固件升级
#define		FLASH_UPDATE_INV		2	//逆变固件升级

//--------------------------------------------------------------
//位操作
#define	SET_BIT0	0x0001
#define	SET_BIT1	0x0002
#define	SET_BIT2	0x0004
#define	SET_BIT3	0x0008
#define	SET_BIT4	0x0010
#define	SET_BIT5	0x0020
#define	SET_BIT6	0x0040
#define	SET_BIT7	0x0080
#define	SET_BIT8	0x0100
#define	SET_BIT9	0x0200
#define	SET_BIT10	0x0400
#define	SET_BIT11	0x0800
#define	SET_BIT12	0x1000
#define	SET_BIT13	0x2000
#define	SET_BIT14	0x4000
#define	SET_BIT15	0x8000

#define	CLR_BIT0	0xfffe
#define	CLR_BIT1	0xfffd
#define	CLR_BIT2	0xfffb
#define	CLR_BIT3	0xfff7
#define	CLR_BIT4	0xffef
#define	CLR_BIT5	0xffdf
#define	CLR_BIT6	0xffbf
#define	CLR_BIT7	0xff7f
#define	CLR_BIT8	0xfeff
#define	CLR_BIT9	0xfdff
#define	CLR_BIT10	0xfbff
#define	CLR_BIT11	0xf7ff
#define	CLR_BIT12	0xefff
#define	CLR_BIT13	0xdfff
#define	CLR_BIT14	0xbfff
#define	CLR_BIT15	0x7fff

//-----------------------------------------------------------------
//AC记录状态
#define AC_RECORD_VALID	0xaa		/*对应的AC记录状态有效*/
#define AC_RECORD_INVALID	0x55			/*对应的AC记录状态无效*/

//--------------------------------------------------------------------------
//external IO status
#define	OUT_PORT_OPEN		0
#define	OUT_PORT_CLOSE	1

//--------------------------------------------------------------------------
//系统故障类型定义

#define	SYS_FAULT_MIN_NUM	0x80	//最小故障号码

//以下故障发生，将导致系统复位
#define	OUT_OF_MEMORY		0x80
#define	EVENT_LOG_ERR			0x81
#define	THREAD_NO_RESPONSE	0x82
#define	NOT_FREE_TIMER		0x83
#define	SIG_REG_FAILED			0x84

//以下故障发生时，系统进入维护模式，需手动复位
#define	SEGMENT_FAULT				0xC0
#define	CFG_FILE_ERR				0xC1
#define	CANNOT_OPEN_SCI_PORT		0xC2
#define	FLASH_OP_FAIL				0xC3
#define	EXT_IO_INITIAL_FAILED		0xC4
#define	RECORD_MANAG_INIT_FAILED	0xc5
#define	GUI_INIT_FAILED			0xC6
#define	CAN_NOT_OPEN_LOG_FILE	0xc7
#define	THREAD_CREAT_FAILED		0xc8
#define	ILLEGAL_FUALT				0xc9
#define	BUS_ERROR					0xCA




//------------------------------------------------------------------------
//最大值最小值
#define	MAX_VAL(x, y)	(((x)>(y)) ? (x) : (y))
#define	MIN_VAL(x, y)	(((x) < (y)) ? (x) : (y))

//--------------------------------------------------------------------
//debug

#define	FILE_NAME	__FILE__
#define	LINE_NUM	__LINE__

//#define	DEBUG_CODE	1


#ifdef DEBUG_CODE
	#define DEBUG_TIMER 	0
	#define DEBUG_CAN 	0
	#define DEBUG_DATA_POOL 0
	#define DEBUG_CAN_APP 0
	#define DEBUG_CAN_PTROTOCOL 0
	#define DEBUG_CAN_PORT 0
#else
	#define DEBUG_TIMER 	1
	#define DEBUG_CAN 	1
	#define DEBUG_DATA_POOL 1
	#define DEBUG_CAN_APP 1
	#define DEBUG_CAN_PTROTOCOL 1
	#define DEBUG_CAN_PORT 1
#endif

#define TIMER_TRACE	\
	DEBUG_TIMER ? (void)0 : (void)printf("[%s: %d]---", FILE_NAME, LINE_NUM);\
	DEBUG_TIMER ? (void)0 : (void)printf

#define CAN_TRACE	\
	DEBUG_CAN ? (void)0 : (void)printf("[%s: %d]---", FILE_NAME, LINE_NUM);\
	DEBUG_CAN ? (void)0 : (void)printf

#define DATA_POOL_TRACE \
	DEBUG_DATA_POOL ? (void)0 : (void)printf("[%s: %d]---", FILE_NAME, LINE_NUM);\
	DEBUG_DATA_POOL ? (void)0 : (void)printf

#define CAN_APP_TRACE \
	DEBUG_CAN_APP ? (void)0 : (void)printf("[%s: %d]---", FILE_NAME, LINE_NUM);\
	DEBUG_CAN_APP ? (void)0 : (void)printf

#define CAN_PROTOCOL_TRACE \
	DEBUG_CAN_PTROTOCOL ? (void)0 : (void)printf("[%s: %d]---", FILE_NAME, LINE_NUM);\
	DEBUG_CAN_PTROTOCOL ? (void)0 : (void)printf

#define CAN_PORT_TRACE \
	DEBUG_CAN_PORT ? (void)0 : (void)printf("[%s: %d]---", FILE_NAME, LINE_NUM);\
	DEBUG_CAN_PORT ? (void)0 : (void)printf

 

	

//后台通信调试信息
#define	DEBUG_HOST	0
#define	HOST_TRACE		\
	DEBUG_HOST ? (void)0 : (void)printf("[%s: %d]---", FILE_NAME, LINE_NUM);\
	DEBUG_HOST ? (void)0 : (void)printf

//模块通信调试信息
#define	DEBUG_MODULE		0
#define	MODULE_TRACE		\
	DEBUG_MODULE ? (void)0 : (void)printf("[%s: %d]---", FILE_NAME, LINE_NUM);\
	DEBUG_MODULE ? (void)0 : (void)printf

//配置文件调试信息
#define	DEBUG_CFG		0
#define	CFG_TRACE		\
	DEBUG_CFG ? (void)0 : (void)printf("[%s: %d]---", FILE_NAME, LINE_NUM);\
	DEBUG_CFG ? (void)0 : (void)printf

//主线程调试信息
#define	DEBUG_MAIN_THREAD	0
#define	MAIN_THREAD_TRACE	\
	DEBUG_MAIN_THREAD ? (void)0 : (void)printf("[%s: %d]---", FILE_NAME, LINE_NUM);\
	DEBUG_MAIN_THREAD ? (void)0 : (void)printf


//dry_Temp debug info
#define	DEBUG_DRY_TEMP		1
#define	DRY_TEMP_TRACE		\
	DEBUG_DRY_TEMP ? (void)0 : (void)printf("[%s: %d]---", FILE_NAME, LINE_NUM);\
	DEBUG_DRY_TEMP ? (void)0 : (void)printf

//断言宏
#define	ASSERT_ENABLE	1
#define	__ASSERT__	ASSERT_ENABLE ? (void)assert(0) : (void) 0

//-----------------------------------------------------------------------------
//BOOL 值
#define TRUE	 1
#define FALSE 0

#define EXIO_ENABLE		1
#define EXIO_DISABLE	0


#endif




