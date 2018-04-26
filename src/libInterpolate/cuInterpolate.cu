#include <libInterpolate\cuInterpolate.h>
#include <libInterpolate\interval_lib.h>
#include <libInterpolate\cuInterval_kernel.cuh>
#include <helper_cuda.h>

//T：表示维度
//M：表示malloc的方式
//@_table:插值表
//@_posIn：待插值点向量
//@_DataOut:插值所得数值向量
template< unsigned int T>
float cuInterpolate(HBXDef::cuTable<T>* _table,
					HBXDef::cuLocation<T>* _posIn,
					HBXDef::UserCalPrec* _DataOut)
{
	using namespace  HBXDef;
	using namespace  HBXDef::HbxCuDef;

	float elapsedTime;//花费时长


	//查看该表的大小以便控制shared memory的大小
	if ( _table->size() < 1024*16 )
	{
		//偏重于L1 32k缓存。因为L1缓存内可以放置相关的临时变量
//		cudaFuncSetCacheConfig(cuInter_kernel<T>, cudaFuncCachePreferL1);
	}
	else if( _table->size()< 1024* 32 && _table->size() > 1024 * 16)
	{
		//32-32缓存。因为L1缓存内可以放置相关的临时变量
//		cudaFuncSetCacheConfig(cuInter_kernel<T>, cudaFuncCachePreferEqual);
	}
	else if (_table->size()< 1024 * 48 && _table->size() > 1024 * 32)
	{
		//16-48缓存。因为L1缓存内可以放置相关的临时变量
//		cudaFuncSetCacheConfig(cuInter_kernel<T>, cudaFuncCachePreferShared);
	}

	//保证栈有足够的空间存放非内联函数
//	cudaDeviceSetLimit(cudaLimitStackSize, 8192);

	checkCudaErrors(cudaSetDevice(0));

	cudaEvent_t start, stop;
	checkCudaErrors(cudaEventCreate(&start));
	checkCudaErrors(cudaEventCreate(&stop));
	checkCudaErrors(cudaEventRecord(start, 0));//开始计时

	//核函数，important
	cuInter_kernel<T> << < GRIDSIZE, BLOCKSIZE, MAX_SHARED, 0 >> > (_table, _posIn, _DataOut);
//	cuInter_TEST<T> << < GRIDSIZE, BLOCKSIZE, MAX_SHARED, 0 >> > (_table, _posIn, _DataOut);

	cudaEventRecord(stop, 0);	//计时结束
	cudaThreadSynchronize();	//线程同步
	cudaEventSynchronize(start);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsedTime, start, stop);//获取用时

	std::cout << "time eclaps:" << elapsedTime << std::endl;

	return elapsedTime;
};


#ifdef __cplusplus
	extern "C" {
#endif

#ifdef __cplusplus
}
#endif