#pragma once

#include <NumericalMehod/BaseConjugate.h>
#include <ExportDef.h>
#include <CudaPreDef.h>


namespace HBXFEMDef
{
	class Domain;
	class Engng;

	//ILU�ֽ��Ԥ�������ݶȷ������Լ����ΪICC��Ԥ�������ݶȷ�
	class CUFEM_API PcgConjugate :
		public BaseConjugate
	{
	private:

		//Ԥ�������ݶȷ�����ر���
		cusparseSolveAnalysisInfo_t	infoA;
		cusparseSolveAnalysisInfo_t	info_u;
		cusparseMatDescr_t descrL;
		cusparseMatDescr_t descrU;

		HBXDef::UserCalPrec *h_ILUvals;
		HBXDef::UserCalPrec *d_ILUvals;
		HBXDef::UserCalPrec *d_p, *d_omega, *d_y;	//PCG�������豸����ʱ����
		HBXDef::UserCalPrec *d_zm1, *d_zm2, *d_rm2;
	public:
		PcgConjugate(Domain* _dm, Engng* _eng);
		virtual ~PcgConjugate();

		//�������ԽǾ���,������ʽ�ұ�B,�����ٵ���resetmem
		void	genTridiag(size_t _genRowNum, const char* _savepath = "", bool _bsave = false);


		virtual void	ResetMem(int _nnzA = -1, int _nA= -1, HbxCuDef::HostMalloc_t _hostAlloc = HbxCuDef::HostMalloc_t::NORMAL);//����HostMalloc����Ϊ������Ҫ��������Ĳ���
		virtual void	ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac = HbxCuDef::CudaMalloc_t::CUMALLOC);//����devicemalloc����Ϊ������ʱ����

		//�豸�������˵��ڴ濽��,�ڴ˴����أ���Ϊ��Ҫ��������Ĳ���
		virtual HBXDef::DataAloc_t		MemCpy(HBXDef::CopyType_t _temp = HBXDef::CopyType_t::HostToDevice);

		//��ʼ���������ȶ���
		bool		InitialDescr(cudaStream_t _stream = 0, cusparseMatrixType_t _MatrixType = CUSPARSE_MATRIX_TYPE_GENERAL);

		//��������ֵ�㷨���,���ؼ���ʱ�䣬msΪ��λ.
		//@_tol:�в�
		//@_iter:��������
		double	ConjugateWithGPU(const double &_tol = 1e-9f, const int &_iter = 1000);

		//У��в�,��һ���������������������ConjugateWithGPU�У����Ǽ���Ч�ʸ�У�鲽�費һ����Ҫ���ʶ����г�
		//��Ҫ�Ǽ��鷶��
		virtual double CheckNormInf(bool _useGPU = false);

		virtual	void		FreeCPUResource();
		//�ͷ��豸����Դ,��������cublas��cusparse��������
		virtual	void		FreeGPUResource();		//�����ͷŵ���Դ��һ���������麯��
	};
};