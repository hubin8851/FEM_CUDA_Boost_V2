#pragma once
#ifndef INTERVAL_LIB
#define INTERVAL_LIB



namespace HBXDef
{
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
		__host__ __device__ cuTable(const _SameTable& _rhs) 
		{
			cudaMemcpy(m_numperdim, _rhs.m_numperdim, T * sizeof(unsigned int), cudaMemcpyHostToDevice);
			cudaMemcpy(m_lag_cordinate, _rhs.m_lag_cordinate, T * sizeof(unsigned int), cudaMemcpyHostToDevice);
			cudaMemcpy(&m_cordials, &_rhs.m_cordials, sizeof(void*), cudaMemcpyDeviceToDevice);
			cudaMemcpy(&m_data, &_rhs.m_data, sizeof(void*), cudaMemcpyDeviceToDevice);
		};
		__host__ __device__ int size() { return 0; };

	};

	template __declspec(dllexport) class  cuTable<2>;
	template __declspec(dllexport) class  cuTable<3>;


	//存储的
	template< unsigned int T >
	class cuInterval
	{
	private:
		typedef cuInterval<T> _SameInterval;
		enum
		{
			VALUE = T
		};

	public:
		HBXDef::UserCalPrec	m_Loc[T];	//插值坐标标位置
	public:
		__host__ __device__ cuInterval() {};
		__host__ __device__ cuInterval(const _SameInterval& _rhs) {};
		__host__ __device__ void Initial() {};

	};

	template __declspec(dllexport) class  cuInterval<2>;
	template __declspec(dllexport) class  cuInterval<3>;


}


#endif	INTERVAL_LIB