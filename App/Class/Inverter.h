/*==========================================================*/ 	
/* Title		:	Inverter.h													*/ 	
/* Description	: 	Class_Inverter definition										*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/
#ifndef    INVERTER_H
       
#define    INVERTER_H


		struct	structFlagVinvABC
		{
			UINT16	bVinvRMS					:1;					//BIT00		有效值标志  0正常  1异常													
			UINT16	bVoutAbnormal				:1;					//BIT01		输出电压异常有效值标志:[120%,70%] 0正常  1异常	
			UINT16	bInvCurrUnbalanced			:1;
			UINT16	bInvCurrUnbalancedWarn		:1;
			UINT16	bRev1504					:12;                //BIT15-02		
		};							
	
		struct	structFlagInv
		{
			UINT16	bInv						:2;					//BIT01--00	 逆变器状态 	00关闭	01软启动11启动完毕			
			UINT16	bDspCrazyOff				:1;					//BIT02		逆变DSP异常关机  0：非逆变DSP异常关机1：逆变DSP异常关机			
			UINT16	bPLL2PhaseTrace				:1;			        //BIT03  逆变二级锁相跟踪标志 0：跟旁路 1：跟工频同步信号
			UINT16	bPhaseTrace					:2;					//BIT05--04	相位跟踪标志 	00本振	01跟旁路 10跟输出 11跟LBS源			
			UINT16	bAmpTrace					:2;					//BIT07--06	幅度跟踪标志 	00额定	01跟旁路(预留)10跟输出			
			UINT16	bPhaseLock					:1;					//BIT08		相位锁定标志 	bPhaseLock			
			UINT16	bAmpLock					:1;					//BIT09		幅度锁定标志 	bAmpLock			
			UINT16  bMaxcapacityInvFlag			:1;					//BIT10		逆变器已开启机子中容量最大标志	0非最大	1最大			
			UINT16  bMaxcapacityOnFlag			:1;					//BIT11		逆变器已供电机子中容量最大标志	0非最大	1最大			
			UINT16	bInvTraceConf				:1;					//BIT12		Inv nomal trace confirm signal(0: not confirm, 1: confirm)			
			UINT16	bRev1513					:3;
		};


class Class_Inverter
	{
	public:
		volatile struct	structFlagInv		m_st_wFlagInv;		//逆变状态标志
		volatile struct	structFlagVinvABC	m_st_FlagVinvABC;	//逆变电压标志

		INT16			m_i16wVrefTgtMd;
		INT16  			m_i16VrefSetMd;				//逆变额定模值(控制)
		INT16			m_i16Ila_0;					//A相电感电流 
		INT16			m_i16Ilb_0;					//B相电感电流 
		INT16			m_i16Ilc_0;					//C相电感电流 

		INT16			m_i16IlaFilter_0;			//A相电感电流直流分量 
		INT16			m_i16IlbFilter_0;			//B相电感电流直流分量 
		INT16			m_i16IlcFilter_0;	 		//C相电感电流直流分量 		

		INT16			m_i16Iinva_0;				//A相逆变电流     
		INT16			m_i16Iinvb_0;				//B相逆变电流     
		INT16			m_i16Iinvc_0;				//C相逆变电流     
		
		INT16			m_i16Vinva_0;				//A相逆变电压
		INT16			m_i16Vinvb_0;				//B相逆变电压
		INT16			m_i16Vinvc_0;				//C相逆变电压						 

		INT16			m_i16VinvaDc_0;				//A相逆变电压直流分量控制量
		INT16			m_i16VinvbDc_0;				//B相逆变电压直流分量控制量
		INT16			m_i16VinvcDc_0;

		

		INT16			m_i16VinvMd_0;				//逆变电压模值d轴分量
		INT16			m_i16VinvMq_0;				//逆变电压q轴分量  
		INT16			m_i16VinvMz_0;              //逆变电压z轴分量 
		
		INT16			m_i16SinQInv_0;				//逆变器输出电压相角
		INT16			m_i16CosQInv_0;				//逆变矢量角余弦值	
		UNLONG			m_unVinvMMean_0;			//逆变电压模滤波值
		INT16			m_i16FinvMean;				//逆变平均频率
		INT16			m_i16PointcntInvM_0;		//逆变电压周期点数,主程序求有效值用
		INT16			m_i16PointcntInv_0;			//逆变电压累加点数,中断累加用

		UNLONG			m_unVinvaDcFilter_0;	                                 
		UNLONG			m_unVinvbDcFilter_0;                                    
		UNLONG			m_unVinvcDcFilter_0;  
		
		INT16      	    m_i16IinvMz_0;              //逆变电流z轴分量 	
		INT16			m_i16IinvMd_0;				//逆变电流d轴分量
		INT16			m_i16IinvMq_0;				//逆变电流q轴分量  
		
		INT16      	    m_i16IinvMz_1;              //逆变电流z轴分量,用于电流前馈 	
		INT16			m_i16IinvMd_1;				//逆变电流d轴分量,用于电流前馈 
		INT16			m_i16IinvMq_1;				//逆变电流q轴分量,用于电流前馈   
		    	               	
		UNLONG			m_unVbusSubFilter_0;		//用于母线差Z轴环流的控制     	               	
		UNLONG			m_unIinvMdFilter_0; 		//用于通过CAN在并机间传输
		UNLONG			m_unIinvMqFilter_0;			//用于通过CAN在并机间传输
		UNLONG			m_unIinvMzFilter_0;			//用于通过CAN在并机间传输
		      	         
		INT16  	        m_i16IlMz_0;       			//电感电流z轴分量
		INT16			m_i16IlMd_0;				//电感电流d轴分量
		INT16			m_i16IlMq_0;				//电感电流q轴分量
		
		INT16			m_i16VinvaPrms;				//逆变A相电压即时有效值	
		INT16			m_i16VinvbPrms;				//逆变B相电压即时有效值
		INT16			m_i16VinvcPrms;				//逆变C相电压即时有效值
		UNLONG			m_unVinvaRms;				//逆变A相电压有效值(含滤波尾数)	
		UNLONG			m_unVinvbRms;				//逆变B相电压有效值(含滤波尾数)		
		UNLONG			m_unVinvcRms;				//逆变C相电压有效值(含滤波尾数)	
		
		INT16			m_i16VinvabPrms;			//逆变AB线电压即时有效值		
		INT16			m_i16VinvbcPrms;			//逆变BC线电压即时有效值
		INT16			m_i16VinvcaPrms;			//逆变CA线电压即时有效值
		UNLONG			m_unVinvabRms;				//逆变AB线电压有效值(含滤波尾数)
		UNLONG			m_unVinvbcRms;				//逆变BC线电压有效值(含滤波尾数)		
		UNLONG			m_unVinvcaRms;				//逆变CA线电压有效值(含滤波尾数)		

		
		INT16			m_i16IindaPrms;				//逆变电感A相电流即时有效值	
		INT16			m_i16IindbPrms;				//逆变电感B相电流即时有效值
		INT16			m_i16IindcPrms;				//逆变电感C相电流即时有效值
		UNLONG			m_unIindaRms;				//逆变电感A相电流有效值(含滤波尾数)	
		UNLONG			m_unIindbRms;				//逆变电感B相电流有效值(含滤波尾数)		
		UNLONG			m_unIindcRms;				//逆变电感C相电流有效值(含滤波尾数)	
		
		INT16			m_i16IinvaPrms;				//逆变A相电流即时有效值	
		INT16			m_i16IinvbPrms;				//逆变B相电流即时有效值
		INT16			m_i16IinvcPrms;				//逆变C相电流即时有效值
		UNLONG			m_unIinvaRms;				//逆变A相电流有效值(含滤波尾数)	
		UNLONG			m_unIinvbRms;				//逆变B相电流有效值(含滤波尾数)		
		UNLONG			m_unIinvcRms;				//逆变C相电流有效值(含滤波尾数)
		
		INT16			m_i16Pinva;					//逆变A相功率,由累加平均求取
		INT16			m_i16Pinvb;					//逆变B相功率,由累加平均求取
		INT16			m_i16Pinvc;					//逆变C相功率,由累加平均求取 

		INT16			m_i16PinvaforShow;
		INT16			m_i16PinvbforShow;
		INT16			m_i16PinvcforShow;
		
		INT16			m_i16Sinva;					//逆变A相容量,由相应有效值求取
		INT16			m_i16Sinvb;					//逆变B相容量,由相应有效值求取
		INT16			m_i16Sinvc;					//逆变C相容量,由相应有效值求取

		INT16			m_i16SinvaforShow;
		INT16			m_i16SinvbforShow;
		INT16			m_i16SinvcforShow;
		               
		INT16			m_i16SLinva;				//逆变电感A相容量,由相应有效值求取
		INT16			m_i16SLinvb;				//逆变电感B相容量,由相应有效值求取
		INT16			m_i16SLinvc;				//逆变电感C相容量,由相应有效值求取
		               
		INT16			m_i16IinvaforShow;
		INT16			m_i16IinvbforShow;
		INT16			m_i16IinvcforShow;


		INT16			m_u16IinvaCrest_0;				//逆变电流峰值比
		INT16			m_u16IinvbCrest_0;				//逆变电流峰值比
		INT16			m_u16IinvcCrest_0;				//逆变电流峰值比

		INT16			m_i16IinvaCrestAck_0;			//逆变电流峰值比
		INT16			m_i16IinvbCrestAck_0;			//逆变电流峰值比
		INT16			m_i16IinvcCrestAck_0;			//逆变电流峰值比


		INT16			m_i16IlaforShow;
		INT16			m_i16IlbforShow;
		INT16			m_i16IlcforShow;

		INT16			m_i16wEnvirTemp;                //环境温度
		INT16			m_i16TempKWGrant;				//温度授权率，根据温度对功率进行调节,并包含有输出功率因素
		INT16			m_i16TempKVAGrant;				//温度授权率，根据温度对功率进行调节		
		
		
		INT16			m_i16wComIndSatConfFlag;
		/*			0--不饱和
					1--饱和：确认9.5s
					2--饱和：确认30s
		*/
	
		//zqf 060413--共性问题更改，逆变开机时屏蔽逆变不同步信息上报
		INT16			m_i16wInvWarn1LockConfCntPer40ms;
		INT16                    m_i16_InvaOverCurrentChkCnt;		//大电流快检确认计数器
		INT16                    m_i16_InvbOverCurrentChkCnt;
		INT16                    m_i16_InvcOverCurrentChkCnt;
		INT16                    m_i16_InvOverCurrentChkCnt;
		INT16			m_i16_InvOverCurrentChkCnt1;		//大电流快检确认计数器，用于逆变接触器短路保护

		//功率因素
		INT16		m_i16PFinva;
		INT16		m_i16PFinvb;
		INT16		m_i16PFinvc;
		INT16		m_i16LoadRatea;
		INT16		m_i16LoadRateb;
		INT16		m_i16LoadRatec;

		//输出功率因素
		INT16		m_i16SystemPF;
	private:
		INT16			m_i16VoutAbnormalCntPer14ms;//输出电压异常计数器
		INT16			m_i16Vinvab_0;				//AB逆变线电压 
		INT16			m_i16Vinvbc_0;				//BC逆变线电压 
		INT16			m_i16Vinvca_0;				//CA逆变线电压 

		UNLONG			m_unIlaFilter_0;			//A相电感电流直流分量滤波值
		UNLONG			m_unIlbFilter_0;			//B相电感电流直流分量滤波值
		UNLONG			m_unIlcFilter_0;			//C相电感电流直流分量滤波值
		 
//		INT16			m_i16VinvaDc_0;				//A相逆变电压直流分量	
//		INT16			m_i16VinvbDc_0;				//B相逆变电压直流分量	
//		INT16			m_i16VinvcDc_0;				//C相逆变电压直流分量 
		                                                              
		INT16			m_i16Iinva2_0;				//A相逆变电流+电感电流直流分量
		INT16			m_i16Iinvb2_0;				//B相逆变电流+电感电流直流分量
		INT16			m_i16Iinvc2_0;				//C相逆变电流+电感电流直流分量

		INT16			m_i16IinvMd2_0;			//逆变电流+电感电流直流分量检测d轴分量
		INT16			m_i16IinvMq2_0;			//逆变电流+电感电流直流分量检测q轴分量		
		INT16			m_i16IinvMz2_0;			//逆变电流+电感电流直流分量检测z轴分量
		
		INT16			m_i16KVinvNormal;
		INT16			m_i16KIinvNormal;
		
		INT16			m_i16KVinva_0;				//逆变AB线电压定标系数  
		INT16			m_i16KVinvb_0;				//逆变BC线电压定标系数  
		INT16			m_i16KVinvc_0;				//逆变CA线电压定标系数  
		INT16			m_i16KIl_0;					//电感电流定标系数      
		INT16			m_i16KIlFilter_0;			//滤波后电感电流定标系
		INT16			m_i16KIinva_0;				//A相逆变电流定标系数  
		INT16			m_i16KIinvb_0;				//B相逆变电流定标系数  
		INT16			m_i16KIinvc_0;				//C相逆变电流定标系数  
		INT16			m_i16KVinvdc_0;				//输出电压直流分量定标系数	

		INT16			m_i16VinvM_0;				//逆变电压模值
		  	                    	
		UNLONG			m_unDthetaRefBase_0;		//给定矢量角基本步长
		
                
		UNLONG			m_unVinvaSum_0;				//逆变A相电压平方累加和,中断累加用
		UNLONG			m_unVinvaSumM_0;			//逆变A相电压平方周期和,主程序求有效值用
		UNLONG			m_unVinvbSum_0;				//逆变B相电压平方累加和,中断累加用
		UNLONG			m_unVinvbSumM_0;			//逆变B相电压平方周期和,主程序求有效值用
		UNLONG			m_unVinvcSum_0;				//逆变C相电压平方累加和,中断累加用
		UNLONG			m_unVinvcSumM_0;			//逆变C相电压平方周期和,主程序求有效值用
	            		                    	
		UNLONG			m_unVinvabSum_0;			//逆变AB线电压平方累加和,中断累加用
		UNLONG			m_unVinvabSumM_0;			//逆变AB线电压平方周期和,主程序求有效值用
		UNLONG			m_unVinvbcSum_0;			//逆变BC线电压平方累加和,中断累加用
		UNLONG			m_unVinvbcSumM_0;			//逆变BC线电压平方周期和,主程序求有效值用
		UNLONG			m_unVinvcaSum_0;			//逆变CA线电压平方累加和,中断累加用
		UNLONG			m_unVinvcaSumM_0;			//逆变CA线电压平方周期和,主程序求有效值用
        
		UNLONG			m_unIinvaSum_0;				//逆变A相电流平方累加和,中断累加用
		UNLONG			m_unIinvaSumM_0;			//逆变A相电流平方周期和,主程序求有效值用
		UNLONG			m_unIinvbSum_0;				//逆变B相电流平方累加和,中断累加用
		UNLONG			m_unIinvbSumM_0;			//逆变B相电流平方周期和,主程序求有效值用
		UNLONG			m_unIinvcSum_0;				//逆变C相电流平方累加和,中断累加用
		UNLONG			m_unIinvcSumM_0;			//逆变C相电流平方周期和,主程序求有效值用
		              	
		UNLONG			m_unIindaSum_0;				//逆变电感A相电流平方累加和,中断累加用
		UNLONG			m_unIindaSumM_0;			//逆变电感A相电流平方周期和,主程序求有效值用
		UNLONG			m_unIindbSum_0;				//逆变电感B相电流平方累加和,中断累加用
		UNLONG			m_unIindbSumM_0;			//逆变电感B相电流平方周期和,主程序求有效值用
		UNLONG			m_unIindcSum_0;				//逆变电感C相电流平方累加和,中断累加用
		UNLONG			m_unIindcSumM_0;			//逆变电感C相电流平方周期和,主程序求有效值用
		
		UNLONG			m_unDthetaRefMean_0;		//给定矢量角平均步长
		               	
		UNLONG			m_unPinvaAcc_0;				//逆变A相功率累加和,中断累加用
		UNLONG			m_unPinvaAccM_0;			//逆变A相功率周期和,主程序求功率用
                      	
		UNLONG			m_unPinvbAcc_0;				//逆变B相功率累加和,中断累加用
		UNLONG			m_unPinvbAccM_0;			//逆变B相功率周期和,主程序求功率用
                       	
		UNLONG			m_unPinvcAcc_0;				//逆变C相功率累加和,中断累加用
		UNLONG			m_unPinvcAccM_0;			//逆变C相功率周期和,主程序求功率用
		        	          
		INT16			m_i16VinvFault1CntPer14ms;	//逆变电压故障计数器
		INT16			m_i16VinvFault2CntPer14ms;

		INT16			m_i16TempGrant1Cnt;			//温度授权率确认延时
		INT16			m_i16TempGrant9Cnt;
		INT16			m_i16TempGrant8Cnt;
		//三单下判断旁路SCR或者逆变KM其中一相开路
		INT16		m_i16IinvUnbalancedCntPer40ms;	//三单下三个桥臂不均流标志

		INT16		m_i16BpSTSShortPWMShutCnt_0;

		INT32		m_i32IinvaPSum_0;
		INT32		m_i32IinvaPSumM_0;
		INT32		m_i32IinvaQSum_0;
		INT32		m_i32IinvaQSumM_0;
		
		INT32		m_i32IinvbPSum_0;
		INT32		m_i32IinvbPSumM_0;
		INT32		m_i32IinvbQSum_0;
		INT32		m_i32IinvbQSumM_0;
		
		INT32		m_i32IinvcPSum_0;
		INT32		m_i32IinvcPSumM_0;
		INT32		m_i32IinvcQSum_0;
		INT32		m_i32IinvcQSumM_0;


		INT16		m_i16IinvaP_0;
		INT16		m_i16IinvaQ_0;

		
		INT16		m_i16IinvbP_0;
		INT16		m_i16IinvbQ_0;

		
		INT16		m_i16IinvcP_0;
		INT16		m_i16IinvcQ_0;

		INT16		m_i16IlaFilter2_0;
		INT16		m_i16IlbFilter2_0;
		INT16		m_i16IlcFilter2_0;

		INT16		m_i16VbusDiff_0;

//		INT16		m_i16VaNoDc_0;
//		INT16		m_i16VbNoDc_0;
//		INT16		m_i16VcNoDc_0;

		INT16		m_i16PointcntInvMDtheta;
		
	public:
		void			App_InvVarInit(void);
		void			App_InvParamConfig();
		        		
		void			Dat_Int_InvPreDeal(void);
		void 			Dat_Int_InvCal(void);
		void			Dat_VinvRMS(void);
		void			Dat_IinvRMS(void);
		void			Dat_TempPowerGrant(void);
		void			Dat_PinvRMS(void);
		            	
		void			Dat_VinvFlag(void);
		        		
		void			Alg_InvFault(void);

		void			Alg_Int_InvOverCurrentChk(void);
		void			Alg_IinvUnbalanced(void);
		void			Dat_Int_IinvDFTCal(void);
		void			Dat_InvPQRMS(void);
		void			Dat_IinvPQRMS(void);

		friend	class	Class_Debug;	
		friend	class	Class_PowCANScheduler;
		friend	class	Class_InverterAlgorithm;
	};
	
#endif		
	


//===========================================================================
// End of file.
//===========================================================================
