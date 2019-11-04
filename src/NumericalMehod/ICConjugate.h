#pragma once


#include <NumericalMehod/BaseConjugate.h>
#include <ExportDef.h>
#include <CudaPreDef.h>


namespace HBXFEMDef
{
	class Domain;
	class Engng;

	//ICCG分解的预处理共轭梯度法，采用内置solver,并进行了带宽缩减
	class CUFEM_API ICConjugate :
		public BaseConjugate
	{
	private:
		//迭代次数及对应残差统计
		std::vector<UserCalPrec> _residual;
		int k = 0;//迭代次数

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


		virtual void	ResetMem(int _nnzA = -1, int _nA = -1, HbxCuDef::HostMalloc_t _hostAlloc = HbxCuDef::HostMalloc_t::NORMAL);//重载HostMalloc，因为可能需要拷贝更多的参数
		//virtual void	ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac = HbxCuDef::CudaMalloc_t::CUMALLOC);//重载devicemalloc，因为多了临时数组

		//设备和主机端的内存拷贝,在此处重载，因为需要拷贝更多的参数
		virtual HBXDef::DataAloc_t		MemCpy(HBXDef::CopyType_t _temp = HBXDef::CopyType_t::HostToDevice);

		//预处理,对矩阵进行带宽缩减
		double Preconditioning(ReorderType_t _type = ReorderType_t::SYMRCM);

		//初始化描述器等对象
		bool		InitialDescr(cudaStream_t _stream = 0, cusparseMatrixType_t _MatrixType = CUSPARSE_MATRIX_TYPE_GENERAL);

		//各种特征值算法求解,返回计算时间，ms为单位.
		//@_tol:残差
		//@_iter:迭代次数
		double	ConjugateWithGPU(const double &_tol = 1e-9f, const int &_iter = 1000);

		//校验残差,有一部分派生类放入了主函数ConjugateWithGPU中，考虑计算效率该校验步骤不一定需要，故额外列出
		//主要是检验范数
		virtual double CheckNormInf(bool _useGPU = false);

	};
}