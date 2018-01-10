#include <HBXPreDef.h>
#include "BaseComponent.h"

namespace HBXFEMDef
{
	void BaseComponent::ResetID(unsigned int _id)
	{
		Assert((m_MyID >= _id));
		m_MyID = _id;
		m_iNextID = m_MyID + 1;
	}
}


