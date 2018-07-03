#pragma once
#include <ExportDef.h>
#include <HbxDefMacro.h>
#include <libCUFEM\InputRecord.h>

namespace HBXFEMDef
{
	class Engng;

	//使用boost::log，使得具备输出的多样性
	class CUFEM_API ExportModule
	{
	private:
		//.
		int _id;
		/// Problem pointer.
		Engng* _myModel;

	public:
		ExportModule(Engng* _model, int _id);
		virtual ~ExportModule();

		ExportModule(const ExportModule&) = delete;

		ExportModule& operator = (const ExportModule&) = delete;

		virtual ExportModule* ExportModuleCreator(const char* _name, Engng* _model, int n) { return nullptr; };

		virtual InputFileResult_t InitFrom(InputRecord* _dr) { return InputFileResult_t::IRRT_OK; };
	};

}