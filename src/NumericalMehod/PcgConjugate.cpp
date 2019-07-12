#include "PcgConjugate.h"
#include <userCuda\helper_cuda.h>

namespace HBXFEMDef
{

	PcgConjugate::PcgConjugate(Domain * _dm, Engng * _eng)
		:BaseConjugate( _dm, _eng )
	{
		descrL = 0;
		descrU = 0;
		infoA = 0;
		info_u = 0;
		h_ILUvals = nullptr;
	}

	PcgConjugate::~PcgConjugate()
	{
	}

	void PcgConjugate::ResetMem(int _nnzA, int _nA)
	{
		BaseConjugate::ResetMem(_nnzA,_nA);

		if (nullptr != h_ILUvals)
		{
			delete[] h_ILUvals;
		}
		h_ILUvals = (HBXDef::UserCalPrec *)malloc(m_nnzA * sizeof(HBXDef::UserCalPrec));
		memset(h_ILUvals, 0, m_nnzA * sizeof(h_ILUvals[0]));
	}

	void PcgConjugate::ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac)
	{
		BaseConjugate::ResetGraphMem(_cuMac);
		if (HbxCuDef::NORMAL == _cuMac)
		{
			HBXDef::CheckUserDefErrors( cudaMalloc((void **)&d_y, m_RowNum * sizeof(HBXDef::UserCalPrec)) );
			//方程等式右边向量显存分配
			HBXDef::CheckUserDefErrors( cudaMalloc((void **)&d_p, m_RowNum * sizeof(HBXDef::UserCalPrec)) );

			HBXDef::CheckUserDefErrors( cudaMalloc((void **)&d_omega, m_RowNum * sizeof(HBXDef::UserCalPrec)) );

			//ILU分解的参数
			HBXDef::CheckUserDefErrors( cudaMalloc((void **)&d_ILUvals, m_nnzA * sizeof(HBXDef::UserCalPrec)) );
			HBXDef::CheckUserDefErrors( cudaMalloc((void **)&d_zm1, m_RowNum * sizeof(HBXDef::UserCalPrec)) );
			HBXDef::CheckUserDefErrors( cudaMalloc((void **)&d_zm2, m_RowNum * sizeof(HBXDef::UserCalPrec)) );
			HBXDef::CheckUserDefErrors( cudaMalloc((void **)&d_rm2, m_RowNum * sizeof(HBXDef::UserCalPrec)) );
		}
		cudaDeviceSynchronize();
		m_bCudaFree = false;
	}


	bool PcgConjugate::InitialDescr(cudaStream_t _stream, cusparseMatrixType_t _MatrixType)
	{

		BaseConjugate::InitialDescr(_stream, _MatrixType);

		double ttl, ttl2;

		/* 创建矩阵A的分析信息 */
		_cusparseError_id = cusparseCreateSolveAnalysisInfo(&infoA);
		checkCudaErrors(_cusparseError_id);

		ttl = GetTimeStamp();

		// 创建对该非转置矩阵的描述器,相当于绑定
		_cusparseError_id = cusparseDcsrsv_analysis(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE,
													m_RowNum, m_nnzA, Matdescr,
													d_NonZeroVal, d_iNonZeroRowSort, d_iColSort, infoA);
		checkCudaErrors(_cusparseError_id);


		/* 将A数据作为ILU0的值传入*/
		cudaMemcpy(d_ILUvals, d_NonZeroVal, m_nnzA * sizeof(HBXDef::UserCalPrec), cudaMemcpyDeviceToDevice);

		/* 描述器，以CSR格式存储的A矩阵的不完全LU分解以便于后期计算 */
		_cusparseError_id = cusparseDcsrilu0(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE, m_RowNum,
			Matdescr, d_ILUvals, d_iNonZeroRowSort, d_iColSort, infoA);
#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
		checkCudaErrors(_cusparseError_id);
		ttl2 = GetTimeStamp();
#ifdef _DEBUG
		printf("The cusparseDcsrilu0 Init  %f (s) \n", ttl2 - ttl );
#endif		
#endif
		/* 创建对LU分解预处理的相关信息 */
		_cusparseError_id = cusparseCreateSolveAnalysisInfo(&info_u);

		_cusparseError_id = cusparseCreateMatDescr(&descrL);
		_cusparseError_id = cusparseSetMatType(descrL, CUSPARSE_MATRIX_TYPE_GENERAL);
		_cusparseError_id = cusparseSetMatIndexBase(descrL, m_CSRIndexBase);
		_cusparseError_id = cusparseSetMatFillMode(descrL, CUSPARSE_FILL_MODE_LOWER);
		_cusparseError_id = cusparseSetMatDiagType(descrL, CUSPARSE_DIAG_TYPE_UNIT);

		_cusparseError_id = cusparseCreateMatDescr(&descrU);
		_cusparseError_id = cusparseSetMatType(descrU, CUSPARSE_MATRIX_TYPE_GENERAL);
		_cusparseError_id = cusparseSetMatIndexBase(descrU, m_CSRIndexBase);
		_cusparseError_id = cusparseSetMatFillMode(descrU, CUSPARSE_FILL_MODE_UPPER);
		_cusparseError_id = cusparseSetMatDiagType(descrU, CUSPARSE_DIAG_TYPE_NON_UNIT);

		_cusparseError_id = cusparseDcsrsv_analysis(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE,
													m_RowNum, m_nnzA, descrU, 
													d_NonZeroVal, d_iNonZeroRowSort, d_iColSort, info_u);


		return true;
	}

	double PcgConjugate::ConjugateWithGPU(const double & _tol, const int & _iter)
	{
		printf("\n使用ILU0分解的预处理共轭梯度法，starting...: \n");
		// 创建计时的CUDA事件
		cudaEvent_t	Event_Start;
		cudaEvent_t Event_Stop;
		checkCudaErrors(cudaEventCreate(&Event_Start));
		checkCudaErrors(cudaEventCreate(&Event_Stop));

		double r0, r1, alpha, beta;
		double numerator, denominator, nalpha;
		const double floatone = 1.0;
		const double floatzero = 0.0;
		float	msecUsed = 0.0f;	//时间统计float型变量
		m_qaerr1 = 0.0;//误差结果
		int k = 0;//迭代次数
		int	nzILU0 = 2 * m_RowNum - 1;//具体在哪赋值待定，先放在这

#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
		checkCudaErrors(cudaDeviceSynchronize());
		double th2d = GetTimeStamp();
#endif
		_cuError_id = cudaMemcpy(d_r, h_rhs, m_RowNum * sizeof(double), cudaMemcpyHostToDevice);
		_cuError_id = cudaMemcpy(d_x, h_x, m_RowNum * sizeof(double), cudaMemcpyHostToDevice);
#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
		checkCudaErrors(cudaDeviceSynchronize());
		double th2d2 = GetTimeStamp();
#endif
		fprintf(stdout, "H2D trans actual data time(s) = %10.8f \n", th2d2 - th2d);
//		//记录内存拷贝的结束时间
//		checkCudaErrors(cudaEventRecord(Event_MemEnd, NULL));
//		// 等待所有的停止事件完成
//		checkCudaErrors(cudaEventSynchronize(Event_MemEnd));
//		checkCudaErrors(cudaEventElapsedTime(&CopysecUsed, Event_Start, Event_MemEnd));
		
		checkCudaErrors(cudaEventRecord(Event_Start, NULL));	//开始计时
		cublasDdot(cublasHandle, m_RowNum, d_r, 1, d_r, 1, &r1);
		
		//	cudaMemcpy(m_CSRInput.h_x, d_r, m_RowNum*sizeof(double), cudaMemcpyDeviceToHost);//测试用的，540M转560发送错误，供测试用...

		while (r1 > _tol*_tol && k <= _iter)
		{
			// 预处理，使用A的L部分
			//向前解算，因为infoA是矩阵A的L部分，可重使用

			_cusparseError_id = cusparseDcsrsv_solve(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE, m_RowNum, &floatone, descrL,
				d_ILUvals, d_iNonZeroRowSort, d_iColSort, infoA, d_r, d_y);
			//回溯子步
			_cusparseError_id = cusparseDcsrsv_solve(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE, m_RowNum, &floatone, descrU,
				d_ILUvals, d_iNonZeroRowSort, d_iColSort, info_u, d_y, d_zm1);


			checkCudaErrors(_cusparseError_id);
			k++;
			if (1 == k)
			{
				cublasDcopy(cublasHandle, m_RowNum, d_zm1, 1, d_p, 1);
			}
			else
			{//步骤(2)
				cublasDdot(cublasHandle, m_RowNum, d_r, 1, d_zm1, 1, &numerator);
				cublasDdot(cublasHandle, m_RowNum, d_rm2, 1, d_zm2, 1, &denominator);
				beta = numerator / denominator;
				cublasDscal(cublasHandle, m_RowNum, &beta, d_p, 1);
				cublasDaxpy(cublasHandle, m_RowNum, &floatone, d_zm1, 1, d_p, 1);
			}
			/*		y = floatone * op(U) * p  + floatzero * y	*/
			_cusparseError_id = cusparseDcsrmv(	cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE, 
												m_RowNum, m_RowNum,
												nzILU0, &floatone, descrU,
												d_NonZeroVal, d_iNonZeroRowSort, d_iColSort, d_p, &floatzero, d_omega);
			cublasDdot(cublasHandle, m_RowNum, d_r, 1, d_zm1, 1, &numerator);//步骤(3)
			cublasDdot(cublasHandle, m_RowNum, d_p, 1, d_omega, 1, &denominator);//步骤(4)
			alpha = numerator / denominator;//步骤(4)
			cublasDaxpy(cublasHandle, m_RowNum, &alpha, d_p, 1, d_x, 1);//步骤(5)
			cublasDcopy(cublasHandle, m_RowNum, d_r, 1, d_rm2, 1);
			cublasDcopy(cublasHandle, m_RowNum, d_zm1, 1, d_zm2, 1);
			nalpha = -alpha;
			cublasDaxpy(cublasHandle, m_RowNum, &nalpha, d_omega, 1, d_r, 1);//步骤(6)
			cublasDdot(cublasHandle, m_RowNum, d_r, 1, d_r, 1, &r1);
		}
//记录共轭梯度法的结束时间
		checkCudaErrors(cudaEventRecord(Event_Stop, NULL));
		// 等待所有的停止事件完成
		checkCudaErrors(cudaEventSynchronize(Event_Stop));
		checkCudaErrors(cudaEventElapsedTime(&msecUsed, Event_Start, Event_Stop));
		fprintf(stdout, "计算时长time(s) = %10.8f \n", msecUsed/1000);
		printf("  iteration = %3d, residual = %e \n", k, sqrt(r1));

#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
		checkCudaErrors(cudaDeviceSynchronize());
		double tD2H = GetTimeStamp();
#endif
		cudaMemcpy(h_x, d_x, m_RowNum * sizeof(double), cudaMemcpyDeviceToHost);
#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
		checkCudaErrors(cudaDeviceSynchronize());
		double tD2H2 = GetTimeStamp();
#endif
		fprintf(stdout, "D2H trans actual data time(s) = %10.8f \n", tD2H2 - tD2H);

		/*结果检验*/
		m_qaerr1 = CheckResult(m_nnzA, m_nA,
			h_NoneZeroVal, h_iColSort, h_iNonZeroRowSort,
			h_x, h_rhs);
		printf("  Convergence Test: %s \n", (k <= MAX_GPUITER) ? "OK" : "FAIL");
		m_iters = k;
		return msecUsed;


	}

}

