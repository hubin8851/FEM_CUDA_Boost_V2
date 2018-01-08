#pragma once

#include <ExportDef.h>
#include <libCUFEM\solverEnum.h>

namespace HBXFEMDef
{
	class BaseReader;
	class Engng;

	//�����������ͳ�ʼ��Ϊ��ͬ�Ľ�����
	//@dr����ȡ�����ָ�룬�������������
	//@_mode������ģʽ���֮Ϊ��ģ
	//@_master�����ģ����ĸ����⣬�Ա��ģ�����
	CUFEM_EXPORT Engng* InstanceProblem( BaseReader* dr, problemMode_t _mode, int contextFlag, Engng* _Master = nullptr, bool parallelFlag = false );
}