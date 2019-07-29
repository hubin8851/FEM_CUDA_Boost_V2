#include "RefactorConjugate.h"
#include <helper_cuda.h>
#include <helper_cusolver.h>
#include "helper_hbx.h"

namespace HBXFEMDef
{
	double RefactorConjugate::GetB()
	{

		fprintf(stdout,"step 3: B = Q*A*Q^T\n");
		memcpy(h_csrRowPtrB, h_iRowSort, sizeof(int)*(m_RowNum + 1));
		memcpy(h_csrColIndB, h_iColIndex, sizeof(int)*m_nnzA);

		startT = GetTimeStamp();

		checkCudaErrors(cusolverSpXcsrperm_bufferSizeHost(
			cusolverSpH, m_RowNum, m_ColNum, m_nnzA,
			Matdescr, h_csrRowPtrB, h_csrColIndB,
			h_Qreorder, h_Qreorder,
			&size_perm));

		if (buffer_cpu) {
			free(buffer_cpu);
		}
		buffer_cpu = (void*)malloc(sizeof(char)*size_perm);
		assert(nullptr != buffer_cpu);

		// h_mapBfromA = Identity 
		for (int j = 0; j < m_nnzA; j++) {
			h_mapBfromA[j] = j;
		}
		checkCudaErrors(cusolverSpXcsrpermHost(
			cusolverSpH, m_RowNum, m_ColNum, m_nnzA,
			Matdescr, h_csrRowPtrB, h_csrColIndB,
			h_Qreorder, h_Qreorder,
			h_mapBfromA,
			buffer_cpu));

		// B = A( mapBfromA )
		for (int j = 0; j < m_nnzA; j++) {
			h_csrValB[j] = h_NoneZeroVal[h_mapBfromA[j]];
		}

		stopT = GetTimeStamp();
		time_permT = stopT - startT;
		fprintf(stdout, " B = Q*A*Q^T : %f sec\n", time_permT);

		return time_permT;
	}


	RefactorConjugate::RefactorConjugate(Domain * _dm, Engng * _eng)
		:BaseConjugate(_dm, _eng)
	{

	}

	RefactorConjugate::~RefactorConjugate()
	{

	}

	void RefactorConjugate::ResetMem(int _nnzA, int _nA)
	{
		BaseConjugate::ResetMem(_nnzA, _nA);

		h_Qreorder = (int*)malloc(sizeof(int)*m_ColNum);

		h_csrRowPtrB = (int*)malloc(sizeof(int)*(m_RowNum + 1));
		h_csrColIndB = (int*)malloc(sizeof(int)*_nnzA);
		h_csrValB = (double*)malloc(sizeof(double)*_nnzA);
		h_mapBfromA = (int*)malloc(sizeof(int)*_nnzA);

		h_b = (double*)malloc(sizeof(double)*m_RowNum);
		h_xhat = (double*)malloc(sizeof(double)*m_ColNum);
		h_bhat = (double*)malloc(sizeof(double)*m_RowNum);

	}

	void RefactorConjugate::ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac)
	{
		BaseConjugate::ResetGraphMem(_cuMac);
		assert(nullptr != h_Qreorder);
		assert(nullptr != h_csrRowPtrB);
		assert(nullptr != h_csrColIndB);
		assert(nullptr != h_csrValB);
		assert(nullptr != h_mapBfromA);

		assert(nullptr != h_x);
		assert(nullptr != h_b);
		assert(nullptr != h_rhs);
		assert(nullptr != h_xhat);
		assert(nullptr != h_bhat);

		if (HbxCuDef::NORMAL == _cuMac)
		{
			checkCudaErrors(cudaMalloc((void **)&d_b, sizeof(double)*m_RowNum));
			checkCudaErrors(cudaMalloc((void **)&d_P, sizeof(int)*m_RowNum));
			checkCudaErrors(cudaMalloc((void **)&d_Q, sizeof(int)*m_ColNum));
			checkCudaErrors(cudaMalloc((void **)&d_T, sizeof(double)*m_RowNum * 1));

		}
		cudaDeviceSynchronize();
		m_bCudaFree = false;
	}

	HBXDef::DataAloc_t RefactorConjugate::MemCpy(HBXDef::CopyType_t _temp)
	{
		using namespace HBXDef;
		double start_matrix_copy, stop_matrix_copy;


		if (HBXDef::HostToDevice == _temp)
		{
			start_matrix_copy = GetTimeStamp();

			stop_matrix_copy = GetTimeStamp();
		}

		std::cout << "双共轭梯度法内存拷贝耗时共计:" << stop_matrix_copy - start_matrix_copy << std::endl;
		return BaseConjugate::MemCpy(_temp);
	}

	bool RefactorConjugate::InitialDescr(cudaStream_t _stream, cusparseMatrixType_t _MatrixType)
	{
		m_stream = _stream;
		BaseConjugate::InitialDescr(_stream, _MatrixType);

		checkCudaErrors(cusolverSpCreate(&cusolverSpH));
		checkCudaErrors(cusparseCreate(&cusparseHandle));
		checkCudaErrors(cudaStreamCreate(&m_stream));

		checkCudaErrors(cusolverSpSetStream(cusolverSpH, m_stream));
		checkCudaErrors(cusparseSetStream(cusparseHandle, m_stream));

		checkCudaErrors(cusparseCreateMatDescr(&Matdescr));
		checkCudaErrors(cusparseSetMatType(Matdescr, CUSPARSE_MATRIX_TYPE_GENERAL));

		if (m_CSRIndexBase)
		{
			checkCudaErrors(cusparseSetMatIndexBase(Matdescr, CUSPARSE_INDEX_BASE_ONE));
		}
		else
		{
			checkCudaErrors(cusparseSetMatIndexBase(Matdescr, CUSPARSE_INDEX_BASE_ZERO));
		}


		return true;
	}

	double RefactorConjugate::Preconditioning(ReorderType_t _type)
	{
		printf("step 1.2: set right hand side vector (b) to 1\n");
		for (int row = 0; row < m_RowNum; row++) {
			h_rhs[row] = 1.0;
		}

		fprintf(stdout, "step 2: reorder the matrix to reduce zero fill-in\n");
		fprintf(stdout, "        Q = symrcm(A) or Q = symamd(A) \n");

		startT = GetTimeStamp();

		if (SYMRCM == _type )
		{
			checkCudaErrors(cusolverSpXcsrsymrcmHost(
				cusolverSpH, m_RowNum, m_nnzA,
				Matdescr, h_iRowSort, h_iColIndex,
				h_Qreorder));
		}
		else if (SYMAMD == _type)
		{
			checkCudaErrors(cusolverSpXcsrsymamdHost(
				cusolverSpH, m_RowNum, m_nnzA,
				Matdescr, h_iRowSort, h_iColIndex,
				h_Qreorder));
		}
		else
		{
			fprintf(stderr, "Error: unknow reordering\n");
			return -1;
		}
		stopT = GetTimeStamp();
		time_reorderT = stopT - startT;

		return time_reorderT;
	}

	double RefactorConjugate::ConjugateWithGPU(const double & _tol, const int & _iter)
	{
		fprintf(stdout,"step 4: solve A*x = b by LU(B) in cusolverSp\n");

		printf("step 4.1: create opaque info structure\n");
		checkCudaErrors(cusolverSpCreateCsrluInfoHost(&info));

		printf("step 4.2: analyze LU(B) to know structure of Q and R, and upper bound for nnz(L+U)\n");
		startT = GetTimeStamp();

		checkCudaErrors(cusolverSpXcsrluAnalysisHost(
			cusolverSpH, m_RowNum, m_nnzA,
			Matdescr, h_csrRowPtrB, h_csrColIndB,
			info));

		stopT = GetTimeStamp();
		time_sp_analysisT = stopT - startT;

		printf("step 4.3: workspace for LU(B)\n");
		checkCudaErrors(cusolverSpDcsrluBufferInfoHost(
			cusolverSpH, m_RowNum, m_nnzA,
			Matdescr, h_csrValB, h_csrRowPtrB, h_csrColIndB,
			info,
			&size_internal,
			&size_lu));

		if (buffer_cpu) {
			free(buffer_cpu);
			}
		buffer_cpu = (void*)malloc(sizeof(char)*size_lu);
		assert(NULL != buffer_cpu);

		printf("step 4.4: compute Ppivot*B = L*U \n");

		startT = GetTimeStamp();

		checkCudaErrors(cusolverSpDcsrluFactorHost(
			cusolverSpH, m_RowNum, m_nnzA,
			Matdescr, h_csrValB, h_csrRowPtrB, h_csrColIndB,
			info, pivot_threshold,
			buffer_cpu));

		stopT = GetTimeStamp();
		time_sp_factorT = stopT - startT;

		// TODO: check singularity by tol
		printf("step 4.5: check if the matrix is singular \n");
		checkCudaErrors(cusolverSpDcsrluZeroPivotHost(
			cusolverSpH, info, tol, &singularity));

		if (0 <= singularity) {
			fprintf(stderr, "Error: A is not invertible, singularity=%d\n", singularity);
			return 1;
		}

		printf("step 4.6: solve A*x = b \n");
		printf("    i.e.  solve B*(Qx) = Q*b \n");
		startT = GetTimeStamp();

		// b_hat = Q*b
		for (int j = 0; j < m_RowNum; j++) {
			h_bhat[j] = h_rhs[h_Qreorder[j]];
		}
		// B*x_hat = b_hat
		checkCudaErrors(cusolverSpDcsrluSolveHost(
			cusolverSpH, m_RowNum, h_bhat, h_xhat, info, buffer_cpu));

		// x = Q^T * x_hat
		for (int j = 0; j < m_RowNum; j++) {
			h_x[h_Qreorder[j]] = h_xhat[j];
		}

		stopT = GetTimeStamp();
		time_sp_solveT = stopT - startT;

		printf("step 4.7: evaluate residual r = b - A*x (result on CPU)\n");
		// use GPU gemv to compute r = b - A*x
		checkCudaErrors(cudaMemcpy(d_iRowSort, h_iRowSort, sizeof(int)*(m_RowNum + 1), cudaMemcpyHostToDevice));
		checkCudaErrors(cudaMemcpy(d_iColIndex, h_iColIndex, sizeof(int)*m_nnzA, cudaMemcpyHostToDevice));
		checkCudaErrors(cudaMemcpy(d_NoneZeroVal, h_NoneZeroVal, sizeof(double)*m_nnzA, cudaMemcpyHostToDevice));

		checkCudaErrors(cudaMemcpy(d_r, h_b, sizeof(double)*m_RowNum, cudaMemcpyHostToDevice));
		checkCudaErrors(cudaMemcpy(d_x, h_x, sizeof(double)*m_ColNum, cudaMemcpyHostToDevice));

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

		checkCudaErrors(cudaMemcpy(h_rhs, d_r, sizeof(double)*m_RowNum, cudaMemcpyDeviceToHost));

		x_inf = vec_norminf(m_ColNum, h_x);
		r_inf = vec_norminf(m_RowNum, h_rhs);
		A_inf = csr_mat_norminf(m_RowNum, m_ColNum, m_nnzA, Matdescr, h_NoneZeroVal, h_iRowSort, h_iColIndex);

		printf("(CPU) |b - A*x| = %E \n", r_inf);
		printf("(CPU) |A| = %E \n", A_inf);
		printf("(CPU) |x| = %E \n", x_inf);
		printf("(CPU) |b - A*x|/(|A|*|x|) = %E \n", r_inf / (A_inf * x_inf));

		return 0.0;
	}


	double RefactorConjugate::ExtractPQLU()
	{
		printf("step 5: extract P, Q, L and U from P*B*Q^T = L*U \n");
		printf("        L has implicit unit diagonal\n");
		startT = second();

		checkCudaErrors(cusolverSpXcsrluNnzHost(
			cusolverSpH,
			&nnzL,
			&nnzU,
			info));
		h_Plu = (int*)malloc(sizeof(int)*m_RowNum);
		h_Qlu = (int*)malloc(sizeof(int)*m_ColNum);

		h_csrValL = (double*)malloc(sizeof(double)*nnzL);
		h_csrRowPtrL = (int*)malloc(sizeof(int)*(m_RowNum + 1));
		h_csrColIndL = (int*)malloc(sizeof(int)*nnzL);

		h_csrValU = (double*)malloc(sizeof(double)*nnzU);
		h_csrRowPtrU = (int*)malloc(sizeof(int)*(m_RowNum + 1));
		h_csrColIndU = (int*)malloc(sizeof(int)*nnzU);

		assert(NULL != h_Plu);
		assert(NULL != h_Qlu);

		assert(NULL != h_csrValL);
		assert(NULL != h_csrRowPtrL);
		assert(NULL != h_csrColIndL);

		assert(NULL != h_csrValU);
		assert(NULL != h_csrRowPtrU);
		assert(NULL != h_csrColIndU);

		checkCudaErrors(cusolverSpDcsrluExtractHost(
			cusolverSpH,
			h_Plu,
			h_Qlu,
			Matdescr,
			h_NoneZeroVal,
			h_iRowSort,
			h_iColIndex,
			Matdescr,
			h_csrValU,
			h_csrRowPtrU,
			h_csrColIndU,
			info,
			buffer_cpu));

		stopT = second();
		time_sp_extractT = stopT - startT;

		printf("nnzL = %d, nnzU = %d\n", nnzL, nnzU);
	}

}

