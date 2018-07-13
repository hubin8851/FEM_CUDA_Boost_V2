#include <libCUFEM\BaseNumMethod.h>
#include <libCUFEM\EngngMod.h>

namespace HBXFEMDef
{

	BaseNumMethod::BaseNumMethod(Domain * _dm, Engng * _eng, int _id) :BaseComponent(_id),
		MyDomain(_dm), MyEngng(_eng)
	{
		MyEngng->RegistEntity(classType::NUMERICALMETHOD, this);
	};
	BaseNumMethod::~BaseNumMethod()
	{

	}

	void BaseNumMethod::Reset()
	{
	}

}
