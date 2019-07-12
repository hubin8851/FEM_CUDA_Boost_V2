#pragma once	//2018-01，hbx, redef

#include <NumericalMehod/BaseConjugate.h>
#include <ExportDef.h>
#include <CudaPreDef.h>
namespace HBXFEMDef
{
	class Domain;
	class Engng;

	class CUFEM_API BiCGStab :
		public BaseConjugate
	{
	protected:
		//预处理共轭梯度法的相关变量
		cusparseSolveAnalysisInfo_t	info_l;
		cusparseSolveAnalysisInfo_t	info_u;
		cusparseMatDescr_t	descra;
		cusparseMatDescr_t	descrm;
		cusparseOperation_t	_trans;

		int arraySizeX, arraySizeF, arraySizeR, arraySizeRW, arraySizeP, arraySizePW, arraySizeS, arraySizeT, arraySizeV;
		//主机端计算中的中间变量
		//hbx,20190507,似乎都用不到？？？？
		UserCalPrec *h_ptrF;
		UserCalPrec *h_ptrR, *h_ptrRW;
		UserCalPrec *h_ptrP, *h_ptrPW;
		UserCalPrec *h_ptrS, *h_ptrT, *h_ptrV, *h_ptrTX;
		UserCalPrec *h_ptrMval;
		//设备端计算中的中间变量
		UserCalPrec *d_ptrF;
		UserCalPrec *d_ptrR, *d_ptrRW;
		UserCalPrec *d_ptrP, *d_ptrPW;
		UserCalPrec *d_ptrS, *d_ptrT, *d_ptrV;
		UserCalPrec *d_ptrMval;
	protected:
		//@maxit:最大迭代次数
		//@tol:许用误差精度
		//@ttt_sv:传入的预处理所用时间
		void gpu_pbicgstab(int maxit, double tol, double ttt_sv);
	public:
		BiCGStab(Domain* _dm, Engng* _eng);
		virtual ~BiCGStab();
//		void	FreeGPUResource();

		virtual void	ResetMem(int _nnzA, int _nA);//重载HostMalloc，因为可能需要拷贝更多的参数
		virtual void	ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac = HbxCuDef::CudaMalloc_t::NORMAL);//重载devicemalloc，因为多了临时数组

							   //设备和主机端的内存拷贝,在此处重载，因为需要拷贝更多的参数
		virtual HBXDef::DataAloc_t		MemCpy(HBXDef::CopyType_t _temp = HBXDef::CopyType_t::HostToDevice);

		//初始化描述器等对象
		bool		InitialDescr(cudaStream_t _stream = 0, cusparseMatrixType_t _MatrixType = CUSPARSE_MATRIX_TYPE_GENERAL);

		//各种特征值算法求解,返回计算时间，ms为单位.
		//@_tol:残差
		//@_iter:迭代次数
		double	ConjugateWithGPU(const double &_tol, const int &_iter);



	};

}



