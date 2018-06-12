#include <libInterpolate\cuInterpolate.h>
#include <libInterpolate\interval_lib.h>
#include <libInterpolate\cuInterval_kernel.cuh>
#include <libInterpolate\spBaseLag.h>
#include <helper_cuda.h>
#include <time.h>

//#include <thrust\host_vector.h>//���ܵ���NVCC����������
//#include <thrust\transform.h>

//�ú�������ɺ˺����ⲿ�ĳ����Ż������漰�ڴ���������
//T����ʾά��
//M����ʾmalloc�ķ�ʽ
//@_table:��ֵ��
//@_posIn������ֵ������
//@_DataOut:��ֵ������ֵ����
template< unsigned int T>
float cuInterpolate(HBXDef::cuTable<T>* _table,
					HBXDef::cuLocation<T>* _posIn,
					HBXDef::UserCalPrec* _DataOut,
					unsigned int N)
{
	using namespace  HBXDef;
	using namespace  HBXDef::HbxCuDef;

	checkCudaErrors(cudaSetDevice(0));

	float elapsedTime;	//����ʱ��

	//�鿴�ñ�Ĵ�С�Ա����shared memory�Ĵ�С
	if ( _table->size() < 1024*16 )
	{
		//ƫ����L1 32k���档��ΪL1�����ڿ��Է�����ص���ʱ����
		cudaFuncSetCacheConfig(cuInter_kernel<T>, cudaFuncCachePreferL1);
	}
	else if( _table->size()< 1024* 32 && _table->size() > 1024 * 16)
	{
		//32-32���档��ΪL1�����ڿ��Է�����ص���ʱ����
		cudaFuncSetCacheConfig(cuInter_kernel<T>, cudaFuncCachePreferEqual);
	}
	else if (_table->size()< 1024 * 48 && _table->size() > 1024 * 32)
	{
		//16-48���档��ΪL1�����ڿ��Է�����ص���ʱ����
		cudaFuncSetCacheConfig(cuInter_kernel<T>, cudaFuncCachePreferShared);
	}

	//��֤ջ���㹻�Ŀռ��ŷ���������
//	cudaDeviceSetLimit(cudaLimitStackSize, 8192);

	cudaEvent_t start, stop;
	checkCudaErrors(cudaEventCreate(&start));
	checkCudaErrors(cudaEventCreate(&stop));
	checkCudaErrors(cudaEventRecord(start, 0));//��ʼ��ʱ


	//�˺�����important
	cuInter_kernel<T> << < GRIDSIZE, BLOCKSIZE, MAX_SHARED, 0 >> > (_table, _posIn, _DataOut, N);
//	cuInter_TEST<T> << < GRIDSIZE, BLOCKSIZE, MAX_SHARED, 0 >> > (_table, _posIn, _DataOut);

	cudaEventRecord(stop, 0);	//��ʱ����
	cudaThreadSynchronize();	//�߳�ͬ��
	cudaEventSynchronize(start);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsedTime, start, stop);//��ȡ��ʱ

//	std::cout << "�˺���ִ��ʱ��:" << elapsedTime << std::endl;

	return elapsedTime;
};

//CUDA��ֵ����У�����
template< unsigned int T>
float Interpolate_cpu(HBXDef::cuTable<T>* _CuTable,
	HBXDef::cuLocation<T>* h_loc,
	HBXDef::UserCalPrec* h_result,
	unsigned int _num)
{
	//�ڴ˵���sp��CPU���ֵ��


	baselag<T>* thelag = new baselag<T>(_CuTable, 0);

	clock_t start, end;
	double duration;
	start = clock();

	float rsum = 0.0;
	float err = 0.0;
	for (unsigned int i = 0; i < _num; i++)
	{
		float diff = 0;
		h_result[i] = thelag->get(h_loc[i].m_Loc);
	}

	end = clock();

	duration = (double)(end - start) / CLOCKS_PER_SEC;

	std::cout << "���а�ִ��ʱ��:" << duration << std::endl;

	return duration;
}


#ifdef __cplusplus
	extern "C" {
#endif

#ifdef __cplusplus
}
#endif