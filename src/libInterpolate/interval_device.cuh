#include <HbxDefMacro.h>
#include <TPOW.h>


using namespace HBXDef;

//寻找待插值点所围成空间所构成的点
//@_tableIn:待索引的插值表
//@_tmp_position[TPOW2(T)]:围成该点所有点的位置索引
//@_tmp_proper[T]:递归比例系数
//@_vecIn[T]:传入的坐标数据
template<unsigned int T>
__host__  __device__ void	findcordi(HBXDef::cuTable<T>* _tableIn,
	unsigned int _tmp_position[HBXDef::TPOW2(T)],
	HBXDef::UserCalPrec _tmp_proper[T],
	HBXDef::UserCalPrec _vecIn[T])
{
	{
		unsigned int tempmul = 1;		//每个维度插值区间上点个数的乘积
		unsigned int temppow = 1;		//用于控制迭代过程中vec每个维度的偏移量
		unsigned int _tmpoffset = 0;	//各维度起始坐标在一维数组中的索引
		unsigned int		_length = 0;
		HBXDef::UserCalPrec*	_cordials = nullptr;
		unsigned int		_cordial = 0;

		for (unsigned int idx1 = 0; idx1 < T; idx1++)
		{
			_length = _tableIn->m_numperdim[idx1];	//寻找当前维度下每个维度的节点数目

			_cordials = &_tableIn->m_cordials[_tmpoffset];
			_cordial = _tableIn->m_lag_cordinate[idx1];

			//判断当前插值点在上一步的左方亦或右方
			if (_vecIn[idx1] > _cordials[_cordial])
			{
				for (int idx2 = _cordial; idx2 < _length; idx2++)
				{
					if ((_length - 1) == idx2)	//判断是否在右边界
					{
						_tmp_proper[idx1] = 0;	//递归因子置0
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
			{//左方
				for (int idx2 = _cordial; idx2 >= 0; idx2--)
				{
					if (idx2 == 0)//若该插值点靠近左端边界
					{
						_tmp_proper[idx1] = 1;				//递归因数
						_tableIn->m_lag_cordinate[idx1] = 1;
						break;
					}
					else
					{
						if (_vecIn[idx1] > _cordials[idx2 - 1])	//若在_cordials[idx2-1]和_cordials[idx2]之间
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


//该函数置于CUDA环境内的global函数内调用
//@_ArrayIn:传入的带插值的坐标向量
template<unsigned int T>
__host__ __device__ HBXDef::UserCalPrec	ReadTableData(	HBXDef::cuTable<T>* _tableIn,
												HBXDef::cuLocation<T>* _ArrayIn,
												HBXDef::UserCalPrec* _Output)
{
	unsigned int _tmp_position[HBXDef::TPOW2(T)];	//递归临时数组
	HBXDef::UserCalPrec	_tmp_reducdata[HBXDef::TPOW2(T)];	//递归数据临时数组
	HBXDef::UserCalPrec	_tmp_proper[T];	//递归比例系数

	memset((void*)&_tmp_position, 0, HBXDef::TPOW2(T) * sizeof(unsigned int));

	findcordi(_tableIn, _tmp_position, _tmp_proper, _ArrayIn->m_Loc);
	//迭代递归算法
	unsigned int _tmpidx = HBXDef::TPOW2(T) / 2;
	for (unsigned int idx = 0; idx < HBXDef::TPOW2(T); idx++)//在临时数组中
	{
		_tmp_reducdata[idx] = _tableIn->m_data[ _tmp_position[idx] ];
	}
	for (unsigned int idx = T - 1; idx > 0; idx--)
	{
		for (int idx2 = 0; idx2 < _tmpidx; idx2++)
		{//循环内表达式简而言之化为b（i） = a（i+offset）+coef*(a(i+offset)-a(i)),两点插值
			_tmp_reducdata[idx2] = _tmp_reducdata[idx2 + _tmpidx] - _tmp_proper[idx] * (_tmp_reducdata[idx2 + _tmpidx] - _tmp_reducdata[idx2]);
		}
		_tmpidx = _tmpidx >> 1;//通过左移操作对另一维度的数据进行插值
	}//在此没有将最后一步放入循环中，便于未优化情况下提高速度
	return _tmp_reducdata[1] - _tmp_proper[0] * (_tmp_reducdata[1] - _tmp_reducdata[0]);
};


//template __declspec(dllexport) __host__ __device__ HBXDef::UserCalPrec	ReadTableData(HBXDef::cuTable<2>* _tableIn,
//	HBXDef::cuInterval<2>* _ArrayIn,
//	HBXDef::UserCalPrec* _Output);
//
//template __declspec(dllexport) __host__ __device__ HBXDef::UserCalPrec	ReadTableData(HBXDef::cuTable<3>* _tableIn,
//	HBXDef::cuInterval<3>* _ArrayIn,
//	HBXDef::UserCalPrec* _Output);



