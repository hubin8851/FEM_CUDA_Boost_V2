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
		BaseSection(Domain* _dm, int _id) {};
		~BaseSection();



	};

	template<typename T> BaseSection* SecCreator() { return (new T()); };
	template<typename T> BaseSection* SecCreator(Domain* _dm, int _id) { return (new T(_dm, _id)); };

}