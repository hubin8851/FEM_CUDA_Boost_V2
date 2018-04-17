
#include <cublas.h>
#include <cublas_v2.h>
#include <cublas_api.h>
#include "helper_cuda.h"
#include <CuDefMacro.h>
#include <HbxCudaDef\AeroTable.h>
#include "cuCElastic.h"


//插值计算Xpos处振型
extern "C" 
void GetXposShape( HBXDef::UserCalPrec* _pSite, HBXDef::UserCalPrec* _pOutput )
{
	//完成每个点的插值计算并输出
}


//插值计算Xpos处振型斜率
extern "C" 
void GetXposDShape( HBXDef::UserCalPrec* _pSite, HBXDef::UserCalPrec* _pOutput )
{
	//完成每个点的插值计算并输出
}

template<unsigned int _T, HBXDef::CudaMalloc_t _M>
cuCElastic<_T, _M>::cuCElastic()
{
	d_Cna = nullptr;		//所有站点气动力系数
	d_Alpha = nullptr;		//当地攻角
	d_Aero_Ni = nullptr;	//截面临时变量向量
	isInit = false;

	m_cordlgth = 0;
	m_datalgth = 1;

	memset( m_lag_cordinate, 0, _T * sizeof(unsigned int) );
}

template<unsigned int _T, HBXDef::CudaMalloc_t _M>
cuCElastic<_T, _M>::~cuCElastic()
{
	delete[] d_Cna;
	delete[] d_Alpha;
	delete[] d_Aero_Ni;
}

//获取表号和块号的构造函数
//@_IptTable:表结构体指针
//@_blkId:块号
template<unsigned int _T, HBXDef::CudaMalloc_t _M>
cuCElastic<_T, _M>::cuCElastic(HBXDef::_AeroTable* _IptTable, size_t _blkId)
{
	Assert(_T>0)

	if (nullptr == _IptTable)
	{
		std::cerr << "导入气动数据表错误..." << std::endl;
	}

	d_Cna = nullptr;		//所有站点气动力系数
	d_Alpha = nullptr;		//当地攻角
	d_Aero_Ni = nullptr;	//截面临时变量向量

	m_cordlgth = 0;
	m_datalgth = 1;

	isInit = false;
	m_table_p = _IptTable;
	this->SetBlkId(_blkId);

	memset(m_lag_cordinate, 0, _T * sizeof(unsigned int));
}


//深拷贝构造函数
template<unsigned int _T, HBXDef::CudaMalloc_t _M>
cuCElastic<_T, _M>::cuCElastic(const _sameCElastic& _rhs)
{
	Assert(_T>0)
	memcpy(m_numperdim, _rhs.m_numperdim, _T * sizeof(unsigned int));
	memset(m_lag_cordinate, 0, _T * sizeof(unsigned int));

	m_cordlgth = _rhs.m_cordlgth;
	m_datalgth = _rhs.m_datalgth;

	cudaMemcpy( d_cordials, _rhs.d_cordials, m_cordlgth*sizeof(HBXDef::UserDefFloat), cudaMemcpyDeviceToDevice );

	cudaMemcpy(d_data, _rhs.d_data, m_datalgth*sizeof(HBXDef::UserDefFloat), cudaMemcpyDeviceToDevice );


	isInit = true;
}


//@__blkId:待索引的block的ID,以0为起始索引
//@UserStatusError_t:返回状态，可用CheckUserDefErrors宏判断
template<unsigned int _T, HBXDef::CudaMalloc_t _M>
UserStatusError_t cuCElastic<_T, _M>::SetBlkId(size_t _blkId)
{
	if (_blkId >= m_table_p->_blocknum)
	{
		return UserStatusError_t::USER_STATUS_INVALID_VALUE;
	}
	m_slct_block_num = _blkId;
	m_dim = m_table_p->_blocks[_blkId]._dim;
	if (m_dim <= 0 || _T != m_dim)
	{
		return UserStatusError_t::USER_STATUS_INVALID_VALUE;
	}
	return UserStatusError_t::USER_STATUS_SUCCESS;
}

template<unsigned int _T, HBXDef::CudaMalloc_t _M>
void cuCElastic<_T, _M>::GenerateData()
{
	

#ifdef _CONST_MEM_	//使用常量内存
	HBXDef::TrajInput	_tmpTraj;
	HBXDef::GenTraj(_tmpTraj);
	checkCudaErrors(cudaMalloc((void**)&d_TrajInput_, sizeof(d_TrajInput_)));
	checkCudaErrors(cudaMemcpyToSymbol(&d_TrajInput_, &_tmpTraj, sizeof(d_TrajInput_), cudaMemcpyHostToDevice));

	HBXDef::MonitorInput _tmpMonitor;
	HBXDef::GenMonitor(_tmpMonitor);
	checkCudaErrors(cudaMalloc((void**)&d_Monitor_, sizeof(d_Monitor_)));
	checkCudaErrors(cudaMemcpyToSymbol(&d_Monitor_, &_tmpMonitor, sizeof(d_Monitor_), cudaMemcpyHostToDevice));

	HBXDef::ShakeInput _tmpShake;
	HBXDef::GenShake(_tmpShake);
	checkCudaErrors(cudaMalloc((void**)&d_ShakeInput_, sizeof(d_ShakeInput_)));
	checkCudaErrors(cudaMemcpyToSymbol(&d_ShakeInput_, &_tmpShake, sizeof(d_ShakeInput_), cudaMemcpyHostToDevice));

	HBXDef::MassInput _tmpMass;
	HBXDef::GenMassInput(_tmpMass, nsMax);
	checkCudaErrors(cudaMalloc((void**)&d_MassInput_, sizeof(d_MassInput_)));
	checkCudaErrors(cudaMemcpyToSymbol(&d_MassInput_, &_tmpMass, sizeof(d_MassInput_), cudaMemcpyHostToDevice));
#else

	HBXDef::TrajInput	_tmpTraj;
	HBXDef::GenTraj(_tmpTraj);
	checkCudaErrors( cudaMalloc( (void**)&d_TrajInput_, sizeof(d_TrajInput_)) );
	checkCudaErrors( cudaMemcpy( &d_TrajInput_, &_tmpTraj, sizeof(d_TrajInput_), cudaMemcpyHostToDevice ) );

	HBXDef::MonitorInput _tmpMonitor;
	HBXDef::GenMonitor(_tmpMonitor);
	checkCudaErrors(cudaMalloc((void**)&d_Monitor_, sizeof(d_Monitor_)));
	checkCudaErrors(cudaMemcpy(&d_Monitor_, &_tmpMonitor, sizeof(d_Monitor_), cudaMemcpyHostToDevice));

	HBXDef::ShakeInput _tmpShake;
	HBXDef::GenShake(_tmpShake);
	checkCudaErrors(cudaMalloc((void**)&d_ShakeInput_, sizeof(d_ShakeInput_)));
	checkCudaErrors(cudaMemcpy(&d_ShakeInput_, &_tmpShake, sizeof(d_ShakeInput_), cudaMemcpyHostToDevice));

	HBXDef::MassInput _tmpMass;
	HBXDef::GenMassInput(_tmpMass, nsMax);
	checkCudaErrors(cudaMalloc((void**)&d_MassInput_, sizeof(d_MassInput_)));
	checkCudaErrors(cudaMemcpy(&d_MassInput_, &_tmpMass, sizeof(d_MassInput_), cudaMemcpyHostToDevice));

#endif // _CONST_MEM_
}


//初始化函数完成插值表数据的传入，显存的分配等
//@_DataIn:当前类下传入插值表数据，其他模块参数在GetParam模块中传入
template<unsigned int _T, HBXDef::CudaMalloc_t _M>
void cuCElastic<_T, _M>::Initial( void* _DataIn, cudaStream_t* _stream )
{
	using namespace HBXDef;
	if (true == isInit)
	{
		std::cout << "在此之前已经完成了初始化" << std::endl;
		return;
	}

	IFNULL(_DataIn, "弹性模块初始化时传入空指针...");

	_plan = (HBXDef::OptionPlan*)_DataIn;
//	m_table_p = (HBXDef::OptionPlan*)_DataIn;//获取气动数据表指针

	this->SetBlkId(0);	//目前赋值为0，因为每个table中只有一个block

//	memcpy( m_numperdim, m_table_p->_blocks[m_slct_block_num]._numperdim, sizeof(unsigned int) * _T );
	memcpy(m_numperdim, _plan->_pAeroBlock->_numperdim, sizeof(unsigned int) * _T);

	for (size_t i = 0; i < m_dim; i++)
	{
		m_cordlgth += m_numperdim[i];
		m_datalgth *= m_numperdim[i];
	}
	//在此判断数据的大小是否符合共享内存的需求
	if (49152 < 4*m_datalgth)
	{
		printf("无法使用共享内存...，插值表的大小超过了共享内存的48K限制...");
 	}

	size_t d_pitchBytes;	//用以地址对齐
	size_t h_pitchBytes = m_numperdim[0] * sizeof(UserDefFloat);	//case 为 pitch 时使用 Error，在此有误，未有地址对齐
	cudaChannelFormatDesc _channelDesc;	//case 为 array 时使用
	switch (_M)
	{
	case HBXDef::CudaMalloc_t::PAGELOCK:	//页锁定内存采用了异步操作
		checkCudaErrors( cudaMalloc((void**)&d_cordials, m_cordlgth * sizeof(UserDefFloat)) );	//为坐标分配内存
		checkCudaErrors( cudaMalloc((void**)&d_data, m_datalgth * sizeof(UserDefFloat)) );	//为数据分配内存
//		checkCudaErrors(cudaMemcpyAsync(d_cordials, _plan->_pAeroBlock._corddata, sizeof(UserDefFloat)*m_cordlgth, cudaMemcpyHostToDevice, *_stream));
//		checkCudaErrors(cudaMemcpyAsync(d_data, _plan->_pAeroBlock._data, sizeof(UserDefFloat)*m_datalgth, cudaMemcpyHostToDevice, *_stream));
		checkCudaErrors( cudaMemcpyAsync(d_cordials, m_table_p->_blocks[m_slct_block_num]._corddata, sizeof(UserDefFloat)*m_cordlgth, cudaMemcpyHostToDevice, *_stream) );
		checkCudaErrors( cudaMemcpyAsync(d_data, m_table_p->_blocks[m_slct_block_num]._data, sizeof(UserDefFloat)*m_datalgth, cudaMemcpyHostToDevice, *_stream));
		break;
	case HBXDef::CudaMalloc_t::NORMAL: default:	//正常形式的内存分配
		checkCudaErrors(cudaMalloc((void**)&d_cordials, m_cordlgth * sizeof(UserDefFloat)));	//为坐标分配内存
		checkCudaErrors(cudaMalloc((void**)&d_data, m_datalgth * sizeof(UserDefFloat)));	//为数据分配内存
		checkCudaErrors(cudaMemcpy(d_cordials, m_table_p->_blocks[m_slct_block_num]._corddata, sizeof(UserDefFloat)*m_cordlgth, cudaMemcpyHostToDevice));
		checkCudaErrors(cudaMemcpy(d_data, m_table_p->_blocks[m_slct_block_num]._data, sizeof(UserDefFloat)*m_datalgth, cudaMemcpyHostToDevice));
		break;
	case HBXDef::CudaMalloc_t::ZEROCPY:		//零拷贝模式,不在GPU端分配显存
#if CUDART_VERSION >= 2020	//只有大于等于2.2的设备才能实现...
		checkCudaErrors( cudaHostGetDevicePointer((void**)&d_cordials, (void*)m_table_p->_blocks[m_slct_block_num]._corddata, 0) );
		checkCudaErrors( cudaHostGetDevicePointer((void**)&d_data, (void*)m_table_p->_blocks[m_slct_block_num]._data, 0) );
#endif
		break;
	case HBXDef::CudaMalloc_t::PITCH:	//地址对齐形式，适用于快速查找
		if (2 == m_dim)
		{
			checkCudaErrors( cudaMallocPitch((void**)&d_cordials, &d_pitchBytes, m_numperdim[0] * sizeof(UserDefFloat), m_numperdim[1]) );
			checkCudaErrors( cudaMemcpy2D( d_cordials, d_pitchBytes, m_table_p->_blocks[m_slct_block_num]._corddata, h_pitchBytes, m_numperdim[0] * sizeof(UserDefFloat), m_numperdim[1], cudaMemcpyHostToDevice) );
			checkCudaErrors( cudaMallocPitch((void**)&d_data, &d_pitchBytes, m_numperdim[0] * sizeof(UserDefFloat), m_numperdim[1]));
		}
		else if (3 == m_dim)
		{
			checkCudaErrors(cudaMallocPitch((void**)&d_cordials, &d_pitchBytes, m_numperdim[0] * m_numperdim[1] * sizeof(UserDefFloat), m_numperdim[2]));
			printf("只有维度为2的该类采用地址对齐方式存储，当前类维度为3...，现将1-2维合并为1维");
		}
		else std::cerr << "其他维度目前尚不适用..." << std::endl;
		break;
	case HBXDef::CudaMalloc_t::ARRAY2D:
		if (2 == m_dim)
		{
			_channelDesc = cudaCreateChannelDesc<UserDefFloat>();
			checkCudaErrors(cudaMallocArray(&d_cordialsArray, &_channelDesc, m_numperdim[0], m_numperdim[1]));
			checkCudaErrors(cudaMemcpy2DToArray(d_cordialsArray, 0, 0,	//dst array和offset
							m_table_p->_blocks[m_slct_block_num]._corddata, m_numperdim[0] * sizeof(UserDefFloat),	//src,pitch 
							m_numperdim[0] * sizeof(UserDefFloat), m_numperdim[1], cudaMemcpyHostToDevice));	//宽和高
		}
		else if (3 == m_dim)
		{
			_channelDesc = cudaCreateChannelDesc<UserDefFloat>();
			checkCudaErrors(cudaMallocArray(&d_cordialsArray, &_channelDesc, m_numperdim[0]* m_numperdim[1], m_numperdim[2]));
			checkCudaErrors(cudaMemcpy2DToArray(d_cordialsArray, 0, 0,	//dst array和offset
				m_table_p->_blocks[m_slct_block_num]._corddata, m_numperdim[0] * m_numperdim[1] * sizeof(UserDefFloat),	//src,pitch 
				m_numperdim[0] * m_numperdim[1] * sizeof(UserDefFloat), m_numperdim[2], cudaMemcpyHostToDevice));	//宽和高
			printf("只有维度为2的该类采用地址对齐方式存储，当前类维度为3...，现将1-2维合并为1维");
		}
		else std::cerr << "其他维度目前尚不适用..." << std::endl;
		break;
	}

//	_cordlgth = 0;	//在此做二维转一维数组的临时变量
//	for (int i = 0; i < m_dim; i++)
//	{
//		for (int j = 0; j < m_numperdim[i]; j++)
//		{
//			m_cordials[_cordlgth] = m_table_p->_blocks[m_slct_block_num]._corddata[i][j];
//			_cordlgth++;
//		}
//	}

	//振型和振型斜率分配内存。
	checkCudaErrors(cudaMalloc((void**)&d_ModeData_.ShapeStationElastic, g_nE * g_nStation*sizeof(hbxFloat)) );
	checkCudaErrors(cudaMalloc((void**)&d_ModeData_.DShapeStationElastic, g_nE * g_nStation*sizeof(hbxFloat)) );

	checkCudaErrors( cudaMalloc((void**)&m_Interoplate, sizeof(hbxFloat)*(g_nStation+8)) );	//

//	cuElastic_Aero_Force << <16, 16 >> >(1, this->d_TrajInput_, this->d_ModeData_, this->d_MassInput_, 100);
	isInit = true;
}


//获取每一计算步所用数据
template<unsigned int _T, HBXDef::CudaMalloc_t _M>
void cuCElastic<_T, _M>::getdata( void* _dataStep, cudaStream_t* _stream )
{
	if (nullptr == _dataStep || false == isInit)
	{
		this->GenerateData();
	}
	

}

//该函数获取所有节点（包括各站点）在各阶模态插值所得参数，函数内部分三个方向分别计算，便于使用流。20170626
template<unsigned int _T, HBXDef::CudaMalloc_t _M>
void cuCElastic<_T, _M>::GetInterpolatePointParam()
{
//	cubaselag<_T><<<>>>()
}

//考虑弹性变形之后的气动力计算,20170626,此后以多个流的形式改成异步操作
//template<unsigned int _T>
//void CElastic<_T>::Elastic_Aero_Force( cublasHandle_t handle )
//{
//	cublasStatus_t	_errorcode;
//	HBXDef::UserDefFloat _Alpha[nsMax];	//	各分站点局部攻角
////	HBXDef::UserDefFloat* d_Qs;
//	HBXDef::UserDefFloat* d_Qsl;
//	HBXDef::UserDefFloat* d_Df_q;
//	HBXDef::UserDefFloat* d_f_Dq;
//
//	cudaMalloc((void**)&d_Df_q, sizeof(HBXDef::UserDefFloat)* nsMax);
//	cudaMalloc((void**)&d_f_Dq, sizeof(HBXDef::UserDefFloat)* nsMax);
//
//	float const_one = 1.0;
//	float const_DEG = 57.4;
//	float const_nDEG = -57.4;
//
//#ifdef _DOUBLEPRECISION_
//	//双精度的CUBLAS版本，目前尚未填写，20170626
//#endif
//	_errorcode = cublasSscal( handle, nsMax, &d_TrajInput_.Q, d_ModeData_.S, 1);	//此时S向量乘以Q变成Qs向量
//
//	/* 计算局部攻角和侧滑角 */
//	GetXposShape( d_MassInput_.xStation, d_ModeData_.qi );	//计算所有站点的振型
////	_errorcode = cublasSasum( handle, );
//	GetXposDShape( d_MassInput_.xStation, d_ModeData_.Dqi );	//计算所有站点的振型斜率
////	_errorcode = cublasSasum(handle, );
//	_errorcode = cublasSaxpy( handle, nsMax, &const_one, d_Df_q, 1, d_f_Dq, 1 );
//	_errorcode = cublasSscal( handle, nsMax, &const_DEG, d_f_Dq, 1 );
////还差一步加个常数，to be continue
//
//	// 计算分布气动力和分布气动力矩 
//	_errorcode = cublasSdot( handle, nsMax, d_Cna, 1, d_Alpha, 1, d_Aero_Ni);	// 分布气动力 !!!
//	_errorcode = cublasSdot( handle, nsMax, d_ModeData_.S, 1, d_Aero_Ni, 1, d_Aero_Ni);
////	_errorcode = cublasSscal( handle, nsMax, &Qs, d_Aero_Ni, 1 );	//如果Qs为常数，采用该函数
//}


//广义力计算
template<unsigned int _T, HBXDef::CudaMalloc_t _M>
void cuCElastic<_T, _M>::Gen_Force_Cal()
{

}

//GPU计算接口,空缺
template<unsigned int _T, HBXDef::CudaMalloc_t _M>
void cuCElastic<_T, _M>::CalculateGPU(cudaStream_t _stream)
{

}