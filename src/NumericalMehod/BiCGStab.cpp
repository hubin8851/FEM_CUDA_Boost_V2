#include "BiCGStab.h"
#include <helper_cuda.h>
#include "helper_hbx.h"

namespace HBXFEMDef
{

	extern double HBXDef::GetTimeStamp();

	BiCGStab::BiCGStab(Domain* _dm, Engng* _eng)
		: BaseConjugate(_dm, _eng)
	{
		_trans = CUSPARSE_OPERATION_NON_TRANSPOSE;

		h_ptrF = nullptr;
		h_ptrR = nullptr;
		h_ptrRW = nullptr;
		h_ptrP = nullptr;
		h_ptrPW = nullptr;
		h_ptrS = nullptr;
		h_ptrT = nullptr;
		h_ptrV = nullptr;
		h_ptrTX = nullptr;
		h_ptrMval = nullptr;
	}


	BiCGStab::~BiCGStab(void)
	{
	}

	void	BiCGStab::ResetMem(int _nnzA, int _nA, HbxCuDef::HostMalloc_t _hostAlloc)
	{
		BaseConjugate::ResetMem(_nnzA, _nA, _hostAlloc);
		if (nullptr != h_ptrF)
		{
			delete[] h_ptrF;
		}
		h_ptrF = (UserCalPrec *)malloc(m_RowNum * sizeof(h_ptrF[0]));

		if (nullptr != h_ptrR)
		{
			delete[] h_ptrR;
		}
		h_ptrR = (UserCalPrec *)malloc(m_RowNum * sizeof(h_ptrR[0]));

		if (nullptr != h_ptrRW)
		{
			delete[] h_ptrRW;
		}
		h_ptrRW = (UserCalPrec *)malloc(m_RowNum * sizeof(h_ptrRW[0]));

		if (nullptr != h_ptrP)
		{
			delete[] h_ptrP;
		}
		h_ptrP = (UserCalPrec *)malloc(m_RowNum * sizeof(h_ptrP[0]));

		if (nullptr != h_ptrPW)
		{
			delete[] h_ptrPW;
		}
		h_ptrPW = (UserCalPrec *)malloc(m_RowNum * sizeof(h_ptrPW[0]));

		if (nullptr != h_ptrS)
		{
			delete[] h_ptrS;
		}
		h_ptrS = (UserCalPrec *)malloc(m_RowNum * sizeof(h_ptrS[0]));

		if (nullptr != h_ptrT)
		{
			delete[] h_ptrT;
		}
		h_ptrT = (UserCalPrec *)malloc(m_RowNum * sizeof(h_ptrT[0]));
		h_ptrV = (UserCalPrec *)malloc(m_RowNum * sizeof(h_ptrV[0]));
		h_ptrTX = (UserCalPrec *)malloc(m_RowNum * sizeof(h_ptrTX[0]));
		h_ptrMval = (UserCalPrec *)malloc(m_nnzA * sizeof(h_ptrMval[0]));

		memset(h_ptrF, 0, m_RowNum * sizeof(h_ptrF[0]));
		memset(h_ptrR, 0, m_RowNum * sizeof(h_ptrR[0]));
		memset(h_ptrRW, 0, m_RowNum * sizeof(h_ptrRW[0]));
		memset(h_ptrP, 1, m_RowNum * sizeof(h_ptrP[0]));
		memset(h_ptrPW, 0, m_RowNum * sizeof(h_ptrPW[0]));
		memset(h_ptrS, 0, m_RowNum * sizeof(h_ptrS[0]));
		memset(h_ptrT, 0, m_RowNum * sizeof(h_ptrT[0]));
		memset(h_ptrV, 0, m_RowNum * sizeof(h_ptrV[0]));
		memset(h_ptrTX, 0, m_RowNum * sizeof(h_ptrTX[0]));

	}

	//重载devicemalloc，因为多了临时数组
	void	BiCGStab::ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac)
	{
		if (HbxCuDef::CUMALLOC == _cuMac)
		{
			/* allocate device memory for csr matrix and vectors */
			HBXDef::CheckUserDefErrors(cudaMalloc((void**)&d_ptrF, sizeof(d_ptrF[0]) * m_RowNum));
			HBXDef::CheckUserDefErrors(cudaMalloc((void**)&d_ptrR, sizeof(d_ptrR[0]) * m_RowNum));
			HBXDef::CheckUserDefErrors(cudaMalloc((void**)&d_ptrRW, sizeof(d_ptrRW[0])* m_RowNum));
			HBXDef::CheckUserDefErrors(cudaMalloc((void**)&d_ptrP, sizeof(d_ptrP[0]) * m_RowNum));
			HBXDef::CheckUserDefErrors(cudaMalloc((void**)&d_ptrPW, sizeof(d_ptrPW[0])* m_RowNum));
			HBXDef::CheckUserDefErrors(cudaMalloc((void**)&d_ptrS, sizeof(d_ptrS[0]) * m_RowNum));
			HBXDef::CheckUserDefErrors(cudaMalloc((void**)&d_ptrT, sizeof(d_ptrT[0]) * m_RowNum));
			HBXDef::CheckUserDefErrors(cudaMalloc((void**)&d_ptrV, sizeof(d_ptrV[0]) * m_RowNum));
			HBXDef::CheckUserDefErrors(cudaMalloc((void**)&d_ptrMval, sizeof(d_ptrMval[0]) * m_nnzA));
		}

		BaseConjugate::ResetGraphMem(_cuMac);
		cudaDeviceSynchronize();
		m_bCudaFree = false;
	}

	HBXDef::DataAloc_t	BiCGStab::MemCpy(HBXDef::CopyType_t _temp)
	{
		using namespace HBXDef;
		double start_matrix_copy = GetTimeStamp();

		if (HBXDef::HostToDevice == _temp)
		{
			HBXDef::CheckUserDefErrors(cudaMemcpy(d_ptrMval, h_NoneZeroVal, (size_t)(m_nnzA * sizeof(d_ptrMval[0])), cudaMemcpyDeviceToDevice));//此算法多传入一个长度为nnz的数组
			//传入额外的中间变量,以下步骤是否可以省略？？？
			//hbx,20190507,不省略了。
			HBXDef::CheckUserDefErrors(cudaMemcpy(d_ptrF, h_ptrF, (size_t)(m_RowNum * sizeof(d_ptrF[0])), cudaMemcpyHostToDevice));
			HBXDef::CheckUserDefErrors(cudaMemcpy(d_ptrR, h_ptrR, (size_t)(m_RowNum * sizeof(d_ptrR[0])), cudaMemcpyHostToDevice));
			HBXDef::CheckUserDefErrors(cudaMemcpy(d_ptrRW, h_ptrRW, (size_t)(m_RowNum * sizeof(d_ptrRW[0])), cudaMemcpyHostToDevice));
			HBXDef::CheckUserDefErrors(cudaMemcpy(d_ptrP, h_ptrP, (size_t)(m_RowNum * sizeof(d_ptrP[0])), cudaMemcpyHostToDevice));
			HBXDef::CheckUserDefErrors(cudaMemcpy(d_ptrPW, h_ptrPW, (size_t)(m_RowNum * sizeof(d_ptrPW[0])), cudaMemcpyHostToDevice));
			HBXDef::CheckUserDefErrors(cudaMemcpy(d_ptrS, h_ptrS, (size_t)(m_RowNum * sizeof(d_ptrS[0])), cudaMemcpyHostToDevice));
			HBXDef::CheckUserDefErrors(cudaMemcpy(d_ptrT, h_ptrT, (size_t)(m_RowNum * sizeof(d_ptrT[0])), cudaMemcpyHostToDevice));
			HBXDef::CheckUserDefErrors(cudaMemcpy(d_ptrV, h_ptrV, (size_t)(m_RowNum * sizeof(d_ptrV[0])), cudaMemcpyHostToDevice));
		}

		double stop_matrix_copy = GetTimeStamp();
		std::cout << "双共轭梯度法内存拷贝耗时共计:" << stop_matrix_copy - start_matrix_copy << std::endl;
		return BaseConjugate::MemCpy(_temp);
	}

	//初始化描述器等对象
	bool	BiCGStab::InitialDescr(cudaStream_t _stream, cusparseMatrixType_t _MatrixType)
	{
		m_stream = _stream;
		BaseConjugate::InitialDescr(_stream, _MatrixType);

		/* create three matrix descriptors */
		cusparseStatus_t	status1 = cusparseCreateMatDescr(&descra);
		cusparseStatus_t	status2 = cusparseCreateMatDescr(&descrm);
		if ((status1 != CUSPARSE_STATUS_SUCCESS) || (status2 != CUSPARSE_STATUS_SUCCESS))
		{
			fprintf(stderr, "!!!! CUSPARSE cusparseCreateMatDescr (coefficient matrix or preconditioner) error\n");
			return EXIT_FAILURE;
		}

		//设定矩阵A和M的相关属性，比如转置与否，及首索引号
		HBXDef::CheckUserDefErrors(cusparseSetMatType(descra, CUSPARSE_MATRIX_TYPE_GENERAL));
		if (m_CSRIndexBase == CUSPARSE_INDEX_BASE_ZERO)
		{
			HBXDef::CheckUserDefErrors(cusparseSetMatIndexBase(descra, CUSPARSE_INDEX_BASE_ZERO));
		}
		else
		{
			HBXDef::CheckUserDefErrors(cusparseSetMatIndexBase(descra, CUSPARSE_INDEX_BASE_ONE));
		}

		HBXDef::CheckUserDefErrors(cusparseSetMatType(descrm, CUSPARSE_MATRIX_TYPE_GENERAL));
		if (m_CSRIndexBase == CUSPARSE_INDEX_BASE_ZERO)
		{
			HBXDef::CheckUserDefErrors(cusparseSetMatIndexBase(descrm, CUSPARSE_INDEX_BASE_ZERO));
		}
		else
		{
			HBXDef::CheckUserDefErrors(cusparseSetMatIndexBase(descrm, CUSPARSE_INDEX_BASE_ONE));
		}


		return true;
	}

	//@maxit:最大迭代次数
	//@tol:许用误差精度
	//@ttt_sv:传入的预处理所用时间
	void BiCGStab::gpu_pbicgstab(int maxit, double tol, double ttt_sv)
	{
		using namespace HBXDef;
		double rho, rhop, beta, alpha, negalpha, omega, negomega, temp, temp2;
		double nrmr, nrmr0;
		rho = 0.0;
		double zero = 0.0;
		double one = 1.0;
		double mone = -1.0;
		int i = 0;
		int j = 0;
		double ttl, ttl2, ttu, ttu2, ttm, ttm2;
		double ttt_mv = 0.0;

		//在给定初值条件下计算r0=b-Ax0的残差
#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
		checkCudaErrors(cudaDeviceSynchronize());
		ttm = GetTimeStamp();
#endif
		//dr = A*x0
		checkCudaErrors(cusparseDcsrmv(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE, m_RowNum, m_RowNum, m_nnzA, &one,
			descra, d_NoneZeroVal, d_iRowSort, d_iColIndex, d_x, &zero, d_ptrR));
#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
		cudaDeviceSynchronize();
		ttm2 = GetTimeStamp();
		ttt_mv += (ttm2 - ttm);
		//printf("matvec %f (s)\n",ttm2-ttm);
#endif
		//d_r = -d_r
		checkCudaErrors(cublasDscal(cublasHandle, m_RowNum, &mone, d_ptrR, 1));
		//d_r= d_f + d_r
		checkCudaErrors(cublasDaxpy(cublasHandle, m_RowNum, &one, d_ptrF, 1, d_ptrR, 1));

		//将r拷贝至r^{\hat} 和 p并计算残差
		checkCudaErrors(cublasDcopy(cublasHandle, m_RowNum, d_ptrR, 1, d_ptrRW, 1));
		checkCudaErrors(cublasDcopy(cublasHandle, m_RowNum, d_ptrR, 1, d_ptrP, 1));
		checkCudaErrors(cublasDnrm2(cublasHandle, m_RowNum, d_ptrR, 1, &nrmr0));
#ifdef _DEBUG
		printf("gpu, init residual:norm %20.16f\n", nrmr0);
#endif // _DEBUG

		for (i = 0; i < maxit; i++)
		{
			rhop = rho;
			checkCudaErrors(cublasDdot(cublasHandle, m_RowNum, d_ptrRW, 1, d_ptrR, 1, &rho));	//d_rw .d_r
			if (i > 0)	//即初次迭代后
			{
				beta = (rho / rhop) * (alpha / omega);
				negomega = -omega;
				checkCudaErrors(cublasDaxpy(cublasHandle, m_RowNum, &negomega, d_ptrV, 1, d_ptrP, 1));	//d_p = - omega * d_v + d_p
				checkCudaErrors(cublasDscal(cublasHandle, m_RowNum, &beta, d_ptrP, 1));					//d_p *= beta;
				checkCudaErrors(cublasDaxpy(cublasHandle, m_RowNum, &one, d_ptrR, 1, d_ptrP, 1));		//d_p = d_r+ d_p
			}
#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
			//预处理阶段 (lower and upper triangular solve)
			checkCudaErrors(cudaDeviceSynchronize());
			ttl = GetTimeStamp();
#endif
			checkCudaErrors(cusparseSetMatFillMode(descrm, CUSPARSE_FILL_MODE_LOWER));
			checkCudaErrors(cusparseSetMatDiagType(descrm, CUSPARSE_DIAG_TYPE_UNIT));
			checkCudaErrors(cusparseDcsrsv_solve(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE,
				m_RowNum, &one, descrm,
				d_ptrMval, d_iRowSort, d_iColIndex, info_l, d_ptrP, d_ptrT));
#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
			checkCudaErrors(cudaDeviceSynchronize());
			ttl2 = GetTimeStamp();
			ttu = GetTimeStamp();
#endif
			checkCudaErrors(cusparseSetMatFillMode(descrm, CUSPARSE_FILL_MODE_UPPER));
			checkCudaErrors(cusparseSetMatDiagType(descrm, CUSPARSE_DIAG_TYPE_NON_UNIT));
			checkCudaErrors(cusparseDcsrsv_solve(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE,
				m_RowNum, &one, descrm,
				d_ptrMval, d_iRowSort, d_iColIndex, info_u, d_ptrT, d_ptrPW));
#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
			checkCudaErrors(cudaDeviceSynchronize());
			ttu2 = GetTimeStamp();
			ttt_sv += (ttl2 - ttl) + (ttu2 - ttu);
			//printf("The %d times solve lower %f (s), upper %f (s) \n", i, ttl2 - ttl, ttu2 - ttu);
#endif
			checkCudaErrors(cudaDeviceSynchronize());
			ttm = GetTimeStamp();
			// d_v = A * d_pw;
			checkCudaErrors(cusparseDcsrmv(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE,
				m_RowNum, m_RowNum, m_nnzA, &one, descra,
				d_NoneZeroVal, d_iRowSort, d_iColIndex, d_ptrPW, &zero, d_ptrV));
#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
			checkCudaErrors(cudaDeviceSynchronize());
			ttm2 = GetTimeStamp();
			ttt_mv += (ttm2 - ttm);
			//printf("matvec %f (s)\n",ttm2-ttm);
#endif

			checkCudaErrors(cublasDdot(cublasHandle, m_RowNum, d_ptrRW, 1, d_ptrV, 1, &temp));  // d_v *= d_rw
			alpha = rho / temp;
			negalpha = -(alpha);
			checkCudaErrors(cublasDaxpy(cublasHandle, m_RowNum, &negalpha, d_ptrV, 1, d_ptrR, 1));
			checkCudaErrors(cublasDaxpy(cublasHandle, m_RowNum, &alpha, d_ptrPW, 1, d_x, 1));
			checkCudaErrors(cublasDnrm2(cublasHandle, m_RowNum, d_ptrR, 1, &nrmr));

			if (nrmr < tol*nrmr0)
			{
				j = 5;
				break;
			}
			//预处理步骤
#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
			checkCudaErrors(cudaDeviceSynchronize());
			ttl = GetTimeStamp();
#endif
			checkCudaErrors(cusparseSetMatFillMode(descrm, CUSPARSE_FILL_MODE_LOWER));
			checkCudaErrors(cusparseSetMatDiagType(descrm, CUSPARSE_DIAG_TYPE_UNIT));
			//d_t = M * d_r
			checkCudaErrors(cusparseDcsrsv_solve(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE,
				m_RowNum, &one, descrm,
				d_ptrMval, d_iRowSort, d_iColIndex, info_l, d_ptrR, d_ptrT));
#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
			checkCudaErrors(cudaDeviceSynchronize());
			ttl2 = GetTimeStamp();
			ttu = GetTimeStamp();
#endif
			checkCudaErrors(cusparseSetMatFillMode(descrm, CUSPARSE_FILL_MODE_UPPER));
			checkCudaErrors(cusparseSetMatDiagType(descrm, CUSPARSE_DIAG_TYPE_NON_UNIT));
			//d_s = M * d_t
			checkCudaErrors(cusparseDcsrsv_solve(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE,
				m_RowNum, &one, descrm,
				d_ptrMval, d_iRowSort, d_iColIndex, info_u, d_ptrT, d_ptrS));
#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
			checkCudaErrors(cudaDeviceSynchronize());
			ttu2 = GetTimeStamp();
			ttt_sv += (ttl2 - ttl) + (ttu2 - ttu);
#endif
			//printf("solve lower %f (s), upper %f (s) \n",ttl2-ttl,ttu2-ttu);
			//matrix-vector multiplication
#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
			checkCudaErrors(cudaDeviceSynchronize());
			ttm = GetTimeStamp();
#endif
			//d_t = A * d_s;
			checkCudaErrors(cusparseDcsrmv(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE,
				m_RowNum, m_RowNum, m_nnzA, &one, descra,
				d_NoneZeroVal, d_iRowSort, d_iColIndex, d_ptrS, &zero, d_ptrT));
#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
			checkCudaErrors(cudaDeviceSynchronize());
			ttm2 = GetTimeStamp();
			ttt_mv += (ttm2 - ttm);
			//printf("matvec %f (s)\n",ttm2-ttm);
#endif

			checkCudaErrors(cublasDdot(cublasHandle, m_RowNum, d_ptrT, 1, d_ptrR, 1, &temp));
			checkCudaErrors(cublasDdot(cublasHandle, m_RowNum, d_ptrT, 1, d_ptrT, 1, &temp2));
			omega = temp / temp2;
			negomega = -(omega);
			checkCudaErrors(cublasDaxpy(cublasHandle, m_RowNum, &omega, d_ptrS, 1, d_x, 1));
			checkCudaErrors(cublasDaxpy(cublasHandle, m_RowNum, &negomega, d_ptrT, 1, d_ptrR, 1));

			checkCudaErrors(cublasDnrm2(cublasHandle, m_RowNum, d_ptrR, 1, &nrmr));

			if (nrmr < tol*nrmr0)
			{
				i++;
				j = 0;
				break;
			}

		}//end for
#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
		printf("gpu total solve time %f (s), matvec time %f (s)\n", ttt_sv, ttt_mv);
#endif
	}


	//各种特征值算法求解,返回计算时间，ms为单位.
	//@_tol:许用残差
	//@_iter:许用迭代次数
	double	BiCGStab::ConjugateWithGPU(const double &_tol, const int &_iter)
	{
		using namespace HBXDef;
		/* 创建对LU分解预处理的相关信息 */
		HBXDef::CheckUserDefErrors(cusparseCreateSolveAnalysisInfo(&info_l));
		HBXDef::CheckUserDefErrors(cusparseCreateSolveAnalysisInfo(&info_u));

		double ttl = HBXDef::GetTimeStamp();
		//对矩阵M进行下三角和上三角的分解
		HBXDef::CheckUserDefErrors(cusparseSetMatFillMode(descrm, CUSPARSE_FILL_MODE_LOWER));
		HBXDef::CheckUserDefErrors(cusparseSetMatDiagType(descrm, CUSPARSE_DIAG_TYPE_UNIT));
		HBXDef::CheckUserDefErrors(cusparseDcsrsv_analysis(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE,
			m_RowNum, m_nnzA, descrm,
			d_NoneZeroVal, d_iRowSort, d_iColIndex, info_l));

		HBXDef::CheckUserDefErrors(cudaThreadSynchronize());
		double ttl2 = HBXDef::GetTimeStamp();

		double ttu = HBXDef::GetTimeStamp();

		HBXDef::CheckUserDefErrors(cusparseSetMatFillMode(descrm, CUSPARSE_FILL_MODE_UPPER));
		HBXDef::CheckUserDefErrors(cusparseSetMatDiagType(descrm, CUSPARSE_DIAG_TYPE_NON_UNIT));
		HBXDef::CheckUserDefErrors(cusparseDcsrsv_analysis(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE,
			m_RowNum, m_nnzA, descrm,
			d_NoneZeroVal, d_iRowSort, d_iColIndex, info_u));

		HBXDef::CheckUserDefErrors(cudaThreadSynchronize());
		double ttu2 = HBXDef::GetTimeStamp();
		double ttt_sv = 0.0;
		ttt_sv += (ttl2 - ttl) + (ttu2 - ttu);

		printf("下三角分解耗时 %f (s), 上三角分解耗时 %f (s) \n", ttl2 - ttl, ttu2 - ttu);

		/* 在GPU上用不完全的LU分解完成矩阵M的下三角和上三角分解 */
		double start_ilu, stop_ilu;
		printf("CUSPARSE csrilu0 ");

		start_ilu = HBXDef::GetTimeStamp();

		HBXDef::CheckUserDefErrors(cusparseDcsrilu0(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE,
			m_RowNum, descra,
			d_ptrMval, d_iRowSort, d_iColIndex, info_l));

#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
		HBXDef::CheckUserDefErrors(cudaThreadSynchronize());
		stop_ilu = HBXDef::GetTimeStamp();
#endif // TIME_INDIVIDUAL_LIBRARY_CALLS

		fprintf(stdout, "time(s) = %10.8f \n", stop_ilu - start_ilu);

		double calcstart = HBXDef::GetTimeStamp();
		gpu_pbicgstab(_iter, _tol, ttt_sv);
		HBXDef::CheckUserDefErrors(cudaThreadSynchronize());
		double calcstop = HBXDef::GetTimeStamp();

		/* copy the result into host memory */
		checkCudaErrors(cudaMemcpy(h_ptrTX, d_x, (size_t)(m_RowNum * sizeof(h_ptrTX[0])), cudaMemcpyDeviceToHost));
		fprintf(stdout, "calculate time(s) = %10.8f \n", calcstop - calcstart);
		return calcstop - calcstart;
	}

}