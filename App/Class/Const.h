/*=======================================================*/
/* Filename：	const.h                                  */
/* Description: Constants Definition		             */
/* Author: 	Zhuo Qingfeng, Zhang Xuejie					 */
/* Date:	Dec.2005									 */
/*=======================================================*/
 #ifndef	CONST_H
            
 #define    CONST_H

#define		KSqrt3invCnst	0x093D				// 1/sqrt(3)
#define		K1div3Cnst		0x0555				// 1/3
#define		KSqrt2Div3Cnst	0x078A              // sqrt(2)/3
#define		K1Div2Cnst	    0x0800              // 1/2
#define		K1DivSqrt2Cnst	0x0B50              // 1/sqrt(2)
#define		KSqrt3Div2Cnst	0x0DDB              // sqrt(3)/2
#define 	K1div10Cnst		0x019A				// 1/10
#define 	K8div30Cnst		0x0444				// 8/30
#define		KRadToDegCnst	0x0E52				// (180/pi)*2^6

#define		VdcUplimCnst	4450				//4096*880/810	直流电压上限值
//母线电压370V，更改故障判断值
#define		VdcDnlimCnst	2832				//4096*560/810	直流电压下限值
#define		VdcLowCnst		3135				//4096*620/810	直流电压低阀值

//基于时基(1ms,14ms/16ms,40ms)的定时常量，注意1ms时基是否溢出
#define		K2msBased1msCnst		2			//2ms=2*1ms
#define		K3msBased1msCnst		3			//3ms=3*1ms
#define		K4msBased1msCnst		4
#define		K5msBased1msCnst		5
#define		K10msBased1msCnst		10			//10ms=10ms*1ms
#define		K200msBased1msCnst		200			//200ms=200ms*1ms
#define		K9sBased1msCnst			9000		//9s=9000*1ms
#define		K30sBased1msCnst		30000		//30s=30000*1ms

#define		K180sBased40msCnst		4500		//180s=4500*40ms

//与主循环执行周期有关的常量定义（目前主循环执行周期Tmain约1000us）
#define		K2msMainCnst			2			//2ms/Tmain, 2ms常数

#define		K20msFilterCnst			4			//log2(20000/Tmain),20ms滤波常数
#define		K30msFilterCnst			5			//log2(30000/Tmain),30ms滤波常数
#define		K50msFilterCnst			6			//log2(50000/Tmain),50ms滤波常数

//与开关频率有关的常量定义（CPU频率fcpu=100MHz,开关频率fs=24kHz,PWM周期中断频率fpwm=12kHz）
#define 	CLKUSPRDCNST			9000		//CLKUS周期计数器

#define		K1CperiodCnst			1			//1个控制周期
#define		K166usCnst				2
#define 	K550usCnst				7			//0.00055*fpwm
#define 	K1msCnst				12			//0.001*fpwm
#define		KBig1msCnst				13			//0.001111*fpwm
#define 	K2msCnst				24			//0.002*fpwm
#define 	K3msCnst				36			//0.003*fpwm
#define		K4msCnst				48			//0.004*fpwm
#define		K5msCnst				60			//0.005*fpwm
#define		K5Point4msCnst			65			//0.0054
#define		K6msCnst				72			//0.006*fpwm
#define		K7msCnst				84			//0.007*fpwm
#define		K8msCnst				96			//0.008*fpwm
#define 	K10msCnst				120			//0.01*fpwm
#define 	K12msCnst				144			//0.012*fpwm
#define 	K13msCnst				156			//0.013*fpwm
#define 	K14msCnst				168			//0.014*fpwm
#define 	K15msCnst				180			//0.015*fpwm
#define 	K16msCnst				192			//0.016*fpwm
#define 	K17msCnst				204			//0.017*fpwm

#define		K20msCnst				240			//0.02*fpwm
#define 	K40msCnst				480			//0.04*fpwm
#define 	K42msCnst				504			//0.042*fpwm
#define 	K60msCnst				720			//0.06*fpwm
#define 	K80msCnst				960			//0.08*fpwm
#define 	K84msCnst				1008		//0.084*fpwm
#define 	K100msCnst				1200		//0.1*fpwm
#define 	K200msCnst				2400		//0.2*fpwm
#define 	K400msCnst				4800		//0.4*fpwm

#define 	KDthetaDiffCnst			3			//0.1Hz, 360*0.1*1024/fpwm
#define 	KDthetaDiff2Cnst		12			//0.4Hz, 360*0.4*1024/fpwm
#define 	K1HzDthetaCnst			30			//1Hz,   360*1*1024/fpwm
#define 	K30HzDthetaCnst			922			//30Hz,  360*30*1024/fpwm
#define 	K45HzDthetaCnst			1382		//45Hz,  360*45*1024/fpwm
#define 	K46HzDthetaCnst			1413		//46Hz,  360*46*1024/fpwm
#define 	K50HzDthetaCnst			1536		//50Hz,  360*50*1024/fpwm
#define 	K54HzDthetaCnst			1659		//54Hz,  360*54*1024/fpwm
#define 	K55HzDthetaCnst			1690		//55Hz,  360*55*1024/fpwm
#define 	K56HzDthetaCnst			1720		//56Hz,  360*56*1024/fpwm
#define 	K60HzDthetaCnst			1843		//60Hz,  360*60*1024/fpwm
#define 	K64HzDthetaCnst			1966		//64Hz,  360*64*1024/fpwm
#define 	KThetaStepBpCnst		17			//0.1Hz/s,  360*0.1*1024*2^16/(fpwm^2)
#define 	KFMeanCalCnst			6667		//fpwm*100*(2^11)/(360*1024)
#define 	K30HzPointCntCnst		400			//fpwm/30
#define 	K40HzPointCntCnst		300			//fpwm/40
#define 	K45HzPointCntCnst		267			//fpwm/45,  要修改rpt_con模块中的汇编代码
#define 	K50HzPointCntCnst		240			//fpwm/50
#define 	K60HzPointCntCnst		200			//fpwm/60
#define 	K66HzPointCntCnst		182			//fpwm/66
#define 	K80HzPointCntCnst		150			//fpwm/80
#define 	KDthetaLbsBaseCnst		768000		//360*1024*10^8/(4*fpwm)

#define 	KSwPrdCnst				2083		//fcpu/fs/2
#define 	KIntPrdCnst				4166		//fcpu/fpwm/2		

#define 	KAgeSPWMCnst			712			//277*KSwPrdCnst/810
#define 	KDeadTimeCnst			1			//10ns
#define  	KKeepOnINVMaxCnst       20
#define   	KTotalOnINVMaxCnst      10
#endif
//===========================================================================
// End of file.
//===========================================================================
