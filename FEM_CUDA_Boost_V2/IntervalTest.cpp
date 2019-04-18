#ifdef USESTDAFX	//ʹ��Ԥ����ͷ�ļ��� 
#include "stdafx.h"
#endif
#include "IntervalTest.h"
#include <helper_cuda.h>
#include <HbxCudaDef\Points.h>
#include <libInterpolate\Quadtree2D.h>
#include <libInterpolate\QuadParam.h>
#include <libInterpolate\cuInterval_kernel.cuh>
#include <HbxCudaDef\multithreading.h>
#include <HbxCudaDef\HbxCuDefStruct.h>

CUTBarrier thread_barrier;
const int N_workloads = 10;

void CUDART_CB myStreamCallback(cudaStream_t event, cudaError_t status, void *data);

extern "C"
CUT_THREADPROC InterpolateTest(void* void_arg)
{
	unsigned int _N = 0;
	if (nullptr == void_arg)
	{
		_N = 200;
	}
	else
	{
		_N = *(unsigned int*)void_arg;
	}

	using namespace HBXDef;
	using namespace HBXDef::HbxCuDef;

	float totalTime = 0.0f;	//��ʱ��
	cudaEvent_t startTotal, stopTotal;
	checkCudaErrors(cudaEventCreate(&startTotal));
	checkCudaErrors(cudaEventCreate(&stopTotal));

	HBXFEMDef::BaseReader* g_AeroTableReader = HBXFEMDef::InstanceAeroCoefReader();

	g_AeroTableReader->SetSourceFilePath("CA_da.xml",
		"F:\\data_from_HBX_phd\\database\\x33��������\\");
	g_AeroTableReader->SetInputData();

	HBXDef::_AEROTABLE* g_Aerotable = g_AeroTableReader->GetInputRecord()->GiveField("ca_da");

	checkCudaErrors(cudaSetDevice(0));

	const unsigned int g_T = 2;//���ά��
	unsigned int g_zhandian = _N;
	
	HBXDef::cuTable<g_T>* g_cuTable;
	//	g_cuTable = new HBXDef::cuTable<g_T>(g_Aerotable->_blocks[0]);
	checkCudaErrors(cudaMallocManaged(&g_cuTable, sizeof(HBXDef::cuTable<g_T>)));//ͳһ��ַ�����ڴ�
	g_cuTable->GetDataFromTable(g_Aerotable->_blocks[0]);//���������ݱ��ڿ����������ֵ����ֵ��
	//��ȡ���ά�ȵ����½�
	UserCalPrec	g_min[g_T], g_max[g_T];
	for (size_t i = 0; i < g_T; i++)
	{
		g_min[i] = g_cuTable->GetMinInDim(i);
		g_max[i] = g_cuTable->GetMaxInDim(i);
	}

for (int i=0; i<MULTIPARAM; i++)
{
	
	thrust::host_vector<HBXDef::cuLocation<g_T>> h_Local(g_zhandian);	//�����������ڴ�
	thrust::device_vector<HBXDef::cuLocation<g_T>> d_Local(g_zhandian);	//�����Դ�ռ�
	thrust::host_vector<HBXDef::UserCalPrec> h_OutData(g_zhandian);
	thrust::host_vector<HBXDef::UserCalPrec> h_CheckData(g_zhandian);
	thrust::device_vector<HBXDef::UserCalPrec> d_OutData(g_zhandian);

	for (unsigned int i = 0; i < g_zhandian; i++)//���������
	{
		//���������
		h_Local[i].Rnd_Generat(g_min, g_max);
	}

//	HBXDef::cuLocation<g_T>* d_Location;
//	checkCudaErrors( cudaMalloc((void**)&d_Location, g_zhandian*sizeof(HBXDef::cuLocation<g_T>)) );

#ifdef _DEBUG
	std::cout << sizeof(h_Local[0]) << std::endl;
	std::cout << sizeof(d_OutData[0]) << std::endl;
#endif // DEBUG

//	checkCudaErrors( cudaMalloc(&d_Local, g_zhandian * sizeof(d_OutData)) );

	checkCudaErrors(cudaEventRecord(startTotal, 0));//��ʼ��ʱ
	d_Local = h_Local;//�ڴ濽����GPU
	//ִ�к˺���
	float timeuserd = cuInterpolate<g_T>(g_cuTable,
										thrust::raw_pointer_cast(&d_Local[0]), 
										thrust::raw_pointer_cast(&d_OutData[0]),
										g_zhandian);
	h_OutData = d_OutData;//���ݻش�

	cudaEventRecord(stopTotal, 0);	//��ʱ����
	cudaThreadSynchronize();	//�߳�ͬ��
	cudaEventSynchronize(startTotal);
	cudaEventSynchronize(stopTotal);
	cudaEventElapsedTime(&totalTime, startTotal, stopTotal);//��ȡ��ʱ
	std::cout << "�ڴ����+����+ִ��+������ʱ��:" << totalTime << std::endl;

	float _timeUsed = Interpolate_cpu(g_cuTable,
		thrust::raw_pointer_cast(&h_Local[0]),
		thrust::raw_pointer_cast(&h_CheckData[0]),
		g_zhandian );

	double errorNorm = 0.f;
	double refNorm = 0.f;//����ֵ����
	for (unsigned int i = 0; i < g_zhandian; i++)
	{
		double diff = h_CheckData[i] - h_OutData[i];
		double ref = h_CheckData[i] + h_OutData[i];
		if (fabs(diff> FLOATERROR) )
		{
			printf("�ڲ�ֵ��%d��ʱ����...\n", i);
		}
		errorNorm += diff * diff;
		refNorm += ref * ref;
	}
	errorNorm = sqrt((double)errorNorm);
	refNorm = sqrt((double)refNorm);

	printf("��ǰվ����%d,���ƻ���ʱ��%f\n", g_zhandian, _timeUsed);
	printf("��ǰ�����׼��%f��ƫ�����%f\n\n", errorNorm, refNorm);

//	checkCudaErrors(cudaFree(d_Location));
//	checkCudaErrors( cudaFree(g_cuTable) );

	g_zhandian *= 2;

//	getchar();

}
	return totalTime;
}


CUT_THREADPROC AsynInterpolateTest(void* void_arg)
{
	using namespace HBXDef;
	using namespace HBXDef::HbxCuDef;

	const unsigned int g_T = 2;//���ά��
	if (nullptr == void_arg)
	{
		CUT_THREADEND;
	}
	Interpolateworkload *workload = (Interpolateworkload *)void_arg;
	//��Ե�ǰ�߳�ѡ����Ӧ��GPU
	checkCudaErrors(cudaSetDevice(workload->cudaDeviceID));
	checkCudaErrors(cudaStreamCreate(&workload->MyStream));

	cuTable<g_T>* _table = (cuTable<g_T>*)(workload->MyTable);
	cuLocation<g_T>* h_Local = (cuLocation<g_T>*)workload->h_data;
	cuLocation<g_T>* d_Local = (cuLocation<g_T>*)workload->d_data;

	checkCudaErrors(cudaMemcpyAsync(d_Local,
					h_Local,
					workload->PNum * sizeof(cuLocation<g_T>),
					cudaMemcpyHostToDevice, workload->MyStream));

	float timeuserd = cuInterpolate<g_T>(_table,
		d_Local,
		workload->d_Outdata,
		workload->PNum);

	checkCudaErrors(cudaMemcpyAsync(workload->h_Outdata, workload->d_Outdata,
					workload->PNum * sizeof(HBXDef::UserCalPrec),
					cudaMemcpyDeviceToHost, workload->MyStream));//���ݻش�

	checkCudaErrors( cudaStreamAddCallback(workload->MyStream, myStreamCallback,workload, 0) );
#ifdef _DEBUG
	printf("��ǰվ����%d,���ƻ���ʱ��%f\n", workload->PNum, timeuserd);
#endif // _DEBUG

//	printf("��ǰ�����׼��%f��ƫ�����%f\n\n", errorNorm, refNorm);
	CUT_THREADEND;
}

CUT_THREADPROC CheckProcess(void *void_arg)
{
	const unsigned int g_T = 2;//���ά��
	Interpolateworkload *workload = (Interpolateworkload *)void_arg;
	// ... GPU is done with processing, continue on new CPU thread...

	// Select GPU for this CPU thread
	checkCudaErrors(cudaSetDevice(workload->cudaDeviceID));

	// У��GPU�������Ƿ���ȷ
	workload->success = true;

	cuTable<g_T>* _table = (cuTable<g_T>*)(workload->MyTable);
	cuLocation<g_T>* h_Local = (cuLocation<g_T>*)workload->h_data;
	thrust::host_vector<HBXDef::UserCalPrec> h_CheckData(workload->PNum);

	float _timeUsed = Interpolate_cpu(_table,
							h_Local,
							thrust::raw_pointer_cast(&h_CheckData[0]),
							workload->PNum);

	double errorNorm = 0.f;
	double refNorm = 0.f;//����ֵ����
	for (unsigned int i = 0; i < workload->PNum; i++)
	{
		double diff = h_CheckData[i] - workload->h_Outdata[i];
		double ref = h_CheckData[i] + workload->h_Outdata[i];
		bool _check = fabs(diff < FLOATERROR);
		workload->success &= _check;
		errorNorm += diff * diff;
		refNorm += ref * ref;
	}
	errorNorm = sqrt((double)errorNorm);
	refNorm = sqrt((double)refNorm);

	printf("��ǰվ����%d,���ƻ���ʱ��%f\n", workload->PNum, _timeUsed);
	printf("��ǰ�����׼��%f��ƫ�����%f\n\n", errorNorm, refNorm);


	// Free Resources
	checkCudaErrors(cudaFree(workload->d_data));
	checkCudaErrors(cudaFree(workload->d_Outdata));
	checkCudaErrors(cudaFreeHost(workload->h_data));
	checkCudaErrors(cudaFreeHost(workload->h_Outdata));
	checkCudaErrors(cudaStreamDestroy(workload->MyStream));

	// Signal the end of the heterogeneous workload to main thread
	//����첽�����Ľ������������߳�
	cutIncrementBarrier(&thread_barrier);

	CUT_THREADEND;
}

void CUDART_CB myStreamCallback(cudaStream_t stream, cudaError_t status, void *data)
{
	// Check status of GPU after stream operations are done
	checkCudaErrors(status);

	// Spawn new CPU worker thread and continue processing on the CPU
	cutStartThread(CheckProcess, data);
}

extern "C"
float MultiInterpolateTest(void* void_arg)
{
	unsigned int _N = 0;
	if (nullptr == void_arg)
	{
		_N = 200;
	}
	else
	{
		_N = *(unsigned int*)void_arg;
	}

	const unsigned int g_T = 2;//���ά��
	unsigned int g_zhandian = _N;

	int N_gpus, max_gpus = 1;
	int gpuInfo[32] = {0, 1, 2 }; // assume a maximum of 32 GPUs in a system configuration

	Interpolateworkload *workloads;
	workloads = (Interpolateworkload *)malloc(N_workloads * sizeof(Interpolateworkload));;
	thread_barrier = cutCreateBarrier(N_workloads);

	//��ʼ��XML��
	HBXFEMDef::BaseReader* g_AeroTableReader = HBXFEMDef::InstanceAeroCoefReader();
	g_AeroTableReader->SetSourceFilePath("CA_da.xml",
		"F:\\data_from_HBX_phd\\database\\x33��������\\");
	g_AeroTableReader->SetInputData();

	//���õ�ǰ��ȡ���������ݱ�
	HBXDef::_AEROTABLE* g_Aerotable = g_AeroTableReader->GetInputRecord()->GiveField("ca_da");
	HBXDef::cuTable<g_T>* g_cuTable;
	checkCudaErrors(cudaMallocManaged(&g_cuTable, sizeof(HBXDef::cuTable<g_T>)));//ͳһ��ַ�����ڴ�
	g_cuTable->GetDataFromTable(g_Aerotable->_blocks[0]);//���������ݱ��ڿ����������ֵ����ֵ��
	//��ȡ���ά�ȵ����½�
	UserCalPrec	g_min[g_T], g_max[g_T];
	for (size_t i = 0; i < g_T; i++)
	{
		g_min[i] = g_cuTable->GetMinInDim(i);
		g_max[i] = g_cuTable->GetMinInDim(i);
	}

	for (int i = 0; i < N_workloads; ++i)
	{
		workloads[i].id = i;
		workloads[i].cudaDeviceID = gpuInfo[i % max_gpus]; // i % N_gpus;
		workloads[i].MyTable = (void*)g_cuTable;
		workloads[i].PNum = _N << i;

		workloads[i].h_data = nullptr;
		workloads[i].d_data = nullptr;
		workloads[i].h_Outdata = nullptr;
		workloads[i].d_Outdata = nullptr;

		//���������������Դ
		cuLocation<g_T>* h_Local;
//		= (cuLocation<g_T>*)(workloads[i]->h_data);
		if (nullptr == workloads[i].h_data)//��Ϊ�գ��ȷ����ڴ�����������
		{
			checkCudaErrors(cudaHostAlloc(&h_Local, (workloads[i].PNum) * sizeof(cuLocation<g_T>), cudaHostAllocPortable));

			for (unsigned int i = 0; i < g_zhandian; i++)//���������
			{
				//���������
				h_Local[i].Rnd_Generat(g_min, g_max);
			}
		}
		workloads[i].h_data = (void*)h_Local;

		if (nullptr == workloads[i].h_Outdata)
		{
			checkCudaErrors(cudaHostAlloc(&workloads[i].h_Outdata,
				(workloads[i].PNum) * sizeof(cuLocation<g_T>),
				cudaHostAllocPortable));
		}

		//�����豸�������Դ
//		cuLocation<g_T>* d_Local;
//		= (cuLocation<g_T>*)(workloads[i]->d_data);
		if (nullptr == workloads[i].d_data)
		{
			checkCudaErrors(cudaMalloc(&workloads[i].d_data, 
				(workloads[i].PNum) * sizeof(cuLocation<g_T>)));//�����Դ�ռ�
		}

		if (nullptr == workloads[i].d_Outdata)
		{
			checkCudaErrors(cudaMalloc(&workloads[i].d_Outdata,
				(workloads[i].PNum) * sizeof(cuLocation<g_T>)));//�����Դ�ռ�
		}
	}

	clock_t start, end;
	double duration;
	start = clock();

	for (int i = 0; i < N_workloads; ++i)
	{
		cutStartThread(AsynInterpolateTest, &workloads[i]);
	}
	cutWaitForBarrier(&thread_barrier);

	end = clock();

	duration = (double)(end - start) / CLOCKS_PER_SEC;

	printf("�첽����ִ����ʱ��%f:\n", duration);


	printf("Total workloads finished:\n");
	getchar();

	return 0;
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