#include "FSM\BaseElementOwnedStates.h"
#include "libCUFEM\BaseElement.h"

namespace HBXFEMDef
{

	


	ReadyToCal * ReadyToCal::Instance()
	{
		static ReadyToCal instance;
		return &instance;
	}

}

