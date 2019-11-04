#include "PcgConjugate.h"
#include <helper_cuda.h>

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

	void PcgConjugate::genTridiag(size_t _genRowNum, const char * _savepath, bool _bsave)
	{
		m_nnzA = 5 * _genRowNum - 4 * (int)sqrt((double)_genRowNum);
		m_nA = _genRowNum + 1;
		m_RowNum = _genRowNum;

		this->ResetMem();

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

	void PcgConjugate::ResetMem(int _nnzA, int _nA, HbxCuDef::HostMalloc_t _hostAlloc)
	{
		if ((_nnzA == m_nnzA) && (_nA == m_nA))//��CSR��ʽ��Ŀ����δ�����仯��ֱ�ӷ���
		{
			return;
		}
		if (-1 != _nnzA || -1 != _nA)
		{
			m_nnzA = _nnzA;
			m_nA = _nA;
			m_RowNum = m_ColNum = _nA - 1;
			m_HostMalloc = _hostAlloc;
		}

		BaseConjugate::ResetMem();

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
		if (HbxCuDef::CUMALLOC == _cuMac)
		{
			HBXDef::CheckUserDefErrors( cudaMalloc((void **)&d_r, m_RowNum * sizeof(HBXDef::UserCalPrec)) );
			//���̵�ʽ�ұ������Դ����
			HBXDef::CheckUserDefErrors( cudaMalloc((void **)&d_p, m_RowNum * sizeof(HBXDef::UserCalPrec)) );

			HBXDef::CheckUserDefErrors( cudaMalloc((void **)&d_omega, m_RowNum * sizeof(HBXDef::UserCalPrec)) );

			//ILU�ֽ�Ĳ���
			HBXDef::CheckUserDefErrors( cudaMalloc((void **)&d_ILUvals, m_nnzA * sizeof(HBXDef::UserCalPrec)) );
			HBXDef::CheckUserDefErrors( cudaMalloc((void **)&d_zm1, m_RowNum * sizeof(HBXDef::UserCalPrec)) );
			HBXDef::CheckUserDefErrors( cudaMalloc((void **)&d_zm2, m_RowNum * sizeof(HBXDef::UserCalPrec)) );
			HBXDef::CheckUserDefErrors( cudaMalloc((void **)&d_rm2, m_RowNum * sizeof(HBXDef::UserCalPrec)) );
		}
		cudaDeviceSynchronize();
		m_bCudaFree = false;
	}

	HBXDef::DataAloc_t PcgConjugate::MemCpy(HBXDef::CopyType_t _temp)
	{
		using namespace HBXDef;
		double start_matrix_copy, stop_matrix_copy;


		if (HBXDef::HostToDevice == _temp)
		{
			start_matrix_copy = HBXDef::GetTimeStamp();

			HBXDef::CheckUserDefErrors(cudaMemcpy(d_r, h_b,
				m_RowNum * sizeof(UserCalPrec), cudaMemcpyHostToDevice));

			HBXDef::CheckUserDefErrors(cudaMemcpy(d_x, h_x,
				m_RowNum * sizeof(UserCalPrec), cudaMemcpyHostToDevice));

			stop_matrix_copy = HBXDef::GetTimeStamp();
			time_MemcpyHostToDev = stop_matrix_copy - start_matrix_copy;
			std::cout << "�����ݶȷ��ڴ濽��: ";//<< time_MemcpyHostToDev << std::endl;
		}

		BaseConjugate::MemCpy(_temp);


		return HBXDef::DataAloc_t::DATAINGPU;
	}


	bool PcgConjugate::InitialDescr(cudaStream_t _stream, cusparseMatrixType_t _MatrixType)
	{

		BaseConjugate::InitialDescr(_stream, _MatrixType);

		double ttl, ttl2;

		/* ��������A�ķ�����Ϣ */
		_cusparseError_id = cusparseCreateSolveAnalysisInfo(&infoA);
		checkCudaErrors(_cusparseError_id);

		ttl = GetTimeStamp();

		// �����Ը÷�ת�þ����������,�൱�ڰ�
		_cusparseError_id = cusparseDcsrsv_analysis(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE,
													m_RowNum, m_nnzA, Matdescr,
													d_NoneZeroVal, d_iRowSort, d_iColIndex, infoA);
		checkCudaErrors(_cusparseError_id);


		/* ��A������ΪILU0��ֵ����*/
		cudaMemcpy(d_ILUvals, d_NoneZeroVal, m_nnzA * sizeof(HBXDef::UserCalPrec), cudaMemcpyDeviceToDevice);

		/* ��������ʹ��cudsparseScsrilu0���ɾ���A�Ĳ���ȫLU����H */
		_cusparseError_id = cusparseDcsrilu0(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE, m_RowNum,
			Matdescr, d_ILUvals, d_iRowSort, d_iColIndex, infoA);

#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
		checkCudaErrors(_cusparseError_id);
		ttl2 = GetTimeStamp();
		time_sp_analysisT = ttl2 - ttl;
#ifdef _DEBUG
		printf("The cusparseDcsrilu0 Init  %f (ms) \n", time_sp_analysisT*1000);
#endif		
#endif

		double start_fact, end_fact;

		start_fact = HBXDef::GetTimeStamp();

		/* ������LU�ֽ�Ԥ����������Ϣ */
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
													d_NoneZeroVal, d_iRowSort, d_iColIndex, info_u);

		end_fact = HBXDef::GetTimeStamp();

		time_sp_factorT = end_fact - start_fact;
#ifdef _DEBUG
		printf("factor the matrix A use time:  %f (ms) \n", time_sp_factorT *1000);
#endif	

		return true;
	}

	double PcgConjugate::ConjugateWithGPU(const double & _tol, const int & _iter)
	{
		printf("\nʹ��ILU0�ֽ��Ԥ�������ݶȷ���starting...: \n");
		// ������ʱ��CUDA�¼�
		cudaEvent_t	Event_Start;
		cudaEvent_t Event_Stop;
		checkCudaErrors(cudaEventCreate(&Event_Start));
		checkCudaErrors(cudaEventCreate(&Event_Stop));

		double r0, r1, alpha, beta;
		double numerator, denominator, nalpha;
		const double floatone = 1.0;
		const double floatzero = 0.0;
		float	msecUsed = 0.0f;	//ʱ��ͳ��float�ͱ���
		m_qaerr1 = 0.0;//�����
		int k = 0;//��������
		int	nzILU0 = 2 * m_RowNum - 1;//�������ĸ�ֵ�������ȷ�����

#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
//		checkCudaErrors(cudaDeviceSynchronize());
//		double th2d = GetTimeStamp();
#endif
//		_cuError_id = cudaMemcpy(d_r, h_b, m_RowNum * sizeof(double), cudaMemcpyHostToDevice);
//		_cuError_id = cudaMemcpy(d_x, h_x, m_RowNum * sizeof(double), cudaMemcpyHostToDevice);
#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
//		checkCudaErrors(cudaDeviceSynchronize());
//		double th2d2 = GetTimeStamp();
#endif
//		fprintf(stdout, "H2D trans actual data time(s) = %10.8f \n", th2d2 - th2d);
//		//��¼�ڴ濽���Ľ���ʱ��
//		checkCudaErrors(cudaEventRecord(Event_MemEnd, NULL));
//		// �ȴ����е�ֹͣ�¼����
//		checkCudaErrors(cudaEventSynchronize(Event_MemEnd));
//		checkCudaErrors(cudaEventElapsedTime(&CopysecUsed, Event_Start, Event_MemEnd));

		// ����ʱ���
		_residual.resize(_iter);

		checkCudaErrors(cudaEventRecord(Event_Start, NULL));	//��ʼ��ʱ
		cublasDdot(cublasHandle, m_RowNum, d_r, 1, d_r, 1, &r1);
		
		//	cudaMemcpy(m_CSRInput.h_x, d_r, m_RowNum*sizeof(double), cudaMemcpyDeviceToHost);//�����õģ�540Mת560���ʹ��󣬹�������...

		while (r1 > _tol*_tol && k <= _iter)
		{
			// Ԥ����ʹ��A��L����
			//��ǰ���㣬��ΪinfoA�Ǿ���A��L���֣�����ʹ��

			_cusparseError_id = cusparseDcsrsv_solve(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE, m_RowNum, &floatone, descrL,
				d_ILUvals, d_iRowSort, d_iColIndex, infoA, d_r, d_b);
			//�����Ӳ�
			_cusparseError_id = cusparseDcsrsv_solve(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE, m_RowNum, &floatone, descrU,
				d_ILUvals, d_iRowSort, d_iColIndex, info_u, d_b, d_zm1);


			checkCudaErrors(_cusparseError_id);
			k++;
			if (1 == k)
			{
				cublasDcopy(cublasHandle, m_RowNum, d_zm1, 1, d_p, 1);
			}
			else
			{//����(2)
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
												d_NoneZeroVal, d_iRowSort, d_iColIndex, d_p, &floatzero, d_omega);
			cublasDdot(cublasHandle, m_RowNum, d_r, 1, d_zm1, 1, &numerator);//����(3)
			cublasDdot(cublasHandle, m_RowNum, d_p, 1, d_omega, 1, &denominator);//����(4)
			alpha = numerator / denominator;//����(4)
			cublasDaxpy(cublasHandle, m_RowNum, &alpha, d_p, 1, d_x, 1);//����(5)
			cublasDcopy(cublasHandle, m_RowNum, d_r, 1, d_rm2, 1);
			cublasDcopy(cublasHandle, m_RowNum, d_zm1, 1, d_zm2, 1);
			nalpha = -alpha;
			cublasDaxpy(cublasHandle, m_RowNum, &nalpha, d_omega, 1, d_r, 1);//����(6)
			cublasDdot(cublasHandle, m_RowNum, d_r, 1, d_r, 1, &r1);

			_residual[k - 1] = r1;//��õ�ǰ�������Ĳв�
		}
//��¼�����ݶȷ��Ľ���ʱ��
		checkCudaErrors(cudaEventRecord(Event_Stop, NULL));
		// �ȴ����е�ֹͣ�¼����
		checkCudaErrors(cudaEventSynchronize(Event_Stop));
		checkCudaErrors(cudaEventElapsedTime(&msecUsed, Event_Start, Event_Stop));
		time_sp_GPUSolve = msecUsed;
		fprintf(stdout, "����ʱ��time(ms) = %10.8f \n", time_sp_GPUSolve);
		for (int i = 0; i < k; i++)
		{
			printf("  iteration = %3d, residual = %e \n", i, sqrt(_residual[i]));
		}
		_residual.resize(k);

#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
		checkCudaErrors(cudaDeviceSynchronize());
		double tD2H = GetTimeStamp();
#endif
		cudaMemcpy(h_x, d_x, m_RowNum * sizeof(double), cudaMemcpyDeviceToHost);
#ifdef TIME_INDIVIDUAL_LIBRARY_CALLS
		checkCudaErrors(cudaDeviceSynchronize());
		double tD2H2 = GetTimeStamp();
#endif
		time_MemcpyDevToHost = tD2H2 - tD2H;
		fprintf(stdout, "D2H trans actual data time(s) = %10.8f \n", time_MemcpyDevToHost);

		return time_sp_GPUSolve;
		
	}

	double PcgConjugate::CheckNormInf(bool _useGPU)
	{
		cudaMemcpy(h_x, d_x, m_RowNum * sizeof(float), cudaMemcpyDeviceToHost);

		/*�������*/
		m_qaerr1 = CheckResult(m_nnzA, m_nA,
			h_NoneZeroVal, h_iColIndex, h_iRowSort,
			h_x, h_b);
		printf("  Convergence Test: %s \n", (k <= MAX_GPUITER) ? "OK" : "FAIL");
		m_iters = k;

		return m_qaerr1;
	}

	void PcgConjugate::FreeCPUResource()
	{
		HBXDEFFREE(h_ILUvals);
	}

	void PcgConjugate::FreeGPUResource()
	{
		


		if (infoA) { checkCudaErrors(cusparseDestroySolveAnalysisInfo(infoA)); }
		if (info_u) { checkCudaErrors(cusparseDestroySolveAnalysisInfo(info_u)); }
		if (descrL) { checkCudaErrors(cusparseDestroyMatDescr(descrL)); }
		if (descrU) { checkCudaErrors(cusparseDestroyMatDescr(descrU)); }

		if (d_ILUvals) { checkCudaErrors(cudaFree(d_ILUvals)); }
		if (d_p) { checkCudaErrors(cudaFree(d_p)); }
		if (d_omega) { checkCudaErrors(cudaFree(d_omega)); }
		if (d_y) { checkCudaErrors(cudaFree(d_y)); }
		if (d_zm1) { checkCudaErrors(cudaFree(d_zm1)); }
		if (d_zm2) { checkCudaErrors(cudaFree(d_zm2)); }
		if (d_rm2) { checkCudaErrors(cudaFree(d_rm2)); }
	}

}

