/************************************************************************************
//函数名称：    公共函数实现及类变量定义
***********************************************************************************/
//Class_Monitor				objMonitor;
//Class_CAN					objCAN;


//Class_Settings				objSettings;
Class_System				objSystem;
Class_PowCANProtocol		objPowCANPro;
Class_PowCANScheduler		objPowCANSched;
Class_DigitalIO				objDigitalIO;
Class_Switches				objSwitches;
Class_Bypass				objBypass;
Class_Inverter				objInverter;
Class_Output				objOutput;
Class_Parallel				objParallel;
Class_DCBus					objDCBus;
Class_InverterAlgorithm		objInvAlg;
Class_MonitorInterface		objMonInterface;
Class_Load					objLoad;
Class_InverterControl		objInvCtrl;
Class_Transfer				objXfer;
Class_LBS					objLBS;
Class_Service				objService;
Class_FaultRecord			objFaultRecord;
Class_EEPROM				objEEPROM;
Class_SelfAgingAlgorithm	objSelfAgingAlg;
Class_Debug					objDebug;
Class_FlashUpdate			objFlashUpdate;

//-----System CAN 相关定义--------
SysCanProtocol		ProtocolObj;
SysCanApp			SysCanAppObj;
//应用层消息处理对象
SysCanDataExchg 	DataExchgObj;

//Inter Process Communication





int isqrt(long ax)
{
	UNLONG temp;
 	unsigned int temp1;
 	int xn1,xn; 
 
 	if (ax<=0) return(0);
 	if (ax<=3) return(1);
 	temp.dword=ax;
 	temp1=0;
 
 	while(temp.half.hword<0x4000)
  	{
    	temp1++;
    	temp.dword=temp.dword<<1;
  	}
  
 	temp1=(temp1>>1)+1;
 	temp.dword=ax;
 
 	while (temp1>0)  
 	{
 		temp.dword=temp.dword<<1;
 		temp1--;
 	}
 
 	xn=temp.dword>>16;
 	xn1=(xn+ax/xn)>>1;
 
 	while (abs(xn1-xn)>1) 
 	{
 		xn=xn1;
 		xn1=(xn+ax/xn)>>1;
 	}

	 return (xn1);

}

//------------------------------
int igap(int curr_value, const FLAG_TABLE * f_table, int old_flag)
{
	UINT16 i, j;

	/*reseach range table.*/
	for(i = 0; i <= (*f_table).num; i++)
	{
		if(curr_value < (*f_table).range[i])
			break;
	}
	/*if reseach failure, return old flag.*/
	if(i == 0 || i > (*f_table).num)
	{
		return(old_flag);
	}

	/*reseach flags table.*/
	for(j = 1; j <= (*f_table).num; j++)
	{
		if(old_flag == (*f_table).flags[j-1])
			break;
	}
	/*if reseach failure, return curr flag.*/
	if( j> (*f_table).num )
	{
		return((*f_table).flags[i-1]);
	}

	/*judge igap acrossed.*/
	if(i < j && curr_value > ((*f_table).range[i] - (*f_table).half_gap) )
	{
		i++;
	}
	else if (i > j && curr_value < ((*f_table).range[i-1] + (*f_table).half_gap) )
	{
		i--;
	}

	/*return ture flag.*/
	return((*f_table).flags[i-1]);
}


//===========================================================================
// End of file.
//===========================================================================
