#pragma once

#include <NumericalMehod/BaseConjugate.h>
#include <ExportDef.h>
#include <CudaPreDef.h>
#include "cusolverSp.h"
#include "cusolverRf.h"
#include "cusolverSp_LOWLEVEL_PREVIEW.h"

namespace HBXFEMDef
{
	//低阶cholesky分解，属于直接法,参考
	//本不应该属于迭代法，但是应继承自某一基类，便于注册
	//参见sample cusolversp_lowlevelcholesky的demo
	//本类有CPU和GPU两个版本
	//公共部分resetMem->SetStiffMat->InitialDescr
	//CPU版本的顺序是AnalyzeCholAWithCPU->ConjugateWithCPU
	//GPU版本的顺序是resetGraphMem->memcpy->AnalyzeCholAWithGPU->->ConjugateWithGPU
	class CUFEM_API LowLevelCholesky
		:public BaseConjugate
	{
	private:
		double time_sp_factorT;

		cusolverSpHandle_t cusolverSpH = nullptr; // reordering, permutation and 1st LU factorization

		//流有了，cudaStream_t
		//稀疏矩阵描述器有了 cusparseMatDescr_t
		
		csrcholInfoHost_t h_info = NULL; // opaque info structure for LU with parital pivoting
		csrcholInfo_t d_info = NULL; // opaque info structure for LU with parital pivoting

		//基类都有了
		//int rowsA = 0; // number of rows of A
		//int colsA = 0; // number of columns of A
		//int nnzA = 0; // number of nonzeros of A
		//int baseA = 0; // base index in CSR format
		//// CSR(A) from I/O
		//int *h_csrRowPtrA = NULL; // <int> n+1 
		//int *h_csrColIndA = NULL; // <int> nnzA 
		//double *h_csrValA = NULL; // <double> nnzA 

		HBXDef::UserCalPrec* h_r = NULL; // <double> n, r = b - A*x,是因为有CPU版本，故在此计算残差

		size_t size_internal = 0;//chol分解辅助变量
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

		virtual void	ResetMem(int _nnzA, int _nA, HbxCuDef::HostMalloc_t _hostAlloc = HbxCuDef::HostMalloc_t::NORMAL);//重载HostMalloc，因为可能需要拷贝更多的参数
		virtual void	ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac = HbxCuDef::CudaMalloc_t::CUMALLOC);//重载devicemalloc，因为多了临时数组

		//生成随机的等式右端向量
		virtual void	genRhs(size_t _genRowNum = -1, const char* _savepath = "..\\data\\check", bool _bsave = false);

		//初始化描述器等对象
		//完成对A矩阵的Chol分析，并创建相应内存空间,必须在reset后执行
		bool		InitialDescr(cudaStream_t _stream = 0, cusparseMatrixType_t _MatrixType = CUSPARSE_MATRIX_TYPE_GENERAL);

		//Cholsky分解预处理，如果需要，放入initialDescr函数中
		bool	AnalyzeCholAWithCPU();

		//Cholsky分解预处理，如果需要，放入initialDescr函数中
		bool	AnalyzeCholAWithGPU();

		//设备和主机端的内存拷贝,在此处重载，因为需要拷贝更多的参数
		//在基类中有总计时 memcpy_totalT
		virtual HBXDef::DataAloc_t		MemCpy(HBXDef::CopyType_t _temp = HBXDef::CopyType_t::HostToDevice);		

		//用CPU解算Ax=b，本类迭代算法已封装，传入的迭代次数无用
		double	ConjugateWithCPU(const double &_tol = ERRORTOLERATE, const int &_iter = 1000);

		//各种特征值算法求解,返回计算时间，ms为单位.本类迭代算法已封装，传入的迭代次数无用
		//@_tol:残差
		//@_iter:迭代次数
		double	ConjugateWithGPU(const double &_tol = ERRORTOLERATE, const int &_iter = 1000);


		//校验残差,有一部分派生类放入了主函数ConjugateWithGPU中，考虑计算效率该校验步骤不一定需要，故额外列出
		//主要是检验范数
		virtual double CheckNormInf(bool _useGPU= false);

		//释放主机端资源
		virtual	void		FreeCPUResource();

		//释放设备端资源,不仅限于cublas，cusparse等描述器
		//各类释放的资源不一样，故用虚函数
		virtual	void		FreeGPUResource();		
	};

}