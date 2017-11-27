/*==========================================================*/
/* Title		:	SysDrv.cpp								*/
/* Description	: 	Class_SysDrv realization 				*/
/* Date			:	Apr.2008								*/
/*==========================================================*/
#include "SysDrv.h"

/**********************************************************************************
函数名：	Drv_InitPll()			
功能描述：	PLL模块初始化
函数调用：	无
***********************************************************************************/
Uint16	Class_SysDrv::Drv_InitPll(Uint16 clock_source, Uint16 imult, Uint16 fmult, Uint16 divsel)
{  
   
       Uint16 SCSR, WDCR, WDWCR, intStatus;
    if((clock_source == ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL)    &&
       (imult        == ClkCfgRegs.SYSPLLMULT.bit.IMULT)           &&
       (fmult        == ClkCfgRegs.SYSPLLMULT.bit.FMULT)           &&
       (divsel       == ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV))
    {
        //
        // Everything is set as required, so just return
        //
        return;
    }

    if(clock_source != ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL)
    {
        switch (clock_source)
        {
            case INT_OSC1:
				EALLOW;
                	ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL = 2;     // Clk Src = INTOSC1
				EDIS;
				break;

            case INT_OSC2:
                EALLOW;
    				ClkCfgRegs.CLKSRCCTL1.bit.INTOSC2OFF=0;         // Turn on INTOSC2
    				ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL = 0;     // Clk Src = INTOSC2
    			EDIS;
                break;

            case XTAL_OSC:
				EALLOW;
				   ClkCfgRegs.CLKSRCCTL1.bit.XTALOFF=0; 		   // Turn on XTALOSC
				   ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL = 1;	   // Clk Src = XTAL
				EDIS;
                break;
        }
    }

    EALLOW;
    if(imult != ClkCfgRegs.SYSPLLMULT.bit.IMULT ||
       fmult != ClkCfgRegs.SYSPLLMULT.bit.FMULT)
    {
        Uint16 i;

        //
        // This bit is reset only by POR
        //
        if(DevCfgRegs.SYSDBGCTL.bit.BIT_0 == 1)
        {
            //
            // The user can optionally insert handler code here. This will only
            // be executed if a watchdog reset occurred after a failed system
            // PLL initialization. See your device user's guide for more
            // information.
            //
            // If the application has a watchdog reset handler, this bit should
            // be checked to determine if the watchdog reset occurred because
            // of the PLL.
            //
            // No action here will continue with retrying the PLL as normal.
            //
        }

        //
        // Bypass PLL and set dividers to /1
        //
        ClkCfgRegs.SYSPLLCTL1.bit.PLLCLKEN = 0;
        asm(" RPT #20 || NOP");
        ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = 0;

        //
        // Lock the PLL five times. This helps ensure a successful start.
        // Five is the minimum recommended number. The user can increase this
        // number according to allotted system initialization time.
        //
        for(i = 0; i < 5; i++)
        {
            //
            // Turn off PLL
            //
            ClkCfgRegs.SYSPLLCTL1.bit.PLLEN = 0;
            asm(" RPT #20 || NOP");

            //
            // Write multiplier, which automatically turns on the PLL
            //
            ClkCfgRegs.SYSPLLMULT.all = ((fmult << 8U) | imult);

            //
            // Wait for the SYSPLL lock counter
            //
            while(ClkCfgRegs.SYSPLLSTS.bit.LOCKS != 1)
            {
                //
                // Uncomment to service the watchdog
                //
                // ServiceDog();
            }
        }
    }

    //
    // Set divider to produce slower output frequency to limit current increase
    //
    if(divsel != PLLCLK_BY_126)
    {
         ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = divsel+1;
    }
	else
    {
         ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = divsel;
    }

    //
    //      *CAUTION*
    // It is recommended to use the following watchdog code to monitor the PLL
    // startup sequence. If your application has already cleared the watchdog
    // SCRS[WDOVERRIDE] bit this cannot be done. It is recommended not to clear
    // this bit until after the PLL has been initiated.
    //

    //
    // Backup User Watchdog
    //
    SCSR = WdRegs.SCSR.all;
    WDCR = WdRegs.WDCR.all;
    WDWCR = WdRegs.WDWCR.all;

    //
    // Disable windowed functionality, reset counter
    //
    EALLOW;
    WdRegs.WDWCR.all = 0x0;
    WdRegs.WDKEY.bit.WDKEY = 0x55;
    WdRegs.WDKEY.bit.WDKEY = 0xAA;

    //
    // Disable global interrupts
    //
    intStatus = __disable_interrupts();

    //
    // Configure for watchdog reset and to run at max frequency
    //
    WdRegs.SCSR.all = 0x0;
    WdRegs.WDCR.all = 0x28;

    //
    // This bit is reset only by power-on-reset (POR) and will not be cleared
    // by a WD reset
    //
    DevCfgRegs.SYSDBGCTL.bit.BIT_0 = 1;

    //
    // Enable PLLSYSCLK is fed from system PLL clock
    //
    ClkCfgRegs.SYSPLLCTL1.bit.PLLCLKEN = 1;

    //
    // Delay to ensure system is clocking from PLL prior to clearing status bit
    //
    asm(" RPT #20 || NOP");

    //
    // Clear bit
    //
    DevCfgRegs.SYSDBGCTL.bit.BIT_0 = 0;

    //
    // Restore user watchdog, first resetting counter
    //
    WdRegs.WDKEY.bit.WDKEY = 0x55;
    WdRegs.WDKEY.bit.WDKEY = 0xAA;

    WDCR |= 0x2C;                     // Setup WD key--KEY bits always read 0
	WdRegs.WDCR.all = WDCR;
	WdRegs.WDWCR.all = WDWCR;
    WdRegs.SCSR.all = SCSR & 0xFFFE;  // Mask write to bit 0 (W1toClr)

    //
    // Restore state of ST1[INTM]. This was set by the __disable_interrupts()
    // intrinsic previously.
    //
    if(!(intStatus & 0x1))
    {
        EINT;
    }

    //
    // Restore state of ST1[DBGM]. This was set by the __disable_interrupts()
    // intrinsic previously.
    //
    if(!(intStatus & 0x2))
    {
        asm(" CLRC DBGM");
    }

    //
    // 200 PLLSYSCLK delay to allow voltage regulator to stabilize prior
    // to increasing entire system clock frequency.
    //
    asm(" RPT #200 || NOP");

    //
    // Set the divider to user value
    //
    ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = divsel;
    EDIS;
}
/**********************************************************************************
函数名：	Drv_InitSysCtrl()			
功能描述：	SysCtrl初始化
函数调用：	无
***********************************************************************************/
void	Class_SysDrv::Drv_InitSysCtrl(void)
{
		//
		// Disable the watchdog
		//
	objWatchDogDrv.Drv_DisableDog();

	
		//
		//		*IMPORTANT*
		//
		// The Device_cal function, which copies the ADC & oscillator calibration
		// values from TI reserved OTP into the appropriate trim registers, occurs
		// automatically in the Boot ROM. If the boot ROM code is bypassed during
		// the debug process, the following function MUST be called for the ADC and
		// oscillators to function according to specification. The clocks to the
		// ADC MUST be enabled before calling this function.
		//
		// See the device data manual and/or the ADC Reference Manual for more
		// information.
		//
#ifdef CPU1
	EALLOW;
	
		//
		// Enable pull-ups on unbonded IOs as soon as possible to reduce power
		// consumption.
		//
		EALLOW;
        GpioCtrlRegs.GPCPUD.all = ~0x80000000;  //GPIO 95
        GpioCtrlRegs.GPDPUD.all = ~0xFFFFFFF7;  //GPIOs 96-127
        GpioCtrlRegs.GPEPUD.all = ~0xFFFFFFDF;  //GPIOs 128-159 except for 133
        GpioCtrlRegs.GPFPUD.all = ~0x000001FF;  //GPIOs 160-168
        EDIS;
	
		CpuSysRegs.PCLKCR13.bit.ADC_A = 1;
		CpuSysRegs.PCLKCR13.bit.ADC_B = 1;
		CpuSysRegs.PCLKCR13.bit.ADC_C = 1;
		CpuSysRegs.PCLKCR13.bit.ADC_D = 1;
	
		//
		// Check if device is trimmed
		//
		if(*((Uint16 *)0x5D1B6) == 0x0000)
		{
			//
			// Device is not trimmed--apply static calibration values
			//
			AnalogSubsysRegs.ANAREFTRIMA.all = 31709;
			AnalogSubsysRegs.ANAREFTRIMB.all = 31709;
			AnalogSubsysRegs.ANAREFTRIMC.all = 31709;
			AnalogSubsysRegs.ANAREFTRIMD.all = 31709;
		}
	
		CpuSysRegs.PCLKCR13.bit.ADC_A = 0;
		CpuSysRegs.PCLKCR13.bit.ADC_B = 0;
		CpuSysRegs.PCLKCR13.bit.ADC_C = 0;
		CpuSysRegs.PCLKCR13.bit.ADC_D = 0;
		EDIS;
	
		//
		// Initialize the PLL control: SYSPLLMULT and SYSCLKDIVSEL.
		//
		// Defined options to be passed as arguments to this function are defined
		// in F2837xD_Examples.h.
		//
		// Note: The internal oscillator CANNOT be used as the PLL source if the
		// PLLSYSCLK is configured to frequencies above 194 MHz.
		//
		//	PLLSYSCLK = (XTAL_OSC) * (IMULT + FMULT) / (PLLSYSCLKDIV)
		//
#ifdef _TMS320_F28377D
	objSysDrv.Drv_InitPll(XTAL_OSC, IMULT_20, FMULT_0, PLLCLK_BY_2);
#else
	objSysDrv.Drv_InitPll(XTAL_OSC, IMULT_10, FMULT_0, PLLCLK_BY_2);
#endif // _TMS320_F28377D
	
#endif // CPU1


		objSysDrv.InitPeripheralClock();



}

/**********************************************************************************
函数名：	Drv_InitPieCtrl()			
功能描述：	PIECtrl初始化
函数调用：	无
***********************************************************************************/
void	Class_SysDrv::Drv_InitPieCtrl(void)
{
    //
    // Disable Interrupts at the CPU level:
    //
    DINT;

    //
    // Disable the PIE
    //
    PieCtrlRegs.PIECTRL.bit.ENPIE = 0;

	//
    // Clear all PIEIER registers:
    //
	PieCtrlRegs.PIEIER1.all = 0;
	PieCtrlRegs.PIEIER2.all = 0;
	PieCtrlRegs.PIEIER3.all = 0;
	PieCtrlRegs.PIEIER4.all = 0;
	PieCtrlRegs.PIEIER5.all = 0;
	PieCtrlRegs.PIEIER6.all = 0;
	PieCtrlRegs.PIEIER7.all = 0;
	PieCtrlRegs.PIEIER8.all = 0;
	PieCtrlRegs.PIEIER9.all = 0;
	PieCtrlRegs.PIEIER10.all = 0;
	PieCtrlRegs.PIEIER11.all = 0;
	PieCtrlRegs.PIEIER12.all = 0;

	//
    // Clear all PIEIFR registers:
    //
	PieCtrlRegs.PIEIFR1.all = 0;
	PieCtrlRegs.PIEIFR2.all = 0;
	PieCtrlRegs.PIEIFR3.all = 0;
	PieCtrlRegs.PIEIFR4.all = 0;
	PieCtrlRegs.PIEIFR5.all = 0;
	PieCtrlRegs.PIEIFR6.all = 0;
	PieCtrlRegs.PIEIFR7.all = 0;
	PieCtrlRegs.PIEIFR8.all = 0;
	PieCtrlRegs.PIEIFR9.all = 0;
	PieCtrlRegs.PIEIFR10.all = 0;
	PieCtrlRegs.PIEIFR11.all = 0;
	PieCtrlRegs.PIEIFR12.all = 0;	
    //
    // Disable CPU interrupts and clear all CPU interrupt flags:
    //
    IER = 0x0000;
    IFR = 0x0000;


}
/**********************************************************************************
	Peripheral Clock initial
**********************************************************************************/
void Class_SysDrv::InitPeripheralClock(void)
{
	//Enable Peripherials CLOCK
		EALLOW;

		ClkCfgRegs.LOSPCP.all = 0x0002;		// SYSCLK/4
		CpuSysRegs.PCLKCR0.bit.CLA1 = 1;
		CpuSysRegs.PCLKCR0.bit.DMA = 1;
		CpuSysRegs.PCLKCR0.bit.CPUTIMER0 = 1;
		CpuSysRegs.PCLKCR0.bit.CPUTIMER1 = 1;
		CpuSysRegs.PCLKCR0.bit.CPUTIMER2 = 1;
		
#ifdef CPU1
		CpuSysRegs.PCLKCR0.bit.HRPWM = 0;
#endif
		
		CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
		
#ifdef CPU1
		CpuSysRegs.PCLKCR1.bit.EMIF1 = 0;
		CpuSysRegs.PCLKCR1.bit.EMIF2 = 0;
#endif
		
		CpuSysRegs.PCLKCR2.bit.EPWM1 = 1;
		CpuSysRegs.PCLKCR2.bit.EPWM2 = 1;
		CpuSysRegs.PCLKCR2.bit.EPWM3 = 1;
		CpuSysRegs.PCLKCR2.bit.EPWM4 = 1;
		CpuSysRegs.PCLKCR2.bit.EPWM5 = 1;
		CpuSysRegs.PCLKCR2.bit.EPWM6 = 1;
		CpuSysRegs.PCLKCR2.bit.EPWM7 = 1;
		CpuSysRegs.PCLKCR2.bit.EPWM8 = 1;
		CpuSysRegs.PCLKCR2.bit.EPWM9 = 1;
		CpuSysRegs.PCLKCR2.bit.EPWM10 = 1;
		CpuSysRegs.PCLKCR2.bit.EPWM11 = 1;
		CpuSysRegs.PCLKCR2.bit.EPWM12 = 1;
		
		CpuSysRegs.PCLKCR3.bit.ECAP1 = 1;
		CpuSysRegs.PCLKCR3.bit.ECAP2 = 1;
		CpuSysRegs.PCLKCR3.bit.ECAP3 = 1;
		CpuSysRegs.PCLKCR3.bit.ECAP4 = 1;
		CpuSysRegs.PCLKCR3.bit.ECAP5 = 1;
		CpuSysRegs.PCLKCR3.bit.ECAP6 = 1;
		
		CpuSysRegs.PCLKCR4.bit.EQEP1 = 0;
		CpuSysRegs.PCLKCR4.bit.EQEP2 = 0;
		CpuSysRegs.PCLKCR4.bit.EQEP3 = 0;
		
		CpuSysRegs.PCLKCR6.bit.SD1 = 0;
		CpuSysRegs.PCLKCR6.bit.SD2 = 0;
		
		CpuSysRegs.PCLKCR7.bit.SCI_A = 1;
		CpuSysRegs.PCLKCR7.bit.SCI_B = 1;
		CpuSysRegs.PCLKCR7.bit.SCI_C = 1;
		CpuSysRegs.PCLKCR7.bit.SCI_D = 1;
		
		CpuSysRegs.PCLKCR8.bit.SPI_A = 1;
		CpuSysRegs.PCLKCR8.bit.SPI_B = 1;
		CpuSysRegs.PCLKCR8.bit.SPI_C = 1;
		
		CpuSysRegs.PCLKCR9.bit.I2C_A = 1;
		CpuSysRegs.PCLKCR9.bit.I2C_B = 1;
		
		CpuSysRegs.PCLKCR10.bit.CAN_A = 1;
		CpuSysRegs.PCLKCR10.bit.CAN_B = 1;
		
		CpuSysRegs.PCLKCR11.bit.McBSP_A = 0;
		CpuSysRegs.PCLKCR11.bit.McBSP_B = 0;
		
#ifdef CPU1
		CpuSysRegs.PCLKCR11.bit.USB_A = 1;
		
		CpuSysRegs.PCLKCR12.bit.uPP_A = 0;
#endif
		CpuSysRegs.PCLKCR13.bit.ADC_A = 1;
		CpuSysRegs.PCLKCR13.bit.ADC_B = 1;
		CpuSysRegs.PCLKCR13.bit.ADC_C = 1;
		CpuSysRegs.PCLKCR13.bit.ADC_D = 1;
		
		CpuSysRegs.PCLKCR14.bit.CMPSS1 = 0;
		CpuSysRegs.PCLKCR14.bit.CMPSS2 = 0;
		CpuSysRegs.PCLKCR14.bit.CMPSS3 = 0;
		CpuSysRegs.PCLKCR14.bit.CMPSS4 = 0;
		CpuSysRegs.PCLKCR14.bit.CMPSS5 = 0;
		CpuSysRegs.PCLKCR14.bit.CMPSS6 = 0;
		CpuSysRegs.PCLKCR14.bit.CMPSS7 = 0;
		CpuSysRegs.PCLKCR14.bit.CMPSS8 = 0;
		
		CpuSysRegs.PCLKCR16.bit.DAC_A = 0;
		CpuSysRegs.PCLKCR16.bit.DAC_B = 0;
		CpuSysRegs.PCLKCR16.bit.DAC_C = 0;
		
		EDIS;

}
/**********************************************************************************
函数名：	Drv_InitPieVectTable()			
功能描述：	PieVectTable初始化
函数调用：	无
***********************************************************************************/
const struct PIE_VECT_TABLE PieVectTableInit = {

	PIE_RESERVED,             // Reserved
    PIE_RESERVED,             // Reserved
    PIE_RESERVED,             // Reserved
    PIE_RESERVED,             // Reserved
    PIE_RESERVED,             // Reserved
    PIE_RESERVED,             // Reserved
    PIE_RESERVED,             // Reserved
    PIE_RESERVED,             // Reserved
    PIE_RESERVED,             // Reserved
    PIE_RESERVED,             // Reserved
    PIE_RESERVED,             // Reserved
    PIE_RESERVED,             // Reserved
    PIE_RESERVED,             // Reserved
    PIE_RESERVED,             // CPU Timer 1 Interrupt
    PIE_RESERVED,             // CPU Timer 2 Interrupt
    PIE_RESERVED,             // Datalogging Interrupt
    PIE_RESERVED,             // RTOS Interrupt
    PIE_RESERVED,             // Emulation Interrupt
    PIE_RESERVED,             // Non-Maskable Interrupt
    PIE_RESERVED,             // Illegal Operation Trap
    PIE_RESERVED,             // User Defined Trap 1
    PIE_RESERVED,             // User Defined Trap 2
    PIE_RESERVED,             // User Defined Trap 3
    PIE_RESERVED,             // User Defined Trap 4
    PIE_RESERVED,             // User Defined Trap 5
    PIE_RESERVED,             // User Defined Trap 6
    PIE_RESERVED,             // User Defined Trap 7
    PIE_RESERVED,             // User Defined Trap 8
    PIE_RESERVED,             // User Defined Trap 9
    PIE_RESERVED,             // User Defined Trap 10
    PIE_RESERVED,             // User Defined Trap 11
    PIE_RESERVED,             // User Defined Trap 12
    
    PIE_RESERVED,             // 1.1 - ADCA Interrupt 1
    PIE_RESERVED,             // 1.2 - ADCB Interrupt 1
    PIE_RESERVED,             // 1.3 - ADCC Interrupt 1
    PIE_RESERVED,             // 1.4 - XINT1 Interrupt
    PIE_RESERVED,             // 1.5 - XINT2 Interrupt
    PIE_RESERVED,             // 1.6 - ADCD Interrupt 1
    PIE_RESERVED,             // 1.7 - Timer 0 Interrupt
    PIE_RESERVED,             // 1.8 - Standby and Halt Wakeup Interrupt
    
    PIE_RESERVED,             // 2.1 - ePWM1 Trip Zone Interrupt
    PIE_RESERVED,             // 2.2 - ePWM2 Trip Zone Interrupt
    PIE_RESERVED,             // 2.3 - ePWM3 Trip Zone Interrupt
    PIE_RESERVED,             // 2.4 - ePWM4 Trip Zone Interrupt
    PIE_RESERVED,             // 2.5 - ePWM5 Trip Zone Interrupt
    PIE_RESERVED,             // 2.6 - ePWM6 Trip Zone Interrupt
    PIE_RESERVED,             // 2.7 - ePWM7 Trip Zone Interrupt
    PIE_RESERVED,             // 2.8 - ePWM8 Trip Zone Interrupt
    
    EPWM1_INT_ISR,            // 3.1 - ePWM1 Interrupt
    PIE_RESERVED,             // 3.2 - ePWM2 Interrupt
    PIE_RESERVED,             // 3.3 - ePWM3 Interrupt
    PIE_RESERVED,             // 3.4 - ePWM4 Interrupt
    PIE_RESERVED,             // 3.5 - ePWM5 Interrupt
    PIE_RESERVED,             // 3.6 - ePWM6 Interrupt
    PIE_RESERVED,             // 3.7 - ePWM7 Interrupt
    PIE_RESERVED,             // 3.8 - ePWM8 Interrupt
    
    PIE_RESERVED,             // 4.1 - eCAP1 Interrupt
    PIE_RESERVED,             // 4.2 - eCAP2 Interrupt
    PIE_RESERVED,             // 4.3 - eCAP3 Interrupt
    PIE_RESERVED,             // 4.4 - eCAP4 Interrupt
    PIE_RESERVED,             // 4.5 - eCAP5 Interrupt
    PIE_RESERVED,             // 4.6 - eCAP6 Interrupt
    PIE_RESERVED,             // 4.7 - Reserved
    PIE_RESERVED,             // 4.8 - Reserved
    
    PIE_RESERVED,             // 5.1 - eQEP1 Interrupt
    PIE_RESERVED,             // 5.2 - eQEP2 Interrupt
    PIE_RESERVED,             // 5.3 - eQEP3 Interrupt
    PIE_RESERVED,             // 5.4 - Reserved
    PIE_RESERVED,             // 5.5 - Reserved
    PIE_RESERVED,             // 5.6 - Reserved
    PIE_RESERVED,             // 5.7 - Reserved
    PIE_RESERVED,             // 5.8 - Reserved
    
    PIE_RESERVED,             // 6.1 - SPIA Receive Interrupt
    PIE_RESERVED,             // 6.2 - SPIA Transmit Interrupt
    PIE_RESERVED,             // 6.3 - SPIB Receive Interrupt
    PIE_RESERVED,             // 6.4 - SPIB Transmit Interrupt
    PIE_RESERVED,             // 6.5 - McBSPA Receive Interrupt
    PIE_RESERVED,             // 6.6 - McBSPA Transmit Interrupt
    PIE_RESERVED,             // 6.7 - McBSPB Receive Interrupt
    PIE_RESERVED,             // 6.8 - McBSPB Transmit Interrupt
    
    PIE_RESERVED,             // 7.1 - DMA Channel 1 Interrupt
    PIE_RESERVED,             // 7.2 - DMA Channel 2 Interrupt
    PIE_RESERVED,             // 7.3 - DMA Channel 3 Interrupt
    PIE_RESERVED,             // 7.4 - DMA Channel 4 Interrupt
    PIE_RESERVED,             // 7.5 - DMA Channel 5 Interrupt
    PIE_RESERVED,             // 7.6 - DMA Channel 6 Interrupt
    PIE_RESERVED,             // 7.7 - Reserved
    PIE_RESERVED,             // 7.8 - Reserved
    
    PIE_RESERVED,             // 8.1 - I2CA Interrupt 1
    PIE_RESERVED,             // 8.2 - I2CA Interrupt 2
    PIE_RESERVED,             // 8.3 - I2CB Interrupt 1
    PIE_RESERVED,             // 8.4 - I2CB Interrupt 2
    PIE_RESERVED,             // 8.5 - SCIC Receive Interrupt
    PIE_RESERVED,             // 8.6 - SCIC Transmit Interrupt
    PIE_RESERVED,             // 8.7 - SCID Receive Interrupt
    PIE_RESERVED,             // 8.8 - SCID Transmit Interrupt
    
    PIE_RESERVED,             // 9.1 - SCIA Receive Interrupt
    PIE_RESERVED,             // 9.2 - SCIA Transmit Interrupt
    PIE_RESERVED,             // 9.3 - SCIB Receive Interrupt
    PIE_RESERVED,             // 9.4 - SCIB Transmit Interrupt
    ECAN0INTA_ISR,            // 9.5 - CANA Interrupt 0
    PIE_RESERVED,             // 9.6 - CANA Interrupt 1
    PIE_RESERVED,             // 9.7 - CANB Interrupt 0
    PIE_RESERVED,             // 9.8 - CANB Interrupt 1
    
    PIE_RESERVED,             // 10.1 - ADCA Event Interrupt
    PIE_RESERVED,             // 10.2 - ADCA Interrupt 2
    PIE_RESERVED,             // 10.3 - ADCA Interrupt 3
    PIE_RESERVED,             // 10.4 - ADCA Interrupt 4
    PIE_RESERVED,             // 10.5 - ADCB Event Interrupt
    PIE_RESERVED,             // 10.6 - ADCB Interrupt 2
    PIE_RESERVED,             // 10.7 - ADCB Interrupt 3
    PIE_RESERVED,             // 10.8 - ADCB Interrupt 4
    
    PIE_RESERVED,             // 11.1 - CLA1 Interrupt 1
    PIE_RESERVED,             // 11.2 - CLA1 Interrupt 2
    PIE_RESERVED,             // 11.3 - CLA1 Interrupt 3
    PIE_RESERVED,             // 11.4 - CLA1 Interrupt 4
    PIE_RESERVED,             // 11.5 - CLA1 Interrupt 5
    PIE_RESERVED,             // 11.6 - CLA1 Interrupt 6
    PIE_RESERVED,             // 11.7 - CLA1 Interrupt 7
    PIE_RESERVED,             // 11.8 - CLA1 Interrupt 8
    
    PIE_RESERVED,             // 12.1 - XINT3 Interrupt
    PIE_RESERVED,             // 12.2 - XINT4 Interrupt
    PIE_RESERVED,             // 12.3 - XINT5 Interrupt
    PIE_RESERVED,             // 12.4 - Reserved
    PIE_RESERVED,             // 12.5 - Reserved
    PIE_RESERVED,             // 12.6 - VCU Interrupt
    PIE_RESERVED,             // 12.7 - FPU Overflow Interrupt
    PIE_RESERVED,             // 12.8 - FPU Underflow Interrupt
    
    PIE_RESERVED,             // 1.9 - Reserved
    PIE_RESERVED,             // 1.10 - Reserved
    PIE_RESERVED,             // 1.11 - Reserved
    PIE_RESERVED,             // 1.12 - Reserved
    PIE_RESERVED,             // 1.13 - IPC Interrupt 0
    PIE_RESERVED,             // 1.14 - IPC Interrupt 1
    PIE_RESERVED,             // 1.15 - IPC Interrupt 2
    PIE_RESERVED,             // 1.16 - IPC Interrupt 3
    
    PIE_RESERVED,             // 2.9 - ePWM9 Trip Zone Interrupt
    PIE_RESERVED,             // 2.10 - ePWM10 Trip Zone Interrupt
    PIE_RESERVED,             // 2.11 - ePWM11 Trip Zone Interrupt
    PIE_RESERVED,             // 2.12 - ePWM12 Trip Zone Interrupt
    PIE_RESERVED,             // 2.13 - Reserved
    PIE_RESERVED,             // 2.14 - Reserved
    PIE_RESERVED,             // 2.15 - Reserved
    PIE_RESERVED,             // 2.16 - Reserved
    
    PIE_RESERVED,             // 3.9 - ePWM9 Interrupt
    PIE_RESERVED,             // 3.10 - ePWM10 Interrupt
    PIE_RESERVED,             // 3.11 - ePWM11 Interrupt
    PIE_RESERVED,             // 3.12 - ePWM12 Interrupt
    PIE_RESERVED,             // 3.13 - Reserved
    PIE_RESERVED,             // 3.14 - Reserved
    PIE_RESERVED,             // 3.15 - Reserved
    PIE_RESERVED,             // 3.16 - Reserved
    
    PIE_RESERVED,             // 4.9 - Reserved
    PIE_RESERVED,             // 4.10 - Reserved
    PIE_RESERVED,             // 4.11 - Reserved
    PIE_RESERVED,             // 4.12 - Reserved
    PIE_RESERVED,             // 4.13 - Reserved
    PIE_RESERVED,             // 4.14 - Reserved
    PIE_RESERVED,             // 4.15 - Reserved
    PIE_RESERVED,             // 4.16 - Reserved
    
    PIE_RESERVED,             // 5.9 - SD1 Interrupt
    PIE_RESERVED,             // 5.10 - SD2 Interrupt
    PIE_RESERVED,             // 5.11 - Reserved
    PIE_RESERVED,             // 5.12 - Reserved
    PIE_RESERVED,             // 5.13 - Reserved
    PIE_RESERVED,             // 5.14 - Reserved
    PIE_RESERVED,             // 5.15 - Reserved
    PIE_RESERVED,             // 5.16 - Reserved
    
    PIE_RESERVED,             // 6.9 - SPIC Receive Interrupt
    PIE_RESERVED,             // 6.10 - SPIC Transmit Interrupt
    PIE_RESERVED,             // 6.11 - Reserved
    PIE_RESERVED,             // 6.12 - Reserved
    PIE_RESERVED,             // 6.13 - Reserved
    PIE_RESERVED,             // 6.14 - Reserved
    PIE_RESERVED,             // 6.15 - Reserved
    PIE_RESERVED,             // 6.16 - Reserved
    
    PIE_RESERVED,             // 7.9 - Reserved
    PIE_RESERVED,             // 7.10 - Reserved
    PIE_RESERVED,             // 7.11 - Reserved
    PIE_RESERVED,             // 7.12 - Reserved
    PIE_RESERVED,             // 7.13 - Reserved
    PIE_RESERVED,             // 7.14 - Reserved
    PIE_RESERVED,             // 7.15 - Reserved
    PIE_RESERVED,             // 7.16 - Reserved
    
    PIE_RESERVED,             // 8.9 - Reserved
    PIE_RESERVED,             // 8.10 - Reserved
    PIE_RESERVED,             // 8.11 - Reserved
    PIE_RESERVED,             // 8.12 - Reserved
    PIE_RESERVED,             // 8.13 - Reserved
    PIE_RESERVED,             // 8.14 - Reserved
#ifdef CPU1    
    PIE_RESERVED,             // 8.15 - uPPA Interrupt
    PIE_RESERVED,             // 8.16 - Reserved
#elif defined(CPU2)
    PIE_RESERVED,             // 8.15 - Reserved
    PIE_RESERVED,             // 8.16 - Reserved
#endif    

    PIE_RESERVED,             // 9.9 - Reserved
    PIE_RESERVED,             // 9.10 - Reserved
    PIE_RESERVED,             // 9.11 - Reserved
    PIE_RESERVED,             // 9.12 - Reserved
    PIE_RESERVED,             // 9.13 - Reserved
    PIE_RESERVED,             // 9.14 - Reserved
#ifdef CPU1    
    PIE_RESERVED,             // 9.15 - USBA Interrupt
#elif defined(CPU2)
    PIE_RESERVED,             // 9.15 - Reserved
#endif    
    PIE_RESERVED,             // 9.16 - Reserved
    
    PIE_RESERVED,             // 10.9 - ADCC Event Interrupt
    PIE_RESERVED,             // 10.10 - ADCC Interrupt 2
    PIE_RESERVED,             // 10.11 - ADCC Interrupt 3
    PIE_RESERVED,             // 10.12 - ADCC Interrupt 4
    PIE_RESERVED,             // 10.13 - ADCD Event Interrupt
    PIE_RESERVED,             // 10.14 - ADCD Interrupt 2
    PIE_RESERVED,             // 10.15 - ADCD Interrupt 3
    PIE_RESERVED,             // 10.16 - ADCD Interrupt 4
    
    PIE_RESERVED,             // 11.9 - Reserved
    PIE_RESERVED,             // 11.10 - Reserved
    PIE_RESERVED,             // 11.11 - Reserved
    PIE_RESERVED,             // 11.12 - Reserved
    PIE_RESERVED,             // 11.13 - Reserved
    PIE_RESERVED,             // 11.14 - Reserved
    PIE_RESERVED,             // 11.15 - Reserved
    PIE_RESERVED,             // 11.16 - Reserved
    
    PIE_RESERVED,             // 12.9 - EMIF Error Interrupt
    PIE_RESERVED,    		  // 12.10 - RAM Correctable Error Interrupt
    PIE_RESERVED,  			  // 12.11 - Flash Correctable Error Interrupt
    PIE_RESERVED,     		  // 12.12 - RAM Access Violation Interrupt
    PIE_RESERVED,             // 12.13 - System PLL Slip Interrupt
    PIE_RESERVED,             // 12.14 - Auxiliary PLL Slip Interrupt
    PIE_RESERVED,             // 12.15 - CLA Overflow Interrupt
    PIE_RESERVED              // 12.16 - CLA Underflow Interrupt

};


void	Class_SysDrv::Drv_InitPieVectTable(void)
{
	INT16	i;
	UNLONG *Source = (UNLONG *) &PieVectTableInit;
	UNLONG *Dest = (UNLONG *) &PieVectTable;
		
    Source = Source + 3;
    Dest = Dest + 3;

    EALLOW;
    for(i = 0; i < 221; i++)
    {
    	*Dest++ = *Source++;
    }
    EDIS;


	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;	    // Enable the PIE Vector Table
	PieCtrlRegs.PIEIER9.bit.INTx5 = 1;		// ENABLE eCAN A0
	PieCtrlRegs.PIEIER3.bit.INTx1 = 1;		// Enable EPWM1 Interrupt
	PieCtrlRegs.PIEIER3.bit.INTx7 = 1;		// Enable EPWM7 Interrupt
}



//===========================================================================
// End of file.
//===========================================================================

