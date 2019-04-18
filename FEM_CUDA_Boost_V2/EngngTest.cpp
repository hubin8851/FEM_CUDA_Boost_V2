#include "EngngTest.h"
#include <libCUFEM\solverEnum.h>
#include <libCUFEM\util.h>
#include <libEngng\LinearStaticEngng.h>
#include <libReader\PWDataReader.h>
#include <libReader\InpDataReader.h>

extern "C"
void EngngTest(void * void_arg)
{

	HBXFEMDef::BaseReader* __Reader = HBXFEMDef::InstanceInpReader("EltProperty.xml", "F:\\data_from_HBX_phd\\VS2015\\FEM_CUDA_Boost_V2");

	using namespace HBXFEMDef;
	__Reader->SetSourceFilePath("spaceshuttle_quadmain.inp", "C:\\Users\\hbx\\Desktop\\");
	__Reader->SetSourceFilePath("beam.inp", "D:\\AbaqusTemp\\");
	__Reader->SetInputData();

	HBXFEMDef::Engng* em = HBXFEMDef::InstanceProblem(__Reader, nullptr, problemMode_t::_processor, 0, nullptr, true);
	em->Solve();

}
