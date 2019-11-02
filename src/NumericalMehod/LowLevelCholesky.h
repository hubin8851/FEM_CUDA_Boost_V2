#pragma once

#include <NumericalMehod/BaseConjugate.h>
#include <ExportDef.h>
#include <CudaPreDef.h>
#include "cusolverSp.h"
#include "cusolverRf.h"
#include "cusolverSp_LOWLEVEL_PREVIEW.h"

namespace HBXFEMDef
{
	//�ͽ�cholesky�ֽ⣬����ֱ�ӷ�,�ο�
	//����Ӧ�����ڵ�����������Ӧ�̳���ĳһ���࣬����ע��
	//�μ�sample cusolversp_lowlevelcholesky��demo
	//������CPU��GPU�����汾
	//��������resetMem->SetStiffMat->InitialDescr
	//CPU�汾��˳����AnalyzeCholAWithCPU->ConjugateWithCPU
	//GPU�汾��˳����resetGraphMem->memcpy->AnalyzeCholAWithGPU->->ConjugateWithGPU
	class CUFEM_API LowLevelCholesky
		:public BaseConjugate
	{
	private:
		double time_sp_factorT;

		cusolverSpHandle_t cusolverSpH = nullptr; // reordering, permutation and 1st LU factorization

		//�����ˣ�cudaStream_t
		//ϡ��������������� cusparseMatDescr_t
		
		csrcholInfoHost_t h_info = NULL; // opaque info structure for LU with parital pivoting
		csrcholInfo_t d_info = NULL; // opaque info structure for LU with parital pivoting

		//���඼����
		//int rowsA = 0; // number of rows of A
		//int colsA = 0; // number of columns of A
		//int nnzA = 0; // number of nonzeros of A
		//int baseA = 0; // base index in CSR format
		//// CSR(A) from I/O
		//int *h_csrRowPtrA = NULL; // <int> n+1 
		//int *h_csrColIndA = NULL; // <int> nnzA 
		//double *h_csrValA = NULL; // <double> nnzA 

		HBXDef::UserCalPrec* h_r = NULL; // <double> n, r = b - A*x,����Ϊ��CPU�汾�����ڴ˼���в�

		size_t size_internal = 0;//chol�ֽ⸨������
		size_t size_chol = 0; // size of working space for csrlu
		void *buffer_cpu = NULL; // working space for Cholesky
		void *buffer_gpu = NULL; // working space for Cholesky

		 // the constants used in residual evaluation, r = b - A*x
		const double minus_one = -1.0;
		const double one = 1.0;

		// singularity is -1 if A is invertible under tol
		int singularity = 0;

		double x_inf = 0.0; // |x|
		double r_inf = 0.0; // |r|
		double A_inf = 0.0; // |A|

	protected:
	public:
		LowLevelCholesky(Domain* _dm, Engng* _eng);
		virtual ~LowLevelCholesky();

		//		void	genLaplace(size_t _genRowNum, bool _bsave, boost::filesystem::path _savepath = "");

		virtual void	ResetMem(int _nnzA, int _nA, HbxCuDef::HostMalloc_t _hostAlloc = HbxCuDef::HostMalloc_t::NORMAL);//����HostMalloc����Ϊ������Ҫ��������Ĳ���
		virtual void	ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac = HbxCuDef::CudaMalloc_t::CUMALLOC);//����devicemalloc����Ϊ������ʱ����

		//��������ĵ�ʽ�Ҷ�����
		virtual void	genRhs(size_t _genRowNum = -1, const char* _savepath = "..\\data\\check", bool _bsave = false);

		//��ʼ���������ȶ���
		//��ɶ�A�����Chol��������������Ӧ�ڴ�ռ�,������reset��ִ��
		bool		InitialDescr(cudaStream_t _stream = 0, cusparseMatrixType_t _MatrixType = CUSPARSE_MATRIX_TYPE_GENERAL);

		//Cholsky�ֽ�Ԥ���������Ҫ������initialDescr������
		bool	AnalyzeCholAWithCPU();

		//Cholsky�ֽ�Ԥ���������Ҫ������initialDescr������
		bool	AnalyzeCholAWithGPU();

		//�豸�������˵��ڴ濽��,�ڴ˴����أ���Ϊ��Ҫ��������Ĳ���
		//�ڻ��������ܼ�ʱ memcpy_totalT
		virtual HBXDef::DataAloc_t		MemCpy(HBXDef::CopyType_t _temp = HBXDef::CopyType_t::HostToDevice);		

		//��CPU����Ax=b����������㷨�ѷ�װ������ĵ�����������
		double	ConjugateWithCPU(const double &_tol = ERRORTOLERATE, const int &_iter = 1000);

		//��������ֵ�㷨���,���ؼ���ʱ�䣬msΪ��λ.��������㷨�ѷ�װ������ĵ�����������
		//@_tol:�в�
		//@_iter:��������
		double	ConjugateWithGPU(const double &_tol = ERRORTOLERATE, const int &_iter = 1000);


		//У��в�,��һ���������������������ConjugateWithGPU�У����Ǽ���Ч�ʸ�У�鲽�費һ����Ҫ���ʶ����г�
		//��Ҫ�Ǽ��鷶��
		virtual double CheckNormInf(bool _useGPU= false);

		//�ͷ���������Դ
		virtual	void		FreeCPUResource();

		//�ͷ��豸����Դ,��������cublas��cusparse��������
		//�����ͷŵ���Դ��һ���������麯��
		virtual	void		FreeGPUResource();		
	};

}