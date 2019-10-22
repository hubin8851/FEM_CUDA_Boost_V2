#include "LowLevelCholesky.h"
#include "helper_cuda.h"
#include "helper_cusolver.h"


namespace HBXFEMDef
{
	LowLevelCholesky::LowLevelCholesky(Domain * _dm, Engng * _eng)
		:BaseConjugate(_dm, _eng)
	{
		time_sp_factorT = 0;
	}

	LowLevelCholesky::~LowLevelCholesky()
	{
		this->FreeGPUResource();
	}

	void LowLevelCholesky::ResetMem(int _nnzA, int _nA)
	{
		BaseConjugate::ResetMem(_nnzA, _nA);

		if (nullptr != h_r)
		{
			delete[] h_r;
		}
		h_r = (HBXDef::UserCalPrec *)malloc(m_nnzA * sizeof(HBXDef::UserCalPrec));
		memset(h_r, 0, m_nnzA * sizeof(h_r[0]));
	}

	void LowLevelCholesky::ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac)
	{
		BaseConjugate::ResetGraphMem(_cuMac);
		if (HbxCuDef::NORMAL == _cuMac)
		{
			HBXDef::CheckUserDefErrors(cudaMalloc((void **)&d_r, m_RowNum * sizeof(HBXDef::UserCalPrec)));
		}
		cudaDeviceSynchronize();
		m_bCudaFree = false;
	}


	void LowLevelCholesky::genRhs(size_t _genRowNum, bool _bsave, const char * _savepath)
	{
		for (int row = 0; row < m_RowNum; row++)
		{
			h_b[row] = 1.0;
		}
	}

	bool LowLevelCholesky::InitialDescr(cudaStream_t _stream, cusparseMatrixType_t _MatrixType)
	{
		//构建cusolver求解器
		if ( BaseConjugate::InitialDescr(_stream, _MatrixType) )
		{
			checkCudaErrors(cusolverSpCreate(&cusolverSpH));
			checkCudaErrors(cusolverSpSetStream(cusolverSpH, m_stream));
		}
		else
		{
			return false;
		}

		return true;
	}

	bool LowLevelCholesky::AnalyzeCholAWithCPU()
	{
		double start_spFactorT, stop_spFactorT;
		start_spFactorT = GetTimeStamp();

#ifdef _DEBUG
		printf("create opaque info structure\n");
#endif // DEBUG

		checkCudaErrors(cusolverSpCreateCsrcholInfoHost(&h_info));

#ifdef _DEBUG
		printf("analyze chol(A) to know structure of L\n");
#endif 
		checkCudaErrors(cusolverSpXcsrcholAnalysisHost(
			cusolverSpH, m_RowNum, m_nnzA,
			Matdescr, h_iRowSort, h_iColIndex,
			h_info));

#ifdef _DEBUG
		printf("workspace for chol(A)\n");
#endif
		checkCudaErrors(cusolverSpDcsrcholBufferInfoHost(
			cusolverSpH, m_RowNum, m_nnzA,
			Matdescr, h_NoneZeroVal, h_iRowSort, h_iColIndex,
			h_info,
			&size_internal,
			&size_chol));

		if (buffer_cpu)
		{
			free(buffer_cpu);
		}
		buffer_cpu = (void*)malloc(sizeof(char)*size_chol);
		assert(NULL != buffer_cpu);

#ifdef _DEBUG
		printf("compute A = L*L^T \n");
#endif
		checkCudaErrors(cusolverSpDcsrcholFactorHost(
			cusolverSpH, m_RowNum, m_nnzA,
			Matdescr, h_NoneZeroVal, h_iRowSort, h_iColIndex,
			h_info,
			buffer_cpu));

#ifdef _DEBUG
		printf("check if the matrix is singular \n");
#endif
		checkCudaErrors(cusolverSpDcsrcholZeroPivotHost(
			cusolverSpH, h_info, m_qaerr1, &singularity));

		stop_spFactorT = GetTimeStamp();

		time_sp_factorT = stop_spFactorT - start_spFactorT;
		std::cout << "factor the matrix A use time: " << time_sp_factorT << "s" << std::endl;

		if (0 <= singularity)
		{
			fprintf(stderr, "Error: A is not invertible, singularity=%d\n", singularity);
			return false;
		}


		return true;
	}

	bool LowLevelCholesky::AnalyzeCholAWithGPU()
	{
		double start_spFactorT, stop_spFactorT;
		start_spFactorT = GetTimeStamp();
#ifdef _DEBUG
		printf(" create opaque info structure\n");
#endif
		checkCudaErrors(cusolverSpCreateCsrcholInfo(&d_info));

#ifdef _DEBUG
		printf("analyze chol(A) to know structure of L\n");
#endif
		checkCudaErrors(cusolverSpXcsrcholAnalysis(
			cusolverSpH, m_RowNum, m_nnzA,
			Matdescr, d_iRowSort, d_iColIndex,
			d_info));

#ifdef _DEBUG
		printf("workspace for chol(A)\n");
#endif
		checkCudaErrors(cusolverSpDcsrcholBufferInfo(
			cusolverSpH, m_RowNum, m_nnzA,
			Matdescr, d_NoneZeroVal, d_iRowSort, d_iColIndex,
			d_info,
			&size_internal,
			&size_chol));

		if (buffer_gpu) {
			checkCudaErrors(cudaFree(buffer_gpu));
		}
		checkCudaErrors(cudaMalloc(&buffer_gpu, sizeof(char)*size_chol));

#ifdef _DEBUG
		printf("compute A = L*L^T \n");
#endif
		checkCudaErrors(cusolverSpDcsrcholFactor(
			cusolverSpH, m_RowNum, m_nnzA,
			Matdescr, d_NoneZeroVal, d_iRowSort, d_iColIndex,
			d_info,
			buffer_gpu));

#ifdef _DEBUG
		printf("check if the matrix is singular \n");
#endif
		checkCudaErrors(cusolverSpDcsrcholZeroPivot(
			cusolverSpH, d_info, m_qaerr1, &singularity));

		stop_spFactorT = GetTimeStamp();

		time_sp_factorT = stop_spFactorT - start_spFactorT;
		std::cout << "factor the matrix A use time: " << time_sp_factorT << "s" << std::endl;

		if (0 <= singularity) {
			fprintf(stderr, "Error: A is not invertible, singularity=%d\n", singularity);
			return false;
		}

		return true;
	}

	HBXDef::DataAloc_t LowLevelCholesky::MemCpy(HBXDef::CopyType_t _temp)
	{
		using namespace HBXDef;

		double start_matrix_copy, stop_matrix_copy;

		if (HBXDef::HostToDevice == _temp)
		{
			//似乎没有什么额外要传输的数据...
			//start_matrix_copy = GetTimeStamp();
			

			//stop_matrix_copy = GetTimeStamp();
		}
		time_MemcpyHostToDev = stop_matrix_copy - start_matrix_copy;
		std::cout << "双共轭梯度法内存拷贝耗时共计:" << stop_matrix_copy - start_matrix_copy << std::endl;
		return BaseConjugate::MemCpy(_temp);
	}


	double LowLevelCholesky::ConjugateWithCPU(const double & _tol, const int & _iter)
	{
		printf("solve A*x = b \n");
		double start_SolveT, stop_SolveT;
		start_SolveT = GetTimeStamp();
		checkCudaErrors(cusolverSpDcsrcholSolveHost(
			cusolverSpH, m_RowNum, h_b, h_x, h_info, buffer_cpu));
		stop_SolveT = GetTimeStamp();
		time_sp_CPUSolve = stop_SolveT - start_SolveT;
		std::cout << "(CPU) solve Ax=b use :" << time_sp_CPUSolve <<"s"<< std::endl;
		//printf("step 8: evaluate residual r = b - A*x (result on CPU)\n");

		return time_sp_CPUSolve;
	}

	double LowLevelCholesky::ConjugateWithGPU(const double & _tol, const int & _iter)
	{
		double start_SolveT, stop_SolveT;
		printf(" solve A*x = b \n");
		start_SolveT = GetTimeStamp();
		checkCudaErrors(cusolverSpDcsrcholSolve(
			cusolverSpH, m_RowNum, d_b, d_x, d_info, buffer_gpu));

		stop_SolveT = GetTimeStamp();
		time_sp_GPUSolve = stop_SolveT - start_SolveT;
		std::cout << "(CPU) solve Ax=b use :" << time_sp_CPUSolve << "s" << std::endl;

		return time_sp_GPUSolve;
	}

	double LowLevelCholesky::CheckNormInf()
	{
		checkCudaErrors(cudaMemcpy(d_r, h_b, sizeof(double)*m_RowNum, cudaMemcpyHostToDevice));

		r_inf = vec_norminf(m_RowNum, h_r);
		m_qaerr1 = r_inf / (A_inf * x_inf);
		printf("(GPU) |b - A*x| = %E \n", r_inf);
		printf("(GPU) |b - A*x|/(|A|*|x|) = %E \n", m_qaerr1);

		return m_qaerr1;
	}

	void LowLevelCholesky::FreeGPUResource()
	{
		BaseConjugate::FreeGPUResource();

		//CPU部分
		if (buffer_cpu) { free(buffer_cpu); }

		if (h_info) { checkCudaErrors(cusolverSpDestroyCsrcholInfoHost(h_info)); }

		//GPU部分
		if (buffer_gpu) { checkCudaErrors(cudaFree(buffer_gpu)); }
		if (d_info) { checkCudaErrors(cusolverSpDestroyCsrcholInfo(d_info)); }

		if (cusolverSpH) { checkCudaErrors(cusolverSpDestroy(cusolverSpH)); }
	}

}

