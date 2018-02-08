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
	//����ģ����ز���
	HBXDef::UserCalPrec*	d_Cna;		//����վ��������ϵ��
	HBXDef::UserCalPrec*	d_Alpha;	//���ع���
	HBXDef::UserCalPrec*	d_Aero_Ni;	//������ʱ��������

	HBXDef::d_TrajInput		d_TrajInput_;
	HBXDef::d_MonitorInput	d_Monitor_;
	HBXDef::d_ShakeInput	d_ShakeInput_;
	HBXDef::d_MassInput		d_MassInput_;
	HBXDef::d_OriginData<hbxFloat>	d_ModeData_;	//��ǰģ������Ĳ���

	//����ֵ�����е�ļ���
	typedef  HBXDef::UserDefFloat _point[_T];
	_point	*m_Interoplate;	//���еĴ���ֵ
	HBXDef::OptionPlan*	_plan;	//��ǰ���������õ�����
private:
	cudaArray*	d_cordialsArray;
protected:
	size_t	m_slct_block_num;	//��ѡ��block
	size_t	m_dim;	//��ֵ��ά��
	HBXDef::_AeroTable* m_table_p;	//��ֵ��ָ��

	HBXDef::UserCalPrec*	d_cordials;	//���Ի����ÿ��ά���ϵĲ�ֵ�ڵ�����
	HBXDef::UserCalPrec*	d_data;

	unsigned int	m_cordlgth;
	unsigned int	m_datalgth;

public:
	unsigned int			m_numperdim[_T];	//ջ�Ϸ���,һ��block��ÿ��ά���ϵ���ֵ�����
	unsigned int			m_lag_cordinate[_T];	//��ֵ�����λ��
	HBXDef::UserCalPrec		m_lag[_T];
protected:
	void GenerateData();
public:
	cuCElastic();
	~cuCElastic();

	//��ȡ��źͿ�ŵĹ��캯��
	//@_IptTable:��ṹ��ָ��
	//@_blkId:���
	cuCElastic( HBXDef::_AeroTable* _IptTable, size_t _blkId );
	//������캯��
	cuCElastic( const _sameCElastic& _rhs );

	//@__blkId:��������block��ID,��0Ϊ��ʼ����
	//@UserStatusError_t:����״̬������CheckUserDefErrors���ж�
	UserStatusError_t SetBlkId(size_t _blkId);

	//��ʼ����������Դ�ķ���
	void Initial( void* _DataIn = nullptr, cudaStream_t* _stream = 0);

	//��ȡÿһ���㲽��������
	void getdata( void* _dataStep, cudaStream_t* _stream );
	//�ú�����ȡ���нڵ㣨������վ�㣩�ڸ���ģ̬��ֵ���ò����������ڲ�����������ֱ���㣬����ʹ������20170626
	void GetInterpolatePointParam();

	//���ǵ��Ա���֮�������������
	void Elastic_Aero_Force( cublasHandle_t handle );


	//����������
	void Gen_Force_Cal();

	//GPU����ӿ�,��ȱ
	void CalculateGPU(cudaStream_t _stream);

	//�����������Դ�;��
	void CloseGPU() {};
};

// ��ʽģ��ʾ����û�еĻ��޷�����ͨ��
template class cuCElastic<2>;
template class cuCElastic<2, HBXDef::CudaMalloc_t::PAGELOCK>;
template class cuCElastic<3>;
template class cuCElastic<3, HBXDef::CudaMalloc_t::PAGELOCK>;
template class cuCElastic<4>;
template class cuCElastic<4, HBXDef::CudaMalloc_t::PAGELOCK>;

//��ֵ����Xpos������
//@k:�ᷨ��ĳ����
//@xj:����ֵ��λ��
//@iE:����
//@_dataIn:�������ģ����������
//@_massIn:�����������Ϣ
extern "C" void GetXposShape( HBXDef::UserCalPrec* _pSite, HBXDef::UserCalPrec* _pOutput );

//��ֵ����Xpos������б��
//@k:�ᷨ��ĳ����
//@xj:����ֵ��λ��
//@iE:����
//@_dataIn:�������ģ����������
//@_massIn:�����������Ϣ
extern "C" void GetXposDShape( HBXDef::UserCalPrec* _pSite, HBXDef::UserCalPrec* _pOutput );

extern "C"  HBXDef::UserCalPrec d2AngleCal( int k, HBXDef::UserCalPrec xj );

extern "C"  HBXDef::UserCalPrec d2AngleSpeedCal(int k, HBXDef::UserCalPrec xj);

//���㵯�Ա�������ļ��ٶȸ�����
extern "C"	HBXDef::UserCalPrec d2AcceCal(int k, HBXDef::UserCalPrec xj);




#endif	//_CUCELASTIC_H