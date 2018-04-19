#pragma once
#include <CudaPreDef.h>
#include <libInterpolate\cuInterpolate.h>
#include <libReader\AeroCoefReader.h>

#include <libInterpolate\interval_lib.h>

void InterpolateTest()
{
	using namespace HBXDef;

	HBXFEMDef::BaseReader* g_AeroTableReader = HBXFEMDef::InstanceAeroCoefReader();

	g_AeroTableReader->SetSourceFilePath("CA_da.xml",
								"F:\\data from HBX_phd\\database\\x33气动数据\\");
	g_AeroTableReader->SetInputData();

	HBXDef::_AEROTABLE* g_Aerotable =  g_AeroTableReader->GetInputRecord()->GiveField("CA_d0");

	const int num_points = 2048;
	const int max_depth = 12;
	const int min_points_per_node = 16;

	//为当前点分配内存
	thrust::device_vector<double>	x_d0(num_points);
	thrust::device_vector<float> x_d1(num_points);
	thrust::device_vector<float> y_d0(num_points);
	thrust::device_vector<float> y_d1(num_points);

	// Generate random points.
	Random_generator rnd;
	thrust::generate(
		thrust::make_zip_iterator(thrust::make_tuple(x_d0.begin(), y_d0.begin())),
		thrust::make_zip_iterator(thrust::make_tuple(x_d0.end(), y_d0.end())),
		rnd);



//	cuInterpolate( ,);


}
