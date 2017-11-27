/*文件名： INVC.Cpp											    */
/*说  明： CPP总控程序											*/

#include "..\Driver\include\Types.h"
#include "Vector.h"
#include "..\Driver\Drivers.h"


#include <stdbool.h>
#include <stdint.h>
#include <string.h>


#include "PubFuncs.h"
#include "Class\SysCanDefine.h"
#include "Class\Const.h"
#include "Class\Define.h"
#include "Class\System.h"
#include "Class\PowCANScheduler.h"
#include "Class\PowCANProtocol.h"
#include "Class\DigitalIO.h"
#include "Class\Switches.h"
#include "Class\Bypass.h"
#include "Class\Inverter.h"
#include "Class\Output.h"
#include "Class\Parallel.h"
#include "Class\DCBus.h"
#include "Class\InverterAlgorithm.h"

#include "Class\MonitorInterface.h"
#include "Class\Load.h"
#include "Class\InverterControl.h"
#include "Class\Transfer.h"
#include "Class\LBS.h"
#include "Class\Service.h"
#include "Class\FaultRecord.h"
#include "Class\EEPROM.h"
#include "Class\SelfAgingAlgorithm.h"
#include "Class\Debug.h"
#include "Class\FlashUpdate.h"


#include "CntrISR.h"



//----System CAN------
#include "Class\TemplateCmd.h"
//#include "Classes\TemplateQueue.h"
#include "Class\TemplateList.h"


#include "Class\SysCanProtocol.h"
#include "Class\SysCanApp.h"
#include "Class\SysCanDataExchg.h"
#include "Class\SysCanQueue.h"
//-----

#include "..\Driver\include\F2837xD_GlobalVariableDefs.cpp"
#include "..\Driver\Drivers.cpp"

#include "..\Driver\GPIODrv.cpp"
#include "..\Driver\SCIDrv.cpp"
#include "..\Driver\SPIDrv.cpp"
#include "..\Driver\CANDrv.cpp"
#include "..\Driver\I2CDrv.cpp"
#include "..\Driver\ADCDrv.cpp"
#include "..\Driver\PWMDrv.cpp"
#include "..\Driver\CAPDrv.cpp"
#include "..\Driver\SysDrv.cpp"
#include "..\Driver\WatchDogDrv.cpp"
#include "..\Driver\RAMDrv.cpp"
#include "..\Driver\FlashDrv.cpp"
#include "PubFuncs.cpp"
#include "..\Driver\TimerDrv.cpp"


#include "vector.cpp"



#include "CntrISR.cpp"
//#include "Sci\Mntr.cpp"
//#include "SciISR.cpp"
//#include "Class\Can.cpp"



#include "Class\System.cpp"
#include "Class\PowCANScheduler.cpp"
#include "Class\PowCANProtocol.cpp"
#include "Class\DigitalIO.cpp"
#include "Class\Switches.cpp"
#include "Class\Bypass.cpp"
#include "Class\Inverter.cpp"
#include "Class\Output.cpp"
#include "Class\Parallel.cpp"
#include "Class\DCBus.cpp"
#include "Class\InverterAlgorithm.cpp"
#include "Class\MonitorInterface.cpp"
#include "Class\Load.cpp"
#include "Class\InverterControl.cpp"
#include "Class\Transfer.cpp"
#include "Class\LBS.cpp"
#include "Class\Service.cpp"
#include "Class\FaultRecord.cpp"
#include "Class\EEPROM.cpp"
#include "Class\SelfAgingAlgorithm.cpp"
#include "Class\Debug.cpp"
#include "Class\FlashUpdate.CPP"

//----System CAN-------
#include "Class\SysCanHal.cpp"
#include "Class\SysCanProtocol.cpp"
#include "Class\SysCanApp.cpp"
#include "Class\SysCanDataExchg.cpp"

#include "Main.cpp"


//===========================================================================
// End of file.
//===========================================================================

