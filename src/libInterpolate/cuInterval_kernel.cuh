#include <HbxDefMacro.h>
#include <CuDefMacro.h>
#include <libInterpolate\interval_lib.h>
#include <libInterpolate\interval_device.cuh>

//核函数
//@_TabelIn:	传入的表结构体
//@_ArrayIn:	传入的待插值点的坐标
//@_outData:	输出的数据
template<unsigned int T>
__global__	void cuInter_kernel(HBXDef::cuTable<T>* _TabelIn,
								HBXDef::cuInterval<T>* _ArrayIn,
								HBXDef::UserCalPrec* _outData)
{
	using namespace HBXDef;
	unsigned int idx = blockIdx.x*blockDim.x + threadIdx.x;
	

	while (idx < MAX_GPUITER)
	{
#ifdef USERDEFSHARED//为了控制共享内存大小

#else // USERDEFSHARED//为了控制共享内存大小

#endif  //USERDEFSHARED

		for (size_t i = 0; i < SLICE; i++)
		{
			_outData[idx] = ReadTableData<T>(_TabelIn, &_ArrayIn[idx], &_outData[idx] );
		}

		idx += gridDim.x*blockDim.x;
	}
	__syncthreads();
}