#pragma once

#include <ExportDef.h>
#include <libCUFEM\solverEnum.h>

namespace HBXFEMDef
{
	class BaseReader;
	class Engng;

	//根据问题类型初始化为不同的解算器
	//@dr：读取类基类指针，包括问题的描述
	//@_mode：计算模式或称之为规模
	//@_master：多规模计算的父问题，以便多模块耦合
	CUFEM_EXPORT Engng* InstanceProblem( BaseReader* dr, problemMode_t _mode, int contextFlag, Engng* _Master = nullptr, bool parallelFlag = false );
}