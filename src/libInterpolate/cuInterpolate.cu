#include <libInterpolate\cuInterpolate.h>
#include <libInterpolate\interval_lib.h>
#include <libInterpolate\cuInterval_kernel.cuh>


//@T：表示维度
//@M：表示malloc的方式
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
		//偏重于L1缓存。因为L1缓存内可以放置相关的临时变量
//		cudaFuncSetCacheConfig(cuInter_kernel<T>, cudaFuncCachePreferL1);
	}
	else if( _table->size()< 1024* 32 && _table->size() > 1024 * 16)
	{
		//偏重于L1缓存。因为L1缓存内可以放置相关的临时变量
//		cudaFuncSetCacheConfig(cuInter_kernel<T>, cudaFuncCachePreferEqual);
	}
	else if (_table->size()< 1024 * 48 && _table->size() > 1024 * 32)
	{
		//偏重于L1缓存。因为L1缓存内可以放置相关的临时变量
//		cudaFuncSetCacheConfig(cuInter_kernel<T>, cudaFuncCachePrefershared);
	}

	//保证栈有足够的空间存放非内联函数
	cudaDeviceSetLimit(cudaLimitStackSize, 8192);

	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start, 0);//开始计时

	//核函数，important
	cuInter_kernel<T> << < GRIDSIZE, BLOCKSIZE, MAX_SHARED, 0 >> > (_table, _posIn, _DataOut);

	cudaEventRecord(stop, 0);	//计时结束
	cudaThreadSynchronize();	//线程同步
	cudaEventSynchronize(start);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsedTime, start, stop);//获取用时

//	std::cout << "time eclaps:" << elapsedTime << std::endl;

	return elapsedTime;
};


#ifdef __cplusplus
	extern "C" {
#endif

#ifdef __cplusplus
}
#endif