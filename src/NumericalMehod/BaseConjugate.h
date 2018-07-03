#pragma once
#include <ExportDef.h>
#include <HbxDefMacro.h>
#include <HBXDefStruct.h>
#include <CuDefMacro.h>

#include <libCUFEM\BaseNumMethod.h>
#include <HbxGloFunc.h>

namespace HBXFEMDef
{
	using namespace HBXDef;

	class Domain;
	class Engng;
	class BaseNumMethod;


	//�ο���FEMCal_CUDA_mpi_Boost�µ�BaseXConjugate���࣬�����޸�
	class CUFEM_API BaseConjugate:
		public BaseNumMethod
	{
	protected:
		bool		m_bCudaFree;		//�ж��Ƿ����ͷ�CUDA��Դ
		bool		m_bGenTridiag;	//�ж������Ƿ��Ѿ�malloc�������malloc����hostmalloc�����ﵱ���ָ�벻Ϊ0ʱ����delete[]�����û����delete
		bool		m_bGPUFlag;		//�Ƿ�GPU����
		int		m_DevID;			//��ѡ�����õ�GPU��ID��
		bool		m_bSave;			//�Ƿ����

		cudaError_t			_cuError_id;			//cuda�ڴ濽���ȴ��󷵻�ֵ
		cublasStatus_t		_cublasError_id;		//cublas����ֵ
		cusparseStatus_t		_cusparseError_id;	//cusparse�÷���ֵ
		HBXDef::DataAloc_t	m_DataAloc;

		float	msecUsed;	//ʱ��ͳ��double�ͱ���
		double	m_qaerr1;	//�в�
		size_t	m_iters;		//��������

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
		std::string	m_SavePath;
		//������buffer
		int			*h_iColSort, *h_iNonZeroRowSort;
		HBXDef::UserCalPrec		*h_NoneZeroVal;
		HBXDef::UserCalPrec		*h_x;
		HBXDef::UserCalPrec		*h_rhs;
		//�Դ�����ָ��
		HBXDef::UserCalPrec*		d_NonZeroVal;
		int*						d_iColSort;
		int*						d_iNonZeroRowSort;
		HBXDef::UserCalPrec*		d_x;	//��������ֵ
		HBXDef::UserCalPrec*		d_r;		//��ʽ�ұ�����

		std::vector<double>	h_vNoneZeroVal;
		std::vector<int>		h_viColSort;
		std::vector<int>		h_viNonZeroRowSort;
		std::vector<double>	h_vRhs;
	protected:
		//������������ԽǶԳ���
		void	genTridiag(size_t _genRowNum, const char* _savepath = "", bool _bsave = false);
		//��������ĵ�ʽ�Ҷ�����
		void	genRhs(size_t _genRowNum, bool _bsave = false, const char* _savepath = "..\\data\\check");
	public:
		BaseConjugate(Domain* _dm, Engng* _eng);
		virtual ~BaseConjugate();

		virtual void	ResetMem();									//�����ڴ�
		virtual void	ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac = HbxCuDef::CudaMalloc_t::NORMAL);		//��GPU�������Դ�

		//�豸�������˵��ڴ濽��
		virtual	HBXDef::DataAloc_t		MemCpy(HBXDef::CopyType_t _temp = HBXDef::CopyType_t::HostToDevice);

		/************************************************************************/
		/*                ���ݻ�ȡģ��                                          */
		/************************************************************************/

		//��Mtx��ʽ���ļ���ȡ��Ԫ����ϡ�������Ϣ
		HBXDef::DataAloc_t		SetStiffMatFromMTX(const char* _mat_filename = "gr_900_900_crg.mtx", const const char* _dir = "..\\data\\check");

		//���ⲿ��ȡ��������ָ�뼰�䳤�ȵõ��նȾ����CSR��ʽ
		HBXDef::DataAloc_t	SetStiffMat(const void *const _srcVal, const void *const _srcCol, const void *const _srcRow,
			size_t _nnA, size_t _nA, bool _bsave = false);
		//���ļ��ж�ȡCSR��ʽ�ĸնȾ���
		HBXDef::DataAloc_t		SetStiffMat(const char* _NoneZeroVal = "NoneZeroVal.data",
			const char*	_ColSort = "ColSort.data",
			const char*	_ValRowSort = "NoneZeroRowSort.data",
			const const char* _dir = "..\\data\\source");

		//���ļ��ж�ȡ�غ�����
		virtual HBXDef::DataAloc_t SetLoadVec(const char* _FileName = "LoadVec.data", const const char* _dir = "..\\data\\source");
		//�����ⲿ����غ�����ָ��
		HBXDef::DataAloc_t	SetLoadVec(const void* _LoadVec, size_t _RowNum);

		virtual const char* GetClassName() const { return typeid(BaseConjugate).name(); };

		/************************************************************************/
		/*                CUDA����ģ��                                          */
		/************************************************************************/
		//ѯ���Ƿ�CUDA����
		bool		isDevicePossible() const;

		//��ȡGPU�豸���ԣ��������е��豸�����ں����ڻ�������豸��
		virtual	size_t	GetGPUDeviceQuery();

		virtual void Reset();

		virtual UserStatusError solve();

		//��ʼ���������ȶ���
		virtual	bool		InitialDescr(cudaStream_t _stream = 0, 
									cusparseMatrixType_t _MatrixType = CUSPARSE_MATRIX_TYPE_GENERAL);

		//��������ֵ�㷨���,���ؼ���ʱ�䣬msΪ��λ.���麯����ÿ����������д˺����������
		//@_tol:�в�
		//@_iter:��������
		virtual	double	ConjugateWithGPU(const double &_tol = ERRORTOLERATE, const int &_iter = MAX_GPUITER) = 0;
		size_t		GetIters()const { return m_iters; };
		double		GetsecUsed()const { return msecUsed; };
		double		Getqaerr()const { return m_qaerr1; };

		//�ͷ��豸����Դ,��������cublas��cusparse��������
		virtual	void		FreeGPUResource();		//�����ͷŵ���Դ��һ���������麯��
	};

}
