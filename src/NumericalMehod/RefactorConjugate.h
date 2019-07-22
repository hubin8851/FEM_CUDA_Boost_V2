#pragma once
#include <NumericalMehod/BaseConjugate.h>
#include <ExportDef.h>
#include <CudaPreDef.h>
#include "cusolverSp.h"
#include "cusolverRf.h"
#include "cusolverSp_LOWLEVEL_PREVIEW.h"

namespace HBXFEMDef
{
	class Domain;
	class Engng;

	typedef enum ReorderType
	{
		SYMRCM = 0,	//稀疏反向 Cuthill-McKee 排序
		SYMAMD,		//对称近似最小阶数置换
	}ReorderType_t;
	

	//基于RCM，缩减带宽的预处理共轭梯度法
	//流程为resetmem->resetGraphmem->Preconditioning->getB
	class CUFEM_API RefactorConjugate 
		:public BaseConjugate
	{
	private:
		cusolverRfHandle_t cusolverRfH = nullptr; // refactorization
		cusolverSpHandle_t cusolverSpH = nullptr; // reordering, permutation and 1st LU factorization

		csrluInfoHost_t info = nullptr; // opaque info structure for LU with parital pivoting

		int *h_Qreorder = nullptr; // <int> n
							// reorder to reduce zero fill-in
							// Qreorder = symrcm(A) or Qreroder = symamd(A)

		// B = Q*A*Q^T，就是RCM后的矩阵A~，因为只是减小带宽，故nnzA不变
		int *h_csrRowPtrB = nullptr; // <int> n+1
		int *h_csrColIndB = nullptr; // <int> nnzA
		HBXDef::UserCalPrec *h_csrValB = nullptr; // <HBXDef::UserCalPrec> nnzA
		int *h_mapBfromA = nullptr;  // <int> nnzA

		HBXDef::UserCalPrec *h_r = nullptr; // <double> n, r = b - A*x

		 // solve B*(Qx) = Q*b
		HBXDef::UserCalPrec *h_xhat = nullptr; // <HBXDef::UserCalPrec> n, Q*x_hat = x
		HBXDef::UserCalPrec *h_bhat = nullptr; // <HBXDef::UserCalPrec> n, b_hat = Q*b 

		size_t size_perm = 0;
		size_t size_internal = 0;
		size_t size_lu = 0; // size of working space for csrlu

		void *buffer_cpu = nullptr; // working space for
							 // - permutation: B = Q*A*Q^T
							 // - LU with partial pivoting in cusolverSp

		//cusplverSp通过部分主消元的办法计算LU分解
		//     Plu*B*Qlu^T = L*U
		//直至因式分解完成后nnzL和nnzU才能知道
		int *h_Plu = nullptr; // <int> n
		int *h_Qlu = nullptr; // <int> n

		int nnzL = 0;
		int *h_csrRowPtrL = nullptr; // <int> n+1
		int *h_csrColIndL = nullptr; // <int> nnzL
		HBXDef::UserCalPrec *h_csrValL = nullptr; // <double> nnzL

		int nnzU = 0;
		int *h_csrRowPtrU = nullptr; // <int> n+1
		int *h_csrColIndU = nullptr; // <int> nnzU
		HBXDef::UserCalPrec *h_csrValU = nullptr; // <double> nnzU

		int *h_P = nullptr; // <int> n, P = Plu * Qreorder
		int *h_Q = nullptr; // <int> n, Q = Qlu * Qreorder

		HBXDef::UserCalPrec *d_b = nullptr; // <double> n, a copy of h_b

		int *d_P = nullptr; // <int> n, P*A*Q^T = L*U
		int *d_Q = nullptr; // <int> n 

		HBXDef::UserCalPrec *d_T = nullptr; // working space in cusolverRfSolve
							// |d_T| = n * nrhs  

		// the constants used in residual evaluation, r = b - A*x
		const double minus_one = -1.0;
		const double one = 1.0;

		// the constants used in cusolverRf
		// nzero is the value below which zero pivot is flagged.
		// nboost is the value which is substitured for zero pivot.
		double nzero = 0.0;
		double nboost = 0.0;

		// the constant used in cusolverSp
		// singularity is -1 if A is invertible under tol
		// tol determines the condition of singularity
		// pivot_threshold decides pivoting strategy  主阈值？          
		int singularity = 0;
		const double tol = 1.e-14;
		const double pivot_threshold = 1.0;
		// the constants used in cusolverRf
		const cusolverRfFactorization_t fact_alg = CUSOLVERRF_FACTORIZATION_ALG0; // default
		const cusolverRfTriangularSolve_t solve_alg = CUSOLVERRF_TRIANGULAR_SOLVE_ALG1; // default

		double x_inf = 0.0; // |x|
		double r_inf = 0.0; // |r|
		double A_inf = 0.0; // |A|
		int errors = 0;

		double time_reorderT;
		double time_permT;
		double time_sp_factorT;
		double time_sp_extractT;
		double time_rf_assembleT;
		double time_rf_resetT;
		double time_rf_refactorT;
		double time_rf_solveT;

	protected:
		//step 3: B = Q*A*Q^T
		//本质上是在CPU上执行的，CUDA调用了别家的库
		double GetB();

	public:
		RefactorConjugate(Domain* _dm, Engng* _eng);
		virtual ~RefactorConjugate();

		virtual void	ResetMem(int _nnzA, int _nA);//重载HostMalloc，因为可能需要拷贝更多的参数
		virtual void	ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac = HbxCuDef::CudaMalloc_t::NORMAL);//重载devicemalloc，因为多了临时数组

		//设备和主机端的内存拷贝,在此处重载，因为需要拷贝更多的参数
		virtual HBXDef::DataAloc_t		MemCpy(HBXDef::CopyType_t _temp = HBXDef::CopyType_t::HostToDevice);

		//初始化描述器等对象
		bool		InitialDescr(cudaStream_t _stream = 0, cusparseMatrixType_t _MatrixType = CUSPARSE_MATRIX_TYPE_GENERAL);

		//预处理,对矩阵进行带宽缩减
		double Preconditioning( ReorderType_t _type = ReorderType_t::SYMRCM );

		//各种特征值算法求解,返回计算时间，ms为单位.
		//@_tol:残差
		//@_iter:迭代次数
		//step 4: solve A*x = b by LU(B) in cusolverSp
		double	ConjugateWithGPU(const double &_tol = 1e-9f, const int &_iter = 1000);

		//从P*B*Q^T = L*U中提取P,Q,L,U
		//返回计时
		double ExtractPQLU();
	};
};