#pragma once
#ifndef INTERVAL_LIB
#define INTERVAL_LIB
#include <cuda_runtime.h>
#include <HbxDefMacro.h>
#include <helper_cuda.h>
#include <HbxXmlDef\AeroTable.h>
#include <thrust\random.h>


namespace HBXDef
{

	//目前仅适用于NVCC编译器编译
	template< unsigned int T >
	class cuTable
	{
	private:
		typedef cuTable<T> _SameTable;
		enum
		{
			VALUE = T
		};
		bool	m_isInit;	//初始化标志位
	//变量
	public:								 
		HBXDef::UserCalPrec*	m_cordials;	//线性化后的每个维度上的插值节点数组
		HBXDef::UserCalPrec*	m_data;
		unsigned int	m_numperdim[T];		//插值坐标维度
		unsigned int	m_lag_cordinate[T];	//插值坐标标位置

	public:
		//CPU、GPU同类型拷贝版本
		__host__ __device__ cuTable(const _SameTable& _rhs)
		{
			unsigned int tmpAdd = 0;
			unsigned int tmpMulti = 1;
			for (unsigned int i = 0; i < this->VALUE; i++)//根据自身表属性获得数据和坐标数组的长度
			{
				tmpAdd += m_numperdim[i];
				tmpMulti *= m_numperdim[i];
			}
			cudaMemcpy(m_numperdim, _rhs.m_numperdim, T * sizeof(unsigned int), cudaMemcpyDeviceToDevice);
			cudaMemcpy(m_lag_cordinate, _rhs.m_lag_cordinate, T * sizeof(unsigned int), cudaMemcpyDeviceToDevice);
			cudaMemcpy(&m_cordials, &_rhs.m_cordials, tmpAdd * sizeof(HBXDef::UserCalPrec), cudaMemcpyDeviceToDevice);
			cudaMemcpy(&m_data, &_rhs.m_data, tmpMulti * sizeof(HBXDef::UserCalPrec), cudaMemcpyDeviceToDevice);
		};

 		__host__ __device__ cuTable(	const unsigned int* _numperdim,
 										const HBXDef::UserCalPrec* _data,
 										const HBXDef::UserCalPrec* _cordials)
 		{
 			unsigned int tmpAdd = 0;
 			unsigned int tmpMulti = 1;
 			for (unsigned int i=0; i<this->VALUE; i++)//根据自身表属性获得数据和坐标数组的长度
 			{
 				tmpAdd += _numperdim[i];
 				tmpMulti *= _numperdim[i];
 			}
 			cudaMemcpy(m_numperdim, _numperdim, T * sizeof(unsigned int), cudaMemcpyDeviceToDevice);
 			cudaMemcpy(&m_cordials, _cordials, sizeof(HBXDef::UserCalPrec)*tmpAdd, cudaMemcpyDeviceToDevice);
 			cudaMemcpy(&m_data, _data, sizeof(HBXDef::UserCalPrec)*tmpMulti, cudaMemcpyDeviceToDevice);
 		};

 		__host__ __device__ cuTable( HBXDef::_AEROBLOCK& _rhs)
 		{
 
 			unsigned int tmpAdd = 0;
 			unsigned int tmpMulti = 1;
 			for (unsigned int i = 0; i < this->VALUE; i++)
 			{
 				tmpAdd += _rhs._numperdim[i];
 				tmpMulti *= _rhs._numperdim[i];
 			}
			checkCudaErrors(cudaMemcpy(m_numperdim, _rhs._numperdim, 
									T * sizeof(unsigned int), 
									cudaMemcpyDeviceToDevice));
 			m_cordials = _rhs._corddata;
 			m_data = _rhs._data;
  		}

 		__host__ void InitMem(unsigned int dataLgth = 1, unsigned int cordLgth = 1)
 		{
 			m_data = new HBXDef::UserCalPrec[dataLgth];
 			m_cordials = new HBXDef::UserCalPrec[cordLgth];
 		}

		__host__ __device__ int size() { return 0; };

	};

	template __declspec(dllexport) class  cuTable<2>;
	template __declspec(dllexport) class  cuTable<3>;


	//存储的位置信息
	template< unsigned int T >
	class cuLocation
	{
	private:
		typedef cuLocation<T> _SameLocation;
// 		enum
// 		{
// 			VALUE = T
// 		};

	public:
		HBXDef::UserCalPrec	m_Loc[T];	//插值坐标标位置
	public:
		//默认构造函数
		__host__ __device__ cuLocation(){};
		//赋值的构造函数
		__host__ __device__ cuLocation(HBXDef::UserCalPrec x[T])
		{
			for (unsigned int i=0; i<T; i++)
			{
				m_Loc[i] = x[i];
			}
		};
		__host__ __device__ cuLocation(const _SameLocation& _rhs) {};

		__host__ __device__ void Initial() {};

		//获取某个点
 		__host__ __device__ __forceinline__ void GetPoint(const HBXDef::UserCalPrec& _x)
 		{
 			
 		}

		//两个参数其中一个为下限，另一个为上限
 		__host__ __device__ __forceinline__ void Rnd_Generat(UserCalPrec _min[T], UserCalPrec _max[T])
 		{
 			int count = 0;
 #ifdef __CUDA_ARCH__
 			unsigned seed = blockIdx.x*blockDim.x + threadIdx.x + count;
 			// thrust::generate may call operator() more than once per thread.
 			// Hence, increment count by grid size to ensure uniqueness of seed
 			count += blockDim.x * gridDim.x;
 #else
 			unsigned seed = 0;
 #endif
 			thrust::default_random_engine rng(seed);

			for (int i=0; i<T; i++)
			{
				thrust::random::uniform_real_distribution<float> distrib(_min[i], _max[i]);
				m_Loc[i] = distrib(rng);
			}
 		}

	};

	template __declspec(dllexport) class  cuLocation<2>;
	template __declspec(dllexport) class  cuLocation<3>;


}


#endif	INTERVAL_LIB