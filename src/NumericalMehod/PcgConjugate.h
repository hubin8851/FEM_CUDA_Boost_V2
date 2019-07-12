#pragma once

#include <NumericalMehod/BaseConjugate.h>
#include <ExportDef.h>
#include <CudaPreDef.h>


namespace HBXFEMDef
{
	class Domain;
	class Engng;

	class CUFEM_API PcgConjugate :
		public BaseConjugate
	{
	private:
		//预处理共轭梯度法的相关变量
		cusparseSolveAnalysisInfo_t	infoA;
		cusparseSolveAnalysisInfo_t	info_u;
		cusparseMatDescr_t descrL;
		cusparseMatDescr_t descrU;

		HBXDef::UserCalPrec *h_ILUvals;
		HBXDef::UserCalPrec *d_ILUvals;
		HBXDef::UserCalPrec *d_p, *d_omega, *d_y;	//PCG法求解的设备端临时数组
		HBXDef::UserCalPrec *d_zm1, *d_zm2, *d_rm2;
	public:
		PcgConjugate(Domain* _dm, Engng* _eng);
		virtual ~PcgConjugate();

//		void	genLaplace(size_t _genRowNum, bool _bsave, boost::filesystem::path _savepath = "");

		virtual void	ResetMem(int _nnzA, int _nA);//重载HostMalloc，因为可能需要拷贝更多的参数
		virtual void	ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac = HbxCuDef::CudaMalloc_t::NORMAL);//重载devicemalloc，因为多了临时数组

		//设备和主机端的内存拷贝,在此处重载，因为需要拷贝更多的参数
		//virtual HBXDef::DataAloc_t		MemCpy(HBXDef::CopyType_t _temp = HBXDef::CopyType_t::HostToDevice);

		//初始化描述器等对象
		bool		InitialDescr(cudaStream_t _stream = 0, cusparseMatrixType_t _MatrixType = CUSPARSE_MATRIX_TYPE_GENERAL);

		//各种特征值算法求解,返回计算时间，ms为单位.
		//@_tol:残差
		//@_iter:迭代次数
		double	ConjugateWithGPU(const double &_tol = 1e-9f, const int &_iter = 1000);

	};
};