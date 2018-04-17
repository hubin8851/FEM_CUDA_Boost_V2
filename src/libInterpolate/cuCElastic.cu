
#include <cublas.h>
#include <cublas_v2.h>
#include <cublas_api.h>
#include "helper_cuda.h"
#include <CuDefMacro.h>
#include <HbxCudaDef\AeroTable.h>
#include "cuCElastic.h"


//��ֵ����Xpos������
extern "C" 
void GetXposShape( HBXDef::UserCalPrec* _pSite, HBXDef::UserCalPrec* _pOutput )
{
	//���ÿ����Ĳ�ֵ���㲢���
}


//��ֵ����Xpos������б��
extern "C" 
void GetXposDShape( HBXDef::UserCalPrec* _pSite, HBXDef::UserCalPrec* _pOutput )
{
	//���ÿ����Ĳ�ֵ���㲢���
}

template<unsigned int _T, HBXDef::CudaMalloc_t _M>
cuCElastic<_T, _M>::cuCElastic()
{
	d_Cna = nullptr;		//����վ��������ϵ��
	d_Alpha = nullptr;		//���ع���
	d_Aero_Ni = nullptr;	//������ʱ��������
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

//��ȡ��źͿ�ŵĹ��캯��
//@_IptTable:��ṹ��ָ��
//@_blkId:���
template<unsigned int _T, HBXDef::CudaMalloc_t _M>
cuCElastic<_T, _M>::cuCElastic(HBXDef::_AeroTable* _IptTable, size_t _blkId)
{
	Assert(_T>0)

	if (nullptr == _IptTable)
	{
		std::cerr << "�����������ݱ����..." << std::endl;
	}

	d_Cna = nullptr;		//����վ��������ϵ��
	d_Alpha = nullptr;		//���ع���
	d_Aero_Ni = nullptr;	//������ʱ��������

	m_cordlgth = 0;
	m_datalgth = 1;

	isInit = false;
	m_table_p = _IptTable;
	this->SetBlkId(_blkId);

	memset(m_lag_cordinate, 0, _T * sizeof(unsigned int));
}


//������캯��
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


//@__blkId:��������block��ID,��0Ϊ��ʼ����
//@UserStatusError_t:����״̬������CheckUserDefErrors���ж�
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
	

#ifdef _CONST_MEM_	//ʹ�ó����ڴ�
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


//��ʼ��������ɲ�ֵ�����ݵĴ��룬�Դ�ķ����
//@_DataIn:��ǰ���´����ֵ�����ݣ�����ģ�������GetParamģ���д���
template<unsigned int _T, HBXDef::CudaMalloc_t _M>
void cuCElastic<_T, _M>::Initial( void* _DataIn, cudaStream_t* _stream )
{
	using namespace HBXDef;
	if (true == isInit)
	{
		std::cout << "�ڴ�֮ǰ�Ѿ�����˳�ʼ��" << std::endl;
		return;
	}

	IFNULL(_DataIn, "����ģ���ʼ��ʱ�����ָ��...");

	_plan = (HBXDef::OptionPlan*)_DataIn;
//	m_table_p = (HBXDef::OptionPlan*)_DataIn;//��ȡ�������ݱ�ָ��

	this->SetBlkId(0);	//Ŀǰ��ֵΪ0����Ϊÿ��table��ֻ��һ��block

//	memcpy( m_numperdim, m_table_p->_blocks[m_slct_block_num]._numperdim, sizeof(unsigned int) * _T );
	memcpy(m_numperdim, _plan->_pAeroBlock->_numperdim, sizeof(unsigned int) * _T);

	for (size_t i = 0; i < m_dim; i++)
	{
		m_cordlgth += m_numperdim[i];
		m_datalgth *= m_numperdim[i];
	}
	//�ڴ��ж����ݵĴ�С�Ƿ���Ϲ����ڴ������
	if (49152 < 4*m_datalgth)
	{
		printf("�޷�ʹ�ù����ڴ�...����ֵ��Ĵ�С�����˹����ڴ��48K����...");
 	}

	size_t d_pitchBytes;	//���Ե�ַ����
	size_t h_pitchBytes = m_numperdim[0] * sizeof(UserDefFloat);	//case Ϊ pitch ʱʹ�� Error���ڴ�����δ�е�ַ����
	cudaChannelFormatDesc _channelDesc;	//case Ϊ array ʱʹ��
	switch (_M)
	{
	case HBXDef::CudaMalloc_t::PAGELOCK:	//ҳ�����ڴ�������첽����
		checkCudaErrors( cudaMalloc((void**)&d_cordials, m_cordlgth * sizeof(UserDefFloat)) );	//Ϊ��������ڴ�
		checkCudaErrors( cudaMalloc((void**)&d_data, m_datalgth * sizeof(UserDefFloat)) );	//Ϊ���ݷ����ڴ�
//		checkCudaErrors(cudaMemcpyAsync(d_cordials, _plan->_pAeroBlock._corddata, sizeof(UserDefFloat)*m_cordlgth, cudaMemcpyHostToDevice, *_stream));
//		checkCudaErrors(cudaMemcpyAsync(d_data, _plan->_pAeroBlock._data, sizeof(UserDefFloat)*m_datalgth, cudaMemcpyHostToDevice, *_stream));
		checkCudaErrors( cudaMemcpyAsync(d_cordials, m_table_p->_blocks[m_slct_block_num]._corddata, sizeof(UserDefFloat)*m_cordlgth, cudaMemcpyHostToDevice, *_stream) );
		checkCudaErrors( cudaMemcpyAsync(d_data, m_table_p->_blocks[m_slct_block_num]._data, sizeof(UserDefFloat)*m_datalgth, cudaMemcpyHostToDevice, *_stream));
		break;
	case HBXDef::CudaMalloc_t::NORMAL: default:	//������ʽ���ڴ����
		checkCudaErrors(cudaMalloc((void**)&d_cordials, m_cordlgth * sizeof(UserDefFloat)));	//Ϊ��������ڴ�
		checkCudaErrors(cudaMalloc((void**)&d_data, m_datalgth * sizeof(UserDefFloat)));	//Ϊ���ݷ����ڴ�
		checkCudaErrors(cudaMemcpy(d_cordials, m_table_p->_blocks[m_slct_block_num]._corddata, sizeof(UserDefFloat)*m_cordlgth, cudaMemcpyHostToDevice));
		checkCudaErrors(cudaMemcpy(d_data, m_table_p->_blocks[m_slct_block_num]._data, sizeof(UserDefFloat)*m_datalgth, cudaMemcpyHostToDevice));
		break;
	case HBXDef::CudaMalloc_t::ZEROCPY:		//�㿽��ģʽ,����GPU�˷����Դ�
#if CUDART_VERSION >= 2020	//ֻ�д��ڵ���2.2���豸����ʵ��...
		checkCudaErrors( cudaHostGetDevicePointer((void**)&d_cordials, (void*)m_table_p->_blocks[m_slct_block_num]._corddata, 0) );
		checkCudaErrors( cudaHostGetDevicePointer((void**)&d_data, (void*)m_table_p->_blocks[m_slct_block_num]._data, 0) );
#endif
		break;
	case HBXDef::CudaMalloc_t::PITCH:	//��ַ������ʽ�������ڿ��ٲ���
		if (2 == m_dim)
		{
			checkCudaErrors( cudaMallocPitch((void**)&d_cordials, &d_pitchBytes, m_numperdim[0] * sizeof(UserDefFloat), m_numperdim[1]) );
			checkCudaErrors( cudaMemcpy2D( d_cordials, d_pitchBytes, m_table_p->_blocks[m_slct_block_num]._corddata, h_pitchBytes, m_numperdim[0] * sizeof(UserDefFloat), m_numperdim[1], cudaMemcpyHostToDevice) );
			checkCudaErrors( cudaMallocPitch((void**)&d_data, &d_pitchBytes, m_numperdim[0] * sizeof(UserDefFloat), m_numperdim[1]));
		}
		else if (3 == m_dim)
		{
			checkCudaErrors(cudaMallocPitch((void**)&d_cordials, &d_pitchBytes, m_numperdim[0] * m_numperdim[1] * sizeof(UserDefFloat), m_numperdim[2]));
			printf("ֻ��ά��Ϊ2�ĸ�����õ�ַ���뷽ʽ�洢����ǰ��ά��Ϊ3...���ֽ�1-2ά�ϲ�Ϊ1ά");
		}
		else std::cerr << "����ά��Ŀǰ�в�����..." << std::endl;
		break;
	case HBXDef::CudaMalloc_t::ARRAY2D:
		if (2 == m_dim)
		{
			_channelDesc = cudaCreateChannelDesc<UserDefFloat>();
			checkCudaErrors(cudaMallocArray(&d_cordialsArray, &_channelDesc, m_numperdim[0], m_numperdim[1]));
			checkCudaErrors(cudaMemcpy2DToArray(d_cordialsArray, 0, 0,	//dst array��offset
							m_table_p->_blocks[m_slct_block_num]._corddata, m_numperdim[0] * sizeof(UserDefFloat),	//src,pitch 
							m_numperdim[0] * sizeof(UserDefFloat), m_numperdim[1], cudaMemcpyHostToDevice));	//��͸�
		}
		else if (3 == m_dim)
		{
			_channelDesc = cudaCreateChannelDesc<UserDefFloat>();
			checkCudaErrors(cudaMallocArray(&d_cordialsArray, &_channelDesc, m_numperdim[0]* m_numperdim[1], m_numperdim[2]));
			checkCudaErrors(cudaMemcpy2DToArray(d_cordialsArray, 0, 0,	//dst array��offset
				m_table_p->_blocks[m_slct_block_num]._corddata, m_numperdim[0] * m_numperdim[1] * sizeof(UserDefFloat),	//src,pitch 
				m_numperdim[0] * m_numperdim[1] * sizeof(UserDefFloat), m_numperdim[2], cudaMemcpyHostToDevice));	//��͸�
			printf("ֻ��ά��Ϊ2�ĸ�����õ�ַ���뷽ʽ�洢����ǰ��ά��Ϊ3...���ֽ�1-2ά�ϲ�Ϊ1ά");
		}
		else std::cerr << "����ά��Ŀǰ�в�����..." << std::endl;
		break;
	}

//	_cordlgth = 0;	//�ڴ�����άתһά�������ʱ����
//	for (int i = 0; i < m_dim; i++)
//	{
//		for (int j = 0; j < m_numperdim[i]; j++)
//		{
//			m_cordials[_cordlgth] = m_table_p->_blocks[m_slct_block_num]._corddata[i][j];
//			_cordlgth++;
//		}
//	}

	//���ͺ�����б�ʷ����ڴ档
	checkCudaErrors(cudaMalloc((void**)&d_ModeData_.ShapeStationElastic, g_nE * g_nStation*sizeof(hbxFloat)) );
	checkCudaErrors(cudaMalloc((void**)&d_ModeData_.DShapeStationElastic, g_nE * g_nStation*sizeof(hbxFloat)) );

	checkCudaErrors( cudaMalloc((void**)&m_Interoplate, sizeof(hbxFloat)*(g_nStation+8)) );	//

//	cuElastic_Aero_Force << <16, 16 >> >(1, this->d_TrajInput_, this->d_ModeData_, this->d_MassInput_, 100);
	isInit = true;
}


//��ȡÿһ���㲽��������
template<unsigned int _T, HBXDef::CudaMalloc_t _M>
void cuCElastic<_T, _M>::getdata( void* _dataStep, cudaStream_t* _stream )
{
	if (nullptr == _dataStep || false == isInit)
	{
		this->GenerateData();
	}
	

}

//�ú�����ȡ���нڵ㣨������վ�㣩�ڸ���ģ̬��ֵ���ò����������ڲ�����������ֱ���㣬����ʹ������20170626
template<unsigned int _T, HBXDef::CudaMalloc_t _M>
void cuCElastic<_T, _M>::GetInterpolatePointParam()
{
//	cubaselag<_T><<<>>>()
}

//���ǵ��Ա���֮�������������,20170626,�˺��Զ��������ʽ�ĳ��첽����
//template<unsigned int _T>
//void CElastic<_T>::Elastic_Aero_Force( cublasHandle_t handle )
//{
//	cublasStatus_t	_errorcode;
//	HBXDef::UserDefFloat _Alpha[nsMax];	//	����վ��ֲ�����
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
//	//˫���ȵ�CUBLAS�汾��Ŀǰ��δ��д��20170626
//#endif
//	_errorcode = cublasSscal( handle, nsMax, &d_TrajInput_.Q, d_ModeData_.S, 1);	//��ʱS��������Q���Qs����
//
//	/* ����ֲ����ǺͲ໬�� */
//	GetXposShape( d_MassInput_.xStation, d_ModeData_.qi );	//��������վ�������
////	_errorcode = cublasSasum( handle, );
//	GetXposDShape( d_MassInput_.xStation, d_ModeData_.Dqi );	//��������վ�������б��
////	_errorcode = cublasSasum(handle, );
//	_errorcode = cublasSaxpy( handle, nsMax, &const_one, d_Df_q, 1, d_f_Dq, 1 );
//	_errorcode = cublasSscal( handle, nsMax, &const_DEG, d_f_Dq, 1 );
////����һ���Ӹ�������to be continue
//
//	// ����ֲ��������ͷֲ��������� 
//	_errorcode = cublasSdot( handle, nsMax, d_Cna, 1, d_Alpha, 1, d_Aero_Ni);	// �ֲ������� !!!
//	_errorcode = cublasSdot( handle, nsMax, d_ModeData_.S, 1, d_Aero_Ni, 1, d_Aero_Ni);
////	_errorcode = cublasSscal( handle, nsMax, &Qs, d_Aero_Ni, 1 );	//���QsΪ���������øú���
//}


//����������
template<unsigned int _T, HBXDef::CudaMalloc_t _M>
void cuCElastic<_T, _M>::Gen_Force_Cal()
{

}

//GPU����ӿ�,��ȱ
template<unsigned int _T, HBXDef::CudaMalloc_t _M>
void cuCElastic<_T, _M>::CalculateGPU(cudaStream_t _stream)
{

}