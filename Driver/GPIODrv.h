
#ifndef   GPIODRV_H
       
#define    GPIODRV_H

//*****************************************************************************
//
//  f2837xd_pinmux.h - Created using TI Pinmux 4.0.1510  on Wed Nov 15 2017 at 14:22:07 GMT+0800 (中国标准时间).
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
//  These values will provide the functionality requested when written into
//  the registers for which the #defines are named.  For example, using the
//  C2000Ware device support header files, use the defines like in this
//  sample function:
//
//  void samplePinMuxFxn(void)
//  {
//      EALLOW;
//      //
//      // Write generated values to mux registers
//      //
//      GpioCtrlRegs.GPAMUX1.all  = GPAMUX1_VALUE;
//      GpioCtrlRegs.GPAMUX2.all  = GPAMUX2_VALUE;
//      GpioCtrlRegs.GPBMUX1.all  = GPBMUX1_VALUE;
//        . . .
//      EDIS;
//  }
//
//  NOTE:  These GPIO control registers are only available on CPU1.
//
//*****************************************************************************

//
// Port A mux register values
//
// Pin 160 (GPIO0) to EPWM1A (mode 1)
// Pin 161 (GPIO1) to EPWM1B (mode 1)
// Pin 162 (GPIO2) to EPWM2A (mode 1)
// Pin 163 (GPIO3) to EPWM2B (mode 1)
// Pin 164 (GPIO4) to EPWM3A (mode 1)
// Pin 165 (GPIO5) to EPWM3B (mode 1)
// Pin 166 (GPIO6) to EPWM4A (mode 1)
// Pin 167 (GPIO7) to EPWM4B (mode 1)
// Pin 18 (GPIO8) to EPWM5A (mode 1)
// Pin 19 (GPIO9) to EPWM5B (mode 1)
// Pin 1 (GPIO10) to EPWM6A (mode 1)
// Pin 2 (GPIO11) to EPWM6B (mode 1)
// Pin 4 (GPIO12) to EPWM7A (mode 1)
// Pin 5 (GPIO13) to EPWM7B (mode 1)
// Pin 6 (GPIO14) to EPWM8A (mode 1)
// Pin 7 (GPIO15) to EPWM8B (mode 1)
// Pin 8 (GPIO16) to EPWM9A (mode 5)
// Pin 9 (GPIO17) to EPWM9B (mode 5)
// Pin 10 (GPIO18) to EPWM10A (mode 5)
// Pin 12 (GPIO19) to EPWM10B (mode 5)
// Pin 13 (GPIO20) to EPWM11A (mode 5)
// Pin 14 (GPIO21) to EPWM11B (mode 5)
// Pin 22 (GPIO22) to EPWM12A (mode 5)
// Pin 23 (GPIO23) to EPWM12B (mode 5)
// Pin 24 (GPIO24) to GPIO24 (mode 0)
// Pin 25 (GPIO25) to GPIO25 (mode 0)
// Pin 27 (GPIO26) to GPIO26 (mode 0)
// Pin 28 (GPIO27) to GPIO27 (mode 0)
// Pin 63 (GPIO30) to GPIO30 (mode 0)
// Pin 66 (GPIO31) to GPIO31 (mode 0)
// Pin 64 (GPIO28) to GPIO28 (mode 0)
// Pin 65 (GPIO29) to GPIO29 (mode 0)
#define GPAMUX1_MASK		0xffffffff
#define GPAMUX2_MASK		0xffffffff
#define GPAMUX1_VALUE		0x55555555
#define GPAMUX2_VALUE		0x00005555
#define GPAGMUX1_VALUE		0x00000000
#define GPAGMUX2_VALUE		0x00005555

//
// Port B mux register values
//
// Pin 83 (GPIO36) to CANRXA (mode 6)
// Pin 84 (GPIO37) to CANTXA (mode 6)
// Pin 86 (GPIO39) to CANRXB (mode 6)
// Pin 85 (GPIO38) to CANTXB (mode 6)
// Pin 71 (GPIO35) to GPIO35 (mode 0)
// Pin 87 (GPIO40) to GPIO40 (mode 0)
// Pin 89 (GPIO41) to GPIO41 (mode 0)
// Pin 113 (GPIO44) to GPIO44 (mode 0)
// Pin 115 (GPIO45) to GPIO45 (mode 0)
// Pin 128 (GPIO46) to GPIO46 (mode 0)
// Pin 129 (GPIO47) to GPIO47 (mode 0)
// Pin 90 (GPIO48) to GPIO48 (mode 0)
// Pin 93 (GPIO49) to GPIO49 (mode 0)
// Pin 94 (GPIO50) to GPIO50 (mode 0)
// Pin 95 (GPIO51) to GPIO51 (mode 0)
// Pin 96 (GPIO52) to GPIO52 (mode 0)
// Pin 97 (GPIO53) to GPIO53 (mode 0)
// Pin 98 (GPIO54) to GPIO54 (mode 0)
// Pin 100 (GPIO55) to GPIO55 (mode 0)
// Pin 101 (GPIO56) to GPIO56 (mode 0)
// Pin 102 (GPIO57) to GPIO57 (mode 0)
// Pin 103 (GPIO58) to GPIO58 (mode 0)
// Pin 104 (GPIO59) to GPIO59 (mode 0)
// Pin 105 (GPIO60) to GPIO60 (mode 0)
// Pin 107 (GPIO61) to GPIO61 (mode 0)
// Pin 108 (GPIO62) to GPIO62 (mode 0)
// Pin 109 (GPIO63) to GPIO63 (mode 0)
// Pin 70 (GPIO34) to GPIO34 (mode 0)
// Pin 67 (GPIO32) to SDAA (mode 1)
// Pin 69 (GPIO33) to SCLA (mode 1)
#define GPBMUX1_MASK		0xff0fffff
#define GPBMUX2_MASK		0xffffffff
#define GPBMUX1_VALUE		0x0000aa05
#define GPBMUX2_VALUE		0x00000000
#define GPBGMUX1_VALUE		0x00005500
#define GPBGMUX2_VALUE		0x00000000

//
// Port C mux register values
//
// Pin 110 (GPIO64) to GPIO64 (mode 0)
// Pin 111 (GPIO65) to GPIO65 (mode 0)
// Pin 112 (GPIO66) to GPIO66 (mode 0)
// Pin 132 (GPIO67) to GPIO67 (mode 0)
// Pin 133 (GPIO68) to GPIO68 (mode 0)
// Pin 140 (GPIO73) to GPIO73 (mode 0)
// Pin 141 (GPIO74) to GPIO74 (mode 0)
// Pin 142 (GPIO75) to GPIO75 (mode 0)
// Pin 143 (GPIO76) to GPIO76 (mode 0)
// Pin 144 (GPIO77) to GPIO77 (mode 0)
// Pin 145 (GPIO78) to GPIO78 (mode 0)
// Pin 146 (GPIO79) to GPIO79 (mode 0)
// Pin 148 (GPIO80) to GPIO80 (mode 0)
// Pin 149 (GPIO81) to GPIO81 (mode 0)
// Pin 176 (GPIO94) to SCIRXDD (mode 6)
// Pin 175 (GPIO93) to SCITXDD (mode 6)
// Pin 134 (GPIO69) to SPISIMOC (mode 15)
// Pin 136 (GPIO71) to SPICLKC (mode 15)
// Pin 139 (GPIO72) to SPISTEC (mode 15)
// Pin 135 (GPIO70) to SPISOMIC (mode 15)
#define GPCMUX1_MASK		0xffffffff
#define GPCMUX2_MASK		0x3c00000f
#define GPCMUX1_VALUE		0x0003fc00
#define GPCMUX2_VALUE		0x28000000
#define GPCGMUX1_VALUE		0x0003fc00
#define GPCGMUX2_VALUE		0x14000000

//
// Port D mux register values
//
// Pin 17 (GPIO99) to GPIO99 (mode 0)
#define GPDMUX1_MASK		0x000000c0
#define GPDMUX2_MASK		0x00000000
#define GPDMUX1_VALUE		0x00000000
#define GPDMUX2_VALUE		0x00000000
#define GPDGMUX1_VALUE		0x00000000
#define GPDGMUX2_VALUE		0x00000000

//
// Port E mux register values
//
// Pin 118 (GPIO133) to GPIO133 (mode 0)
#define GPEMUX1_MASK		0x00000c00
#define GPEMUX1_VALUE		0x00000000
#define GPEGMUX1_VALUE		0x00000000

//
// Port B analog mode register values
//
// Pin 130 (GPIO42) to USB0DM (mode ALT)
// Pin 131 (GPIO43) to USB0DP (mode ALT)
#define GPBAMSEL_MASK		0x00000c00
#define GPBAMSEL_VALUE		0x00000c00


//
// Port A/B/C/D PUD register Enable
// GPIO 0 ~ 99 PUD Enable Exclude Gpio 28,29,34,133
#define GPAPUD_ENABLE		0x30000000
#define	GPBPUD_ENABLE		0x00000004
#define	GPCPUD_ENABLE		0x00000000
#define	GPDPUD_ENABLE		0x00000000

// Port A/B/C/D GPACTRL register :: qualification sampling period control
#define PLLSYSCLKDIV4		0x02020202


// Port A/B/C/D GPAQSEL register :: Qualification 6 samples
#define	ONESAMPLES_QUAL		0x00000000
#define	THREE_SAMPLES_QUAL	0x55555555
#define	SIX_SAMPLES_QUAL	0xAAAAAAAA
#define	NO_SYNC				0xFFFFFFFF


// port A/B/C/D Direction Register 
#define GPADIRSET			0x78FFFFFF
#define	GPBDIRSET			0x7FFB0FF7
#define	GPCDIRSET			0xE0001FE0
#define	GPDDIRSET			0xFFFFFFF7

// Selects which master's GPIODAT/SET/CLEAR/TOGGLE registers control this GPIO pin :: 
//	xx00: CPU1 selected
//	xx01: CPU1.CLA selected
//	xx10: CPU2 selected
//  xx11: CPU2.CAL1 selected
#define GPIO0TO7_CORE_SELECTED		0x00000000
#define	GPIO8TO15_CORE_SELECTED		0X22220000
#define GPIO16TO23_CORE_SELECTED	0X00002222
#define	GPIO24TO31_CORE_SELECTED	0X22220000

#define	GPIO32TO39_CORE_SELECTED	0X00000000
#define GPIO40TO47_CORE_SELECTED	0X00220000
#define GPIO48TO55_CORE_SELECTED	0X22220200
#define GPIO56TO63_CORE_SELECTED	0X22222222

#define GPIO64TO71_CORE_SELECTED	0X00022200
#define	GPIO72TO79_CORE_SELECTED	0X00022220
#define GPIO80TO87_CORE_SELECTED	0x22222200
#define	GPIO88TO95_CORE_SELECTED	0x00000022

#define	GPIO96TO103_CORE_SELECTED	0x00000000

//
// Input X-BAR register values
//
// Pin 174 (GPIO92) to INPUTXBAR1 (mode XBAR1)
// Pin 173 (GPIO91) to INPUTXBAR2 (mode XBAR2)
// Pin 172 (GPIO90) to INPUTXBAR3 (mode XBAR3)
// Pin 171 (GPIO89) to INPUTXBAR4 (mode XBAR4)
// Pin 170 (GPIO88) to INPUTXBAR5 (mode XBAR5)
// Pin 157 (GPIO87) to INPUTXBAR6 (mode XBAR6)
// Pin 27  (GPIO26) to INPUTXBAR7 (mode XBAR7)
// Pin 25  (GPIO25) to INPUTXBAR8 (mode XBAR8)
// Pin 156 (GPIO86) to INPUTXBAR9 (mode XBAR9)
// Pin 24  (GPIO24) to INPUTXBAR10 (mode XBAR10)
// Pin 155 (GPIO85) to INPUTXBAR11 (mode XBAR11)
// Pin 154 (GPIO84) to INPUTXBAR12 (mode XBAR12)
// Pin 151 (GPIO83) to INPUTXBAR13 (mode XBAR13)
// Pin 150 (GPIO82) to INPUTXBAR14 (mode XBAR14)
#define INPUT1SELECT_VALUE	92	//INV U		
#define INPUT2SELECT_VALUE	91	//INV V	
#define INPUT3SELECT_VALUE	90	//INV W
#define INPUT4SELECT_VALUE	89	//REC AP
#define INPUT5SELECT_VALUE	88	//REC BP
#define INPUT6SELECT_VALUE	87	//REC CP
#define INPUT7SELECT_VALUE	26	//CARRIER SYNC
#define INPUT8SELECT_VALUE	25	//LBS SYNC
#define INPUT9SELECT_VALUE	86	//REC AN
#define INPUT10SELECT_VALUE	24	//INV SYNC
#define INPUT11SELECT_VALUE	85	//REC BN
#define	INPUT12SELECT_VALUE 84	//REC CN
#define	INPUT13SELECT_VALUE 83	//CHG P
#define INPUT14SELECT_VALUE 82	//CHG N

//
//EPwm Trip Source Select
//
#define	EPWMXBAR_TRIP7	1
#define EPWMXBAR_TRIP8	1
#define	EPWMXBAR_TRIP9	1
#define	EPWMXBAR_TRIP10	3
#define	EPWMXBAR_TRIP11	3
#define	EPWMXBAR_TRIP12	3


class Class_GPIODrv
{
	private:		
		void	CPUSELConfig(void);
		
	public:
		void 	Drv_GPIOInit(void);
		void	Drv_Int_ADCh1Cs(void);
		void	Drv_Int_ADCh2Cs(void);
		void	Drv_CPUSELConfig(void);

};
	
#endif
//===========================================================================
// End of file.
//===========================================================================


