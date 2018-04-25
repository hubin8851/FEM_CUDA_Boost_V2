#include <HbxDefMacro.h>
#include <TPOW.h>


using namespace HBXDef;

//Ѱ�Ҵ���ֵ����Χ�ɿռ������ɵĵ�
//@_tableIn:�������Ĳ�ֵ��
//@_tmp_position[TPOW2(T)]:Χ�ɸõ����е��λ������
//@_tmp_proper[T]:�ݹ����ϵ��
//@_vecIn[T]:�������������
template<unsigned int T>
__host__  __device__ void	findcordi(HBXDef::cuTable<T>* _tableIn,
	unsigned int _tmp_position[HBXDef::TPOW2(T)],
	HBXDef::UserCalPrec _tmp_proper[T],
	HBXDef::UserCalPrec _vecIn[T])
{
	{
		unsigned int tempmul = 1;		//ÿ��ά�Ȳ�ֵ�����ϵ�����ĳ˻�
		unsigned int temppow = 1;		//���ڿ��Ƶ���������vecÿ��ά�ȵ�ƫ����
		unsigned int _tmpoffset = 0;	//��ά����ʼ������һά�����е�����
		unsigned int		_length = 0;
		HBXDef::UserCalPrec*	_cordials = nullptr;
		unsigned int		_cordial = 0;

		for (unsigned int idx1 = 0; idx1 < T; idx1++)
		{
			_length = _tableIn->m_numperdim[idx1];	//Ѱ�ҵ�ǰά����ÿ��ά�ȵĽڵ���Ŀ

			_cordials = &_tableIn->m_cordials[_tmpoffset];
			_cordial = _tableIn->m_lag_cordinate[idx1];

			//�жϵ�ǰ��ֵ������һ����������ҷ�
			if (_vecIn[idx1] > _cordials[_cordial])
			{
				for (int idx2 = _cordial; idx2 < _length; idx2++)
				{
					if ((_length - 1) == idx2)	//�ж��Ƿ����ұ߽�
					{
						_tmp_proper[idx1] = 0;	//�ݹ�������0
						_tableIn->m_lag_cordinate[idx1] = _length - 1;
					}
					else
					{
						if (_vecIn[idx1] < _cordials[idx2 + 1])
						{
							_tableIn->m_lag_cordinate[idx1] = idx2 + 1;
							_tmp_proper[idx1] = (_cordials[idx2 + 1] - _vecIn[idx1]) / (_cordials[idx2 + 1] - _cordials[idx2]);
							break;
						}
					}
				}
			}
			else
			{//��
				for (int idx2 = _cordial; idx2 >= 0; idx2--)
				{
					if (idx2 == 0)//���ò�ֵ�㿿����˱߽�
					{
						_tmp_proper[idx1] = 1;				//�ݹ�����
						_tableIn->m_lag_cordinate[idx1] = 1;
						break;
					}
					else
					{
						if (_vecIn[idx1] > _cordials[idx2 - 1])	//����_cordials[idx2-1]��_cordials[idx2]֮��
						{
							_tableIn->m_lag_cordinate[idx1] = idx2;
							_tmp_proper[idx1] = (_cordials[idx2] - _vecIn[idx1]) / (_cordials[idx2] - _cordials[idx2 - 1]);
							break;
						}
					}
				}
			}
			for (int idx2 = 0; idx2 < HBXDef::TPOW2(T); idx2++)
			{
				if (0 != (idx2 / temppow) % 2)
					_tmp_position[idx2] += _tableIn->m_lag_cordinate[idx1] * tempmul;
				else
					_tmp_position[idx2] += (_tableIn->m_lag_cordinate[idx1] - 1) * tempmul;
			}
			tempmul *= _length;
			_tmpoffset += _length;
			temppow = temppow << 1;
		}
	}
}


template __declspec(dllexport)  __host__  __device__ void	findcordi(HBXDef::cuTable<2>* _tableIn,
	unsigned int _tmp_position[HBXDef::TPOW2(2)],
	HBXDef::UserCalPrec _tmp_proper[2],
	HBXDef::UserCalPrec _vecIn[2]);

template __declspec(dllexport)  __host__  __device__ void	findcordi(HBXDef::cuTable<3>* _tableIn,
	unsigned int _tmp_position[HBXDef::TPOW2(3)],
	HBXDef::UserCalPrec _tmp_proper[3],
	HBXDef::UserCalPrec _vecIn[3]);


//�ú�������CUDA�����ڵ�global�����ڵ���
//@_ArrayIn:����Ĵ���ֵ����������
template<unsigned int T>
__host__ __device__ HBXDef::UserCalPrec	ReadTableData(	HBXDef::cuTable<T>* _tableIn,
												HBXDef::cuLocation<T>* _ArrayIn,
												HBXDef::UserCalPrec* _Output)
{
	unsigned int _tmp_position[HBXDef::TPOW2(T)];	//�ݹ���ʱ����
	HBXDef::UserCalPrec	_tmp_reducdata[HBXDef::TPOW2(T)];	//�ݹ�������ʱ����
	HBXDef::UserCalPrec	_tmp_proper[T];	//�ݹ����ϵ��

	memset((void*)&_tmp_position, 0, HBXDef::TPOW2(T) * sizeof(unsigned int));

	findcordi(_tableIn, _tmp_position, _tmp_proper, _ArrayIn->m_Loc);
	//�����ݹ��㷨
	unsigned int _tmpidx = HBXDef::TPOW2(T) / 2;
	for (unsigned int idx = 0; idx < HBXDef::TPOW2(T); idx++)//����ʱ������
	{
		_tmp_reducdata[idx] = _tableIn->m_data[ _tmp_position[idx] ];
	}
	for (unsigned int idx = T - 1; idx > 0; idx--)
	{
		for (int idx2 = 0; idx2 < _tmpidx; idx2++)
		{//ѭ���ڱ��ʽ�����֮��Ϊb��i�� = a��i+offset��+coef*(a(i+offset)-a(i)),�����ֵ
			_tmp_reducdata[idx2] = _tmp_reducdata[idx2 + _tmpidx] - _tmp_proper[idx] * (_tmp_reducdata[idx2 + _tmpidx] - _tmp_reducdata[idx2]);
		}
		_tmpidx = _tmpidx >> 1;//ͨ�����Ʋ�������һά�ȵ����ݽ��в�ֵ
	}//�ڴ�û�н����һ������ѭ���У�����δ�Ż����������ٶ�
	return _tmp_reducdata[1] - _tmp_proper[0] * (_tmp_reducdata[1] - _tmp_reducdata[0]);
};


//template __declspec(dllexport) __host__ __device__ HBXDef::UserCalPrec	ReadTableData(HBXDef::cuTable<2>* _tableIn,
//	HBXDef::cuInterval<2>* _ArrayIn,
//	HBXDef::UserCalPrec* _Output);
//
//template __declspec(dllexport) __host__ __device__ HBXDef::UserCalPrec	ReadTableData(HBXDef::cuTable<3>* _tableIn,
//	HBXDef::cuInterval<3>* _ArrayIn,
//	HBXDef::UserCalPrec* _Output);



