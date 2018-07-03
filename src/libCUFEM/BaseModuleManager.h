#pragma once
#include <ExportDef.h>
#include <HbxDefMacro.h>


namespace HBXFEMDef
{
	class Engng;
	class InputRecord;

	template<class _T>
	class CUFEM_API BaseModuleManager
	{
	private:
		Engng * _myEngng;
	protected:
	public:
		BaseModuleManager(Engng* _model)
		{
			this->_myEngng = _model;
		};
		virtual ~BaseModuleManager() {};

		//BaseModuleManager(const BaseModuleManager&) = delete;

		//BaseModuleManager& operator = (const BaseModuleManager&) = delete;


		virtual _T* ExportModuleCreator(const char* _name, Engng* _model, int n) = 0;

		virtual InputFileResult_t InitFrom(InputRecord* _dr) = 0;
	};


}