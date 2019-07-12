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
		//hbx,20190507,�ƺ����ò�����������
		UserCalPrec *h_ptrF;
		UserCalPrec *h_ptrR, *h_ptrRW;
		UserCalPrec *h_ptrP, *h_ptrPW;
		UserCalPrec *h_ptrS, *h_ptrT, *h_ptrV, *h_ptrTX;
		UserCalPrec *h_ptrMval;
		//�豸�˼����е��м����
		UserCalPrec *d_ptrF;
		UserCalPrec *d_ptrR, *d_ptrRW;
		UserCalPrec *d_ptrP, *d_ptrPW;
		UserCalPrec *d_ptrS, *d_ptrT, *d_ptrV;
		UserCalPrec *d_ptrMval;
	protected:
		//@maxit:����������
		//@tol:��������
		//@ttt_sv:�����Ԥ��������ʱ��
		void gpu_pbicgstab(int maxit, double tol, double ttt_sv);
	public:
		BiCGStab(Domain* _dm, Engng* _eng);
		virtual ~BiCGStab();
//		void	FreeGPUResource();

		virtual void	ResetMem(int _nnzA, int _nA);//����HostMalloc����Ϊ������Ҫ��������Ĳ���
		virtual void	ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac = HbxCuDef::CudaMalloc_t::NORMAL);//����devicemalloc����Ϊ������ʱ����

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



