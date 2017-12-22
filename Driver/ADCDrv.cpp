/*==========================================================*/
/* Title		:	ADCDrv.cpp								*/
/* Description	: 	Class_ADCDrv realization 				*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#include "ADCDrv.h"

/************************************************************************************
函数名：	Drv_ADAdjust()
功能描述：	用两点方法对DSP的所有采样通道进行校准
函数调用：	无
************************************************************************************/


void 	Class_ADDrv::Drv_ADAdjust(void)
{	

}

/************************************************************************************
函数名：	Drv_ADInit()
功能描述：	AD模块初始化
函数调用：	无
************************************************************************************/
void 	Class_ADDrv::Drv_ADInit(void)
{
	EALLOW;

	//write configurations
	AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
	AdcbRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
	AdccRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
	AdcdRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4

    objADDrv.AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    objADDrv.AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    objADDrv.AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    objADDrv.AdcSetMode(ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);


	//power up the ADC
	AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
	AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;
	AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;
	AdcdRegs.ADCCTL1.bit.ADCPWDNZ = 1;
	//delay for 1ms to allow ADC time to power up
	objTimerDrv.Drv_usDelay(1000);

	AdcaRegs.ADCINTSEL1N2.bit.INT1CONT = 0;// Disable continuous sampling for ADCINT1
	AdcbRegs.ADCINTSEL1N2.bit.INT1CONT = 0;// Disable continuous sampling for ADCINT1
	AdccRegs.ADCINTSEL1N2.bit.INT1CONT = 0;// Disable continuous sampling for ADCINT1
	AdcdRegs.ADCINTSEL1N2.bit.INT1CONT = 0;// Disable continuous sampling for ADCINT1

    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1; //ADCINTs Trigger at end of conversion
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;
	AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;
	AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 1;

	AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = EOC5; //end of EOC5 will set INT1 interrupt flag
	AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = EOC3; //end of EOC3 will set INT1 interrupt flag
	AdccRegs.ADCINTSEL1N2.bit.INT1SEL = EOC4; //end of EOC4 will set INT1 interrupt flag
	AdcdRegs.ADCINTSEL1N2.bit.INT1SEL = EOC4; //end of EOC4 will set INT1 interrupt flag
	
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 Interrupt
    AdcbRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 Interrupt
    AdccRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 Interrupt
    AdcdRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 Interrupt
    
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
    AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared


	//for 28377D， 20 ADC acitve channel ，ADCA A0-A5，ADCB B0-B3 ADCC C2-C4 ADCD D0-D4 ADCIN14，
	//ADCIN15 for any。 to save time， ADCC will sample them。
	AdcaRegs.ADCSOC0CTL.bit.ACQPS = 25; // minimum acquisition window，75ns
	AdcaRegs.ADCSOC1CTL.bit.ACQPS = 25; // minimum acquisition window，75ns
	AdcaRegs.ADCSOC2CTL.bit.ACQPS = 25; // minimum acquisition window，75ns
	AdcaRegs.ADCSOC3CTL.bit.ACQPS = 25; // minimum acquisition window，75ns
	AdcaRegs.ADCSOC4CTL.bit.ACQPS = 25; // minimum acquisition window，75ns
	AdcaRegs.ADCSOC5CTL.bit.ACQPS = 25; // minimum acquisition window，75ns
	AdcbRegs.ADCSOC0CTL.bit.ACQPS = 25; // minimum acquisition window，75ns
	AdcbRegs.ADCSOC1CTL.bit.ACQPS = 25; // minimum acquisition window，75ns
	AdcbRegs.ADCSOC2CTL.bit.ACQPS = 25; // minimum acquisition window，75ns
	AdcbRegs.ADCSOC3CTL.bit.ACQPS = 25; // minimum acquisition window，75ns
	AdccRegs.ADCSOC0CTL.bit.ACQPS = 25; // minimum acquisition window，75ns
	AdccRegs.ADCSOC1CTL.bit.ACQPS = 25; // minimum acquisition window，75ns
	AdccRegs.ADCSOC2CTL.bit.ACQPS = 25; // minimum acquisition window，75ns
	AdccRegs.ADCSOC3CTL.bit.ACQPS = 25; // minimum acquisition window，75ns
	AdccRegs.ADCSOC4CTL.bit.ACQPS = 25; // minimum acquisition window，75ns
	AdcdRegs.ADCSOC0CTL.bit.ACQPS = 25; // minimum acquisition window，75ns
	AdcdRegs.ADCSOC1CTL.bit.ACQPS = 25; // minimum acquisition window，75ns
	AdcdRegs.ADCSOC2CTL.bit.ACQPS = 25; // minimum acquisition window，75ns
	AdcdRegs.ADCSOC3CTL.bit.ACQPS = 25; // minimum acquisition window，75ns
	AdcdRegs.ADCSOC4CTL.bit.ACQPS = 25; // minimum acquisition window，75ns

	

	//SOC TRIGGER SOURCE SELECT
	AdcaRegs.ADCSOC0CTL.bit.TRIGSEL  = SOFTWARE_TRIG;
	AdcaRegs.ADCSOC1CTL.bit.TRIGSEL  = SOFTWARE_TRIG;
	AdcaRegs.ADCSOC2CTL.bit.TRIGSEL  = SOFTWARE_TRIG;
	AdcaRegs.ADCSOC3CTL.bit.TRIGSEL  = SOFTWARE_TRIG;
	AdcaRegs.ADCSOC4CTL.bit.TRIGSEL  = SOFTWARE_TRIG;
	AdcaRegs.ADCSOC5CTL.bit.TRIGSEL  = SOFTWARE_TRIG;
	AdcbRegs.ADCSOC0CTL.bit.TRIGSEL  = SOFTWARE_TRIG;
	AdcbRegs.ADCSOC1CTL.bit.TRIGSEL  = SOFTWARE_TRIG;
	AdcbRegs.ADCSOC2CTL.bit.TRIGSEL  = SOFTWARE_TRIG;
	AdcbRegs.ADCSOC3CTL.bit.TRIGSEL  = SOFTWARE_TRIG;
	AdccRegs.ADCSOC0CTL.bit.TRIGSEL =  SOFTWARE_TRIG;
	AdccRegs.ADCSOC1CTL.bit.TRIGSEL =  SOFTWARE_TRIG;
	AdccRegs.ADCSOC2CTL.bit.TRIGSEL =  SOFTWARE_TRIG;
	AdccRegs.ADCSOC3CTL.bit.TRIGSEL =  SOFTWARE_TRIG;
	AdccRegs.ADCSOC4CTL.bit.TRIGSEL =  SOFTWARE_TRIG;
	AdcdRegs.ADCSOC0CTL.bit.TRIGSEL  = SOFTWARE_TRIG;
	AdcdRegs.ADCSOC1CTL.bit.TRIGSEL  = SOFTWARE_TRIG;
	AdcdRegs.ADCSOC2CTL.bit.TRIGSEL  = SOFTWARE_TRIG;
	AdcdRegs.ADCSOC3CTL.bit.TRIGSEL  = SOFTWARE_TRIG;
	AdcdRegs.ADCSOC4CTL.bit.TRIGSEL  = SOFTWARE_TRIG;

	//SOC CHANNEL SELECT
	AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;
	AdcaRegs.ADCSOC1CTL.bit.CHSEL = 1;
	AdcaRegs.ADCSOC2CTL.bit.CHSEL = 2;
	AdcaRegs.ADCSOC3CTL.bit.CHSEL = 3;
	AdcaRegs.ADCSOC4CTL.bit.CHSEL = 4;
	AdcaRegs.ADCSOC5CTL.bit.CHSEL = 5;
	
	AdcbRegs.ADCSOC0CTL.bit.CHSEL = 0;
	AdcbRegs.ADCSOC1CTL.bit.CHSEL = 1;
	AdcbRegs.ADCSOC2CTL.bit.CHSEL = 2;
	AdcbRegs.ADCSOC3CTL.bit.CHSEL = 3;
	AdcbRegs.ADCSOC0CTL.bit.CHSEL = 14;
	AdcbRegs.ADCSOC1CTL.bit.CHSEL = 15;
	
	AdccRegs.ADCSOC2CTL.bit.CHSEL = 2;
	AdccRegs.ADCSOC3CTL.bit.CHSEL = 3;
	AdccRegs.ADCSOC4CTL.bit.CHSEL = 4;
	
	AdcdRegs.ADCSOC0CTL.bit.CHSEL = 0;
	AdcdRegs.ADCSOC1CTL.bit.CHSEL = 1;
	AdcdRegs.ADCSOC2CTL.bit.CHSEL = 2;
	AdcdRegs.ADCSOC3CTL.bit.CHSEL = 3;
	AdcdRegs.ADCSOC4CTL.bit.CHSEL = 4;


	EDIS;

}


void	Class_ADDrv::AdcSetMode(Uint16 adc, Uint16 resolution, Uint16 signalmode)
{
	Uint16 adcOffsetTrimOTPIndex; //index into OTP table of ADC offset trims
	Uint16 adcOffsetTrim; //temporary ADC offset trim
	
	//re-populate INL trim
	objADDrv.CalAdcINL(adc);
	
	if(0xFFFF != *((Uint16*)GetAdcOffsetTrimOTP))
	{
		//offset trim function is programmed into OTP, so call it

		//calculate the index into OTP table of offset trims and call
		//function to return the correct offset trim
		adcOffsetTrimOTPIndex = 4*adc + 2*resolution + 1*signalmode;
		adcOffsetTrim = (*GetAdcOffsetTrimOTP)(adcOffsetTrimOTPIndex);
	}
	else 
	{
		//offset trim function is not populated, so set offset trim to 0
		adcOffsetTrim = 0;
	}

	//Apply the resolution and signalmode to the specified ADC.
	//Also apply the offset trim and, if needed, linearity trim correction.
	switch(adc)
	{
		case ADC_ADCA:
			AdcaRegs.ADCCTL2.bit.RESOLUTION = resolution;
			AdcaRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
			AdcaRegs.ADCOFFTRIM.all = adcOffsetTrim;
			if(ADC_RESOLUTION_12BIT == resolution)
			{

				//12-bit linearity trim workaround
				AdcaRegs.ADCINLTRIM1 &= 0xFFFF0000;
				AdcaRegs.ADCINLTRIM2 &= 0xFFFF0000;
				AdcaRegs.ADCINLTRIM4 &= 0xFFFF0000;
				AdcaRegs.ADCINLTRIM5 &= 0xFFFF0000;
			}
		break;
		case ADC_ADCB:
			AdcbRegs.ADCCTL2.bit.RESOLUTION = resolution;
			AdcbRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
			AdcbRegs.ADCOFFTRIM.all = adcOffsetTrim;
			if(ADC_RESOLUTION_12BIT == resolution)
			{

				//12-bit linearity trim workaround
				AdcbRegs.ADCINLTRIM1 &= 0xFFFF0000;
				AdcbRegs.ADCINLTRIM2 &= 0xFFFF0000;
				AdcbRegs.ADCINLTRIM4 &= 0xFFFF0000;
				AdcbRegs.ADCINLTRIM5 &= 0xFFFF0000;
			}
		break;
		case ADC_ADCC:
			AdccRegs.ADCCTL2.bit.RESOLUTION = resolution;
			AdccRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
			AdccRegs.ADCOFFTRIM.all = adcOffsetTrim;
			if(ADC_RESOLUTION_12BIT == resolution)
			{

				//12-bit linearity trim workaround
				AdccRegs.ADCINLTRIM1 &= 0xFFFF0000;
				AdccRegs.ADCINLTRIM2 &= 0xFFFF0000;
				AdccRegs.ADCINLTRIM4 &= 0xFFFF0000;
				AdccRegs.ADCINLTRIM5 &= 0xFFFF0000;
			}
		break;
		case ADC_ADCD:
			AdcdRegs.ADCCTL2.bit.RESOLUTION = resolution;
			AdcdRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
			AdcdRegs.ADCOFFTRIM.all = adcOffsetTrim;
			if(ADC_RESOLUTION_12BIT == resolution)
			{

				//12-bit linearity trim workaround
				AdcdRegs.ADCINLTRIM1 &= 0xFFFF0000;
				AdcdRegs.ADCINLTRIM2 &= 0xFFFF0000;
				AdcdRegs.ADCINLTRIM4 &= 0xFFFF0000;
				AdcdRegs.ADCINLTRIM5 &= 0xFFFF0000;
			}
		break;
	}

}

void Class_ADDrv::CalAdcINL(Uint16 adc)
{
	switch(adc)
	{
		case ADC_ADCA:
			if(0xFFFF != *((Uint16*)CalAdcaINL))
			{
				//trim function is programmed into OTP, so call it
				(*CalAdcaINL)();
			}
			else 
			{
				//do nothing, no INL trim function populated
			}
			break;
		case ADC_ADCB:
			if(0xFFFF != *((Uint16*)CalAdcbINL))
			{
				//trim function is programmed into OTP, so call it
				(*CalAdcbINL)();
			}
			else 
			{
				//do nothing, no INL trim function populated
			}
			break;
		case ADC_ADCC:
			if(0xFFFF != *((Uint16*)CalAdccINL))
			{
				//trim function is programmed into OTP, so call it
				(*CalAdccINL)();
			}
			else 
			{
				//do nothing, no INL trim function populated
			}
			break;
		case ADC_ADCD:
			if(0xFFFF != *((Uint16*)CalAdcdINL))
			{
				//trim function is programmed into OTP, so call it
				(*CalAdcdINL)();
			}
			else 
			{
				//do nothing, no INL trim function populated
			}
			break;
	}
}

/************************************************************************************
//函数名称：    Drv_Int_Sampling1Data()
//功能描述：    接收第一轮采样数据
************************************************************************************/
 
#pragma CODE_SECTION(".Epwm1Funcs") 			

void 	Class_ADDrv::Drv_Int_Sampling1Data(void)
{
    UINT i, j;

	//
    //wait for ADCA-SOCD to complete, then acknowledge flag
    //
    while(AdcaRegs.ADCINTFLG.bit.ADCINT1 == 0);
	while(AdcbRegs.ADCINTFLG.bit.ADCINT1 == 0);
	while(AdccRegs.ADCINTFLG.bit.ADCINT1 == 0);
	while(AdcdRegs.ADCINTFLG.bit.ADCINT1 == 0);
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
	AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
	AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

	
	volatile INT16	i16IntAx_0;
	
	m_i16ADC_Ila_0 = AdcaResultRegs.ADCRESULT0;
	m_i16ADC_Ilb_0 = AdcaResultRegs.ADCRESULT1;
	m_i16ADC_Ilc_0 = AdcaResultRegs.ADCRESULT2;
	
	m_i16ADC_Iinva_0 = AdcaResultRegs.ADCRESULT3;
	m_i16ADC_Iinvb_0 = AdcaResultRegs.ADCRESULT4;
	m_i16ADC_Iinvc_0 = AdcaResultRegs.ADCRESULT5;
	
	m_i16ADC_Vinva_0 = AdcbResultRegs.ADCRESULT0;
	m_i16ADC_Vinvb_0 = AdcbResultRegs.ADCRESULT1;
	m_i16ADC_Vinvc_0 = AdcbResultRegs.ADCRESULT2;
	
	m_i16ADC_Vbpa_0 = AdcbResultRegs.ADCRESULT3;
	m_i16ADC_Vbpb_0 = AdcbResultRegs.ADCRESULT4;
	m_i16ADC_Vbpc_0 = AdcbResultRegs.ADCRESULT5;
	
	//receive Vbus Data from Rec,
	objIPC.Drv_IPC_ADSample_Receive(i, j);
	m_i16ADC_VbusP_0 = (INT16)i;
	m_i16ADC_VbusN_0 = (INT16)j;

	m_i16ADC_Ila_0 = (((INT32)m_i16ADCSlope)*(m_i16ADC_Ila_0-m_unAdIlaMid.half.hword))>>12;
	m_i16ADC_Ilb_0 = (((INT32)m_i16ADCSlope)*(m_i16ADC_Ilb_0-m_unAdIlbMid.half.hword))>>12;
	m_i16ADC_Ilc_0 = (((INT32)m_i16ADCSlope)*(m_i16ADC_Ilc_0-m_unAdIlcMid.half.hword))>>12;
	m_i16ADC_Iinva_0 = (((INT32)m_i16ADCSlope)*(m_i16ADC_Iinva_0-m_unAdIinvaMid.half.hword))>>12;
	m_i16ADC_Iinvb_0 = (((INT32)m_i16ADCSlope)*(m_i16ADC_Iinvb_0-m_unAdIinvbMid.half.hword))>>12;
	m_i16ADC_Iinvc_0 = (((INT32)m_i16ADCSlope)*(m_i16ADC_Iinvc_0-m_unAdIinvcMid.half.hword))>>12;							
	m_i16ADC_Vinva_0 = ((((INT32)m_i16ADCSlope)*m_i16ADC_Vinva_0)>>12)+m_i16ADCOffset-2048;
	m_i16ADC_Vinvb_0 = ((((INT32)m_i16ADCSlope)*m_i16ADC_Vinvb_0)>>12)+m_i16ADCOffset-2048;
	m_i16ADC_Vinvc_0 = ((((INT32)m_i16ADCSlope)*m_i16ADC_Vinvc_0)>>12)+m_i16ADCOffset-2048;
	m_i16ADC_Vbpa_0 = ((((INT32)m_i16ADCSlope)*m_i16ADC_Vbpa_0)>>12)+m_i16ADCOffset-2048;
	m_i16ADC_Vbpb_0 = ((((INT32)m_i16ADCSlope)*m_i16ADC_Vbpb_0)>>12)+m_i16ADCOffset-2048;
	m_i16ADC_Vbpc_0 = ((((INT32)m_i16ADCSlope)*m_i16ADC_Vbpc_0)>>12)+m_i16ADCOffset-2048;
	/*
	Adapter20K增加检测输出电流。
	//m_i16ADC_Iouta_0 = ((((INT32)m_i16ADCSlope)*m_i16ADC_Iouta_0)>>12)+m_i16ADCOffset-2048;
	//m_i16ADC_Ioutb_0 = ((((INT32)m_i16ADCSlope)*m_i16ADC_Ioutb_0)>>12)+m_i16ADCOffset-2048;
	//m_i16ADC_Ioutc_0 = ((((INT32)m_i16ADCSlope)*m_i16ADC_Ioutc_0)>>12)+m_i16ADCOffset-2048;
	m_i16ADC_Iouta_0 = ((((INT32)m_i16ADCSlope) * (m_i16ADC_Iouta_0-m_unAdIoutaMid.half.hword))>>12);
	m_i16ADC_Ioutb_0 = ((((INT32)m_i16ADCSlope) * (m_i16ADC_Ioutb_0-m_unAdIoutbMid.half.hword))>>12);
	m_i16ADC_Ioutc_0 = ((((INT32)m_i16ADCSlope) * (m_i16ADC_Ioutc_0-m_unAdIoutcMid.half.hword))>>12);
	//修改结束。
	*/
	m_i16ADC_VbusP_0 = ((((INT32)m_i16ADCSlope)*m_i16ADC_VbusP_0)>>12)+m_i16ADCOffset;
	m_i16ADC_VbusN_0 = ((((INT32)m_i16ADCSlope)*m_i16ADC_VbusN_0)>>12)+m_i16ADCOffset;


}

/************************************************************************************
//函数名称：    Drv_Int_Sampling2Data()
//功能描述：    接收第二轮采样数据
************************************************************************************/

#pragma CODE_SECTION(".Epwm1Funcs") 			

void 	Class_ADDrv::Drv_Int_Sampling2Data(void)
{	
    UINT i, j, k;
    //
    //wait for ADCA-SOCD to complete, then acknowledge flag
    //
    while(AdcaRegs.ADCINTFLG.bit.ADCINT1 == 0);
	while(AdcbRegs.ADCINTFLG.bit.ADCINT1 == 0);
	while(AdccRegs.ADCINTFLG.bit.ADCINT1 == 0);
	while(AdcdRegs.ADCINTFLG.bit.ADCINT1 == 0);
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
	AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
	AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

		
	volatile INT16	i16IntAx_0;

	m_i16ADC_IlaFilter_0 = AdcaResultRegs.ADCRESULT0;
	m_i16ADC_IlbFilter_0 = AdcaResultRegs.ADCRESULT1;
	m_i16ADC_IlcFilter_0 = AdcaResultRegs.ADCRESULT2;
	
	m_i16ADC_VinvaDc_0 = AdcaResultRegs.ADCRESULT3;
	m_i16ADC_VinvbDc_0 = AdcaResultRegs.ADCRESULT4;
	m_i16ADC_VinvcDc_0 = AdcaResultRegs.ADCRESULT5;
	
	m_i16ADC_Vouta_0 = AdcbResultRegs.ADCRESULT0;
	m_i16ADC_Voutb_0 = AdcbResultRegs.ADCRESULT1;
	m_i16ADC_Voutc_0 = AdcbResultRegs.ADCRESULT2;
	
	m_i16ADC_IgbtTemp = AdcdResultRegs.ADCRESULT4;

	m_i16ADC_Vina_0 = AdcbResultRegs.ADCRESULT3;
	m_i16ADC_Vinb_0 = AdcbResultRegs.ADCRESULT4;
	m_i16ADC_Vinc_0 = AdcbResultRegs.ADCRESULT5;
	//refresh Vin Data,and transmit to REC
	i = (UINT16)m_i16ADC_Vina_0;
	j = (UINT16)m_i16ADC_Vinb_0;
	k = (UINT16)m_i16ADC_Vinc_0;
	objIPC.Drv_IPC_ADSample_Transmit(i, j, k);
	
	m_i16ADC_IlaFilter_0 = (((INT32)m_i16ADCSlope)*(m_i16ADC_IlaFilter_0-m_unAdIlaFilterMid.half.hword))>>12;
	m_i16ADC_IlbFilter_0 = (((INT32)m_i16ADCSlope)*(m_i16ADC_IlbFilter_0-m_unAdIlbFilterMid.half.hword))>>12;
	m_i16ADC_IlcFilter_0 = (((INT32)m_i16ADCSlope)*(m_i16ADC_IlcFilter_0-m_unAdIlcFilterMid.half.hword))>>12;	
	m_i16ADC_VinvaDc_0 = ((((INT32)m_i16ADCSlope)*m_i16ADC_VinvaDc_0)>>12)+m_i16ADCOffset-2048;
	m_i16ADC_VinvbDc_0 = ((((INT32)m_i16ADCSlope)*m_i16ADC_VinvbDc_0)>>12)+m_i16ADCOffset-2048;
	m_i16ADC_VinvcDc_0 = ((((INT32)m_i16ADCSlope)*m_i16ADC_VinvcDc_0)>>12)+m_i16ADCOffset-2048;
	m_i16ADC_Vouta_0 = ((((INT32)m_i16ADCSlope)*m_i16ADC_Vouta_0)>>12)+m_i16ADCOffset-2048;
	m_i16ADC_Voutb_0 = ((((INT32)m_i16ADCSlope)*m_i16ADC_Voutb_0)>>12)+m_i16ADCOffset-2048;
	m_i16ADC_Voutc_0 = ((((INT32)m_i16ADCSlope)*m_i16ADC_Voutc_0)>>12)+m_i16ADCOffset-2048;	

	
}
#pragma CODE_SECTION(".Epwm1Funcs")

void Class_ADDrv::Drv_AD_SOC(void)
{
	//Set SOC0 is highest round robin priority
	AdcaRegs.ADCSOCPRICTL.bit.RRPOINTER = 0x10;
	AdcbRegs.ADCSOCPRICTL.bit.RRPOINTER = 0x10;
	AdccRegs.ADCSOCPRICTL.bit.RRPOINTER = 0x10;
	AdcdRegs.ADCSOCPRICTL.bit.RRPOINTER = 0x10;
	
	AdcaRegs.ADCSOCFRC1.all = 0x003F;
	AdcbRegs.ADCSOCFRC1.all = 0x000F;
	AdccRegs.ADCSOCFRC1.all = 0x001F;
	AdcdRegs.ADCSOCFRC1.all = 0x001F;

}


//===========================================================================
// End of file.
//===========================================================================

