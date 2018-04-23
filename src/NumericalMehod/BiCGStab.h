#pragma once	//2018-01��hbx, redef

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
		//Ԥ�������ݶȷ�����ر���
		cusparseSolveAnalysisInfo_t	info_l;
		cusparseSolveAnalysisInfo_t	info_u;
		cusparseMatDescr_t	descra;
		cusparseMatDescr_t	descrm;
		cusparseOperation_t	_trans;

		int arraySizeX, arraySizeF, arraySizeR, arraySizeRW, arraySizeP, arraySizePW, arraySizeS, arraySizeT, arraySizeV;
		//�����˼����е��м����
		double *h_ptrF;
		double *h_ptrR, *h_ptrRW;
		double *h_ptrP, *h_ptrPW;
		double *h_ptrS, *h_ptrT, *h_ptrV, *h_ptrTX;
		double *h_ptrMval;
		//�豸�˼����е��м����
		double *d_ptrF;
		double *d_ptrR, *d_ptrRW;
		double *d_ptrP, *d_ptrPW;
		double *d_ptrS, *d_ptrT, *d_ptrV;
		double *d_ptrMval;
	protected:
		//@maxit:����������
		//@tol:��������
		//@ttt_sv:�����Ԥ��������ʱ��
		void gpu_pbicgstab(int maxit, double tol, double ttt_sv);
	public:
		BiCGStab(Domain* _dm, Engng* _eng);
		virtual ~BiCGStab();
//		void	FreeGPUResource();

		virtual void	ResetMem();//����HostMalloc����Ϊ������Ҫ��������Ĳ���
		virtual void	ResetGraphMem();//����devicemalloc����Ϊ������ʱ����

							   //�豸�������˵��ڴ濽��,�ڴ˴����أ���Ϊ��Ҫ��������Ĳ���
		virtual HBXDef::DataAloc_t		MemCpy(HBXDef::CopyType_t _temp = HBXDef::CopyType_t::HostToDevice);

		//��ʼ���������ȶ���
		bool		InitialDescr(cudaStream_t _stream = 0, cusparseMatrixType_t _MatrixType = CUSPARSE_MATRIX_TYPE_GENERAL);

		//��������ֵ�㷨���,���ؼ���ʱ�䣬msΪ��λ.
		//@_tol:�в�
		//@_iter:��������
		double	ConjugateWithGPU(const double &_tol, const int &_iter);

	};

}



