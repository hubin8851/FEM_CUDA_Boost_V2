#include "stdafx.h"
#include "NumericalCalTest.h"
#include <..\src\libReader\UFgetMatReader.h>
#include <..\src\NumericalMehod\BaseConjugate.h>
#include <..\src\NumericalMehod\BiCGStab.h>
#include <..\src\NumericalMehod\PcgConjugate.h>
#include <..\src\NumericalMehod\CgConjugate.h>

void NumericalCalTest(void * void_arg)
{
	int		g_Iters;
	float	g_Precision = 1.0e-6f;

	using namespace HBXFEMDef;
	UFgetMatReader  g_UFgetMatReader("RM07R.mat", "G:\\SuiteSparse-2.1.1\\UFget\\mat\\Fluorem");
	InputFileResult _res = g_UFgetMatReader.SetInputData();
	HBXDef::_CSRInput<HBXDef::UserCalPrec>* _tmpCSR = g_UFgetMatReader.GetStiffMat();
	g_Iters = (_tmpCSR->_nA) * 3;
	std::cout << "读取部分结束" << std::endl;

	if (0)
	{
		BiCGStab g_BiCGStab(nullptr, nullptr);
			g_BiCGStab.ResetMem(_tmpCSR->_nnzA, _tmpCSR->_nA);
			g_BiCGStab.SetStiffMat(	_tmpCSR->h_NoneZeroVal, _tmpCSR->h_iColSort, _tmpCSR->h_iNonZeroRowSort,
									_tmpCSR->_nnzA, _tmpCSR->_nA);
//		g_BiCGStab.SetStiffMatFromMTX("gr_900_900_crg.mtx", "F:\\data_from_HBX_phd\\vs2010\\FEMCal_CUDA_MPI_Boost\\data\\check\\");

		g_BiCGStab.SetLoadVec(_tmpCSR->h_rhs, _tmpCSR->_nA - 1);
		g_BiCGStab.ResetGraphMem();
		g_BiCGStab.MemCpy(HBXDef::HostToDevice);
		if (true == g_BiCGStab.InitialDescr())
		{
			std::cout << "\n初始化CG法的描述器及句柄成功！\n" << std::endl;
		}
		double _calTime = g_BiCGStab.ConjugateWithGPU(g_Precision, g_Iters);
	}
	
	if (1)
	{
		PcgConjugate g_PcgConjugate(nullptr, nullptr);
		g_PcgConjugate.ResetMem(_tmpCSR->_nnzA, _tmpCSR->_nA);
		g_PcgConjugate.SetStiffMat(	_tmpCSR->h_NoneZeroVal, _tmpCSR->h_iColSort, _tmpCSR->h_iNonZeroRowSort,
									_tmpCSR->_nnzA, _tmpCSR->_nA);
		g_PcgConjugate.SetLoadVec(_tmpCSR->h_rhs, _tmpCSR->_nA - 1);
		g_PcgConjugate.ResetGraphMem();
		g_PcgConjugate.MemCpy(HBXDef::HostToDevice);
		if (true == g_PcgConjugate.InitialDescr())
		{
			std::cout << "\n初始化PCG法的描述器及句柄成功！\n" << std::endl;
		}
		double _calTime = g_PcgConjugate.ConjugateWithGPU(g_Precision, g_Iters);
	}
	if (1)
	{
		CgConjugate g_CgConjugate(nullptr, nullptr);
		g_CgConjugate.ResetMem(_tmpCSR->_nnzA, _tmpCSR->_nA);
		g_CgConjugate.SetStiffMat(_tmpCSR->h_NoneZeroVal, _tmpCSR->h_iColSort, _tmpCSR->h_iNonZeroRowSort,
			_tmpCSR->_nnzA, _tmpCSR->_nA);
		g_CgConjugate.SetLoadVec(_tmpCSR->h_rhs, _tmpCSR->_nA - 1);
		g_CgConjugate.ResetGraphMem();
		g_CgConjugate.MemCpy(HBXDef::HostToDevice);
		if (true == g_CgConjugate.InitialDescr())
		{
			std::cout << "\n初始化CG法的描述器及句柄成功！\n" << std::endl;
		}
		double _calTime = g_CgConjugate.ConjugateWithGPU(g_Precision, g_Iters);
	}
}
