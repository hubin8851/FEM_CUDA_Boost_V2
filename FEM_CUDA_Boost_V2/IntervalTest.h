#pragma once
#include <libInterpolate\cuInterpolate.h>
#include <libReader\AeroCoefReader.h>
void InterpolateTest()
{
	using namespace HBXDef;

	HBXFEMDef::BaseReader* g_AeroTable = HBXFEMDef::InstanceAeroCoefReader();

	g_AeroTable->SetSourceFilePath("CA_d0.xml", 
								"F:\\data from HBX_phd\\database\\x33气动数据\\");
	g_AeroTable->SetInputData();
}
