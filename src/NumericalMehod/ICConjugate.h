#pragma once


#include <NumericalMehod/BaseConjugate.h>
#include <ExportDef.h>
#include <CudaPreDef.h>


namespace HBXFEMDef
{
	class Domain;
	class Engng;

	//ICCG�ֽ��Ԥ�������ݶȷ�����������solver,�������˴�������
	class CUFEM_API ICConjugate :
		public BaseConjugate
	{
	private:
		//������������Ӧ�в�ͳ��
		std::vector<UserCalPrec> _residual;
		int k = 0;//��������

		cusolverSpHandle_t cusolverSpH = nullptr; // reordering, permutation and 1st LU factorization

		int *h_Q = NULL; // <int> n
					 // reorder to reduce zero fill-in
					 // Q = symrcm(A) or Q = symamd(A)
	// B = Q*A*Q^T
		int *h_csrRowPtrB = NULL; // <int> n+1
		int *h_csrColIndB = NULL; // <int> nnzA
		double *h_csrValB = NULL; // <double> nnzA
		int *h_mapBfromA = NULL;  // <int> nnzA

		size_t size_perm = 0;
		void *buffer_cpu = NULL; // working space for permutation: B = Q*A*Q^T

		int reorder = 0; // no reordering
		int singularity = 0; // -1 if A is invertible under tol.

		// the constants are used in residual evaluation, r = b - A*x
		const double minus_one = -1.0;
		const double one = 1.0;

		int issym = 0;

	protected:
	public:
		ICConjugate(Domain* _dm, Engng* _eng);
		virtual ~ICConjugate();


		virtual void	ResetMem(int _nnzA = -1, int _nA = -1, HbxCuDef::HostMalloc_t _hostAlloc = HbxCuDef::HostMalloc_t::NORMAL);//����HostMalloc����Ϊ������Ҫ��������Ĳ���
		//virtual void	ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac = HbxCuDef::CudaMalloc_t::CUMALLOC);//����devicemalloc����Ϊ������ʱ����

		//�豸�������˵��ڴ濽��,�ڴ˴����أ���Ϊ��Ҫ��������Ĳ���
		virtual HBXDef::DataAloc_t		MemCpy(HBXDef::CopyType_t _temp = HBXDef::CopyType_t::HostToDevice);

		//Ԥ����,�Ծ�����д�������
		double Preconditioning(ReorderType_t _type = ReorderType_t::SYMRCM);

		//��ʼ���������ȶ���
		bool		InitialDescr(cudaStream_t _stream = 0, cusparseMatrixType_t _MatrixType = CUSPARSE_MATRIX_TYPE_GENERAL);

		//��������ֵ�㷨���,���ؼ���ʱ�䣬msΪ��λ.
		//@_tol:�в�
		//@_iter:��������
		double	ConjugateWithGPU(const double &_tol = 1e-9f, const int &_iter = 1000);

		//У��в�,��һ���������������������ConjugateWithGPU�У����Ǽ���Ч�ʸ�У�鲽�費һ����Ҫ���ʶ����г�
		//��Ҫ�Ǽ��鷶��
		virtual double CheckNormInf(bool _useGPU = false);

	};
}