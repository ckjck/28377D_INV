/***************************************************************************************
* 
* Filename: 		SysCanHal.h
* Contributor:
* Author: 		Wang Zhihua	
* Date: 			1/21/2008
* Copyright 		2008 ENPC Corporation, All Rights Reserved
*
* Description: 	
* 
* 
* Version
* Last rev by:		Wang Zhihua
* Last rev date:	1/21/2008
* Last rev desc:	
****************************************************************************************/

//#ifndef CMDTEMPLATE_H
//#define CMDTEMPLATE_H
#ifndef TemplateCmd_H
#define TemplateCmd_H
//---------------------------------------------------------------
//Include header

//-----------------------------------------------------------------------------
//Macro definition and enum
	
//------------------------------------------------------------------------------
//Public variable definition
	
//-------------------------------------------------------------------------------
//func prototype definiton

//-------------------------------------------------------------------------
//Class prototype

//命令基类
class CommandBase
{
public:
	CommandBase(VOID){};
	virtual ~CommandBase(VOID){};

	virtual INT16 Excute(VOID *pArg) = 0;
};
//-----协议层子类命令-------
class ProtocolSubCmd : public CommandBase
{
public:
	typedef INT16 (SysCanProtocol::*ACTION)(VOID *pArg);

//	ProtocolSubCmd(SysCanProtocol *pReceiver, ACTION pAction) : m_pReceiver(pReceiver), m_pAction(pAction){};
//	virtual ~ProtocolSubCmd(VOID){};
	void ProtocolSubCmdIni(SysCanProtocol *pReceiver, ACTION pAction);
	virtual INT16 Excute(VOID *pArg);

private:
	SysCanProtocol *m_pReceiver;
	ACTION m_pAction;
};

//初始化
void ProtocolSubCmd::ProtocolSubCmdIni(SysCanProtocol *pReceiver, ACTION pAction)
{
	m_pReceiver = pReceiver;
	m_pAction = pAction;
}
/********************************************************************************
Excute-----执行具体的命令调用
                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 ProtocolSubCmd::Excute(VOID *pArg)
{
	if (NULL != m_pAction)
	{
		(m_pReceiver->*m_pAction)(pArg);
	}
	
	//TBD
	return (0);
}

//-----应用层子类命令-------
class AppSubCmd : public CommandBase
{
public:
	typedef INT16 (SysCanApp::*ACTION)(VOID *pArg);

//	ProtocolSubCmd(SysCanProtocol *pReceiver, ACTION pAction) : m_pReceiver(pReceiver), m_pAction(pAction){};
//	virtual ~ProtocolSubCmd(VOID){};
	void AppSubCmdIni(SysCanApp *pReceiver, ACTION pAction);
	virtual INT16 Excute(VOID *pArg);

private:
	SysCanApp *m_pReceiver;
	ACTION m_pAction;
};

//初始化
void AppSubCmd::AppSubCmdIni(SysCanApp *pReceiver, ACTION pAction)
{
	m_pReceiver = pReceiver;
	m_pAction = pAction;
}
/********************************************************************************
Excute-----执行具体的命令调用
                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
INT16 AppSubCmd::Excute(VOID *pArg)
{
	if (NULL != m_pAction)
	{
		(m_pReceiver->*m_pAction)(pArg);
	}
	
	//TBD
	return (0);
}

//-------------------------------------------------
//子类命令模板
template <class Receiver>
class TemplateCmd : public CommandBase
{
public:
	typedef INT16 (Receiver::*ACTION)(VOID *pArg);

	TemplateCmd(Receiver *pReceiver, ACTION pAction) : m_pReceiver(pReceiver), m_pAction(pAction){};
	virtual ~TemplateCmd(VOID){};

	virtual INT16 Excute(VOID *pArg);

private:
	Receiver *m_pReceiver;
	ACTION m_pAction;
};


/********************************************************************************
Excute-----执行具体的命令调用
                     
Parameters:              
Return Value: 
Precondition: 
Postcondition: 
********************************************************************************/
template <class Receiver>
INT16 TemplateCmd<Receiver>::Excute(VOID *pArg)
{
	if (NULL != m_pAction)
	{
		(m_pReceiver->*m_pAction)(pArg);
	}
	
	//TBD
	return (0);
}

#endif
