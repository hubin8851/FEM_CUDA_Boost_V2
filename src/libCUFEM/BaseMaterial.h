#pragma once
#include <string.h>
#include <ExportDef.h>
#include <HbxDefMacro.h>
#include <libCUFEM\InputRecord.h>

namespace HBXFEMDef
{
	class Domain;

	//该类主要实现材料相关的算法
	class CUFEM_API BaseMaterial
	{
	private:
	protected:
	public:
		std::string _myName;


		BaseMaterial(Domain* _dm, int _id) {};

		virtual ~BaseMaterial() {};

		virtual InputFileResult_t InitFrom(InputRecord * _dr);
	};



	template<typename T> BaseMaterial* MaterialCreator() { return new T(); };
	template<typename T> BaseMaterial* MaterialCreator(Domain *_dm, int _id) { return new T(_dm, _id); };


}
