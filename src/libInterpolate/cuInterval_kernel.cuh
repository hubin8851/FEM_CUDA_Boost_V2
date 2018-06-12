#include <HbxDefMacro.h>
#include <CuDefMacro.h>
#include <libInterpolate\interval_lib.h>
#include <libInterpolate\interval_device.cuh>
#include <HbxXmlDef\AeroTable.h>
#include <HbxCudaDef\SharedMem.cuh>
//�˺���
//@_TabelIn:	����ı�ṹ��
//@_ArrayIn:	����Ĵ���ֵ�������
//@_outData:	���������
//@N:			�����߳�����
template<unsigned int T>
__global__	void cuInter_kernel(HBXDef::cuTable<T>* _TabelIn,
								HBXDef::cuLocation<T>* _ArrayIn,
								HBXDef::UserCalPrec* _outData,
								unsigned int N = MAX_GPUITER)
{
	using namespace HBXDef;
	unsigned int idx = blockIdx.x*blockDim.x + threadIdx.x;


#ifdef USERDEFSHARED//Ϊ�˿��ƹ����ڴ��С
//	if (0 == threadIdx.x)
//	{
//		unsigned int _Lgth = 0, _Multi = 1;
//		SharedMem<HBXDef::UserCalPrec> smem;
//		HBXDef::cuTable<T>* _sTable = smem.getPointer();
//		for (size_t i = 0; i < _TabelIn->GetDim(); i++)
//		{
//			_sTable->m_numperdim[i] = _TabelIn->m_numperdim[i];
//			_Lgth += _TabelIn->m_cordial_demtion[i];
//			_Multi *= _TabelIn->m_cordial_demtion[i];
//		}
//		memcpy(_sTable->m_lag_cordinate, _TabelIn->m_lag_cordinate, sizeof(unsigned int)*T);
//		memcpy(_sTable->m_cordials, _TabelIn->m_cordials, sizeof(HBXDef::UserCalPrec)*_Lgth);
//		memcpy(_sTable->m_data, _TabelIn->m_data, sizeof(HBXDef::UserCalPrec)*_Multi);
//	}
//
//	__syncthreads();
//
//	while (idx < N)
//	{
//#ifdef _DEBUG
//		for (size_t i = 0; i < 5; i++)
//#else
//		for (size_t i = 0; i < SLICE; i++)
//#endif // _DEBUG
//		{
//			_outData[idx] = ReadTableData<T>(_sTable, &_ArrayIn[idx], &_outData[idx]);
//		}
//
//		idx += gridDim.x*blockDim.x;
//}

#else // USERDEFSHARED//Ϊ�˿��ƹ����ڴ��С

	while (idx < N)
	{
#ifdef _DEBUG
		for (size_t i = 0; i < SLICE; i++)
#else
		for (size_t i = 0; i < SLICE; i++)
#endif // _DEBUG
		{
			_outData[idx] = ReadTableData<T>(_TabelIn, &_ArrayIn[idx], &_outData[idx]);
		}

		idx += gridDim.x*blockDim.x;
	}

#endif  //USERDEFSHARED	

	
	__syncthreads();
}


template<unsigned int T>
__global__	void cuInter_TEST(HBXDef::cuTable<T>* _TabelIn,
	HBXDef::cuLocation<T>* _ArrayIn,
	HBXDef::UserCalPrec* _outData
)
{
	using namespace HBXDef;
	unsigned int idx = blockIdx.x*blockDim.x + threadIdx.x;

	printf("the blockID is: %d, the threadId is: %d \n", blockIdx.x, threadIdx.x);

	while (idx < MAX_GPUITER)
	{
#ifdef USERDEFSHARED//Ϊ�˿��ƹ����ڴ��С

#else // USERDEFSHARED//Ϊ�˿��ƹ����ڴ��С

#endif  //USERDEFSHARED

		for (size_t i = 0; i < SLICE; i++)
		{
			//		_outData[idx] = ReadTableData<T>(_TabelIn, &_ArrayIn[idx], &_outData[idx] );
		}

		idx += gridDim.x*blockDim.x;
	}
	__syncthreads();
}


