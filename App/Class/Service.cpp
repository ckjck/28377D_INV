/*==========================================================*/ 	
/* Title		:	Service.cpp													*/ 	
/* Description	: 	Class_Service definition											*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#include "Service.h"


	void 	Class_Service::App_ServiceMode(void)
	{
		//CARRIER_SYNC=0		
		objDigitalIO.m_st_wLocalSignal.bCarrierSyn = 0;			

		//AD_CS=0					
		objGPIODrv.Drv_Int_ADCh1Cs();
		
		//并机信号清零		
		S2U(objParallel.m_st_wParaSignal)  = 0x0000;	

		//要求只能在MAINT16位
		if ( objDCBus.m_i16VdcSmp_0 >= 506)		                    //母线有电不发波母线电压<100v
		{
			objDigitalIO.m_un_ExOutCReg.BIT.bBPSCR = 0;				// 清INVKM，BPSTS 
			objDigitalIO.m_un_ExOutAReg.BIT.bINVKM = 0;	 
			objPWMDrv.Drv_PWMInactive();	                        // 关PWM		
			m_i16ServiceIntervalCntPerMloop = 2000;								// 5ms	
		}
		else
		{

			if(m_i16ServiceIntervalCntPerMloop > 0) 
			{
				m_i16ServiceIntervalCntPerMloop--;
			}
			else
			{			
//				objDigitalIO.m_st_wLocalSignal.bBpSTS = 1;				// 置INVKM，BPSTS 
				objPWMDrv.Drv_PWMActive();	                        	// 开PWM	
				objDigitalIO.m_un_ExOutAReg.BIT.bPWMEN = 1;
				objXfer.m_st_XferResult.bInvKM = 1;
				objSystem.m_st_wFlagStatus.bInv = 1;
				objDigitalIO.Dat_DigitalOutput();
				
			}	
		}
		if(objDigitalIO.m_st_wLocalSignal.bLed == 0)
   		{
    	  m_i16ServiceModeCount++;
    	  if(m_i16ServiceModeCount == 30000)
    	  {
    	  		objDigitalIO.m_st_wLocalSignal.bLed = 3;
    	  		m_i16ServiceModeCount = 0;
    	  }
    	}
    	else if(objDigitalIO.m_st_wLocalSignal.bLed == 3)
    	{
    	  m_i16ServiceModeCount++;
    	  if(m_i16ServiceModeCount == 30000)
    	  	{
    	  		objDigitalIO.m_st_wLocalSignal.bLed = 0;
    	  		m_i16ServiceModeCount = 0;
    	  	}
    	}
												
	}	


//===========================================================================
// End of file.
//===========================================================================
