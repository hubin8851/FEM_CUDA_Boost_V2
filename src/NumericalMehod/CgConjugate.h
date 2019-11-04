#pragma once
#include <NumericalMehod/BaseConjugate.h>
#include <ExportDef.h>
#include <CudaPreDef.h>


namespace HBXFEMDef
{
	class Domain;
	class Engng;

	class CUFEM_API CgConjugate :
		public BaseConjugate
	{
	private:

		//预处理共轭梯度法的相关变量

		HBXDef::UserCalPrec *d_p;	//
		HBXDef::UserCalPrec	*d_omega;	//CG法求解的设备端临时数组
	public:
		CgConjugate(Domain* _dm, Engng* _eng);
		virtual ~CgConjugate();

		//生成三对角矩阵,连带等式右边B
		void	genTridiag(size_t _genRowNum, const char* _savepath = "", bool _bsave = false);

//		virtual void	ResetMem(int _nnzA, int _nA);//重载HostMalloc，因为可能需要拷贝更多的参数
		virtual void	ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac = HbxCuDef::CudaMalloc_t::CUMALLOC);//重载devicemalloc，因为多了临时数组

		//设备和主机端的内存拷贝,在此处重载，因为需要拷贝更多的参数
		virtual HBXDef::DataAloc_t		MemCpy(HBXDef::CopyType_t _temp = HBXDef::CopyType_t::HostToDevice);

		//初始化描述器等对象
//		bool		InitialDescr(cudaStream_t _stream = 0, cusparseMatrixType_t _MatrixType = CUSPARSE_MATRIX_TYPE_GENERAL);

		//各种特征值算法求解,返回计算时间，ms为单位.
		//@_tol:残差
		//@_iter:迭代次数
		double	ConjugateWithGPU(const double &_tol = 1e-12, const int &_iter = 1000);

		//校验残差,有一部分派生类放入了主函数ConjugateWithGPU中，考虑计算效率该校验步骤不一定需要，故额外列出
		//主要是检验范数
		virtual double CheckNormInf(bool _useGPU = false);

	};
};