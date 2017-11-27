/*==========================================================*/ 	
/* Title		:	Output.h														*/ 	
/* Description	: 	Class_Output definition											*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/ 
#ifndef       OUTPUT_H
       
#define       OUTPUT_H

struct OutFlag
{
	UINT16	bShortCircuit				:1;		//输出短路标志
	UINT16	bDthetaOutFast				:1;				//步长快检超保护
	UINT16	bVoutMFast					:1;				//模值快检超保护
	UINT16	bVoutFast					:1;				//旁路电压超保护
	UINT16	bRev0415					:12;	//Bit0415 保留
};

class Class_Output
	{
	public:
		struct OutFlag m_st_OutFlag;
		INT16			m_i16SinQOut_0;				//输出矢量角正弦值
		INT16			m_i16CosQOut_0;				//输出矢量角余弦值
		UNLONG			m_unVoutMMean_0;			//输出电压模滤波值
		UNLONG			m_unDthetaOutBase_0;		//输出矢量角基本步长		
		
		INT16			m_i16Vouta_0;				//A相输出电压   
		INT16			m_i16Voutb_0;				//B相输出电压   
		INT16			m_i16Voutc_0;				//C相输出电压
		
		INT16			m_i16VoutaPrms;				//输出A相电压即时有效值	
		INT16			m_i16VoutbPrms;				//输出B相电压即时有效值
		INT16			m_i16VoutcPrms;				//输出C相电压即时有效值
		                
		UNLONG			m_unVoutaRms;				//输出A相电压有效值(含滤波尾数)	
		UNLONG			m_unVoutbRms;				//输出B相电压有效值(含滤波尾数)		
		UNLONG			m_unVoutcRms;				//输出C相电压有效值(含滤波尾数)	

		//修改开始。Adpater20K需检测输出电流，增加相关变量
		INT16			m_i16Iouta_0;				//A相输出电流 
		INT16			m_i16Ioutb_0;				//B相输出电流  
		INT16			m_i16Ioutc_0;				//C相输出电流
		
		INT16			m_i16IoutaPrms;				//输出A相电流即时有效值	
		INT16			m_i16IoutbPrms;				//输出B相电流即时有效值
		INT16			m_i16IoutcPrms;				//输出C相电流即时有效值
		               
		INT16			m_i16IoutaforShow;			//降额使用时传送电流时需走很多分支并折算，故专门用变量传送输出电流作显示
		INT16			m_i16IoutbforShow;
		INT16			m_i16IoutcforShow; 

		UINT16			m_u16IoutaCrest_0;     	//A相输出电流峰值比
		UINT16			m_u16IoutbCrest_0;     		//B相输出电流峰值比
		UINT16			m_u16IoutcCrest_0;     		//C相输出电流峰值比
		//修改结束。           
		
		INT16			m_i16Souta;					//输出A相容量,由相应有效值求取
		INT16			m_i16Soutb;					//输出B相容量,由相应有效值求取
		INT16			m_i16Soutc;					//输出C相容量,由相应有效值求取
                	   
		INT16			m_i16Pouta;					//输出A相功率,由累加平均求取
		INT16			m_i16Poutb;					//输出B相功率,由累加平均求取
		INT16			m_i16Poutc;					//输出C相功率,由累加平均求取
		              
		INT16			m_i16PFouta;
		INT16			m_i16PFoutb;
		INT16			m_i16PFoutc;
		                
		INT16			m_i16LoadRatea;
		INT16			m_i16LoadRateb;
		INT16			m_i16LoadRatec;
		
		
		INT16			m_i16wPFoutLeada;			//A 相超前载的过载百分比; 1024=满载
		INT16			m_i16wPFoutLeadb;			//B 相超前载的过载百分比; 1024=满载
		INT16			m_i16wPFoutLeadc;			//c 相超前载的过载百分比; 1024=满载
		                
		INT16			m_i16FoutMean;				//输出平均频率
		                
		INT16			m_i16wVoutFastRef1;			//根据m_i16VbpDnlimit变化
		INT16			m_i16wVoutFastRef2;			//-40%
		INT16			m_i16wVoutFastRef3;			//70%
		INT16			m_i16wVoutFastRef4;			//120%	
		
	                				
		
		INT16                    m_i16VoutFastchkCnt_0;	       //输出快检计数器
	private:
		UNLONG			m_unVoutabRms;				//输出AB线电压有效值(含滤波尾数)
		UNLONG			m_unVoutbcRms;				//输出BC线电压有效值(含滤波尾数)
		UNLONG			m_unVoutcaRms;				//输出CA线电压有效值(含滤波尾数)
		
		INT16			m_i16Voutab_0;				//AB输出线电压
		INT16			m_i16Voutbc_0;				//BC输出线电压
		INT16			m_i16Voutca_0;				//CA输出线电压
				
		INT16			m_i16KVoutNormal;
		INT16			m_i16KIoutNormal;
		               
		INT16			m_i16KVouta_0;				//输出AB线电压定标系数  
		INT16			m_i16KVoutb_0;				//输出BC线电压定标系数  
		INT16			m_i16KVoutc_0;				//输出CA线电压定标系数 
				        	                  	
		INT16			m_i16KIouta_0;				//输出A相电流定标系数  
		INT16			m_i16KIoutb_0;				//输出B相电流定标系数  
		INT16			m_i16KIoutc_0;				//输出C相电流定标系数 

		UNLONG			m_unIoutaRms;				//输出A相电流有效值(含滤波尾数)	
		UNLONG			m_unIoutbRms;				//输出B相电流有效值(含滤波尾数)		
		UNLONG			m_unIoutcRms;				//输出C相电流有效值(含滤波尾数)	
	
		INT16			m_i16CFouta;
		INT16			m_i16CFoutb;
		INT16			m_i16CFoutc;

		UNLONG			m_unIoutaSum_0;				//输出A相电流平方累加和,中断累加用***
		UNLONG			m_unIoutaSumM_0;			//输出A相电流平方周期和,主程序求有效值用
		UNLONG			m_unIoutbSum_0;				//输出B相电流平方累加和,中断累加用
		UNLONG			m_unIoutbSumM_0;			//输出B相电流平方周期和,主程序求有效值用
		UNLONG			m_unIoutcSum_0;				//输出C相电流平方累加和,中断累加用
		UNLONG			m_unIoutcSumM_0;			//输出C相电流平方周期和,主程序求有效值用
		
		INT16			m_i16IoutaCrestAck_0;  		//A相输出电流峰值比峰值比确认次数
		INT16			m_i16IoutbCrestAck_0;  		//B相输出电流峰值比峰值比确认次数
		INT16			m_i16IoutcCrestAck_0;  		//C相输出电流峰值比峰值比确认次数	
		
		
		INT16			m_i16PSouta;				
		INT16			m_i16PSoutb;					
		INT16			m_i16PSoutc;	
	
		INT16			m_i16VoutabPrms;			//输出AB线电压即时有效值				
		INT16			m_i16VoutbcPrms;			//输出BC线电压即时有效值
		INT16			m_i16VoutcaPrms;			//输出CA线电压即时有效值
			


		           
		UNLONG			m_unVoutaFilter_0;			                              
		UNLONG			m_unVoutbFilter_0;			                              
		UNLONG			m_unVoutcFilter_0;
	 
		INT16			m_i16VoutM_0;				//输出电压模值
		 	                    	
		INT16			m_i16DthetaOutInst_0;		//输出矢量角瞬时步长	
		  	                    	
		UNLONG			m_unVoutaSum_0;				//输出A相电压平方累加和,中断累加用***
		UNLONG			m_unVoutaSumM_0;			//输出A相电压平方周期和,主程序求有效值用
		UNLONG			m_unVoutbSum_0;				//输出B相电压平方累加和,中断累加用
		UNLONG			m_unVoutbSumM_0;			//输出B相电压平方周期和,主程序求有效值用
		UNLONG			m_unVoutcSum_0;				//输出C相电压平方累加和,中断累加用
		UNLONG			m_unVoutcSumM_0;			//输出C相电压平方周期和,主程序求有效值用
         		
		UNLONG			m_unVoutabSum_0;			//输出AB线电压平方累加和,中断累加用***
		UNLONG			m_unVoutabSumM_0;			//输出AB线电压平方周期和,主程序求有效值用
		UNLONG			m_unVoutbcSum_0;			//输出BC线电压平方累加和,中断累加用
		UNLONG			m_unVoutbcSumM_0;			//输出BC线电压平方周期和,主程序求有效值用
		UNLONG			m_unVoutcaSum_0;			//输出CA线电压平方累加和,中断累加用
		UNLONG			m_unVoutcaSumM_0;			//输出CA线电压平方周期和,主程序求有效值用
	            	                    	
		        	                   	
		INT16			m_i16PointcntOut_0;			//输出电压累加点数,中断累加用
		INT16			m_i16PointcntOutM_0;		//输出电压周期点数,主程序求有效值用 
		        	                    	
		UNLONG			m_unDthetaOutMean_0;		//输出矢量角平均步长
		
		INT16			m_i16SinQoutFilter_0;
		INT16			m_i16CosQoutFilter_0;
		INT16			m_i16SinQoutOld_0;			//输出矢量角正弦值
		INT16			m_i16CosQoutOld_0;			//输出矢量角余弦值
		        	                    	
		UNLONG			m_unPoutaAcc_0;				//输出A相功率累加和,中断累加用
		UNLONG			m_unPoutaAccM_0;			//输出A相功率周期和,主程序求功率用
                          	
		UNLONG			m_unPoutbAcc_0;				//输出B相功率累加和,中断累加用
		UNLONG			m_unPoutbAccM_0;			//输出B相功率周期和,主程序求功率用
                         	
		UNLONG			m_unPoutcAcc_0;				//输出C相功率累加和,中断累加用
		UNLONG			m_unPoutcAccM_0;			//输出C相功率周期和,主程序求功率用
		
		INT16			m_i16VoutaFastchkCnt_0;		//输出A相电压快速检测计数器
		INT16			m_i16VoutbFastchkCnt_0;		//输出B相电压快速检测计数器
		INT16			m_i16VoutcFastchkCnt_0;		//输出C相电压快速检测计数器
		INT16			m_i16SpanVbpFastAct_0;		//旁路电压快速检测启动计数器
		INT16			m_i16ClkSVbpFastchk_0;		//旁路快速检测用秒时钟备份
		INT16			m_i16VoutaFtFastchkCnt_0;
		INT16			m_i16VoutbFtFastchkCnt_0;
		INT16			m_i16VoutcFtFastchkCnt_0;
		        	    
		INT16			m_i16VoutaFastAbnormalCnt_0;	//zqf 060714--增加“输出电压异常”保护功能
		INT16			m_i16VoutbFastAbnormalCnt_0;
		INT16			m_i16VoutcFastAbnormalCnt_0;
		
		INT16			m_i16VoutFastAbnormalStartCnt_0;// 输出异常检测开始延时计数器
		INT16           m_i16VinvFastchkStartCnt;		//输出电压快检开始延时计数器

		//Adapter20 三单输出过零点检测
		INT16		m_i16VoutaFilter0_0;						//输出电压滤波后的值
		INT16		m_i16VoutaFilter1_0;						//输出电压滤波后的值
		INT16		m_i16VoutaFilter2_0;						//输出电压滤波后的值
		INT16		m_i16VoutaFilter3_0;						//输出电压滤波后的值
		INT16		m_i16VoutaFilter4_0;						//输出电压滤波后的值
		INT16		m_i16VoutaFilter5_0;	
		INT16			m_i16ToutCnt_0;
		INT16			m_i16VoutaOld_0;
		INT16			m_i16VoutCrossCnt;
		UINT32			m_u32Tout;
		UINT32			m_u32ToutClk_0;
		UINT32			m_u32ToutClkOld_0;
		//

		//短路
		INT16		m_i16ShortPointCnt;
		INT16		m_i16IoutaShortCnt;
		INT16		m_i16IoutbShortCnt;
		INT16		m_i16IoutcShortCnt;
		INT16		m_i16IaShortFlag;
		INT16		m_i16IbShortFlag;
		INT16		m_i16IcShortFlag;
		//ECO快检
		INT16		m_i16VoutMFastChkCnt_0;
		INT16		m_i16DthetaoutFastChkCnt_0;
	public:
		void			App_OutVarInit(void);
		void			App_OutParamConfig(void);
		            	
		void			Dat_Int_OutPreDeal(void);		
		void 			Dat_Int_OutCal(void);
		void			Dat_VoutRMS(void);
		void			Dat_IoutRMS(void);
		void			Dat_PoutRMS(void);
		            	
		void			Alg_Int_VoutFastChk(void);
		void			Alg_Int_VoutAbnormal(void);
		void			Alg_VoutAbnormal(void);
		void			Alg_Int_VoutFastChk2(void);
		//void			Alg_Int_ShortCircuit(void);
		void		Alg_Int_VOutRangeFastChk(void);

		friend class Class_Transfer;
	
	};

#endif
//===========================================================================
// End of file.
//===========================================================================
