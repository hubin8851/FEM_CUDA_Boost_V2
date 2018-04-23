#ifdef USESTDAFX	//使用预编译头文件宏 
#include "stdafx.h"
#endif
#include "IntervalTest.h"
#include <helper_cuda.h>
#include <HbxCudaDef\Points.h>
#include <libInterpolate\Quadtree2D.h>

extern "C"
void InterpolateTest()
{
	using namespace HBXDef;
	using namespace HBXDef::HbxCuDef;


	HBXFEMDef::BaseReader* g_AeroTableReader = HBXFEMDef::InstanceAeroCoefReader();

	g_AeroTableReader->SetSourceFilePath("CA_da.xml",
		"F:\\data from HBX_phd\\database\\x33气动数据\\");
	g_AeroTableReader->SetInputData();

	HBXDef::_AEROTABLE* g_Aerotable = g_AeroTableReader->GetInputRecord()->GiveField("ca_da");

	const int num_points = 2048;
	const int max_depth = 12;
	const int min_points_per_node = 16;

	//为当前点分配内存
	thrust::device_vector<UserCalPrec> x_d0(num_points);
	thrust::device_vector<UserCalPrec> x_d1(num_points);
	thrust::device_vector<UserCalPrec> y_d0(num_points);
	thrust::device_vector<UserCalPrec> y_d1(num_points);


	// 生成随机点
	CuRandGenerator rnd;
	thrust::generate(
		thrust::make_zip_iterator(thrust::make_tuple(x_d0.begin(), y_d0.begin())),
		thrust::make_zip_iterator(thrust::make_tuple(x_d0.end(), y_d0.end())),
		rnd);

	//主机结构体
	HBXDef::Points2D points_init[2];
	points_init[0].set(thrust::raw_pointer_cast(&x_d0[0]), thrust::raw_pointer_cast(&y_d0[0]));
	points_init[1].set(thrust::raw_pointer_cast(&x_d1[0]), thrust::raw_pointer_cast(&y_d1[0]));

	// Allocate memory to store points.
	Points2D *points;
	checkCudaErrors(cudaMalloc((void **)&points, 2 * sizeof(Points2D)));
	checkCudaErrors(cudaMemcpy(points, points_init, 2 * sizeof(Points2D), cudaMemcpyHostToDevice));

	int max_nodes = 0;//最大节点数，为树的所有节点的总数
	for (int i = 0, num_nodes_at_level = 1; i < max_depth; ++i, num_nodes_at_level *= 4)
		max_nodes += num_nodes_at_level;

	// Allocate memory to store the tree.
	Quadtree2D root;
	root.set_range(0, num_points);
	Quadtree2D *nodes;
	checkCudaErrors(cudaMalloc((void **)&nodes, max_nodes * sizeof(Quadtree2D)));
	checkCudaErrors(cudaMemcpy(nodes, &root, sizeof(Quadtree2D), cudaMemcpyHostToDevice));

	//	cuInterpolate( ,);


}
