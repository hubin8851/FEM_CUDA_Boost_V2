#include <HbxDefMacro.h>
#include <CuDefMacro.h>
#include <libInterpolate\interval_lib.h>
#include <libInterpolate\interval_device.cuh>

//�˺���
//@_TabelIn:	����ı�ṹ��
//@_ArrayIn:	����Ĵ���ֵ�������
//@_outData:	���������
template<unsigned int T>
__global__	void cuInter_kernel(HBXDef::cuTable<T>* _TabelIn,
								HBXDef::cuInterval<T>* _ArrayIn,
								HBXDef::UserCalPrec* _outData)
{
	using namespace HBXDef;
	unsigned int idx = blockIdx.x*blockDim.x + threadIdx.x;
	

	while (idx < MAX_GPUITER)
	{
#ifdef USERDEFSHARED//Ϊ�˿��ƹ����ڴ��С

#else // USERDEFSHARED//Ϊ�˿��ƹ����ڴ��С

#endif  //USERDEFSHARED

		for (size_t i = 0; i < SLICE; i++)
		{
			_outData[idx] = ReadTableData<T>(_TabelIn, &_ArrayIn[idx], &_outData[idx] );
		}

		idx += gridDim.x*blockDim.x;
	}
	__syncthreads();
}