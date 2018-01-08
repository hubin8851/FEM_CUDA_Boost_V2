#pragma once
#include <boost/dll.hpp>
#include <ExportDef.h>


namespace HBXFEMDef
{
	class Domain;

	class BOOST_SYMBOL_EXPORT BaseSection
	{
	private:
	public:
		BaseSection();
		BaseSection(int _n, Domain* _dm) {};
		~BaseSection();



	};

	template<typename _T> BaseSection* SecCreator() { return new T() };
	template<typename _T> BaseSection* SecCreator(int _n, Domain *_dm) { return new T(_n, _dm) };

}