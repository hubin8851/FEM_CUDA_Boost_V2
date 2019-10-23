#include "CgConjugate.h"
#include <helper_cuda.h>

namespace HBXFEMDef
{

	CgConjugate::CgConjugate(Domain * _dm, Engng * _eng)
		:BaseConjugate(_dm,_eng)
	{

	}

	CgConjugate::~CgConjugate()
	{
	}

	void CgConjugate::ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac)
	{
		if (HbxCuDef::NORMAL == _cuMac)
		{
			/* allocate device memory for csr matrix and vectors */
			HBXDef::CheckUserDefErrors(cudaMalloc((void **)&d_omega, m_RowNum * sizeof(UserCalPrec)));
			HBXDef::CheckUserDefErrors(cudaMalloc((void **)&d_p, m_RowNum * sizeof(UserCalPrec)));
		}

		BaseConjugate::ResetGraphMem(_cuMac);
		cudaDeviceSynchronize();
		m_bCudaFree = false;
	}

	double CgConjugate::ConjugateWithGPU(const double & _tol, const int & _iter)
	{
		printf("\n使用共轭梯度法，starting...: \n");

		int k = 0;
		UserCalPrec r0, r1, alpha, beta;
		UserCalPrec dot, nalpha;
		const UserCalPrec floatone = 1.0;
		const UserCalPrec floatzero = 0.0;
		r0 = 0.f;

		// 创建时间戳
		double tsolve = HBXDef::GetTimeStamp();

		cublasDdot(cublasHandle, m_RowNum, d_r, 1, d_r, 1, &r1);

		while (r1 > _tol*_tol && k <= _iter)
		{
			k++;

			if (k == 1)
			{
				cublasDcopy(cublasHandle, m_RowNum, d_r, 1, d_p, 1);
			}
			else
			{
				beta = r1 / r0;
				cublasDscal(cublasHandle, m_RowNum, &beta, d_p, 1);
				cublasDaxpy(cublasHandle, m_RowNum, &floatone, d_r, 1, d_p, 1);
			}

			cusparseDcsrmv(	cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE,
							m_RowNum, m_RowNum, m_nA, &floatone, Matdescr, 
							d_NoneZeroVal, d_iRowSort, d_iColIndex, d_p, &floatzero, d_omega);
			cublasDdot(cublasHandle, m_RowNum, d_p, 1, d_omega, 1, &dot);
			alpha = r1 / dot;
			cublasDaxpy(cublasHandle, m_RowNum, &alpha, d_p, 1, d_x, 1);
			nalpha = -alpha;
			cublasDaxpy(cublasHandle, m_RowNum, &nalpha, d_omega, 1, d_r, 1);
			r0 = r1;
			cublasDdot(cublasHandle, m_RowNum, d_r, 1, d_r, 1, &r1);
		}

		// 创建时间戳
#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
		checkCudaErrors(cudaDeviceSynchronize());
		double tsolve2 = HBXDef::GetTimeStamp();
#endif
		fprintf(stdout, "计算时长time(s) = %10.8f \n", tsolve2 - tsolve);
		printf("  iteration = %3d, residual = %e \n", k, sqrt(r1));

		/*结果检验*/
		m_qaerr1 = CheckResult(m_nnzA, m_nA,
			h_NoneZeroVal, h_iColIndex, h_iRowSort,
			h_x, h_b);
		printf("  Convergence Test: %s \n", (k <= MAX_GPUITER) ? "OK" : "FAIL");
		m_iters = k;

		return tsolve2 - tsolve;
	}

}

