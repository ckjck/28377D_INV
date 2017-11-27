//*****************************************************************************
//
// f2837xd_pinmux.c - Function to write the generated pin mux values to the
//                    appropriate registers.
// Created using TI Pinmux 4.0.1510  on Wed Nov 15 2017 at 14:21:16 GMT+0800 (中国标准时间).
//
//*****************************************************************************
//
// Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//   Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the
//   distribution.
//
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************
//
// NOTE!! This file uses bit field structures and macros from C2000Ware.
// This function is provided for your convenience and to serve as an example
// of the use of the generated header file, but its use is not required.
//
// To download C2000Ware:  http://www.ti.com/tool/C2000Ware
//
//*****************************************************************************

#include "GPIODrv.h" 


/**********************************************************************************
函数名：	Drv_GPIOInit()			
功能描述：	GPIO模块初始化
函数调用：	objGPIODrv.Drv_CPUSELConfig()
***********************************************************************************/
void	Class_GPIODrv::Drv_GPIOInit()
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
    GpioCtrlRegs.GPAMUX1.all	&= ~GPAMUX1_MASK;
    GpioCtrlRegs.GPAMUX2.all	&= ~GPAMUX2_MASK;
    GpioCtrlRegs.GPAGMUX1.all	&= ~GPAMUX1_MASK;
    GpioCtrlRegs.GPAGMUX2.all	&= ~GPAMUX2_MASK;
    GpioCtrlRegs.GPBMUX1.all	&= ~GPBMUX1_MASK;
    GpioCtrlRegs.GPBMUX2.all	&= ~GPBMUX2_MASK;
    GpioCtrlRegs.GPBGMUX1.all	&= ~GPBMUX1_MASK;
    GpioCtrlRegs.GPBGMUX2.all	&= ~GPBMUX2_MASK;
    GpioCtrlRegs.GPCMUX1.all	&= ~GPCMUX1_MASK;
    GpioCtrlRegs.GPCMUX2.all	&= ~GPCMUX2_MASK;
    GpioCtrlRegs.GPCGMUX1.all	&= ~GPCMUX1_MASK;
    GpioCtrlRegs.GPCGMUX2.all	&= ~GPCMUX2_MASK;
    GpioCtrlRegs.GPDMUX1.all	&= ~GPDMUX1_MASK;
    GpioCtrlRegs.GPDMUX2.all	&= ~GPDMUX2_MASK;
    GpioCtrlRegs.GPDGMUX1.all	&= ~GPDMUX1_MASK;
    GpioCtrlRegs.GPDGMUX2.all	&= ~GPDMUX2_MASK;
    GpioCtrlRegs.GPEMUX1.all	&= ~GPEMUX1_MASK;
    GpioCtrlRegs.GPEGMUX1.all	&= ~GPEMUX1_MASK;

    //
    // Write pin muxing to mux registers
    //
    GpioCtrlRegs.GPAGMUX1.all	|=  GPAGMUX1_VALUE;
    GpioCtrlRegs.GPAGMUX2.all	|=  GPAGMUX2_VALUE;
    GpioCtrlRegs.GPAMUX1.all	|=  GPAMUX1_VALUE;
    GpioCtrlRegs.GPAMUX2.all	|=  GPAMUX2_VALUE;
    GpioCtrlRegs.GPBGMUX1.all	|=  GPBGMUX1_VALUE;
    GpioCtrlRegs.GPBGMUX2.all	|=  GPBGMUX2_VALUE;
    GpioCtrlRegs.GPBMUX1.all	|=  GPBMUX1_VALUE;
    GpioCtrlRegs.GPBMUX2.all	|=  GPBMUX2_VALUE;
    GpioCtrlRegs.GPCGMUX1.all	|=  GPCGMUX1_VALUE;
    GpioCtrlRegs.GPCGMUX2.all	|=  GPCGMUX2_VALUE;
    GpioCtrlRegs.GPCMUX1.all	|=  GPCMUX1_VALUE;
    GpioCtrlRegs.GPCMUX2.all	|=  GPCMUX2_VALUE;
    GpioCtrlRegs.GPDGMUX1.all	|=  GPDGMUX1_VALUE;
    GpioCtrlRegs.GPDGMUX2.all	|=  GPDGMUX2_VALUE;
    GpioCtrlRegs.GPDMUX1.all	|=  GPDMUX1_VALUE;
    GpioCtrlRegs.GPDMUX2.all	|=  GPDMUX2_VALUE;
    GpioCtrlRegs.GPEGMUX1.all	|=  GPEGMUX1_VALUE;
    GpioCtrlRegs.GPEMUX1.all	|=  GPEMUX1_VALUE;

    //
    // Write pin analog mode select to registers
    //
    GpioCtrlRegs.GPBAMSEL.all	&= ~GPBAMSEL_MASK;
    GpioCtrlRegs.GPBAMSEL.all	|= GPBAMSEL_VALUE;

    //
    // Write Input X-BAR pin selection to registers
    //
    InputXbarRegs.INPUT1SELECT	= INPUT1SELECT_VALUE;
    InputXbarRegs.INPUT2SELECT	= INPUT2SELECT_VALUE;
    InputXbarRegs.INPUT3SELECT	= INPUT3SELECT_VALUE;
    InputXbarRegs.INPUT4SELECT	= INPUT4SELECT_VALUE;
    InputXbarRegs.INPUT5SELECT	= INPUT5SELECT_VALUE;
    InputXbarRegs.INPUT6SELECT	= INPUT6SELECT_VALUE;
    InputXbarRegs.INPUT7SELECT	= INPUT7SELECT_VALUE;
    InputXbarRegs.INPUT8SELECT	= INPUT8SELECT_VALUE;
    InputXbarRegs.INPUT9SELECT	= INPUT9SELECT_VALUE;
    InputXbarRegs.INPUT10SELECT	= INPUT10SELECT_VALUE;
    InputXbarRegs.INPUT11SELECT	= INPUT11SELECT_VALUE;
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

	// Set GPIO	Port Direction
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

	objGPIODrv.CPUSELConfig();
	EDIS;

}


/********************************************************
Select peripheral for CPU
********************************************************/

void	Class_GPIODrv::CPUSELConfig(void)
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

#pragma CODE_SECTION("Epwm1Funcs");
void 	Class_GPIODrv::Drv_Int_ADCh1Cs(void)
{
	GpioDataRegs.GPBCLEAR.bit.GPIO51 = 1;
}

#pragma CODE_SECTION("Epwm1Funcs");
void	Class_GPIODrv::Drv_Int_ADCh2Cs(void)
{
	GpioDataRegs.GPBSET.bit.GPIO51= 1;	
}




//===========================================================================
// End of file.
//===========================================================================

