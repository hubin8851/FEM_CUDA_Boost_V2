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

	CUFEM_EXPORT Engng * HBXFEMDef::InstanceProblem( BaseReader* _dr, problemMode_t _mode, int contextFlag, Engng* _Master, bool parallelFlag )
	{
		Engng* problem;

		std::string SourceFileName = _dr->GetSourceName();
		BaseSlnRecord* ModeRec = _dr->GetSlnRecord();

		problem = classFactory.CreateEngng( ModeRec->keyword.c_str(), _Master, 1);
		if (!problem)
		{
			std::cerr << "����������ʧ��...,��δ�����������" << std::endl;
			return nullptr;
		}


		//�ڴ˿��������ļ�����Ϊ����������ļ������e-mode����Ķ�ȡ�������仯������Ҫ������e-modeʵ����ȫ�̼�¼

		problem->SetProblemMode(_mode);
		problem->SetParrallelMode(parallelFlag);

		if (contextFlag)
		{
			problem->SetContextOutputMode(HBXFEMDef::ContextOutputMode_t::ALWAYS);
		}

		problem->InstanceInit(_dr, "");
		problem->postInit();

		return problem;
	}

}

