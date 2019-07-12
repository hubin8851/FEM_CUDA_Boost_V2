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
		//Ԥ�������ݶȷ�����ر���
		cusparseSolveAnalysisInfo_t	infoA;
		cusparseSolveAnalysisInfo_t	info_u;
		cusparseMatDescr_t descrL;
		cusparseMatDescr_t descrU;

		HBXDef::UserCalPrec *h_ILUvals;
		HBXDef::UserCalPrec *d_ILUvals;
		HBXDef::UserCalPrec *d_p, *d_omega, *d_y;	//PCG�������豸����ʱ����
		HBXDef::UserCalPrec *d_zm1, *d_zm2, *d_rm2;
	public:
		PcgConjugate(Domain* _dm, Engng* _eng);
		virtual ~PcgConjugate();

//		void	genLaplace(size_t _genRowNum, bool _bsave, boost::filesystem::path _savepath = "");

		virtual void	ResetMem(int _nnzA, int _nA);//����HostMalloc����Ϊ������Ҫ��������Ĳ���
		virtual void	ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac = HbxCuDef::CudaMalloc_t::NORMAL);//����devicemalloc����Ϊ������ʱ����

		//�豸�������˵��ڴ濽��,�ڴ˴����أ���Ϊ��Ҫ��������Ĳ���
		//virtual HBXDef::DataAloc_t		MemCpy(HBXDef::CopyType_t _temp = HBXDef::CopyType_t::HostToDevice);

		//��ʼ���������ȶ���
		bool		InitialDescr(cudaStream_t _stream = 0, cusparseMatrixType_t _MatrixType = CUSPARSE_MATRIX_TYPE_GENERAL);

		//��������ֵ�㷨���,���ؼ���ʱ�䣬msΪ��λ.
		//@_tol:�в�
		//@_iter:��������
		double	ConjugateWithGPU(const double &_tol = 1e-9f, const int &_iter = 1000);

	};
};