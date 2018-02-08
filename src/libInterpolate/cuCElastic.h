#ifndef _CUCELASTIC_H
#define _CUCELASTIC_H

#include <cublas.h>
#include <HbxDefMacro.h>
#include <HBXDefStruct.h>
#include "HbxCudaDef\AeroTable.h"
#include "..\libInterpolate\CElastic_common.h"

namespace HBXDef
{
	namespace HbxCuDef
	{

	}

}


template<unsigned int _T = 2, HBXDef::HbxCuDef::CudaMalloc_t _M = HBXDef::HbxCuDef::CudaMalloc_t::NORMAL>
class cuCElastic
{
private:
	typedef cuCElastic<_T,_M>	_sameCElastic;

	enum VALUE
	{
		DIM = _T
	};
	enum MALLOC
	{
		TYPE = _M
	};
	//弹性模块相关参数
	HBXDef::UserCalPrec*	d_Cna;		//所有站点气动力系数
	HBXDef::UserCalPrec*	d_Alpha;	//当地攻角
	HBXDef::UserCalPrec*	d_Aero_Ni;	//截面临时变量向量

	HBXDef::d_TrajInput		d_TrajInput_;
	HBXDef::d_MonitorInput	d_Monitor_;
	HBXDef::d_ShakeInput	d_ShakeInput_;
	HBXDef::d_MassInput		d_MassInput_;
	HBXDef::d_OriginData<hbxFloat>	d_ModeData_;	//当前模块所需的参数

	//待插值的所有点的集合
	typedef  HBXDef::UserDefFloat _point[_T];
	_point	*m_Interoplate;	//所有的待插值
	HBXDef::OptionPlan*	_plan;	//当前类所被调用的任务
private:
	cudaArray*	d_cordialsArray;
protected:
	size_t	m_slct_block_num;	//所选的block
	size_t	m_dim;	//插值表维度
	HBXDef::_AeroTable* m_table_p;	//插值表指针

	HBXDef::UserCalPrec*	d_cordials;	//线性化后的每个维度上的插值节点数组
	HBXDef::UserCalPrec*	d_data;

	unsigned int	m_cordlgth;
	unsigned int	m_datalgth;

public:
	unsigned int			m_numperdim[_T];	//栈上分配,一个block内每个维度上的数值点个数
	unsigned int			m_lag_cordinate[_T];	//插值坐标标位置
	HBXDef::UserCalPrec		m_lag[_T];
protected:
	void GenerateData();
public:
	cuCElastic();
	~cuCElastic();

	//获取表号和块号的构造函数
	//@_IptTable:表结构体指针
	//@_blkId:块号
	cuCElastic( HBXDef::_AeroTable* _IptTable, size_t _blkId );
	//深拷贝构造函数
	cuCElastic( const _sameCElastic& _rhs );

	//@__blkId:待索引的block的ID,以0为起始索引
	//@UserStatusError_t:返回状态，可用CheckUserDefErrors宏判断
	UserStatusError_t SetBlkId(size_t _blkId);

	//初始化函数完成显存的分配
	void Initial( void* _DataIn = nullptr, cudaStream_t* _stream = 0);

	//获取每一计算步所用数据
	void getdata( void* _dataStep, cudaStream_t* _stream );
	//该函数获取所有节点（包括各站点）在各阶模态插值所得参数，函数内部分三个方向分别计算，便于使用流。20170626
	void GetInterpolatePointParam();

	//考虑弹性变形之后的气动力计算
	void Elastic_Aero_Force( cublasHandle_t handle );


	//广义力计算
	void Gen_Force_Cal();

	//GPU计算接口,空缺
	void CalculateGPU(cudaStream_t _stream);

	//清除类内相关显存和句柄
	void CloseGPU() {};
};

// 显式模板示例，没有的话无法编译通过
template class cuCElastic<2>;
template class cuCElastic<2, HBXDef::CudaMalloc_t::PAGELOCK>;
template class cuCElastic<3>;
template class cuCElastic<3, HBXDef::CudaMalloc_t::PAGELOCK>;
template class cuCElastic<4>;
template class cuCElastic<4, HBXDef::CudaMalloc_t::PAGELOCK>;

//插值计算Xpos处振型
//@k:轴法横某方向
//@xj:待插值点位置
//@iE:阶数
//@_dataIn:传入的振动模块的相关数据
//@_massIn:传入的质量信息
extern "C" void GetXposShape( HBXDef::UserCalPrec* _pSite, HBXDef::UserCalPrec* _pOutput );

//插值计算Xpos处振型斜率
//@k:轴法横某方向
//@xj:待插值点位置
//@iE:阶数
//@_dataIn:传入的振动模块的相关数据
//@_massIn:传入的质量信息
extern "C" void GetXposDShape( HBXDef::UserCalPrec* _pSite, HBXDef::UserCalPrec* _pOutput );

extern "C"  HBXDef::UserCalPrec d2AngleCal( int k, HBXDef::UserCalPrec xj );

extern "C"  HBXDef::UserCalPrec d2AngleSpeedCal(int k, HBXDef::UserCalPrec xj);

//计算弹性变形引起的加速度附加量
extern "C"	HBXDef::UserCalPrec d2AcceCal(int k, HBXDef::UserCalPrec xj);




#endif	//_CUCELASTIC_H