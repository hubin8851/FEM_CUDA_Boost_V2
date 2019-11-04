#pragma once
#include <ExportDef.h>
#include <HbxDefMacro.h>
#include <HBXDefStruct.h>
#include <CudaPreDef.h>
#include <libCUFEM\BaseNumMethod.h>
#include <HbxGloFunc.h>
#include <HbxDefMalloc.h>


#ifndef _THRUST_

#else

#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/copy.h>
#include <thrust/fill.h>
#include <thrust/sequence.h>

#endif

namespace HBXFEMDef
{
	using namespace HBXDef;

	class Domain;
	class Engng;
	class BaseNumMethod;



	//参考于FEMCal_CUDA_mpi_Boost下的BaseXConjugate基类，略作修改
	class CUFEM_API BaseConjugate
//		:public BaseNumMethod
	{
	protected:
		bool		m_bCudaFree;	//判断是否在释放CUDA资源
		bool		m_bGenTridiag;	//判断数组是否已经malloc过，如果malloc过，hostmalloc函数里当检测指针不为0时，用delete[]；如果没有用delete
		bool		m_bGPUFlag;		//是否GPU可用
		int			m_DevID;		//所选择的最好的GPU的ID号
		bool		m_bSave;		//是否存盘

		cudaError_t			_cuError_id;			//cuda内存拷贝等错误返回值
		cublasStatus_t		_cublasError_id;		//cublas返回值
		cusparseStatus_t	_cusparseError_id;		//cusparse得返回值
		HBXDef::DataAloc_t	m_DataAloc;

		double startT, stopT;
		double time_MemcpyHostToDev;	//从CPU-GPU拷贝耗时
		double time_MemcpyDevToHost;	//从GPU-CPU拷贝耗时
		double time_sp_analysisT;//分析时间，相当于初始化阶段矩阵预处理时间
		double time_sp_factorT;
		double time_sp_CPUSolve;//解算时间
		double time_sp_GPUSolve;//解算时间

		double	msecUsed;	//时间统计double型变量
		double	m_qaerr1;	//残差
		//迭代次数及对应残差统计
		std::vector<UserCalPrec> _residual;
		int k = 0;//真实迭代次数
		size_t	m_iters;		//最大许用迭代次数

		int		m_nnzA;		//CSR格式第一个和第二个数组的长度;
		int		m_nA;		//CSR格式第三个数组的长度，等于矩阵某一维度长度+1;
		int		m_RowNum;	//矩阵的行数，M，方阵则为矩阵的某一维的长度
		int		m_ColNum;	//矩阵的列数，N

		//共轭梯度法的相关变量
		cudaStream_t			m_stream;		//当前对象所使用的流
		cublasHandle_t		cublasHandle;		//CUBLAS对象
		cusparseHandle_t		cusparseHandle;	//CUSPARSE对象
		cusparseMatDescr_t	Matdescr;		//A矩阵的描述器
		cusparseIndexBase_t	m_CSRIndexBase;	//矩阵的索引起始位，为0或者1

		//CSR格式矩阵相关参数,除中间变量的裸指针均放在结构体中
		//	HBXDef::_CSRInput<double>	m_CSRInput;	//存在问题，结构体内存连续可能导致赋值错误
		std::string m_filename;
		std::string	m_SavePath;
		//主机端buffer
		HbxCuDef::HostMalloc_t	m_HostMalloc = HbxCuDef::HostMalloc_t::NORMAL;
		MALLOCFUNPTR			_HostAllocFuncptr = nullptr;//函数指针，CPUmalloc调用的方法
		HBXDef::UserCalPrec		*h_NoneZeroVal = nullptr;// <double/float> nnzA
		int						*h_iColIndex = nullptr;// <int> nnzA
		int						*h_iRowSort = nullptr;// <int> n+1
		HBXDef::UserCalPrec		*h_x = nullptr;//待求特征值
		HBXDef::UserCalPrec		*h_b = nullptr;//等式右边向量
		HBXDef::UserCalPrec		*h_r = nullptr;//r=Ax-b

		//显存内裸指针
		HbxCuDef::CudaMalloc_t		m_CudaMalloc = HbxCuDef::CudaMalloc_t::CUMALLOC;
		MALLOCFUNPTR				_GpuAllocFuncptr = nullptr;//函数指针,GPU的malloc调用的方法
		HBXDef::UserCalPrec*		d_NoneZeroVal = nullptr;
		int*						d_iColIndex = nullptr;
		int*						d_iRowSort = nullptr;
		HBXDef::UserCalPrec*		d_x = nullptr;	//待求特征值
		HBXDef::UserCalPrec*		d_b = nullptr;	//等式右边向量
		HBXDef::UserCalPrec*		d_r = nullptr;	//r=Ax-b

#ifndef _THRUST_
		std::vector<HBXDef::UserCalPrec>	h_vNoneZeroVal;
		std::vector<int>		h_viColSort;
		std::vector<int>		h_viNonZeroRowSort;
		std::vector<HBXDef::UserCalPrec>	h_vRhs;
#else
		thrust::host_vector<HBXDef::UserCalPrec> h_vNoneZeroVal;
		thrust::host_vector<int> h_viColSort;
		thrust::host_vector<int> h_viNonZeroRowSort;
		thrust::host_vector<HBXDef::UserCalPrec> h_vRhs;
#endif
	protected:
	public:
		BaseConjugate(Domain* _dm, Engng* _eng);
		virtual ~BaseConjugate();

		//使用默认参数时，说明是内部函数
		virtual void	ResetMem(int _nnzA = -1, int _nA = -1, HbxCuDef::HostMalloc_t _hostAlloc = HbxCuDef::HostMalloc_t::NORMAL);									//重置内存
		virtual void	ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac = HbxCuDef::CudaMalloc_t::CUMALLOC);		//在GPU上重置显存

		//设备和主机端的内存拷贝
		virtual	HBXDef::DataAloc_t		MemCpy(HBXDef::CopyType_t _temp = HBXDef::CopyType_t::HostToDevice);

		/************************************************************************/
		/*                数据获取模块                                          */
		/************************************************************************/
		//生成随机的三对角对称阵
		virtual void	genTridiag(size_t _genRowNum, const char* _savepath = "", bool _bsave = false);
		//从Mtx格式的文件读取三元数的稀疏矩阵信息
		HBXDef::DataAloc_t		SetStiffMatFromMTX( char* _mat_filename = "gr_900_900_crg.mtx", const char* _dir = "..\\data\\check");

		UserStatusError_t SetStiffMatFrom( HBXDef::_CSRInput<HBXDef::UserCalPrec>* _matIn );

		//从外部获取三个数组指针及其长度得到刚度矩阵的CSR格式
		HBXDef::DataAloc_t	SetStiffMat(HBXDef::UserCalPrec * _srcVal, int * _srcCol, int * _srcRow,
			size_t _nnA, size_t _nA, HbxCuDef::HostMalloc_t _hostAlloc = HbxCuDef::HostMalloc_t::NORMAL);
		//从文件中读取CSR格式的刚度矩阵
		HBXDef::DataAloc_t		SetStiffMat(const char* _NoneZeroVal = "NoneZeroVal.data",
			const char*	_ColSort = "ColSort.data",
			const char*	_ValRowSort = "NoneZeroRowSort.data",
			const char* _dir = "..\\data\\source",
			HbxCuDef::HostMalloc_t _hostAlloc = HbxCuDef::HostMalloc_t::NORMAL);

		//生成随机的等式右端向量，根据测试算例的不同，各派生类的该生成算法不一，但接口统一
		virtual void	genRhs(size_t _genRowNum = -1, const char* _savepath = "..\\data\\check", bool _bsave = false);
		//从文件中读取载荷向量
		virtual HBXDef::DataAloc_t SetLoadVec(const char* _FileName = "LoadVec.data", const char* _dir = "..\\data\\source");
		//从类外部获得载荷数组指针
		HBXDef::DataAloc_t	SetLoadVec(HBXDef::UserCalPrec* _LoadVec, size_t _RowNum);

		virtual const char* GetClassName() const { return typeid(BaseConjugate).name(); };

		/************************************************************************/
		/*                CUDA计算模块                                          */
		/************************************************************************/
		//询问是否CUDA可用
		bool		isDevicePossible() const;

		//获取GPU设备属性，返回所有的设备数并在函数内获得最优设备号
		virtual	size_t	GetGPUDeviceQuery();

		virtual void Reset();

		virtual UserStatusError_t Solve(SparseMat& _A, HBXDef::UserCalPrec& b, HBXDef::UserCalPrec& _x) ;

		//初始化描述器等对象
		//初始化stream，cublas，cusparse
		virtual	bool		InitialDescr(cudaStream_t _stream = 0, 
									cusparseMatrixType_t _MatrixType = CUSPARSE_MATRIX_TYPE_GENERAL);

		//各种特征值算法求解,返回计算时间，ms为单位.纯虚函数，每个派生类必有此函数用以求解
		//@_tol:残差
		//@_iter:迭代次数
		virtual	double	ConjugateWithGPU(const double &_tol = ERRORTOLERATE, const int &_iter = MAX_GPUITER) = 0;

		//校验残差,有一部分派生类放入了主函数ConjugateWithGPU中，考虑计算效率该校验步骤不一定需要，故额外列出
		//主要是检验范数
		virtual double CheckNormInf(bool _useGPU = false) = 0;

		size_t		GetIters()const { return m_iters; };
		double		GetsecUsed()const { return msecUsed; };
		double		Getqaerr()const { return m_qaerr1; };

		//后处理,输出模块
		void SaveResult(const char* _mat_filename = nullptr, const const char* _dir = nullptr);

		//释放资源
		virtual	void		FreeCPUResource();
		//释放设备端资源,不仅限于cublas，cusparse等描述器
		virtual	void		FreeGPUResource();		//各类释放的资源不一样，故用虚函数
	};

}
