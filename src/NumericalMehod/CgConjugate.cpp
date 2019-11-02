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

	//生成三对角矩阵
	void	CgConjugate::genTridiag(size_t _genRowNum, const char* _savepath, bool _bsave)
	{
		m_nnzA = 5 * _genRowNum - 4 * (int)sqrt((double)_genRowNum);
		m_nA = _genRowNum + 1;
		m_RowNum = _genRowNum;

		ResetMem();

		for (int i = 0; i < _genRowNum; i++)
		{
			h_b[i] = 0.0;  // Initialize RHS
			h_x[i] = 0.0;  // Initial approximation of solution
		}
		int n = (int)sqrt((double)_genRowNum);
		printf("laplace dimension = %d\n", n);
		int idx = 0;
		// loop over degrees of freedom
		for (int i = 0; i < _genRowNum; i++)
		{
			int ix = i % n;
			int iy = i / n;

			h_iRowSort[i] = idx;

			// up
			if (iy > 0)
			{
				h_NoneZeroVal[idx] = 1.0;
				h_iColIndex[idx] = i - n;
				idx++;
			}
			else
			{
				h_b[i] -= 1.0;
			}

			// left
			if (ix > 0)
			{
				h_NoneZeroVal[idx] = 1.0;
				h_iColIndex[idx] = i - 1;
				idx++;
			}
			else
			{
				h_b[i] -= 0.0;
			}

			// center
			h_NoneZeroVal[idx] = -4.0;
			h_iColIndex[idx] = i;
			idx++;

			//right
			if (ix < n - 1)
			{
				h_NoneZeroVal[idx] = 1.0;
				h_iColIndex[idx] = i + 1;
				idx++;
			}
			else
			{
				h_b[i] -= 0.0;
			}

			//down
			if (iy < n - 1)
			{
				h_NoneZeroVal[idx] = 1.0;
				h_iColIndex[idx] = i + n;
				idx++;
			}
			else
			{
				h_b[i] -= 0.0;
			}

		}

		h_iRowSort[_genRowNum] = idx;
	}

	void CgConjugate::ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac)
	{
		if (HbxCuDef::CUMALLOC == _cuMac)
		{
			/* allocate device memory for csr matrix and vectors */
			HBXDef::CheckUserDefErrors(cudaMalloc((void **)&d_r, m_RowNum * sizeof(UserCalPrec)));
			HBXDef::CheckUserDefErrors(cudaMalloc((void **)&d_omega, m_RowNum * sizeof(UserCalPrec)));
			HBXDef::CheckUserDefErrors(cudaMalloc((void **)&d_p, m_RowNum * sizeof(UserCalPrec)));
		}

		BaseConjugate::ResetGraphMem(_cuMac);
		cudaDeviceSynchronize();
		m_bCudaFree = false;
	}

	HBXDef::DataAloc_t CgConjugate::MemCpy(HBXDef::CopyType_t _temp)
	{
		using namespace HBXDef;
		double start_matrix_copy, stop_matrix_copy;


		if (HBXDef::HostToDevice == _temp)
		{
			start_matrix_copy = HBXDef::GetTimeStamp();

			HBXDef::CheckUserDefErrors(cudaMemcpy(d_r, h_b,
				m_RowNum * sizeof(UserCalPrec), cudaMemcpyHostToDevice));

			stop_matrix_copy = HBXDef::GetTimeStamp();
			time_MemcpyHostToDev = stop_matrix_copy - start_matrix_copy;
			std::cout << "共轭梯度法内存拷贝: ";//<< time_MemcpyHostToDev << std::endl;
		}
		
		BaseConjugate::MemCpy(_temp);

		
		return HBXDef::DataAloc_t::DATAINGPU;

	}

	double CgConjugate::ConjugateWithGPU(const double & _tol, const int & _iter)
	{
		printf("\n使用共轭梯度法，starting...: \n");

		UserCalPrec r0, r1, alpha, beta;
		UserCalPrec dot, nalpha;
		const UserCalPrec floatone = 1.0;
		const UserCalPrec floatzero = 0.0;
		r0 = 0.f;

#ifndef FLOATCALPREC

		// 创建时间戳
		_residual.resize(_iter);
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

			//y=alpha*op(A)*x + beta*y
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
			_residual[k] = r1;//获得当前迭代时的
		}

		// 创建时间戳
#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
		checkCudaErrors(cudaDeviceSynchronize());
		double tsolve2 = HBXDef::GetTimeStamp();
		time_sp_GPUSolve = tsolve2 - tsolve;
#endif
		fprintf(stdout, "计算时长time(s) = %10.8f \n", time_sp_GPUSolve);
		for (int i=0;i< k;i++)
		{
			printf("  iteration = %3d, residual = %e \n", i, sqrt(_residual[i]));
		}
		

#else	//调用float相关函数
		

#endif

		return time_sp_GPUSolve;
	}


	//校验残差,有一部分派生类放入了主函数ConjugateWithGPU中，考虑计算效率该校验步骤不一定需要，故额外列出
	//主要是检验范数
	double CgConjugate::CheckNormInf(bool _useGPU)
	{
		cudaMemcpy(h_x, d_x, m_RowNum * sizeof(float), cudaMemcpyDeviceToHost);

		/*结果检验*/
		m_qaerr1 = CheckResult(m_nnzA, m_nA,
			h_NoneZeroVal, h_iColIndex, h_iRowSort,
			h_x, h_b);
		printf("  Convergence Test: %s \n", (k <= MAX_GPUITER) ? "OK" : "FAIL");
		m_iters = k;

		return m_qaerr1;
	}

}

