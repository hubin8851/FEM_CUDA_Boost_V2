#include <libInterpolate\cuInterpolate.h>
#include <libInterpolate\interval_lib.h>
#include <libInterpolate\cuInterval_kernel.cuh>


//@T����ʾά��
//@M����ʾmalloc�ķ�ʽ
template< unsigned int T>
float cuInterpolate(HBXDef::cuTable<T>* _table,
					HBXDef::cuLocation<T>* _posIn,
					HBXDef::UserCalPrec* _DataOut)
{
	using namespace  HBXDef;
	using namespace  HBXDef::HbxCuDef;

	float elapsedTime;//����ʱ��

	//�鿴�ñ�Ĵ�С�Ա����shared memory�Ĵ�С
	if ( _table->size() < 1024*16 )
	{
		//ƫ����L1���档��ΪL1�����ڿ��Է�����ص���ʱ����
//		cudaFuncSetCacheConfig(cuInter_kernel<T>, cudaFuncCachePreferL1);
	}
	else if( _table->size()< 1024* 32 && _table->size() > 1024 * 16)
	{
		//ƫ����L1���档��ΪL1�����ڿ��Է�����ص���ʱ����
//		cudaFuncSetCacheConfig(cuInter_kernel<T>, cudaFuncCachePreferEqual);
	}
	else if (_table->size()< 1024 * 48 && _table->size() > 1024 * 32)
	{
		//ƫ����L1���档��ΪL1�����ڿ��Է�����ص���ʱ����
//		cudaFuncSetCacheConfig(cuInter_kernel<T>, cudaFuncCachePrefershared);
	}

	//��֤ջ���㹻�Ŀռ��ŷ���������
	cudaDeviceSetLimit(cudaLimitStackSize, 8192);

	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start, 0);//��ʼ��ʱ

	//�˺�����important
	cuInter_kernel<T> << < GRIDSIZE, BLOCKSIZE, MAX_SHARED, 0 >> > (_table, _posIn, _DataOut);

	cudaEventRecord(stop, 0);	//��ʱ����
	cudaThreadSynchronize();	//�߳�ͬ��
	cudaEventSynchronize(start);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsedTime, start, stop);//��ȡ��ʱ

//	std::cout << "time eclaps:" << elapsedTime << std::endl;

	return elapsedTime;
};


#ifdef __cplusplus
	extern "C" {
#endif

#ifdef __cplusplus
}
#endif