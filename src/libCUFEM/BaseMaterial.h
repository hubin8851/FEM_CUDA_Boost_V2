#pragma once
#include <ExportDef.h>

namespace HBXFEMDef
{
	class Domain;

	class BOOST_SYMBOL_EXPORT BaseMaterial
	{
	private:
	protected:
	public:
		BaseMaterial(int _n, Domain* _dm) {};

	};



	template<typename _T> BaseMaterial* MaterialCreator() { return new T() };
	template<typename _T> BaseMaterial* MaterialCreator(int _n, Domain *_dm) { return new T(_n, _dm) };


}
