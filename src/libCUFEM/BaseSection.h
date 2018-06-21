#pragma once
//#include <boost/dll.hpp>
#include <ExportDef.h>
#include <HbxDefMacro.h>
#include <libCUFEM\InputRecord.h>

namespace HBXFEMDef
{
	class Domain;

	class CUFEM_API BaseSection
	{
	private:
	public:
		BaseSection();
		BaseSection(Domain* _dm, int _id) {};
		~BaseSection();

		virtual InputFileResult_t InitFrom( InputRecord* _dr );

	};

	template<typename T> BaseSection* SecCreator() { return (new T()); };
	template<typename T> BaseSection* SecCreator(Domain* _dm, int _id) { return (new T(_dm, _id)); };

}