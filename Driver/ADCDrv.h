/*==========================================================*/
/* Title		:	ADCDrv.h								*/
/* Description	: 	Class_ADCDrv definition					*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#ifndef		ADCDRV_H
       
#define     ADCDRV_H

//definitions for specifying an ADC
#define ADC_ADCA 0
#define ADC_ADCB 1
#define ADC_ADCC 2
#define ADC_ADCD 3

//definitions for selecting ADC resolution
#define ADC_RESOLUTION_12BIT 0
#define ADC_RESOLUTION_16BIT 1

//definitions for selecting ADC signal mode
//(single-ended mode is only a valid mode for 12-bit resolution)
#define ADC_SIGNALMODE_SINGLE 0
#define ADC_SIGNALMODE_DIFFERENTIAL 1




//EOC define
#define  EOC0            0
#define  EOC1            1
#define  EOC2            2
#define  EOC3            3
#define  EOC4            4
#define  EOC5            5
#define  EOC6            6
#define  EOC7            7
#define  EOC8            8
#define  EOC9            9
#define  EOC10           10
#define  EOC11           11
#define  EOC12           12
#define  EOC13           13
#define  EOC14           14
#define  EOC15           15

//trigger source define
#define  SOFTWARE_TRIG     0
#define  CPU1_TIMER0_TRIG  1
#define  CPU1_TIMER1_TRIG   2
#define  CPU1_TIMER2_TRIG   3
#define  GPIO_TRIG         4
#define  EPWM1_SOCA_C_TRIG 5
#define  EPWM1_SOCB_D_TRIG 6
#define  EPWM2_SOCA_C_TRIG 7
#define  EPWM2_SOCB_D_TRIG 8
#define  EPWM3_SOCA_C_TRIG 9
#define  EPWM3_SOCB_D_TRIG 10
#define  EPWM4_SOCA_C_TRIG 11
#define  EPWM4_SOCB_D_TRIG 12
#define  EPWM5_SOCA_C_TRIG 13
#define  EPWM5_SOCB_D_TRIG 14
#define  EPWM6_SOCA_C_TRIG 15
#define  EPWM6_SOCB_D_TRIG 16
#define  EPWM7_SOCA_C_TRIG 17
#define  EPWM7_SOCB_D_TRIG 18
#define  EPWM8_SOCA_C_TRIG 19
#define  EPWM8_SOCB_D_TRIG 20
#define  EPWM9_SOCA_C_TRIG 21
#define  EPWM9_SOCB_D_TRIG 22
#define  EPWM10_SOCA_C_TRIG 23
#define  EPWM10_SOCB_D_TRIG 24
#define  EPWM11_SOCA_C_TRIG 25
#define  EPWM11_SOCB_D_TRIG 26
#define  EPWM12_SOCA_C_TRIG 27
#define  EPWM12_SOCB_D_TRIG 28
#define  CPU2_TIMER0_TRIG  29
#define  CPU2_TIMER1_TRIG   30
#define  CPU2_TIMER2_TRIG   31


#define ADC_VOLTAGE_RANGE	(3.0)
#define ADC_NUMERIC_RANGE	(0xfff)
#define ADC_VOLTS_PER_BIT	0.000732421875//(ADC_VOLTAGE_RANGE/ADC_NUMERIC_RANGE)
//----------------------------------------------------------------------------

// The following pointer to a function call calibrates the ADC reference,
// DAC offset, and internal oscillators
#define Device_cal (void   (*)(void))0x070282

// The following pointers to functions calibrate the ADC linearity.  Use this
// in the AdcSetMode(...) function only
#define CalAdcaINL (void   (*)(void))0x0703B4
#define CalAdcbINL (void   (*)(void))0x0703B2
#define CalAdccINL (void   (*)(void))0x0703B0
#define CalAdcdINL (void   (*)(void))0x0703AE

// The following pointer to a function call looks up the ADC offset trim for a
// given condition. Use this in the AdcSetMode(...) function only.
#define GetAdcOffsetTrimOTP (Uint16 (*)(Uint16 OTPoffset))0x0703AC



class Class_ADDrv
	{
	private:
		INT16			m_i16ADCSlope;					//ADC校正斜率
		INT16			m_i16ADCOffset;					//ADC校正偏移量
		//UNLONG 			m_unAdResult1V;
		//UNLONG 			m_unAdResult2V;
		UNLONG			m_unAdResult1V5;
		UNLONG			m_unAdResultGND;
		UNLONG  		m_unAdIlaMid;
		UNLONG  		m_unAdIlbMid;
		UNLONG  		m_unAdIlcMid;
		UNLONG  		m_unAdIinvaMid;
		UNLONG  		m_unAdIinvbMid;  
		UNLONG  		m_unAdIinvcMid;  
		UNLONG          m_unAdIlaFilterMid;
		UNLONG          m_unAdIlbFilterMid;
		UNLONG          m_unAdIlcFilterMid;
		//UNLONG			m_unAdIoutaMid;
		//UNLONG			m_unAdIoutbMid;
		//UNLONG			m_unAdIoutcMid;
		  
		INT16			m_i16ADC_Ila_0;
		INT16			m_i16ADC_Ilb_0;  
		INT16			m_i16ADC_Ilc_0;  
		INT16			m_i16ADC_Iinva_0;
		INT16			m_i16ADC_Iinvb_0;
		INT16			m_i16ADC_Iinvc_0;
		INT16			m_i16ADC_Vbpa_0;
		INT16			m_i16ADC_Vbpb_0;
		INT16			m_i16ADC_Vbpc_0;
		
		INT16			m_i16ADC_VbusP_0;
		INT16			m_i16ADC_VbusN_0; 
		
		INT16			m_i16ADC_Vinva_0;						
		INT16			m_i16ADC_Vinvb_0;						
		INT16			m_i16ADC_Vinvc_0;						
		INT16			m_i16ADC_Iouta_0;						
		INT16			m_i16ADC_Ioutb_0;						
		INT16			m_i16ADC_Ioutc_0;						
		     									
		INT16			m_i16ADC_IlaFilter_0;						
		INT16			m_i16ADC_IlbFilter_0;								     									
		INT16			m_i16ADC_IlcFilter_0;						
		INT16			m_i16ADC_VinvaDc_0;
		INT16			m_i16ADC_VinvbDc_0;	
		INT16			m_i16ADC_VinvcDc_0;					
		INT16			m_i16ADC_Vouta_0;				
		INT16			m_i16ADC_Voutb_0;		
		INT16			m_i16ADC_Voutc_0;
		INT16			m_i16ADC_IgbtTemp; 
//	    INT16			m_i16ADC_IgbtTemp
//	    INT16			m_i16ADC_
//	    INT16			m_i16ADC_
		INT16			m_i16ADC_Vina_0;
		INT16			m_i16ADC_Vinb_0;
		INT16			m_i16ADC_Vinc_0;


		void			AdcSetMode(Uint16 adc, Uint16 resolution, Uint16 signalmode);
		void			CalAdcINL(Uint16 adc);
	    
	public:

		void 			Drv_ADInit(void);
		void 			Drv_ADAdjust(void);
		void 			Drv_Int_Sampling1Data(void);
		void 			Drv_Int_Sampling2Data(void);
		void			Drv_AD_SOC(void);

		
	friend	class			Class_DCBus;
	friend	class			Class_Parallel;  
	friend	class			Class_Switches;
	friend	class			Class_Bypass;
	friend	class			Class_Output;
	friend	class			Class_Inverter;
	friend	class			Class_Debug;
	friend	class			Class_IPC;
		
	};
	
#endif

//===========================================================================
// End of file.
//===========================================================================
