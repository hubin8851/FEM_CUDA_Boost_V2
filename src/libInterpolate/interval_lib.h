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
		bool	m_isInit;	//��ʼ����־λ
	//����
	public:								 
		HBXDef::UserCalPrec*	m_cordials;	//���Ի����ÿ��ά���ϵĲ�ֵ�ڵ�����
		HBXDef::UserCalPrec*	m_data;
		unsigned int	m_numperdim[T];		//��ֵ����ά��
		unsigned int	m_lag_cordinate[T];	//��ֵ�����λ��

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


	//�洢��
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
		HBXDef::UserCalPrec	m_Loc[T];	//��ֵ�����λ��
	public:
		__host__ __device__ cuInterval() {};
		__host__ __device__ cuInterval(const _SameInterval& _rhs) {};
		__host__ __device__ void Initial() {};

	};

	template __declspec(dllexport) class  cuInterval<2>;
	template __declspec(dllexport) class  cuInterval<3>;


}


#endif	INTERVAL_LIB