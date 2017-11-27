/*==========================================================*/ 	
/* Title		:	Service.h														*/ 	
/* Description	: 	Class_Service definition											*/ 	
/* Date		:	Apr.2008														*/ 	
/*==========================================================*/				
#ifndef    SERVICE_H
       
#define	   SERVICE_H
	
	class Class_Service
	{
	private:
		INT16 	m_i16ServiceIntervalCntPerMloop;	//Î¬ÐÞ¼ì²âÄ£Ê½
		INT16 	m_i16ServiceModeCount;
	public:
		void 	App_ServiceMode(void);
		
	};

#endif
//===========================================================================
// End of file.
//===========================================================================
