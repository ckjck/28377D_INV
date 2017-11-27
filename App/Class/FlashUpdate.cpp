/*==========================================================*/
/* Title		:	FlashUpdate.cpp							*/
/* Description	: 	Class_FlashUpdate realization 			*/
/* Date			:	Dec.2009								*/

/*------------------------------------------------------------------
 To use the F2808, F2806 or F2801 Flash API, the following steps
 must be followed:

      1. Modify Flash280x_API.config.h for your targets operating
         conditions.
      2. Include Flash280x_API_Library.h in the application.
      3. Add the approparite Flash API library to the project.

  The user's code is responsible for the following:

      4. Initalize the PLL to the proper CPU operating frequency.
      5. If required, copy the flash API functions into on-chip zero waitstate
         RAM.  
      6. Initalize the Flash_CPUScaleFactor variable to SCALE_FACTOR
      7. Initalize the callback function pointer or set it to NULL
      8. Optional: Run the Toggle test to confirm proper frequency configuration
         of the API. 
      9. Optional: Unlock the CSM.
     10. Make sure the PLL is not running in limp mode  
     11. Call the API functions: Flash_Erase(), Flash_Program(), Flash_Verify()
         
  The API functions will:
      
       Disable the watchdog
       Check the device PARTID. 
       Disable interrupts during time critical code.  
       Perform the desired operation and return status
------------------------------------------------------------------*/
/*==========================================================*/

#include  "FlashUpdate.h"


#pragma CODE_SECTION("Flash28Funcs");
void Class_FlashUpdate::Drv_SystemInit(void)
{		
	DINT;

	//disable watchdog
	EALLOW;
	WdRegs.WDCR.all = 0x0068;
	EDIS;
	
	Drv_InitSysCtrl();	
	Drv_GPIOInit();	
	Drv_PWMInit();
	Drv_SPIInit();
	Drv_eCANAInit();
}

#pragma CODE_SECTION("Flash28Funcs");
//This function initializes the PLLCR register.
void	Class_FlashUpdate::Drv_InitPll(Uint16 clock_source, Uint16 imult, Uint16 fmult, Uint16 divsel)
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


	
#pragma CODE_SECTION("Flash28Funcs");
//This function initializes the clocks to the peripheral modules.
void	Class_FlashUpdate::Drv_InitSysCtrl(void)
{	
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
		objFlashUpdate.Drv_InitPll(XTAL_OSC, IMULT_20, FMULT_0, PLLCLK_BY_2);
#else
		objFlashUpdate.Drv_InitPll(XTAL_OSC, IMULT_10, FMULT_0, PLLCLK_BY_2);
#endif // _TMS320_F28377D
	
#endif // CPU1


		objFlashUpdate.InitPeripheralClock();
}


/**********************************************************************************
	Peripheral Clock initial
**********************************************************************************/
void Class_FlashUpdate::InitPeripheralClock(void)
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


#pragma CODE_SECTION("Flash28Funcs");
void	Class_FlashUpdate::Drv_SPIInit(void)
{
	/* <=== SPI FIFO and SPI Registers initialization ===> */

    SpibRegs.SPICCR.all = 0x000F;	         // Reset on, rising edge, 16-bit char bits  
	SpibRegs.SPICTL.all = 0x000E;    		 // Enable master mode, delay half_cycle phase,
                                             // enable talk, and SPI int disabled.
	SpibRegs.SPIBRR.bit.SPI_BIT_RATE = 0x0001;				 // Baud Rate = 6.25MHz(最快20M)
	SpibRegs.SPICCR.all = 0x008F;		     // Relinquish SPI from Reset   
	SpibRegs.SPIPRI.bit.FREE = 1;            // Set so breakpoints don't disturb xmission

}

#pragma CODE_SECTION("Flash28Funcs");
void	Class_FlashUpdate::Drv_GPIOInit(void)
{
	Uint32 lockValA;
   Uint32 lockValB;
   Uint32 lockValC;
   Uint32 lockValD;
   Uint32 lockValE;

   EALLOW;

   //
   // Save the current value of the GPIO lock registers
   //
   lockValA = GpioCtrlRegs.GPALOCK.all;
   lockValB = GpioCtrlRegs.GPBLOCK.all;
   lockValC = GpioCtrlRegs.GPCLOCK.all;
   lockValD = GpioCtrlRegs.GPDLOCK.all;
   lockValE = GpioCtrlRegs.GPELOCK.all;

   //
   // Unlock the GPIO control registers
   //
   GpioCtrlRegs.GPALOCK.all = 0x00000000;
   GpioCtrlRegs.GPBLOCK.all = 0x00000000;
   GpioCtrlRegs.GPCLOCK.all = 0x00000000;
   GpioCtrlRegs.GPDLOCK.all = 0x00000000;
   GpioCtrlRegs.GPELOCK.all = 0x00000000;

   //
   // Clear the mux register fields that are about to be changed
   //
   GpioCtrlRegs.GPAMUX1.all    &= ~GPAMUX1_MASK;
   GpioCtrlRegs.GPAMUX2.all    &= ~GPAMUX2_MASK;
   GpioCtrlRegs.GPAGMUX1.all   &= ~GPAMUX1_MASK;
   GpioCtrlRegs.GPAGMUX2.all   &= ~GPAMUX2_MASK;
   GpioCtrlRegs.GPBMUX1.all    &= ~GPBMUX1_MASK;
   GpioCtrlRegs.GPBMUX2.all    &= ~GPBMUX2_MASK;
   GpioCtrlRegs.GPBGMUX1.all   &= ~GPBMUX1_MASK;
   GpioCtrlRegs.GPBGMUX2.all   &= ~GPBMUX2_MASK;
   GpioCtrlRegs.GPCMUX1.all    &= ~GPCMUX1_MASK;
   GpioCtrlRegs.GPCMUX2.all    &= ~GPCMUX2_MASK;
   GpioCtrlRegs.GPCGMUX1.all   &= ~GPCMUX1_MASK;
   GpioCtrlRegs.GPCGMUX2.all   &= ~GPCMUX2_MASK;
   GpioCtrlRegs.GPDMUX1.all    &= ~GPDMUX1_MASK;
   GpioCtrlRegs.GPDMUX2.all    &= ~GPDMUX2_MASK;
   GpioCtrlRegs.GPDGMUX1.all   &= ~GPDMUX1_MASK;
   GpioCtrlRegs.GPDGMUX2.all   &= ~GPDMUX2_MASK;
   GpioCtrlRegs.GPEMUX1.all    &= ~GPEMUX1_MASK;
   GpioCtrlRegs.GPEGMUX1.all   &= ~GPEMUX1_MASK;

   //
   // Write pin muxing to mux registers
   //
   GpioCtrlRegs.GPAGMUX1.all   |=  GPAGMUX1_VALUE;
   GpioCtrlRegs.GPAGMUX2.all   |=  GPAGMUX2_VALUE;
   GpioCtrlRegs.GPAMUX1.all    |=  GPAMUX1_VALUE;
   GpioCtrlRegs.GPAMUX2.all    |=  GPAMUX2_VALUE;
   GpioCtrlRegs.GPBGMUX1.all   |=  GPBGMUX1_VALUE;
   GpioCtrlRegs.GPBGMUX2.all   |=  GPBGMUX2_VALUE;
   GpioCtrlRegs.GPBMUX1.all    |=  GPBMUX1_VALUE;
   GpioCtrlRegs.GPBMUX2.all    |=  GPBMUX2_VALUE;
   GpioCtrlRegs.GPCGMUX1.all   |=  GPCGMUX1_VALUE;
   GpioCtrlRegs.GPCGMUX2.all   |=  GPCGMUX2_VALUE;
   GpioCtrlRegs.GPCMUX1.all    |=  GPCMUX1_VALUE;
   GpioCtrlRegs.GPCMUX2.all    |=  GPCMUX2_VALUE;
   GpioCtrlRegs.GPDGMUX1.all   |=  GPDGMUX1_VALUE;
   GpioCtrlRegs.GPDGMUX2.all   |=  GPDGMUX2_VALUE;
   GpioCtrlRegs.GPDMUX1.all    |=  GPDMUX1_VALUE;
   GpioCtrlRegs.GPDMUX2.all    |=  GPDMUX2_VALUE;
   GpioCtrlRegs.GPEGMUX1.all   |=  GPEGMUX1_VALUE;
   GpioCtrlRegs.GPEMUX1.all    |=  GPEMUX1_VALUE;

   //
   // Write pin analog mode select to registers
   //
   GpioCtrlRegs.GPBAMSEL.all   &= ~GPBAMSEL_MASK;
   GpioCtrlRegs.GPBAMSEL.all   |= GPBAMSEL_VALUE;

   //
   // Write Input X-BAR pin selection to registers
   //
   InputXbarRegs.INPUT1SELECT  = INPUT1SELECT_VALUE;
   InputXbarRegs.INPUT2SELECT  = INPUT2SELECT_VALUE;
   InputXbarRegs.INPUT3SELECT  = INPUT3SELECT_VALUE;
   InputXbarRegs.INPUT4SELECT  = INPUT4SELECT_VALUE;
   InputXbarRegs.INPUT5SELECT  = INPUT5SELECT_VALUE;
   InputXbarRegs.INPUT6SELECT  = INPUT6SELECT_VALUE;
   InputXbarRegs.INPUT7SELECT  = INPUT7SELECT_VALUE;
   InputXbarRegs.INPUT8SELECT  = INPUT8SELECT_VALUE;
   InputXbarRegs.INPUT9SELECT  = INPUT9SELECT_VALUE;
   InputXbarRegs.INPUT10SELECT = INPUT10SELECT_VALUE;
   InputXbarRegs.INPUT11SELECT = INPUT11SELECT_VALUE;
   InputXbarRegs.INPUT12SELECT = INPUT12SELECT_VALUE;
   InputXbarRegs.INPUT13SELECT = INPUT13SELECT_VALUE;
   InputXbarRegs.INPUT14SELECT = INPUT14SELECT_VALUE;

   //
   // Connect X-BAR with PWM Trip
   //
   EPwmXbarRegs.TRIP7MUX0TO15CFG.bit.MUX7 = EPWMXBAR_TRIP7;
   EPwmXbarRegs.TRIP8MUX0TO15CFG.bit.MUX9 = EPWMXBAR_TRIP8;
   EPwmXbarRegs.TRIP9MUX0TO15CFG.bit.MUX11 = EPWMXBAR_TRIP9;
   EPwmXbarRegs.TRIP10MUX0TO15CFG.bit.MUX4 = EPWMXBAR_TRIP10;
   EPwmXbarRegs.TRIP11MUX0TO15CFG.bit.MUX8 = EPWMXBAR_TRIP11;
   EPwmXbarRegs.TRIP12MUX0TO15CFG.bit.MUX10 = EPWMXBAR_TRIP12;


   // Pull-ups can be enabled or disabled. 
   GpioCtrlRegs.GPAPUD.all = GPAPUD_ENABLE; 	
   GpioCtrlRegs.GPBPUD.all = GPBPUD_ENABLE;
   GpioCtrlRegs.GPCPUD.all = GPCPUD_ENABLE;
   GpioCtrlRegs.GPDPUD.all = GPDPUD_ENABLE;

   // Set Qualification sampling period 
   GpioCtrlRegs.GPACTRL.all = PLLSYSCLKDIV4;
   GpioCtrlRegs.GPBCTRL.all = PLLSYSCLKDIV4;
   GpioCtrlRegs.GPCCTRL.all = PLLSYSCLKDIV4;
   GpioCtrlRegs.GPDCTRL.all = PLLSYSCLKDIV4;

   // Set Qualification Number of Sample
   GpioCtrlRegs.GPAQSEL1.all = SIX_SAMPLES_QUAL;
   GpioCtrlRegs.GPAQSEL2.all = SIX_SAMPLES_QUAL;
   GpioCtrlRegs.GPBQSEL1.all = SIX_SAMPLES_QUAL;
   GpioCtrlRegs.GPBQSEL2.all = SIX_SAMPLES_QUAL;
   GpioCtrlRegs.GPCQSEL1.all = SIX_SAMPLES_QUAL;
   GpioCtrlRegs.GPCQSEL2.all = SIX_SAMPLES_QUAL;
   GpioCtrlRegs.GPDQSEL1.all = SIX_SAMPLES_QUAL;
   GpioCtrlRegs.GPDQSEL2.all = SIX_SAMPLES_QUAL;

   // Set GPIO Port Direction
   GpioCtrlRegs.GPADIR.all = GPADIRSET;
   GpioCtrlRegs.GPBDIR.all = GPBDIRSET;
   GpioCtrlRegs.GPCDIR.all = GPCDIRSET;
   GpioCtrlRegs.GPDDIR.all = GPDDIRSET;
   
   // Set IO ports initial status  
   GpioDataRegs.GPACLEAR.all = 0xF7FFFFFF;
   GpioDataRegs.GPBCLEAR.all = 0xFFFFFFFF;
   GpioDataRegs.GPCCLEAR.all = 0xFFFFFFFF;
   GpioDataRegs.GPDCLEAR.all = 0xFFFFFFFF;
   GpioDataRegs.GPASET.all = 0x08000000;
   GpioDataRegs.GPBSET.all = 0x00000000;
   GpioDataRegs.GPCSET.all = 0x00000000;
   GpioDataRegs.GPDSET.all = 0x00000000;
   
   // Set GPIO Core Select Register
   GpioCtrlRegs.GPACSEL1.all = GPIO0TO7_CORE_SELECTED;
   GpioCtrlRegs.GPACSEL2.all = GPIO8TO15_CORE_SELECTED;
   GpioCtrlRegs.GPACSEL3.all = GPIO16TO23_CORE_SELECTED;
   GpioCtrlRegs.GPACSEL4.all = GPIO24TO31_CORE_SELECTED;
   GpioCtrlRegs.GPBCSEL1.all = GPIO32TO39_CORE_SELECTED;
   GpioCtrlRegs.GPBCSEL2.all = GPIO40TO47_CORE_SELECTED;
   GpioCtrlRegs.GPBCSEL3.all = GPIO48TO55_CORE_SELECTED;
   GpioCtrlRegs.GPBCSEL4.all = GPIO56TO63_CORE_SELECTED;
   GpioCtrlRegs.GPCCSEL1.all = GPIO64TO71_CORE_SELECTED;
   GpioCtrlRegs.GPCCSEL2.all = GPIO72TO79_CORE_SELECTED;
   GpioCtrlRegs.GPCCSEL3.all = GPIO80TO87_CORE_SELECTED;
   GpioCtrlRegs.GPCCSEL4.all = GPIO88TO95_CORE_SELECTED;
   GpioCtrlRegs.GPDCSEL1.all = GPIO96TO103_CORE_SELECTED;
   //
   // Restore GPIO lock register values
   //
   GpioCtrlRegs.GPALOCK.all = lockValA;
   GpioCtrlRegs.GPBLOCK.all = lockValB;
   GpioCtrlRegs.GPCLOCK.all = lockValC;
   GpioCtrlRegs.GPDLOCK.all = lockValD;
   GpioCtrlRegs.GPELOCK.all = lockValE;

   objFlashUpdate.CPUSELConfig();
   EDIS;


}


/********************************************************
Select peripheral for CPU
********************************************************/

void	Class_FlashUpdate::CPUSELConfig(void)
{
	DevCfgRegs.CPUSEL0.all = 0x03C0;	// PWM
	DevCfgRegs.CPUSEL1.all = 0x0000;	// ECAP
	DevCfgRegs.CPUSEL2.all = 0x0000;	// EQEP
	DevCfgRegs.CPUSEL4.all = 0x0000;	// SD
	DevCfgRegs.CPUSEL5.all = 0x0000;	// SCI
	DevCfgRegs.CPUSEL6.all = 0x0000;	// SPI
	DevCfgRegs.CPUSEL7.all = 0x0000;	// I2C
	DevCfgRegs.CPUSEL8.all = 0x0000;	// ECAN
	DevCfgRegs.CPUSEL9.all = 0x0000;	// McBSP
	DevCfgRegs.CPUSEL11.all = 0x000C;	// ADC
	DevCfgRegs.CPUSEL12.all = 0x0000;	// CMPSS
	DevCfgRegs.CPUSEL14.all = 0x0000;	// DAC
}


#pragma CODE_SECTION("Flash28Funcs");
void 	Class_FlashUpdate::Drv_PWMInit(void)
{
    //EPWM1
        EALLOW;
        EPwm1Regs.TZCTL.bit.TZA = TZ_FORCE_LO;          // Forced Low
        EPwm1Regs.TZCTL.bit.TZB = TZ_FORCE_LO;          // Forced Low
        EPwm1Regs.TZCLR.bit.OST = 1;                    // Forces a fault on the OST latch and sets the OSTFLG bit.
        EDIS;
        EPwm1Regs.TBPRD = 2083;                   // Switch Period Counter
        EPwm1Regs.TBPHS.bit.TBPHS = 0;
        EPwm1Regs.CMPA.bit.CMPA = 0;
        EPwm1Regs.TBCTL.bit.PHSDIR = TB_UP;
        EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
        EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;          // Phase loading enabled
        EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;
        EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;      //
        EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;        // TBCLK = SYSCLK
        EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;               //
        EPwm1Regs.TBCTR = 0x0000;
        EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
        EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
        EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;            // CNT = CMP up   ->1
        EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;          // CNT = CMP down ->0
        EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;            // CNT = CMP up   ->0
        EPwm1Regs.AQCTLB.bit.CBD = AQ_SET;              // CNT = CMP down ->1
        EPwm1Regs.AQSFRC.bit.RLDCSF = 3;                // the active register load immediately
        EPwm1Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;     // Software forcing disabled, i.e., has no effect
        EPwm1Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
        //SET DB
        EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
        EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
        EPwm1Regs.DBCTL.bit.IN_MODE = DBA_ALL;
        EPwm1Regs.DBRED.all = KDeadTimeCnst;
        EPwm1Regs.DBFED.all = KDeadTimeCnst;
        EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;       //
        EPwm1Regs.ETSEL.bit.INTEN = 1;
        EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;


        //EPWM2
        EALLOW;
        EPwm2Regs.TZCTL.bit.TZA = TZ_FORCE_LO;          // Forced Low
        EPwm2Regs.TZCTL.bit.TZB = TZ_FORCE_LO;          // Forced Low
        EPwm2Regs.TZCLR.bit.OST = 1;                    // Forces a fault on the OST latch and sets the OSTFLG bit.
        EDIS;
        EPwm2Regs.TBPRD = 2083;                         // Switch Period Counter
        EPwm2Regs.TBPHS.bit.TBPHS = 0;
        EPwm2Regs.CMPA.bit.CMPA = 0;
        EPwm2Regs.TBCTL.bit.PHSDIR = TB_UP;
        EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
        EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;          // Phase loading enabled
        EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;
        EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;     //
        EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;        // TBCLK = SYSCLK
        EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;
        EPwm2Regs.TBCTR = 0x0000;
        EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
        EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
        EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;            // CNT = CMP up   ->1
        EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;              // CNT = CMP down ->0
        EPwm2Regs.AQCTLB.bit.CBU = AQ_CLEAR;            // CNT = CMP up   ->0
        EPwm2Regs.AQCTLB.bit.CBD = AQ_SET;              // CNT = CMP down ->1
        EPwm2Regs.AQSFRC.bit.RLDCSF = 3;                // the active register load immediately
        EPwm2Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;      // Software forcing disabled, i.e., has no effect
        EPwm2Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
        //SET DB
        EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
        EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
        EPwm2Regs.DBCTL.bit.IN_MODE = DBA_ALL;
        EPwm2Regs.DBRED.all = KDeadTimeCnst;
        EPwm2Regs.DBFED.all = KDeadTimeCnst;
        EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
        EPwm2Regs.ETSEL.bit.INTEN = 1;
        EPwm2Regs.ETPS.bit.INTPRD = ET_1ST;


        //EPWM3
        EALLOW;
        EPwm3Regs.TZCTL.bit.TZA = TZ_FORCE_LO;          // Forced Low
        EPwm3Regs.TZCTL.bit.TZB = TZ_FORCE_LO;          // Forced Low
        EPwm3Regs.TZCLR.bit.OST = 1;                    // Forces a fault on the OST latch and sets the OSTFLG bit.
        EDIS;
        EPwm3Regs.TBPRD = 2083;                         // Switch Period Counter
        EPwm3Regs.TBPHS.bit.TBPHS = 0;
        EPwm3Regs.CMPA.bit.CMPA = 0;
        EPwm3Regs.TBCTL.bit.PHSDIR = TB_UP;
        EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
        EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE;          // Phase loading enabled
        EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;
        EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;     //
        EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;        // TBCLK = SYSCLK
        EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;
        EPwm3Regs.TBCTR = 0x0000;
        EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
        EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
        EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;            // CNT = CMP up   ->1
        EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;              // CNT = CMP down ->0
        EPwm3Regs.AQCTLB.bit.CBU = AQ_CLEAR;            // CNT = CMP up   ->0
        EPwm3Regs.AQCTLB.bit.CBD = AQ_SET;              // CNT = CMP down ->1
        EPwm3Regs.AQSFRC.bit.RLDCSF = 3;                // the active register load immediately
        EPwm3Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;      // Software forcing disabled, i.e., has no effect
        EPwm3Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
        //SET DB
        EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
        EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
        EPwm3Regs.DBCTL.bit.IN_MODE = DBA_ALL;
        EPwm3Regs.DBRED.all = KDeadTimeCnst;
        EPwm3Regs.DBFED.all = KDeadTimeCnst;
        EPwm3Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
        EPwm3Regs.ETSEL.bit.INTEN = 1;
        EPwm3Regs.ETPS.bit.INTPRD = ET_1ST;



        //EPWM4
        EALLOW;
        EPwm4Regs.TZCTL.bit.TZA = TZ_FORCE_LO;          // Forced Low
        EPwm4Regs.TZCTL.bit.TZB = TZ_FORCE_LO;          // Forced Low
        EPwm4Regs.TZCLR.bit.OST = 1;                    // Forces a fault on the OST latch and sets the OSTFLG bit.
        EDIS;
        EPwm4Regs.TBPRD = 2083;                         // Switch Period Counter
        EPwm4Regs.TBPHS.bit.TBPHS = 0;
        EPwm4Regs.CMPA.bit.CMPA = 0;
        EPwm4Regs.TBCTL.bit.PHSDIR = TB_UP;
        EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
        EPwm4Regs.TBCTL.bit.PHSEN = TB_ENABLE;          // Phase loading enabled
        EPwm4Regs.TBCTL.bit.PRDLD = TB_SHADOW;
        EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;     //
        EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;        // TBCLK = SYSCLK
        EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV1;
        EPwm4Regs.TBCTR = 0x0000;
        EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
        EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
        EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;            // CNT = CMP up   ->1
        EPwm4Regs.AQCTLA.bit.CAD = AQ_SET;              // CNT = CMP down ->0
        EPwm4Regs.AQCTLB.bit.CBU = AQ_CLEAR;            // CNT = CMP up   ->0
        EPwm4Regs.AQCTLB.bit.CBD = AQ_SET;              // CNT = CMP down ->1
        EPwm4Regs.AQSFRC.bit.RLDCSF = 3;                // the active register load immediately
        EPwm4Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;      // Software forcing disabled, i.e., has no effect
        EPwm4Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
        //SET DB
        EPwm4Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
        EPwm4Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
        EPwm4Regs.DBCTL.bit.IN_MODE = DBA_ALL;
        EPwm4Regs.DBRED.all = KDeadTimeCnst;
        EPwm4Regs.DBFED.all = KDeadTimeCnst;
        EPwm4Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
        EPwm4Regs.ETSEL.bit.INTEN = 1;
        EPwm4Regs.ETPS.bit.INTPRD = ET_1ST;



        //EPWM5
        EALLOW;
        EPwm5Regs.TZCTL.bit.TZA = TZ_FORCE_LO;          // Forced Low
        EPwm5Regs.TZCTL.bit.TZB = TZ_FORCE_LO;          // Forced Low
        EPwm5Regs.TZCLR.bit.OST = 1;                    // Forces a fault on the OST latch and sets the OSTFLG bit.
        EDIS;
        EPwm5Regs.TBPRD = 2083;                         // Switch Period Counter
        EPwm5Regs.TBPHS.bit.TBPHS = 0;
        EPwm5Regs.CMPA.bit.CMPA = 0;
        EPwm5Regs.TBCTL.bit.PHSDIR = TB_UP;
        EPwm5Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
        EPwm5Regs.TBCTL.bit.PHSEN = TB_ENABLE;          // Phase loading enabled
        EPwm5Regs.TBCTL.bit.PRDLD = TB_SHADOW;
        EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;     //
        EPwm5Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;        // TBCLK = SYSCLK
        EPwm5Regs.TBCTL.bit.CLKDIV = TB_DIV1;
        EPwm5Regs.TBCTR = 0x0000;
        EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
        EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
        EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm5Regs.AQCTLA.bit.CAU = AQ_CLEAR;            // CNT = CMP up   ->1
        EPwm5Regs.AQCTLA.bit.CAD = AQ_SET;              // CNT = CMP down ->0
        EPwm5Regs.AQCTLB.bit.CBU = AQ_CLEAR;            // CNT = CMP up   ->0
        EPwm5Regs.AQCTLB.bit.CBD = AQ_SET;              // CNT = CMP down ->1
        EPwm5Regs.AQSFRC.bit.RLDCSF = 3;                // the active register load immediately
        EPwm5Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;      // Software forcing disabled, i.e., has no effect
        EPwm5Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
        //SET DB
        EPwm5Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
        EPwm5Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
        EPwm5Regs.DBCTL.bit.IN_MODE = DBA_ALL;
        EPwm5Regs.DBRED.all = KDeadTimeCnst;
        EPwm5Regs.DBFED.all = KDeadTimeCnst;
        EPwm5Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
        EPwm5Regs.ETSEL.bit.INTEN = 1;
        EPwm5Regs.ETPS.bit.INTPRD = ET_1ST;



        //EPWM6
        //
        EALLOW;
        EPwm6Regs.TZCTL.bit.TZA = TZ_FORCE_LO;          // Forced Hi (EPWM2A = High state)
        EPwm6Regs.TZCTL.bit.TZB = TZ_FORCE_LO;          // Forced Hi (EPWM2B = High state)
        EPwm6Regs.TZCLR.bit.OST = 1;                    // Forces a fault on the OST latch and sets the OSTFLG bit.
        EDIS;
        EPwm6Regs.TBPRD = 2083;                   // Switch Period Counter
        EPwm6Regs.TBPHS.bit.TBPHS = 0;
        EPwm6Regs.CMPA.bit.CMPA = 0;      //
        EPwm6Regs.TBCTL.bit.PHSDIR = TB_UP;
        EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
        EPwm6Regs.TBCTL.bit.PHSEN = TB_ENABLE;          // Phase loading enabled
        EPwm6Regs.TBCTL.bit.PRDLD = TB_SHADOW;
        EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;      //
        EPwm6Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;        // TBCLK = SYSCLK
        EPwm6Regs.TBCTL.bit.CLKDIV = TB_DIV1;
        EPwm6Regs.TBCTR = 0x0000;
        EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
        EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
        EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm6Regs.AQCTLA.bit.CAU = AQ_CLEAR;            // CNT = CMP up   ->1
        EPwm6Regs.AQCTLA.bit.CAD = AQ_SET;          // CNT = CMP down ->0
        EPwm6Regs.AQCTLB.bit.CBU = AQ_CLEAR;            // CNT = CMP up   ->0
        EPwm6Regs.AQCTLB.bit.CBD = AQ_SET;              // CNT = CMP down ->1
        EPwm6Regs.AQSFRC.bit.RLDCSF = 3;                // the active register load immediately
        EPwm6Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;     // Software forcing disabled, i.e., has no effect
        EPwm6Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
        //SET DB
        EPwm6Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
        EPwm6Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
        EPwm6Regs.DBCTL.bit.IN_MODE = DBA_ALL;
        EPwm6Regs.DBRED.all = KDeadTimeCnst;
        EPwm6Regs.DBFED.all = KDeadTimeCnst;


        //Epwm7
        //
        EALLOW;
        EPwm7Regs.TZCTL.bit.TZA = TZ_FORCE_LO;          // Forced Hi (EPWM2A = High state)
        EPwm7Regs.TZCTL.bit.TZB = TZ_FORCE_LO;          // Forced Hi (EPWM2B = High state)
        EPwm7Regs.TZCLR.bit.OST = 1;                    // Forces a fault on the OST latch and sets the OSTFLG bit.
        EDIS;
        EPwm7Regs.TBPRD = 2083;                   // Switch Period Counter
        EPwm7Regs.TBPHS.bit.TBPHS = 0;
        EPwm7Regs.CMPA.bit.CMPA = 0;      //
        EPwm7Regs.TBCTL.bit.PHSDIR = TB_UP;
        EPwm7Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
        EPwm7Regs.TBCTL.bit.PHSEN = TB_ENABLE;          // Phase loading enabled
        EPwm7Regs.TBCTL.bit.PRDLD = TB_SHADOW;
        EPwm7Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;      //
        EPwm7Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;        // TBCLK = SYSCLK
        EPwm7Regs.TBCTL.bit.CLKDIV = TB_DIV1;
        EPwm7Regs.TBCTR = 0x0000;
        EPwm7Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
        EPwm7Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
        EPwm7Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm7Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm7Regs.AQCTLA.bit.CAU = AQ_CLEAR;            // CNT = CMP up   ->1
        EPwm7Regs.AQCTLA.bit.CAD = AQ_SET;          // CNT = CMP down ->0
        EPwm7Regs.AQCTLB.bit.CBU = AQ_CLEAR;            // CNT = CMP up   ->0
        EPwm7Regs.AQCTLB.bit.CBD = AQ_SET;              // CNT = CMP down ->1
        EPwm7Regs.AQSFRC.bit.RLDCSF = 3;                // the active register load immediately
        EPwm7Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;     // Software forcing disabled, i.e., has no effect
        EPwm7Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
        //SET DB
        EPwm7Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
        EPwm7Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
        EPwm7Regs.DBCTL.bit.IN_MODE = DBA_ALL;
        EPwm7Regs.DBRED.all = KDeadTimeCnst;
        EPwm7Regs.DBFED.all = KDeadTimeCnst;





        //Epwm8
        //
        EALLOW;
        EPwm8Regs.TZCTL.bit.TZA = TZ_FORCE_LO;          // Forced Hi (EPWM2A = High state)
        EPwm8Regs.TZCTL.bit.TZB = TZ_FORCE_LO;          // Forced Hi (EPWM2B = High state)
        EPwm8Regs.TZCLR.bit.OST = 1;                    // Forces a fault on the OST latch and sets the OSTFLG bit.
        EDIS;
        EPwm8Regs.TBPRD = 2083;                   // Switch Period Counter
        EPwm8Regs.TBPHS.bit.TBPHS = 0;
        EPwm8Regs.CMPA.bit.CMPA = 0;      //
        EPwm8Regs.TBCTL.bit.PHSDIR = TB_UP;
        EPwm8Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
        EPwm8Regs.TBCTL.bit.PHSEN = TB_ENABLE;          // Phase loading enabled
        EPwm8Regs.TBCTL.bit.PRDLD = TB_SHADOW;
        EPwm8Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;      //
        EPwm8Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;        // TBCLK = SYSCLK
        EPwm8Regs.TBCTL.bit.CLKDIV = TB_DIV1;
        EPwm8Regs.TBCTR = 0x0000;
        EPwm8Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
        EPwm8Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
        EPwm8Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm8Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm8Regs.AQCTLA.bit.CAU = AQ_CLEAR;            // CNT = CMP up   ->1
        EPwm8Regs.AQCTLA.bit.CAD = AQ_SET;          // CNT = CMP down ->0
        EPwm8Regs.AQCTLB.bit.CBU = AQ_CLEAR;            // CNT = CMP up   ->0
        EPwm8Regs.AQCTLB.bit.CBD = AQ_SET;              // CNT = CMP down ->1
        EPwm8Regs.AQSFRC.bit.RLDCSF = 3;                // the active register load immediately
        EPwm8Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;     // Software forcing disabled, i.e., has no effect
        EPwm8Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
        //SET DB
        EPwm8Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
        EPwm8Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
        EPwm8Regs.DBCTL.bit.IN_MODE = DBA_ALL;
        EPwm8Regs.DBRED.all = KDeadTimeCnst;
        EPwm8Regs.DBFED.all = KDeadTimeCnst;






        //Epwm9
        //
        EALLOW;
        EPwm9Regs.TZCTL.bit.TZA = TZ_FORCE_LO;          // Forced Hi (EPWM2A = High state)
        EPwm9Regs.TZCTL.bit.TZB = TZ_FORCE_LO;          // Forced Hi (EPWM2B = High state)
        EPwm9Regs.TZCLR.bit.OST = 1;                    // Forces a fault on the OST latch and sets the OSTFLG bit.
        EDIS;
        EPwm9Regs.TBPRD = 2083;                   // Switch Period Counter
        EPwm9Regs.TBPHS.bit.TBPHS = 0;
        EPwm9Regs.CMPA.bit.CMPA = 0;      //
        EPwm9Regs.TBCTL.bit.PHSDIR = TB_UP;
        EPwm9Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
        EPwm9Regs.TBCTL.bit.PHSEN = TB_ENABLE;          // Phase loading enabled
        EPwm9Regs.TBCTL.bit.PRDLD = TB_SHADOW;
        EPwm9Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;      //
        EPwm9Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;        // TBCLK = SYSCLK
        EPwm9Regs.TBCTL.bit.CLKDIV = TB_DIV1;
        EPwm9Regs.TBCTR = 0x0000;
        EPwm9Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
        EPwm9Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
        EPwm9Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm9Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm9Regs.AQCTLA.bit.CAU = AQ_CLEAR;            // CNT = CMP up   ->1
        EPwm9Regs.AQCTLA.bit.CAD = AQ_SET;          // CNT = CMP down ->0
        EPwm9Regs.AQCTLB.bit.CBU = AQ_CLEAR;            // CNT = CMP up   ->0
        EPwm9Regs.AQCTLB.bit.CBD = AQ_SET;              // CNT = CMP down ->1
        EPwm9Regs.AQSFRC.bit.RLDCSF = 3;                // the active register load immediately
        EPwm9Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;     // Software forcing disabled, i.e., has no effect
        EPwm9Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
        //SET DB
        EPwm9Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
        EPwm9Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
        EPwm9Regs.DBCTL.bit.IN_MODE = DBA_ALL;
        EPwm9Regs.DBRED.all = KDeadTimeCnst;
        EPwm9Regs.DBFED.all = KDeadTimeCnst;




        //Epwm10
        //
        EALLOW;
        EPwm10Regs.TZCTL.bit.TZA = TZ_FORCE_LO;          // Forced Hi (EPWM2A = High state)
        EPwm10Regs.TZCTL.bit.TZB = TZ_FORCE_LO;          // Forced Hi (EPWM2B = High state)
        EPwm10Regs.TZCLR.bit.OST = 1;                    // Forces a fault on the OST latch and sets the OSTFLG bit.
        EDIS;
        EPwm10Regs.TBPRD = 2083;                   // Switch Period Counter
        EPwm10Regs.TBPHS.bit.TBPHS = 0;
        EPwm10Regs.CMPA.bit.CMPA = 0;      //
        EPwm10Regs.TBCTL.bit.PHSDIR = TB_UP;
        EPwm10Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
        EPwm10Regs.TBCTL.bit.PHSEN = TB_ENABLE;          // Phase loading enabled
        EPwm10Regs.TBCTL.bit.PRDLD = TB_SHADOW;
        EPwm10Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;      //
        EPwm10Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;        // TBCLK = SYSCLK
        EPwm10Regs.TBCTL.bit.CLKDIV = TB_DIV1;
        EPwm10Regs.TBCTR = 0x0000;
        EPwm10Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
        EPwm10Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
        EPwm10Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm10Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm10Regs.AQCTLA.bit.CAU = AQ_CLEAR;            // CNT = CMP up   ->1
        EPwm10Regs.AQCTLA.bit.CAD = AQ_SET;          // CNT = CMP down ->0
        EPwm10Regs.AQCTLB.bit.CBU = AQ_CLEAR;            // CNT = CMP up   ->0
        EPwm10Regs.AQCTLB.bit.CBD = AQ_SET;              // CNT = CMP down ->1
        EPwm10Regs.AQSFRC.bit.RLDCSF = 3;                // the active register load immediately
        EPwm10Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;     // Software forcing disabled, i.e., has no effect
        EPwm10Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
        //SET DB
        EPwm10Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
        EPwm10Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
        EPwm10Regs.DBCTL.bit.IN_MODE = DBA_ALL;
        EPwm10Regs.DBRED.all = KDeadTimeCnst;
        EPwm10Regs.DBFED.all = KDeadTimeCnst;




        //Epwm11
        //
        //改用CANS收发器 20170826
        EALLOW;
        EPwm11Regs.TZCTL.bit.TZA = TZ_FORCE_HI;       // Forced high (EPWM6A = low state)
        EPwm11Regs.TZCTL.bit.TZB = TZ_FORCE_HI;       // Forced high (EPWM6B = low state)
        EPwm11Regs.TZFRC.bit.OST = 1;                     // Forces a fault on the OST latch and sets the OSTFLG bit.
        EDIS;
        EPwm11Regs.TBPRD = 30000;                         // 30000*32*10*10ns=96ms
        EPwm11Regs.CMPB.bit.CMPB = 2500;                       // TBPRD,[3906*2,2232*2]=[40Hz,70Hz]
        EPwm11Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;
        EPwm11Regs.TBCTL.bit.PHSEN = TB_DISABLE;          // Phase loading disabled
        EPwm11Regs.TBCTL.bit.PRDLD = TB_IMMEDIATE;
        EPwm11Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE; // DISABLE
        EPwm11Regs.TBCTL.bit.HSPCLKDIV = 5;           // /10
        EPwm11Regs.TBCTL.bit.CLKDIV = 5;                  // /32
        EPwm11Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
        EPwm11Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
        EPwm11Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm11Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;   // load on CTR = Zero or PRD
        EPwm11Regs.AQCTLA.bit.CAU = AQ_SET;           // CNT = CMP up   ->0
        EPwm11Regs.AQCTLA.bit.CAD = AQ_CLEAR;             // CNT = CMP down ->1
        EPwm11Regs.AQCTLB.bit.CBU = AQ_SET;       // CNT = CMP up   ->0
        EPwm11Regs.AQCTLB.bit.PRD = AQ_CLEAR;             // PRD            ->1
        EPwm11Regs.AQSFRC.bit.RLDCSF = 3;                 // the active register load immediately
        EPwm11Regs.AQCSFRC.bit.CSFA = AQC_NO_ACTION;      // Software forcing disabled, i.e., has no effect
        EPwm11Regs.AQCSFRC.bit.CSFB = AQC_NO_ACTION;
        EPwm11Regs.DBCTL.bit.OUT_MODE = DB_DISABLE;
        EPwm11Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;
        EPwm11Regs.DBRED.all = 0;
        EPwm11Regs.DBFED.all = 0;



        EPwm1Regs.TBPHS.bit.TBPHS = 0;
        EPwm2Regs.TBPHS.bit.TBPHS = 0;
        EPwm3Regs.TBPHS.bit.TBPHS = 0;
        EPwm4Regs.TBPHS.bit.TBPHS = 0;
        EPwm5Regs.TBPHS.bit.TBPHS = 0;
        EPwm6Regs.TBPHS.bit.TBPHS = 0;
        EPwm11Regs.TBPHS.bit.TBPHS = 0;

        EPwm1Regs.TBCTR = 0;
        EPwm2Regs.TBCTR = 0;
        EPwm3Regs.TBCTR = 0;
        EPwm4Regs.TBCTR = 0;
        EPwm5Regs.TBCTR = 0;
        EPwm6Regs.TBCTR = 0;
        EPwm11Regs.TBCTR = 0;
      

  
}


#pragma CODE_SECTION("Flash28Funcs");
void Class_FlashUpdate::Drv_eCANAInit(void)
{
		
}

#pragma CODE_SECTION("Flash28Funcs");
void	Class_FlashUpdate::Drv_RamCheck(void)
{
	
		volatile UINT16 	*memaddr;
		UINT16 	j, u16memerr;

		u16memerr = 0;

		//M0,M1 SARAM(2K*16) Check
    	memaddr = (UINT16 *) 0x0000;
    	for (j = 0x0000; j < 0x0800; j++)
    	{
			*(memaddr + j) = 0x5555;
			if (*(memaddr + j) != 0x5555)
				u16memerr=1;

			*(memaddr + j) = 0xaaaa;
			if (*(memaddr + j) != 0xaaaa)
				u16memerr=1;

			*(memaddr + j) = 0x0000;
    	}


		//CPUx.CLA1 to CPUx MSGRAM (128*16) Check
    	memaddr = (UINT16 *) 0x1480;
    	for (j = 0x0000; j < 0x0080; j++)
    	{
			*(memaddr + j) = 0x5555;
			if (*(memaddr + j) != 0x5555)
				u16memerr=1;

			*(memaddr + j) = 0xaaaa;
			if (*(memaddr + j) != 0xaaaa)
				u16memerr=1;

			*(memaddr + j) = 0x0000;
    	}

		//CPUx to CPUx.CLA1 MSGRAM (128*16) Check
    	memaddr = (UINT16 *) 0x1500;
    	for (j = 0x0000; j < 0x0080; j++)
    	{
			*(memaddr + j) = 0x5555;
			if (*(memaddr + j) != 0x5555)
				u16memerr=1;

			*(memaddr + j) = 0xaaaa;
			if (*(memaddr + j) != 0xaaaa)
				u16memerr=1;

			*(memaddr + j) = 0x0000;
    	}


		//LS0~LS5 SARAM(2K*16*6) Check
    	memaddr = (UINT16 *) 0x8000;
    	for (j = 0x0000; j < 0x3000; j++)
    	{
			*(memaddr + j) = 0x5555;
			if (*(memaddr + j) != 0x5555)
				u16memerr=1;

			*(memaddr + j) = 0xaaaa;
			if (*(memaddr + j) != 0xaaaa)
				u16memerr=1;

			*(memaddr + j) = 0x0000;
    	}


		//D0 D1 SARAM(2K*16,2K*16) Check
    	memaddr = (UINT16 *) 0xB000;
    	for (j = 0x0000; j < 0x1000; j++)
    	{
			*(memaddr + j) = 0x5555;
			if (*(memaddr + j) != 0x5555)
				u16memerr=1;

			*(memaddr + j) = 0xaaaa;
			if (*(memaddr + j) != 0xaaaa)
				u16memerr=1;

			*(memaddr + j) = 0x0000;
    	}

		//GS0~GS15 SARAM(4K*16*16) Check
    	memaddr = (UINT16 *) 0x00C000;
    	for (j = 0x0000; j < 0x10000; j++)					
    	{
			*(memaddr + j) = 0x5555;
			if (*(memaddr + j) != 0x5555)
				u16memerr=1;

			*(memaddr + j) = 0xaaaa;
			if (*(memaddr + j) != 0xaaaa)
				u16memerr=1;

			*(memaddr + j) = 0x0000;
    	}

	//定义回定变量的地址为：0xAFFC-0xAFFF
	if ((DSPCrazyA == 0xEB95) 
		 && (DSPCrazyB == 0xEB95)
		 && (DSPCrazyC == 0xEB95)  
		 && (DSPCrazyD == 0xEB95)  )
	{
		objInverter.m_st_wFlagInv.bDspCrazyOff = 1;					//有DSP死机切旁路开机标志
	}  
	else
	{
	    //Mem  Check for 0xAFFC-0xAFFF
   	 	memaddr = (UINT16 *) 0xAFFC;
	    for (j = 0x0000; j < 0x4; j++)			
 	   	{
			*(memaddr + j) = 0x5555;
			if (*(memaddr + j) != 0x5555)
				u16memerr=1;

			*(memaddr + j) = 0xaaaa;
			if (*(memaddr + j) != 0xaaaa)
				u16memerr=1;

			*(memaddr + j) = 0x0000;
	    }	
	}

		if(u16memerr==1)
		{
			objSystem.m_st_wFlagSystem.bRAMError=1;
			
			while(1)									//死循环		
			{
				//zqf 070111--Limp mode时CPU时钟为1-5MHz，取典型值3MHz
				objTimerDrv.Drv_usDelay(30000);  	//33.3us*30000=1s, LED闪烁
				GpioDataRegs.GPBTOGGLE.bit.GPIO40 = 1;

			}	 	
		}
}

#pragma CODE_SECTION("Flash28_API");
void Class_FlashUpdate::App_FlashAPIInit(void)
{
	/*------------------------------------------------------------------
 	 Initalize Flash_CPUScaleFactor.

 	  Flash_CPUScaleFactor is a 32-bit global variable that the flash
   	API functions use to scale software delays. This scale factor 
  	 must be initalized to SCALE_FACTOR by the user's code prior
  	 to calling any of the Flash API functions. This initalization
  	 is VITAL to the proper operation of the flash API functions.  
  
   	 SCALE_FACTOR is defined in Example_Flash280x_API.h as   
   	  #define SCALE_FACTOR  1048576.0L*( (200L/CPU_RATE) )
     
   	This value is calculated during the compile based on the CPU 
   	rate, in nanoseconds, at which the algorithums will be run.
	------------------------------------------------------------------*/
 

}

#pragma CODE_SECTION("Flash28_API");
void Class_FlashUpdate::App_FlashUpdateMain(void)
{

}


/*------------------------------------------------------------------
   Flash_CsmUnlock

   Unlock the code security module (CSM)
 
   Parameters:
  
   Return Value:
 
            STATUS_SUCCESS         		CSM is unlocked
            STATUS_FAIL_CSM_LOCKED     	CSM did not unlock
   Notes:
     
-----------------------------------------------------------------*/
#pragma CODE_SECTION("Flash28_API");
UINT16 Class_FlashUpdate::App_Flash_CsmUnlock(void)
{


    
} 

#pragma CODE_SECTION("Flash28_API");
void Class_FlashUpdate::App_Delay1ms (void)
{
	UINT16 i;
	for(i=0;i<400;i++)
	{
		asm( " rpt #250||nop");
	}
}

#pragma CODE_SECTION("Flash28_API");
void Class_FlashUpdate::App_Delay20ms (void)
{
	UINT16 i;
	for(i=0;i<8000;i++)
	{
		asm( " rpt #250||nop");
	}
}

#pragma CODE_SECTION("Flash28_API");
// 读取邮箱数据
void Class_FlashUpdate::Dat_MBox_Read(void)
{

	
	
}


#pragma CODE_SECTION("Flash28_API");
//发送数据
void  Class_FlashUpdate::Dat_MBox_Send(UINT16 u16_SerCode, UINT16 u16_Data)
{
	struct	ECAN_REGS	tempECanReg;
	struct  	MBOX		tempMbox;
	WORD_TO_2BYTES_T un_WordIDServiceCode;

	while(1)
	{
		//无发送请求
		if(ECanaRegs.CANTRS.bit.TRS1==0)
		{			

			// 清除发送成功标志位
			tempECanReg.CANTA.all = ECanaRegs.CANTA.all;
			tempECanReg.CANTA.bit.TA1 = 1;
			ECanaRegs.CANTA.all = tempECanReg.CANTA.all;

			//清除发送被忽略标志位
			tempECanReg.CANAA.all = ECanaRegs.CANAA.all;
			tempECanReg.CANAA.bit.AA1 = 1;
			ECanaRegs.CANAA.all = tempECanReg.CANAA.all;

			//禁止邮箱1
			tempECanReg.CANME.all = ECanaRegs.CANME.all;
			tempECanReg.CANME.bit.ME1 = 0;
			ECanaRegs.CANME.all = tempECanReg.CANME.all;

			//配置邮箱1的ID
			tempMbox.MSGID.all =  ECanaMboxes.MBOX1.MSGID.all;
			tempMbox.MSGID.bit.IDE = 0; 				//standard identifier
			tempMbox.MSGID.bit.AME = 0;					//dont care
			tempMbox.MSGID.bit.AAM = 0;					//normal transmit mode
			tempMbox.MSGID.bit.STDMSGID = st_FlashVariable.u16_ModuleID;		// MSGID=SourceMacID;
			tempMbox.MSGID.bit.EXTMSGID_H = 0;		//扩展ID
			tempMbox.MSGID.bit.EXTMSGID_L = 0;		//扩展ID
			ECanaMboxes.MBOX1.MSGID.all = tempMbox.MSGID.all;

			//写数据长度
			ECanaMboxes.MBOX1.MSGCTRL.bit.DLC = 0x04;
			//写数据内容
			un_WordIDServiceCode.ByteWide.Low=MONITOR_ID;
			un_WordIDServiceCode.ByteWide.High=u16_SerCode;		
			ECanaMboxes.MBOX1.MDL.word.LOW_WORD = un_WordIDServiceCode.u16Word;
			ECanaMboxes.MBOX1.MDL.word.HI_WORD= u16_Data;

			//使能邮箱1
			tempECanReg.CANME.all = ECanaRegs.CANME.all;
			tempECanReg.CANME.bit.ME1 = 1;
			ECanaRegs.CANME.all = tempECanReg.CANME.all;

			//启动发送
			tempECanReg.CANTRS.all = ECanaRegs.CANTRS.all;
			tempECanReg.CANTRS.bit.TRS1 = 1;
			ECanaRegs.CANTRS.all = tempECanReg.CANTRS.all;
			break;
		}
	}
}

#pragma CODE_SECTION("Flash28Funcs");
// 从IO获取ModuleID
void	Class_FlashUpdate::App_ModuleIDGet(void)
{	
	UINT16	u16_tempID0,u16_tempID1,u16_Temp0,u16_Temp1;

	while(st_FlashVariable.st_FlashUpdateFlag.bModuleIDLock !=1)
	{
    u16_Temp0 = ((1 - GpioDataRegs.GPADAT.bit.GPIO6) + (1- GpioDataRegs.GPADAT.bit.GPIO7)*2 
                + (1- GpioDataRegs.GPADAT.bit.GPIO8 )*4 + (1- GpioDataRegs.GPADAT.bit.GPIO9)*8);   
		if(u16_Temp0 == u16_tempID0)
		{
			u16_Temp1 ++;
			if(u16_Temp1 >= 1000)		//确认1000次
			{
				u16_Temp1 = 1000;
				u16_tempID1 = u16_Temp0;
				st_FlashVariable.st_FlashUpdateFlag.bModuleIDLock = 1;
			}
		}
		else
		{
			u16_tempID0 = u16_Temp0;
			u16_Temp1 = 0;
		}		
	}

	st_FlashVariable.u16_ModuleID = u16_tempID1-1+0x10;
}


#pragma CODE_SECTION("Flash28_API");
// FlashUpdate升级完成,DSP重启
void Class_FlashUpdate::App_DSPRestart(void)
{
	UINT16	u16_DSPRevision;
	
	EALLOW;
   	u16_DSPRevision = 5;				//Get the DSP Revision
   	EDIS;
	
	if(u16_DSPRevision >= 3)			//版本号为C及以上的DSP才喂狗
	{
		EALLOW;
		WdRegs.WDCR.all = 0x002B;								//enable dog,it take 13ms that the watchdog overflow
		EDIS;
	}
			
	while(1)
	{
		asm(" NOP ");						//死循环,不喂狗复位DSP
	}
}


//所有程序入口
#pragma CODE_SECTION("Flash28Funcs");
void Class_FlashUpdate::MainSwitch(void)
{
    asm(" .label _MainSwitch");

	// 完成c运行环境初始化
	asm(" MOV     @SP,#0x0000");
	asm(" SPM     0");
	asm(" SETC    OBJMODE");
	asm(" CLRC    AMODE");
	asm(" SETC    M0M1MAP");
	asm(" CLRC    PAGE0");
	asm(" MOVW    DP,#0x0000");
	asm(" CLRC    OVM");
	asm(" ASP");     



	// 0x3F3FFF存放程序分支标志:0x5500-收到flashupdate命令;
       // 0x55AA-flash烧写完成;0xFFFF-flash已被擦除
	if(*((UINT16 *) 0x3F3FFF) == 0x55BB)					// 正常主程序分支
	{			
		asm(" LB 3F3FFCH");
		
	}
	else		// FlashUpdate 程序分支
	{	
		Drv_SystemInit();		// FlashUpdate所需初始化		
		Drv_RamCheck();		//内存自检及RAM清0

		App_ModuleIDGet();
		
	//	Drv_MemCopy(&Flash28_API_LoadStart, &Flash28_API_LoadEnd, &Flash28_API_RunStart);
		st_FlashVariable.p_SourceAddr = &Flash28_API_LoadStart;
		st_FlashVariable.p_SourceEndAddr= &Flash28_API_LoadEnd;
		st_FlashVariable.p_DestAddr = &Flash28_API_RunStart;
		while(st_FlashVariable.p_SourceAddr < st_FlashVariable.p_SourceEndAddr)
    		{
       		*st_FlashVariable.p_DestAddr++ = *st_FlashVariable.p_SourceAddr++;
    		}
		

		//FlashUpdate主流程
		App_FlashUpdateMain();		
	}
}

#pragma CODE_SECTION("Flash28_API");
void	Class_FlashUpdate::FlagProgram(void)
{
	
}

