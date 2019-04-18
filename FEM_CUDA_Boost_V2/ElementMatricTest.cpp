#include "stdafx.h"
#include <..\src\libReader\UFgetMatReader.h>
#include <..\src\libReader\InpDataReader.h>

#include "ElementMatricTest.h"

void ElementMatricCalTest(void * void_arg)
{
	using namespace HBXFEMDef;
	InpDataReader g_InpDataReader("B31BEAMTEST.inp", "F:\\data_from_HBX_phd\\database\\B31Test\\");
	g_InpDataReader.SetEltPropFilePath("EltProperty.xml", "F:\\data_from_HBX_phd\\vs2015\\FEM_CUDA_Boost_V2\\");
	g_InpDataReader.SetInputData();
	InputRecord* g_Record = g_InpDataReader.GetInputRecord();


}
