#pragma once
#include <iostream>
#include <HbxDefMacro.h>
#include <ExportDef.h>
#include <libCUFEM\ExportModule.h>
#include <libCUFEM\BaseModuleManager.h>

namespace HBXFEMDef
{
	class Engng;
	class InputRecord;
	class ExportModule;

	//使用了有限向量机的思想
	//是Engng的成员，管理所有的输出模块
	class CUFEM_API ExportModuleManager:public BaseModuleManager<ExportModule>
	{
	public:
		ExportModuleManager( Engng* _master );
		virtual ~ExportModuleManager();

		virtual InputFileResult_t InitFrom(InputRecord* _dr);

		virtual void Init() {};

		ExportModule* ExportModuleCreator(const char* _name, Engng* _eng, int _num);


#pragma region //辅助函数
		virtual const char* GetClassName() const { return typeid(ExportModuleManager).name(); };


#pragma endregion
	};



}