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



	//�ο���FEMCal_CUDA_mpi_Boost�µ�BaseXConjugate���࣬�����޸�
	class CUFEM_API BaseConjugate
//		:public BaseNumMethod
	{
	protected:
		bool		m_bCudaFree;	//�ж��Ƿ����ͷ�CUDA��Դ
		bool		m_bGenTridiag;	//�ж������Ƿ��Ѿ�malloc�������malloc����hostmalloc�����ﵱ���ָ�벻Ϊ0ʱ����delete[]�����û����delete
		bool		m_bGPUFlag;		//�Ƿ�GPU����
		int			m_DevID;		//��ѡ�����õ�GPU��ID��
		bool		m_bSave;		//�Ƿ����

		cudaError_t			_cuError_id;			//cuda�ڴ濽���ȴ��󷵻�ֵ
		cublasStatus_t		_cublasError_id;		//cublas����ֵ
		cusparseStatus_t	_cusparseError_id;		//cusparse�÷���ֵ
		HBXDef::DataAloc_t	m_DataAloc;

		double startT, stopT;
		double time_MemcpyHostToDev;	//��CPU-GPU������ʱ
		double time_MemcpyDevToHost;	//��GPU-CPU������ʱ
		double time_sp_analysisT;//����ʱ�䣬�൱�ڳ�ʼ���׶ξ���Ԥ����ʱ��
		double time_sp_factorT;
		double time_sp_CPUSolve;//����ʱ��
		double time_sp_GPUSolve;//����ʱ��

		double	msecUsed;	//ʱ��ͳ��double�ͱ���
		double	m_qaerr1;	//�в�
		//������������Ӧ�в�ͳ��
		std::vector<UserCalPrec> _residual;
		int k = 0;//��ʵ��������
		size_t	m_iters;		//������õ�������

		int		m_nnzA;		//CSR��ʽ��һ���͵ڶ�������ĳ���;
		int		m_nA;		//CSR��ʽ����������ĳ��ȣ����ھ���ĳһά�ȳ���+1;
		int		m_RowNum;	//�����������M��������Ϊ�����ĳһά�ĳ���
		int		m_ColNum;	//�����������N

		//�����ݶȷ�����ر���
		cudaStream_t			m_stream;		//��ǰ������ʹ�õ���
		cublasHandle_t		cublasHandle;		//CUBLAS����
		cusparseHandle_t		cusparseHandle;	//CUSPARSE����
		cusparseMatDescr_t	Matdescr;		//A�����������
		cusparseIndexBase_t	m_CSRIndexBase;	//�����������ʼλ��Ϊ0����1

		//CSR��ʽ������ز���,���м��������ָ������ڽṹ����
		//	HBXDef::_CSRInput<double>	m_CSRInput;	//�������⣬�ṹ���ڴ��������ܵ��¸�ֵ����
		std::string m_filename;
		std::string	m_SavePath;
		//������buffer
		HbxCuDef::HostMalloc_t	m_HostMalloc = HbxCuDef::HostMalloc_t::NORMAL;
		MALLOCFUNPTR			_HostAllocFuncptr = nullptr;//����ָ�룬CPUmalloc���õķ���
		HBXDef::UserCalPrec		*h_NoneZeroVal = nullptr;// <double/float> nnzA
		int						*h_iColIndex = nullptr;// <int> nnzA
		int						*h_iRowSort = nullptr;// <int> n+1
		HBXDef::UserCalPrec		*h_x = nullptr;//��������ֵ
		HBXDef::UserCalPrec		*h_b = nullptr;//��ʽ�ұ�����
		HBXDef::UserCalPrec		*h_r = nullptr;//r=Ax-b

		//�Դ�����ָ��
		HbxCuDef::CudaMalloc_t		m_CudaMalloc = HbxCuDef::CudaMalloc_t::CUMALLOC;
		MALLOCFUNPTR				_GpuAllocFuncptr = nullptr;//����ָ��,GPU��malloc���õķ���
		HBXDef::UserCalPrec*		d_NoneZeroVal = nullptr;
		int*						d_iColIndex = nullptr;
		int*						d_iRowSort = nullptr;
		HBXDef::UserCalPrec*		d_x = nullptr;	//��������ֵ
		HBXDef::UserCalPrec*		d_b = nullptr;	//��ʽ�ұ�����
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

		//ʹ��Ĭ�ϲ���ʱ��˵�����ڲ�����
		virtual void	ResetMem(int _nnzA = -1, int _nA = -1, HbxCuDef::HostMalloc_t _hostAlloc = HbxCuDef::HostMalloc_t::NORMAL);									//�����ڴ�
		virtual void	ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac = HbxCuDef::CudaMalloc_t::CUMALLOC);		//��GPU�������Դ�

		//�豸�������˵��ڴ濽��
		virtual	HBXDef::DataAloc_t		MemCpy(HBXDef::CopyType_t _temp = HBXDef::CopyType_t::HostToDevice);

		/************************************************************************/
		/*                ���ݻ�ȡģ��                                          */
		/************************************************************************/
		//������������ԽǶԳ���
		virtual void	genTridiag(size_t _genRowNum, const char* _savepath = "", bool _bsave = false);
		//��Mtx��ʽ���ļ���ȡ��Ԫ����ϡ�������Ϣ
		HBXDef::DataAloc_t		SetStiffMatFromMTX( char* _mat_filename = "gr_900_900_crg.mtx", const char* _dir = "..\\data\\check");

		UserStatusError_t SetStiffMatFrom( HBXDef::_CSRInput<HBXDef::UserCalPrec>* _matIn );

		//���ⲿ��ȡ��������ָ�뼰�䳤�ȵõ��նȾ����CSR��ʽ
		HBXDef::DataAloc_t	SetStiffMat(HBXDef::UserCalPrec * _srcVal, int * _srcCol, int * _srcRow,
			size_t _nnA, size_t _nA, HbxCuDef::HostMalloc_t _hostAlloc = HbxCuDef::HostMalloc_t::NORMAL);
		//���ļ��ж�ȡCSR��ʽ�ĸնȾ���
		HBXDef::DataAloc_t		SetStiffMat(const char* _NoneZeroVal = "NoneZeroVal.data",
			const char*	_ColSort = "ColSort.data",
			const char*	_ValRowSort = "NoneZeroRowSort.data",
			const char* _dir = "..\\data\\source",
			HbxCuDef::HostMalloc_t _hostAlloc = HbxCuDef::HostMalloc_t::NORMAL);

		//��������ĵ�ʽ�Ҷ����������ݲ��������Ĳ�ͬ����������ĸ������㷨��һ�����ӿ�ͳһ
		virtual void	genRhs(size_t _genRowNum = -1, const char* _savepath = "..\\data\\check", bool _bsave = false);
		//���ļ��ж�ȡ�غ�����
		virtual HBXDef::DataAloc_t SetLoadVec(const char* _FileName = "LoadVec.data", const char* _dir = "..\\data\\source");
		//�����ⲿ����غ�����ָ��
		HBXDef::DataAloc_t	SetLoadVec(HBXDef::UserCalPrec* _LoadVec, size_t _RowNum);

		virtual const char* GetClassName() const { return typeid(BaseConjugate).name(); };

		/************************************************************************/
		/*                CUDA����ģ��                                          */
		/************************************************************************/
		//ѯ���Ƿ�CUDA����
		bool		isDevicePossible() const;

		//��ȡGPU�豸���ԣ��������е��豸�����ں����ڻ�������豸��
		virtual	size_t	GetGPUDeviceQuery();

		virtual void Reset();

		virtual UserStatusError_t Solve(SparseMat& _A, HBXDef::UserCalPrec& b, HBXDef::UserCalPrec& _x) ;

		//��ʼ���������ȶ���
		//��ʼ��stream��cublas��cusparse
		virtual	bool		InitialDescr(cudaStream_t _stream = 0, 
									cusparseMatrixType_t _MatrixType = CUSPARSE_MATRIX_TYPE_GENERAL);

		//��������ֵ�㷨���,���ؼ���ʱ�䣬msΪ��λ.���麯����ÿ����������д˺����������
		//@_tol:�в�
		//@_iter:��������
		virtual	double	ConjugateWithGPU(const double &_tol = ERRORTOLERATE, const int &_iter = MAX_GPUITER) = 0;

		//У��в�,��һ���������������������ConjugateWithGPU�У����Ǽ���Ч�ʸ�У�鲽�費һ����Ҫ���ʶ����г�
		//��Ҫ�Ǽ��鷶��
		virtual double CheckNormInf(bool _useGPU = false) = 0;

		size_t		GetIters()const { return m_iters; };
		double		GetsecUsed()const { return msecUsed; };
		double		Getqaerr()const { return m_qaerr1; };

		//����,���ģ��
		void SaveResult(const char* _mat_filename = nullptr, const const char* _dir = nullptr);

		//�ͷ���Դ
		virtual	void		FreeCPUResource();
		//�ͷ��豸����Դ,��������cublas��cusparse��������
		virtual	void		FreeGPUResource();		//�����ͷŵ���Դ��һ���������麯��
	};

}
