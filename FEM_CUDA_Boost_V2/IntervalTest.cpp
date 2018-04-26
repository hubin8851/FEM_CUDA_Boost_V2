#ifdef USESTDAFX	//ʹ��Ԥ����ͷ�ļ��� 
#include "stdafx.h"
#endif
#include "IntervalTest.h"
#include <helper_cuda.h>
#include <HbxCudaDef\Points.h>
#include <libInterpolate\Quadtree2D.h>
#include <libInterpolate\QuadParam.h>
#include <libInterpolate\cuInterval_kernel.cuh>

extern "C"
void InterpolateTest()
{
	using namespace HBXDef;
	using namespace HBXDef::HbxCuDef;


	HBXFEMDef::BaseReader* g_AeroTableReader = HBXFEMDef::InstanceAeroCoefReader();

	g_AeroTableReader->SetSourceFilePath("CA_da.xml",
		"F:\\data from HBX_phd\\database\\x33��������\\");
	g_AeroTableReader->SetInputData();

	HBXDef::_AEROTABLE* g_Aerotable = g_AeroTableReader->GetInputRecord()->GiveField("ca_da");

	checkCudaErrors(cudaSetDevice(0));

	const unsigned int g_T = 2;//���ά��
	unsigned int g_zhandian = MAX_GPUITER;
	

	thrust::host_vector<HBXDef::UserCalPrec> h_Localx(g_zhandian);
	thrust::host_vector<HBXDef::UserCalPrec> h_Localy(g_zhandian);
	thrust::host_vector<HBXDef::UserCalPrec> h_Localz(g_zhandian);
	thrust::host_vector<HBXDef::cuLocation<g_T>> h_Local(g_zhandian);
	thrust::device_vector<HBXDef::cuLocation<g_T>> d_Local(g_zhandian);
	thrust::host_vector<HBXDef::UserCalPrec> h_OutData(g_zhandian);
	thrust::device_vector<HBXDef::UserCalPrec> d_OutData(g_zhandian);

	HBXDef::cuLocation<g_T>* d_Location;
	checkCudaErrors( cudaMalloc((void**)&d_Location, g_zhandian*sizeof(HBXDef::cuLocation<g_T>)) );

	std::cout << sizeof(h_Local[0]) << std::endl;
	std::cout << sizeof(d_OutData) << std::endl;

//	checkCudaErrors( cudaMalloc(&d_Local, g_zhandian * sizeof(d_OutData)) );

	HBXDef::cuTable<g_T>* g_cuTable;
//	g_cuTable = new HBXDef::cuTable<g_T>(g_Aerotable->_blocks[0]);
	checkCudaErrors( cudaMallocManaged(&g_cuTable, sizeof(HBXDef::cuTable<g_T>)) );//ͳһ��ַ�����ڴ�
	g_cuTable->GetDataFromTable(g_Aerotable->_blocks[0]);//���������ݱ��ڿ����������ֵ����ֵ��
	//��ȡ���ά�ȵ����½�
	UserCalPrec	g_min[g_T], g_max[g_T];
	for (size_t i = 0; i < g_T; i++)
	{
		g_min[i] = g_cuTable->GetMinInDim(i);
		g_max[i] = g_cuTable->GetMaxInDim(i);
	}

	for (unsigned int i = 0; i < g_zhandian; i++)
	{
		//���������
		h_Local[i].Rnd_Generat(g_min, g_max);
	}
	d_Local = h_Local;

	float timeuserd = cuInterpolate<g_T>(	g_cuTable, 
										thrust::raw_pointer_cast(&d_Local[0]), 
										thrust::raw_pointer_cast(&d_OutData[0]) );

}

extern "C"
void QuadTree_Test()
{
	using namespace HBXDef;
	using namespace HBXDef::HbxCuDef;

	const int num_points = 2048;
	const int max_depth = 12;
	const int min_points_per_node = 16;

	//Ϊ��ǰ������ڴ�
	thrust::device_vector<UserCalPrec> x_d0(num_points);
	thrust::device_vector<UserCalPrec> x_d1(num_points);
	thrust::device_vector<UserCalPrec> y_d0(num_points);
	thrust::device_vector<UserCalPrec> y_d1(num_points);


	// ���������
	RandGenerator2D rnd;
	thrust::generate(
		thrust::make_zip_iterator(thrust::make_tuple(x_d0.begin(), y_d0.begin())),
		thrust::make_zip_iterator(thrust::make_tuple(x_d0.end(), y_d0.end())),
		rnd);

	//�����ṹ��
	HBXDef::Points2D points_init[2];
	points_init[0].set(thrust::raw_pointer_cast(&x_d0[0]), thrust::raw_pointer_cast(&y_d0[0]));
	points_init[1].set(thrust::raw_pointer_cast(&x_d1[0]), thrust::raw_pointer_cast(&y_d1[0]));

	// Allocate memory to store points.
	Points2D *points;
	checkCudaErrors(cudaMalloc((void **)&points, 2 * sizeof(Points2D)));
	checkCudaErrors(cudaMemcpy(points, points_init, 2 * sizeof(Points2D), cudaMemcpyHostToDevice));

	int max_nodes = 0;//���ڵ�����Ϊ�������нڵ������
	for (int i = 0, num_nodes_at_level = 1; i < max_depth; ++i, num_nodes_at_level *= 4)
		max_nodes += num_nodes_at_level;

	// �����ڴ�
	Quadtree2D_node root;
	root.SetRange(0, num_points);
	Quadtree2D_node *nodes;
	checkCudaErrors(cudaMalloc((void **)&nodes, max_nodes * sizeof(Quadtree2D_node)));
	checkCudaErrors(cudaMemcpy(nodes, &root, sizeof(Quadtree2D_node), cudaMemcpyHostToDevice));

	//	cuInterpolate( ,);
	//����CDP�ݹ������
	cudaDeviceSetLimit(cudaLimitDevRuntimeSyncDepth, max_depth);

	// �����Ĳ���
	QuadParam params(max_depth, min_points_per_node);
	std::cout << "Launching CDP kernel to build the quadtree" << std::endl;
	const int NUM_THREADS_PER_BLOCK = 128; // Do not use less than 128 threads.
	int warp_size = 32;
	const int NUM_WARPS_PER_BLOCK = NUM_THREADS_PER_BLOCK / warp_size;

	size_t smem_size = 4 * NUM_WARPS_PER_BLOCK * sizeof(int);
}