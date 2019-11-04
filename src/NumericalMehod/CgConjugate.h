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

		//Ԥ�������ݶȷ�����ر���

		HBXDef::UserCalPrec *d_p;	//
		HBXDef::UserCalPrec	*d_omega;	//CG�������豸����ʱ����
	public:
		CgConjugate(Domain* _dm, Engng* _eng);
		virtual ~CgConjugate();

		//�������ԽǾ���,������ʽ�ұ�B
		void	genTridiag(size_t _genRowNum, const char* _savepath = "", bool _bsave = false);

//		virtual void	ResetMem(int _nnzA, int _nA);//����HostMalloc����Ϊ������Ҫ��������Ĳ���
		virtual void	ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac = HbxCuDef::CudaMalloc_t::CUMALLOC);//����devicemalloc����Ϊ������ʱ����

		//�豸�������˵��ڴ濽��,�ڴ˴����أ���Ϊ��Ҫ��������Ĳ���
		virtual HBXDef::DataAloc_t		MemCpy(HBXDef::CopyType_t _temp = HBXDef::CopyType_t::HostToDevice);

		//��ʼ���������ȶ���
//		bool		InitialDescr(cudaStream_t _stream = 0, cusparseMatrixType_t _MatrixType = CUSPARSE_MATRIX_TYPE_GENERAL);

		//��������ֵ�㷨���,���ؼ���ʱ�䣬msΪ��λ.
		//@_tol:�в�
		//@_iter:��������
		double	ConjugateWithGPU(const double &_tol = 1e-12, const int &_iter = 1000);

		//У��в�,��һ���������������������ConjugateWithGPU�У����Ǽ���Ч�ʸ�У�鲽�費һ����Ҫ���ʶ����г�
		//��Ҫ�Ǽ��鷶��
		virtual double CheckNormInf(bool _useGPU = false);

	};
};