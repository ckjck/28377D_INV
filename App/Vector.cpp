/*=======================================================*/
/* Title:  	Vector File For Inverter F2808 DSP           */
/* Author: 	Zhuo Qingfeng, Zhang Xuejie					 */
/* Date:	Nov.2005									 */
/*=======================================================*/

	asm(" .ref _c_int00");
	asm(" .sect codestart ");
	asm(" LB _MainSwitch");
	asm(" .sect normalfuncs");
	asm(" LB _c_int00");
	asm(" .sect flashflag ");                       
	asm(" .WORD 55BBH ");	
	asm(" .text ");




interrupt void (PIE_RESERVED)(void)  
{
  	asm ("      ESTOP0");
  	while(1)									// À¿—≠ª∑
	{					
		objTimerDrv.Drv_usDelay(1000000);  				// 1s, LED…¡À∏
		GpioDataRegs.GPBTOGGLE.bit.GPIO40 = 1;
	}	 
}


//===========================================================================
// End of file.
//===========================================================================
