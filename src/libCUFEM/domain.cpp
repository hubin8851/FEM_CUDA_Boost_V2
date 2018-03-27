#include "domain.h"


namespace HBXFEMDef
{
	UserStatusError_t Domain::InstanceSelf(InputRecord * _ir)
	{
		return UserStatusError_t::USER_STATUS_SUCCESS;
	}
	BaseBoundary* Domain::GetBoundry(int _n)
	{
#ifdef _DEBUG
		if (_n<0 || _n>=_vLoad.size())
		{
			std::cerr << "访问越界..." << std::endl;
		}
#endif
		return &_vLoad[_n];
	}
	std::vector<BaseBoundary>& Domain::GetBoundry()
	{
		// TODO: 在此处插入 return 语句
		return this->_vLoad;
	}
}

