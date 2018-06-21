#include "util.h"
#include <libCUFEM\ContextOutputMode.h>
#include <libCUFEM\EngngMod.h>
#include <libCUFEM\ClassFactory.h>
#include <libCUFEM\BaseReader.h>

namespace HBXFEMDef
{
	class Engng;
	class BaseReader;
	class BaseSlnRecord;

	CUFEM_API Engng * HBXFEMDef::InstanceProblem( BaseReader* _ir, BaseSlnRecord* _sln, problemMode_t _mode, int contextFlag, Engng* _Master, bool parallelFlag )
	{
		Engng* problem;

		std::string SourceFileName = _ir->GetSourceName();
		BaseSlnRecord* ModeSln = _ir->GetSlnRecord()->GiveCopy();

		problem = classFactory.CreateEngng(ModeSln->keyword.c_str(), _Master, 1);
		if (!problem)
		{
			std::cerr << "创建引擎类失败...,或未定义解算类型" << std::endl;
			return nullptr;
		}

		//在此拷贝输入文件。因为传入的输入文件会跟随e-mode组件的读取而发生变化，而需要在整个e-mode实例中全程记录

		problem->SetProblemMode(_mode);
		problem->SetParrallelMode(parallelFlag);

		if (contextFlag)
		{
			problem->SetContextOutputMode(HBXFEMDef::ContextOutputMode_t::ALWAYS);
		}

		problem->InstanceSelf(_ir, _sln, "");
		problem->postInit();

		return problem;
	}

}


