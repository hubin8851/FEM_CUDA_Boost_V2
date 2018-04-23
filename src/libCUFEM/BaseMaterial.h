#pragma once
#include <ExportDef.h>

namespace HBXFEMDef
{
	class Domain;

	class CUFEM_API BaseMaterial
	{
	private:
	protected:
	public:
		BaseMaterial(Domain* _dm, int _id) {};

	};



	template<typename T> BaseMaterial* MaterialCreator() { return new T(); };
	template<typename T> BaseMaterial* MaterialCreator(Domain *_dm, int _id) { return new T(_dm, _id); };


}
