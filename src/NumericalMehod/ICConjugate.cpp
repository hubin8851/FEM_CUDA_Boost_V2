#include "ICConjugate.h"


namespace HBXFEMDef
{

	ICConjugate::ICConjugate(Domain * _dm, Engng * _eng)
		:BaseConjugate(_dm, _eng)
	{

	}

	ICConjugate::~ICConjugate()
	{

	}

	void ICConjugate::ResetMem(int _nnzA, int _nA, HbxCuDef::HostMalloc_t _hostAlloc)
	{
		if (-1 != _nnzA || -1 != _nA)
		{
			m_nnzA = _nnzA;
			m_nA = _nA;
			m_RowNum = m_ColNum = _nA - 1;
			m_HostMalloc = _hostAlloc;
		}

		BaseConjugate::ResetMem();

		h_Q = (int*)(*_HostAllocFuncptr)(sizeof(int), m_ColNum);
		h_csrRowPtrB = (int*)(*_HostAllocFuncptr)(sizeof(int), m_nA);
		h_csrColIndB = (int*)(*_HostAllocFuncptr)(sizeof(int), m_nnzA);
		h_csrValB = (HBXDef::UserCalPrec*)malloc(sizeof(HBXDef::UserCalPrec)*m_nnzA);
		h_mapBfromA = (int*)malloc(sizeof(int)*m_nnzA);


	}

	HBXDef::DataAloc_t ICConjugate::MemCpy(HBXDef::CopyType_t _temp)
	{
		printf("step 3: prepare data on device\n");
		using namespace HBXDef;
		double start_matrix_copy, stop_matrix_copy;


		//		if (HBXDef::HostToDevice == _temp)
		//		{
		//			start_matrix_copy = HBXDef::GetTimeStamp();
		//
		//			HBXDef::CheckUserDefErrors(cudaMemcpy(d_r, h_b,
		//				m_RowNum * sizeof(UserCalPrec), cudaMemcpyHostToDevice));
		//
		//			HBXDef::CheckUserDefErrors(cudaMemcpy(d_x, h_x,
		//				m_RowNum * sizeof(UserCalPrec), cudaMemcpyHostToDevice));
		//
		//			stop_matrix_copy = HBXDef::GetTimeStamp();
		//			time_MemcpyHostToDev = stop_matrix_copy - start_matrix_copy;
		//			std::cout << "共轭梯度法内存拷贝: ";//<< time_MemcpyHostToDev << std::endl;
		//		}

		BaseConjugate::MemCpy(_temp);


		return HBXDef::DataAloc_t::DATAINGPU;
	}

	double ICConjugate::Preconditioning(ReorderType_t _type)
	{
#ifdef _DEBUG
		int _OriginBW = BandWidthCal(m_RowNum, h_iRowSort, h_iColIndex, m_CSRIndexBase);
#endif

		// verify if A has symmetric pattern or not
		checkCudaErrors(cusolverSpXcsrissymHost(
			cusolverSpH, m_RowNum, m_nnzA, Matdescr, h_iRowSort, h_iRowSort+1, h_iColIndex, &issym));

			if (!issym)
			{
				printf("Error: A has no symmetric pattern, please use LU or QR \n");
				exit(EXIT_FAILURE);
			}

			if (ReorderType_t::SYMRCM == _type)
			{
				checkCudaErrors(cusolverSpXcsrsymrcmHost(
					cusolverSpH, m_RowNum, m_nnzA,
					Matdescr, h_iRowSort, h_iColIndex,
					h_Q));
			}
			else if (ReorderType_t::SYMAMD == _type)
			{
				checkCudaErrors(cusolverSpXcsrsymamdHost(
					cusolverSpH, m_RowNum, m_nnzA,
					Matdescr, h_iRowSort, h_iColIndex,
					h_Q));
			}
			else
			{
				fprintf(stderr, "Error: is unknown reordering\n");
				return -1;
			}

			// B = Q*A*Q^T
			memcpy(h_csrRowPtrB, h_iRowSort, sizeof(int)*m_nA);
			memcpy(h_csrColIndB, h_iColIndex, sizeof(int)*m_nnzA);

			checkCudaErrors(cusolverSpXcsrperm_bufferSizeHost(
				cusolverSpH, m_RowNum, m_ColNum, m_nnzA,
				Matdescr, h_csrRowPtrB, h_csrColIndB,
				h_Q, h_Q,
				&size_perm));

			if (buffer_cpu)
			{
				free(buffer_cpu);
			}
			buffer_cpu = (void*)malloc(sizeof(char)*size_perm);
			assert(NULL != buffer_cpu);

			// h_mapBfromA = Identity
			for (int j = 0; j < m_nnzA; j++)
			{
				h_mapBfromA[j] = j;
			}
			checkCudaErrors(cusolverSpXcsrpermHost(
				cusolverSpH, m_RowNum, m_ColNum, m_nnzA,
				Matdescr, h_csrRowPtrB, h_csrColIndB,
				h_Q, h_Q,
				h_mapBfromA,
				buffer_cpu));

			// B = A( mapBfromA )
			for (int j = 0; j < m_nnzA; j++)
			{
				h_csrValB[j] = h_NoneZeroVal[h_mapBfromA[j]];
			}
#ifdef _DEBUG
			int _OptBW = BandWidthCal(m_RowNum, h_csrRowPtrB, h_csrColIndB, m_CSRIndexBase);
			printf("Origin BandWidth is: %d\n", _OriginBW);
			printf("Opt BandWidth is: %d\n", _OptBW);
#endif // _DEBUG
			
			// A := B
			memcpy(h_iRowSort, h_csrRowPtrB, sizeof(int)*m_nA);
			memcpy(h_iColIndex, h_csrColIndB, sizeof(int)*m_nnzA);
			memcpy(h_NoneZeroVal, h_csrValB, sizeof(double)*m_nnzA);


		return 0.0;
	}

	bool ICConjugate::InitialDescr(cudaStream_t _stream, cusparseMatrixType_t _MatrixType)
	{
		BaseConjugate::InitialDescr(_stream, _MatrixType);

		double ttl, ttl2;
		checkCudaErrors(cusolverSpCreate(&cusolverSpH));

		return false;
	}

	double ICConjugate::ConjugateWithGPU(const double & _tol, const int & _iter)
	{
		printf("step 6: solve A*x = b on GPU\n");

		double start, stop;
		// d_A and d_b are read-only
		start = HBXDef::GetTimeStamp();
		start = HBXDef::GetTimeStamp();

		checkCudaErrors(cusolverSpDcsrlsvchol(
			cusolverSpH, m_RowNum, m_nnzA,
			Matdescr, d_NoneZeroVal, d_iRowSort, d_iColIndex,
			d_b, _tol, reorder, d_x, &singularity));

		checkCudaErrors(cudaDeviceSynchronize());
		stop = HBXDef::GetTimeStamp();

		time_sp_GPUSolve = stop - start;
		
		if (0 <= singularity)
		{
			printf("WARNING: the matrix is singular at row %d under tol (%E)\n", singularity, _tol);
		}


		return time_sp_GPUSolve;
	}

	double ICConjugate::CheckNormInf(bool _useGPU)
	{
		printf("step 7: evaluate residual r = b - A*x (result on GPU)\n");
		checkCudaErrors(cudaMemcpy(d_r, d_b, sizeof(double)*m_RowNum, cudaMemcpyDeviceToDevice));

		checkCudaErrors(cusparseDcsrmv(cusparseHandle,
			CUSPARSE_OPERATION_NON_TRANSPOSE,
			m_RowNum,
			m_ColNum,
			m_nnzA,
			&minus_one,
			Matdescr,
			d_NoneZeroVal,
			d_iRowSort,
			d_iColIndex,
			d_x,
			&one,
			d_r));

		checkCudaErrors(cudaMemcpy(h_x, d_x, sizeof(double)*m_ColNum, cudaMemcpyDeviceToHost));
		checkCudaErrors(cudaMemcpy(h_r, d_r, sizeof(double)*m_RowNum, cudaMemcpyDeviceToHost));

		return 0.0;
	}

}


