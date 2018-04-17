#pragma once

#include <HbxDefMacro.h>
#include <HBXDefStruct.h>
#include <HbxCudaDef\AeroTable.h>
#include <HbxCudaDef\cuComponent.cuh>

//���������ռ����Ⱦ
namespace HBXDef
{
#ifndef  TPOW2(_VAL)
#define  TPOW2(_VAL) TPOW<2, _VAL>::VAL
#endif

	template< unsigned int T >
	class CBaseLag
	{
	private:
		typedef CBaseLag<T> _SameInterval;
		enum 
		{
			VALUE = T
		};
		bool	m_isInit;
	protected:
		size_t	m_slct_block_num;	//��ѡ��block
		size_t	m_dim;
		HBXDef::_AEROTABLE* m_table_p;
	public:
		unsigned int	m_numperdim[T];	//ջ�Ϸ���,һ��block��ÿ��ά���ϵ���ֵ�����
		unsigned int	m_lag_cordinate[T];//?
		HBXDef::UserCalPrec	m_lag[T];//?
		//
		HBXDef::UserCalPrec*	m_cordials;	//���Ի����ÿ��ά���ϵĲ�ֵ�ڵ�����
		HBXDef::UserCalPrec*	m_data;
	public:
		__host__ __device__ CBaseLag( HBXDef::_AEROTABLE* _IptTable, size_t _blkId  );
		__host__ __device__ CBaseLag( const _SameInterval& _rhs );
		~CBaseLag();

		//@__blkId:��������block��ID,��0Ϊ��ʼ����
		//@UserStatusError_t:����״̬������CheckUserDefErrors���ж�
		UserStatusError_t SetBlkId( size_t _blkId );

		//��Ҫ�ǰѽṹ��_AeroTable�еĶ�ά����ת��Ϊһά
		bool Initial();

		HBXDef::UserCalPrec	ReadTableData( HBXDef::UserCalPrec * _ArrayIn );

		//Ѱ�Ҵ���ֵ����Χ�ɿռ������ɵĵ�
		void	findcordi(	unsigned int _tmp_position[TPOW2(T)],
							HBXDef::UserCalPrec _tmp_proper[T],
							HBXDef::UserCalPrec _vecIn[T]);

		//���ص�ǰTable����ά��
		__host__ size_t GetDemtion(){ return T; };

	};

}



#include "BaseLag.inl"
// .inl�ļ�