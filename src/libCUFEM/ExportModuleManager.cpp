#include "ExportModule.h"
#include <libCUFEM\ExportModuleManager.h>
#include <libCUFEM\ClassFactory.h>

namespace HBXFEMDef
{

	ExportModuleManager::ExportModuleManager(Engng * _master):BaseModuleManager<ExportModule>(_master)
	{

	}

	ExportModuleManager::~ExportModuleManager()
	{
	}

	InputFileResult_t ExportModuleManager::InitFrom(InputRecord * _dr)
	{
		return InputFileResult_t();
	}

	ExportModule * ExportModuleManager::ExportModuleCreator(const char * _name, Engng * _eng, int _num)
	{
		return classFactory.CreateExportModule(_name, _eng, _num);
	}


}

