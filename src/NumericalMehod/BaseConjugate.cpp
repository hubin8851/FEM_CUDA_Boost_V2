#include "BaseConjugate.h"
#include <random>
#include <HbxGloFunc.h>
#include <cuda_runtime.h>
#include <CudaPreDef.h>
#include <HBXPreDef.h>
#include <helper_cuda.h>

#include <..\src\NumericalMehod\mmio.h>
#include <..\src\NumericalMehod\mmio_wrapper.h>

namespace HBXFEMDef
{
	using namespace HBXDef;


	void BaseConjugate::genTridiag(size_t _genRowNum, const char* _savepath, bool _bsave )
	{
		using namespace std;
		srand((unsigned)time(NULL));//基于时间的伪随机数分配
		cout << "生成三对角矩阵，原始数据在内存中抹去..." << endl;
		//因为强制设为三对角矩阵，故m_nnA,m_nA,rownum均需要重新赋值
		m_RowNum = (int)_genRowNum;
		m_nnzA = (int)(m_RowNum - 2) * 3 + 4;
		m_nA = (int)m_RowNum + 1;

#pragma region 重新分配内存，并生成随机数
		ResetMem(m_nnzA, m_nA);


#pragma endregion
	}

	void BaseConjugate::genRhs(size_t _genRowNum, bool _bsave, const char* _savepath)
	{
		using std::default_random_engine;
		using std::uniform_real_distribution;

		std::cout << "生成等式右端长度为" << _genRowNum << "随机向量" << std::endl;

		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd());	//Standard mersenne_twister_engine seeded with rd()

		if (0 != h_vNoneZeroVal.size()  )
		{
			h_vRhs.resize(_genRowNum);
			std::vector<HBXDef::UserCalPrec>::iterator max = std::max_element(h_vNoneZeroVal.begin(), h_vNoneZeroVal.end());
			std::vector<double>::iterator min = std::min_element(h_vNoneZeroVal.begin(), h_vNoneZeroVal.end());	
			if (fabs(*max-*min)< 0.00001)//判断最大值和最小值一样，说明矩阵内部的数据为某一特定值，故需要右端做一定的变化
			{
				goto Random01;
			}
			uniform_real_distribution<> dis(*min, *max);
			for (size_t i = 0; i < _genRowNum; i++)
			{
				h_rhs[i] = h_vRhs[i] = dis(gen);
#ifdef _DEBUG
				std::cout << h_vRhs[i] << std::endl;
#endif
			}
		}
		else
		{
			Random01:
			std::cout << "CSR格式矩阵未初始化" << std::endl;
			std::cout << "将默认等式右端初始化为0-1之间数值" << std::endl;
			h_vRhs.resize(_genRowNum);
			uniform_real_distribution<> dis(0, 1);
			for (size_t i = 0; i < _genRowNum; i++)
			{
				h_rhs[i] = h_vRhs[i] = dis(gen);
				std::cout << h_vRhs[i] << std::endl;
			}
		}	

		return;
	}

	BaseConjugate::BaseConjugate(Domain * _dm, Engng * _eng)
//		:BaseNumMethod(_dm, _eng)
	{
		m_nA = (int)m_RowNum + 1;
		m_bGenTridiag = false;
		m_bCudaFree = true;
		m_bGPUFlag = false;
		m_DevID = -1;
		m_DataAloc = HBXDef::INVALID;
		m_bSave = false;				//是否存盘

		memcpy_totalT = 0;
		startT = 0;
		stopT = 0;
		time_sp_analysisT = 0;
		time_sp_solveT = 0;

		//所用流初始化
		m_stream = 0;
		//blas,sparse和matdescr的初始化
		_cuError_id = cudaSuccess;
		_cublasError_id = CUBLAS_STATUS_SUCCESS;
		_cusparseError_id = CUSPARSE_STATUS_SUCCESS;
		cublasHandle = 0;
		cusparseHandle = 0;
		Matdescr = 0;
		m_CSRIndexBase = cusparseIndexBase_t::CUSPARSE_INDEX_BASE_ZERO;

		msecUsed = 0.0f;
		m_iters = 0;

		h_NoneZeroVal = nullptr;
		h_iColIndex = nullptr;
		h_iRowSort = nullptr;
		h_x = nullptr;
		h_rhs = nullptr;
	}

	BaseConjugate::~BaseConjugate()
	{
		if (nullptr == cusparseHandle)
		{
			return;
		}
		else FreeGPUResource();
	}

	void BaseConjugate::ResetMem(int _nnzA, int _nA)
	{
		using namespace HBXDef;
		m_nnzA = _nnzA;
		m_nA = _nA;
		m_RowNum = m_ColNum = _nA - 1;
		if (m_bGenTridiag) return;

		if (nullptr != h_NoneZeroVal)
		{
			delete[] h_NoneZeroVal;
		}
		_cuError_id = cudaHostAlloc((void**)&h_NoneZeroVal, sizeof(UserCalPrec)*m_nnzA, 0);
		if (nullptr != h_iColIndex)
		{
			delete[]h_iColIndex;
			h_iColIndex = nullptr;
		}
		_cuError_id = cudaHostAlloc((void**)&h_iColIndex, sizeof(int)*m_nnzA, 0);
		if (nullptr != h_iRowSort)
		{
			delete[]h_iRowSort;
			h_iRowSort = nullptr;
		}
		_cuError_id = cudaHostAlloc((void**)&h_iRowSort, sizeof(int)*m_nA, 0);
		if (nullptr != h_x)
		{
			delete[]h_x;
			h_x = nullptr;
		}
		_cuError_id = cudaHostAlloc((void**)&h_x, sizeof(UserCalPrec)*m_RowNum, 0);
//		h_x = (UserCalPrec *)malloc(sizeof(UserCalPrec) * m_RowNum);
		/* reset the initial guess of the solution to zero */
		memset(h_x, 0, m_RowNum);
		if (nullptr != h_rhs)
		{
			delete[]h_rhs;
			h_rhs = nullptr;
		}
		_cuError_id = cudaHostAlloc((void**)&h_rhs, sizeof(UserCalPrec)*m_RowNum, 0);
		//	h_rhs = (double *) malloc(sizeof(double) * m_RowNum);	//此为不用锁页内存的地址分配
		memset(h_rhs, 0, m_RowNum);
	}

	void BaseConjugate::ResetGraphMem(HbxCuDef::CudaMalloc_t _cuMac)
	{
		using namespace HBXDef;
		if (HbxCuDef::NORMAL == _cuMac)
		{
#pragma region  裸指针时CSR格式矩阵的显存分配
			//CSR非零值显存分配
			checkCudaErrors( cudaMalloc((void **)&d_NoneZeroVal, m_nnzA * sizeof(double)) );
			
			//CSR列索引值显存分配
			checkCudaErrors( cudaMalloc((void **)&d_iColIndex, m_nnzA * sizeof(int)) );
			
			//CSR每行第一个非零数索引显存分配
			checkCudaErrors( cudaMalloc((void **)&d_iRowSort, m_nA * sizeof(int)) );

			//方程等式右边向量显存分配
			checkCudaErrors( cudaMalloc((void **)&d_r, m_RowNum * sizeof(double)) );

			//待求特征值显存分配
			checkCudaErrors( cudaMalloc((void **)&d_x, m_RowNum * sizeof(double)) );

#pragma  endregion
		}

	}

	HBXDef::DataAloc_t BaseConjugate::MemCpy(HBXDef::CopyType_t _temp)
	{
		using namespace HBXDef;
		if (HBXDef::DeviceToHost == _temp)
		{
			double start_matrix_copy = GetTimeStamp();
			HBXDef::CheckUserDefErrors( cudaMemcpy(h_NoneZeroVal, d_NoneZeroVal, 
										m_nnzA * sizeof(double), cudaMemcpyDeviceToHost) );
			
			HBXDef::CheckUserDefErrors( cudaMemcpy(h_iColIndex, d_iColIndex,
										m_nnzA * sizeof(int), cudaMemcpyDeviceToHost) );
			
			HBXDef::CheckUserDefErrors( cudaMemcpy(h_iRowSort, d_iRowSort,
										m_nA * sizeof(int), cudaMemcpyDeviceToHost) );
			
			HBXDef::CheckUserDefErrors( cudaMemcpy(h_x, d_x, 
										m_RowNum * sizeof(double), cudaMemcpyDeviceToHost) );
			
			HBXDef::CheckUserDefErrors( cudaMemcpy(h_rhs, d_r, 
										m_RowNum * sizeof(double), cudaMemcpyDeviceToHost) );

			double stop_matrix_copy = GetTimeStamp();
			memcpy_totalT += stop_matrix_copy - start_matrix_copy;
			std::cout << "DeviceToHost内存拷贝耗时共计:" << stop_matrix_copy - start_matrix_copy << std::endl;

			return HBXDef::DataAloc_t::DATAINMEM;
		}
		else if (HBXDef::HostToDevice == _temp)
		{
			double start_matrix_copy = HBXDef::GetTimeStamp();
			HBXDef::CheckUserDefErrors( cudaMemcpy(d_NoneZeroVal, h_NoneZeroVal,
										m_nnzA * sizeof(double), cudaMemcpyHostToDevice) );
			
			HBXDef::CheckUserDefErrors( cudaMemcpy(d_iColIndex, h_iColIndex,
										m_nnzA * sizeof(int), cudaMemcpyHostToDevice) );
			
			HBXDef::CheckUserDefErrors( cudaMemcpy(d_iRowSort, h_iRowSort,
										m_nA * sizeof(int), cudaMemcpyHostToDevice) );
			
			HBXDef::CheckUserDefErrors( cudaMemcpy(d_x, h_x,
										m_RowNum * sizeof(double), cudaMemcpyHostToDevice) );
			
			HBXDef::CheckUserDefErrors( cudaMemcpy(d_r, h_rhs,
										m_RowNum * sizeof(double), cudaMemcpyHostToDevice) );
			
			double end_matrix_copy = HBXDef::GetTimeStamp();
			memcpy_totalT += end_matrix_copy - start_matrix_copy;
			std::cout << "基类下系数矩阵及右端向量HostToDevice内存拷贝耗时共计:" << end_matrix_copy - start_matrix_copy << std::endl;
			return HBXDef::DataAloc_t::DATAINGPU;
		}
		else return HBXDef::DataAloc_t::INVALID;
	}

	HBXDef::DataAloc_t BaseConjugate::SetStiffMatFromMTX(char * _mat_filename, const char* _dir)
	{
		if (1)
		{
			std::string _tmppath(_dir);
			_tmppath.append(_mat_filename);
			// 	char *TmpPath = new char[_tmppath.string().size()+1];
			// 	memcpy( TmpPath, _tmppath.string().data(), _tmppath.string().size() );
			// 	TmpPath[_tmppath.string().size()] = '\0';
			if (HBXDef::loadMMSparseMat(_tmppath.c_str(), true, &m_RowNum, &m_ColNum, &m_nnzA, h_vNoneZeroVal, h_viNonZeroRowSort, h_viColSort))
			{
				fprintf(stderr, "!!!! cusparseLoadMMSparseMatrix FAILED\n");
				return HBXDef::DataAloc_t::INVALID;
			}

			ResetMem(m_nnzA, m_RowNum + 1);
			if (0 == h_viColSort[0] && 0 == h_viNonZeroRowSort[0])
			{
				m_CSRIndexBase = CUSPARSE_INDEX_BASE_ZERO;
				std::cout << "压缩格式的矩阵以零为索引起始..." << std::endl;
			}
			else if (1 == h_viColSort[0] && 1 == h_viNonZeroRowSort[0])
			{
				m_CSRIndexBase = CUSPARSE_INDEX_BASE_ONE;
				std::cout << "以一为索引起始..." << std::endl;
			}
			else return HBXDef::DataAloc_t::INVALID;
			//在此需每个元素赋值，因为vector的内存可能不连续
			for (int i = 0; i < m_nnzA; i++)
			{
				h_NoneZeroVal[i] = h_vNoneZeroVal[i];
				h_iColIndex[i] = h_viColSort[i];
			}
			for (int i = 0; i < m_nA; i++)
			{
				h_iRowSort[i] = h_viNonZeroRowSort[i];
			}
		}
		if (0)//另一种方案，sample原生
		{
			int matrixM;
			int matrixN;
			double *Aval = 0;
			int nnz;
			int    *AcolsIndex = 0;
			int    *ArowsIndex = 0;
			/* load the coefficient matrix */
			if (loadMMSparseMatrix(_mat_filename, 'f',
				true, &matrixM, &matrixN, &nnz, &Aval, &ArowsIndex, &AcolsIndex, 0)) {
				fprintf(stderr, "!!!! cusparseLoadMMSparseMatrix FAILED\n");
				return HBXDef::DataAloc_t::INVALID;
			}
			printf("^^^^ M=%d, N=%d, nnz=%d\n", matrixM, matrixN, nnz);
		}
				
		return HBXDef::DataAloc_t::DATAINMEM;
	}

	UserStatusError_t BaseConjugate::SetStiffMatFrom(HBXDef::_CSRInput<HBXDef::UserCalPrec>* _matIn)
	{
		h_vNoneZeroVal.clear();
		h_viColSort.clear();
		h_viNonZeroRowSort.clear();

		return UserStatusError_t::USER_STATUS_SUCCESS;
	}

	HBXDef::DataAloc_t BaseConjugate::SetStiffMat(HBXDef::UserCalPrec *  _srcVal,
													size_t *  _srcCol,
													size_t *  _srcRow, 
													size_t _nnA, size_t _nA, bool _bsave)
	{
		using namespace HBXDef;
//		void *_tmpVal = const_cast<void*>(_srcVal);
//		void *_tmpCol = const_cast<void*>(_srcCol);
//		void *_tmpRow = const_cast<void*>(_srcRow);

//		UserCalPrec *p_NoneZeroVal = static_cast<UserCalPrec*>(_srcVal);
//		size_t *p_iColSort = static_cast<size_t*>(_srcCol);
//		size_t *p_iNonZeroRowSort = static_cast<size_t*>(_srcRow);
		if (m_nnzA!=_nnA && m_nA!=_nA)
		{
			std::cout << "维度与之前初始化数据不一致，重新分配内存！" << std::endl;
			return HBXDef::DataAloc_t::INVALID;
		}
		h_vNoneZeroVal.resize(m_nnzA);
		h_viColSort.resize(m_nnzA);
		h_viNonZeroRowSort.resize(m_nA);
		//在此需每个元素赋值，因为vector的内存可能不连续
		for (int i = 0; i < m_nnzA; i++)
		{
			h_NoneZeroVal[i] = h_vNoneZeroVal[i] = _srcVal[i];
			h_iColIndex[i] = h_viColSort[i] = (int)_srcCol[i];
		}
		for (int i = 0; i < m_nA; i++)
		{
			h_iRowSort[i] = h_viNonZeroRowSort[i] = (int)_srcRow[i];
		}
		return HBXDef::DataAloc_t::DATAINMEM;
	}

	HBXDef::DataAloc_t BaseConjugate::SetStiffMat(const char * _NoneZeroVal, const char * _ColSort, const char * _ValRowSort, const char* _dir)
	{
		if (0 == HBXDef::_ImportCSRdataFromFile<double, int>(h_vNoneZeroVal, h_viColSort, h_viNonZeroRowSort, _NoneZeroVal, _ColSort, _ValRowSort, _dir))
		{
			std::cout << "未能正确读取CSR文件数据..." << std::endl;
			return HBXDef::DataAloc_t::INVALID;
		}
		//矩阵导入后完成自查，检验其内部索引是否出错
		HBXDef::CSRFormatError_t _tmpErr = HBXDef::verify_pattern(h_viNonZeroRowSort.size() - 1, h_viColSort.size(), h_viNonZeroRowSort.data(), h_viColSort.data());
		m_nnzA = (int)h_vNoneZeroVal.size();
		if (m_nnzA == h_vNoneZeroVal.size() && m_nnzA == h_viColSort.size() && m_nA == h_viNonZeroRowSort.size())//m_nA = N+1，在此无误
		{
			std::cout << "正确读取CSR数据,维度正确..." << std::endl;
			ResetMem(m_nnzA, m_nA);
			if (0 == h_viColSort[0] && 0 == h_viNonZeroRowSort[0])
			{
				m_CSRIndexBase = CUSPARSE_INDEX_BASE_ZERO;
				std::cout << "压缩格式的矩阵以零为索引起始..." << std::endl;
			}
			else if (1 == h_viColSort[0] && 1 == h_viNonZeroRowSort[0])
			{
				m_CSRIndexBase = CUSPARSE_INDEX_BASE_ONE;
				std::cout << "以一为索引起始..." << std::endl;
			}
			else return HBXDef::DataAloc_t::INVALID;
			//在此需每个元素赋值，因为vector的内存可能不连续
			for (int i = 0; i < m_nnzA; i++)
			{
				h_NoneZeroVal[i] = h_vNoneZeroVal[i];
				h_iColIndex[i] = h_viColSort[i];
			}
			for (int i = 0; i < m_nA; i++)
			{
				h_iRowSort[i] = h_viNonZeroRowSort[i];
			}
			return	HBXDef::DataAloc_t::DATAINMEM;
		}
		else
		{
			std::cout << "未能正确读取CSR文件数据，可能维度不对..." << std::endl;
			return HBXDef::DataAloc_t::INVALID;
		}
	}


	HBXDef::DataAloc_t BaseConjugate::SetLoadVec(const char * _FileName, const char* _dir)
	{
		size_t _RowNum = HBXDef::ReadVecByRowFromFile(h_vRhs, _FileName, _dir);
		if (0 == _RowNum)
		{
			std::cout << "未读取载荷数据,是否需要生成..." << std::endl;
			char ch;
		there:
			std::cout << "输入Y/N表示生成或不生成右端向量..." << std::endl;
			ch = getchar();
			if (!isalpha(ch))
			{
				std::cout << "请输入字母..." << std::endl;
				goto there;
			}
			else
			{
				switch (ch)
				{
				case 'y':case 'Y':
					genRhs(m_RowNum, false);
					return HBXDef::DataAloc_t::DATAINMEM;
				case  'n':case 'N':
					goto end;
					break;
				default:
					std::cerr << "输入错误,跳出" << std::endl;
					goto end;
				}
			}
		end:
			return HBXDef::DataAloc_t::INVALID;
		}
		else if (m_RowNum == _RowNum)
		{
			std::cout << "载荷数据维度正确..." << std::endl;
			memcpy(h_rhs, h_vRhs.data(), m_RowNum * sizeof(float));
			return HBXDef::DataAloc_t::DATAINMEM;
		}
		std::cout << "载荷数据维度错误..." << std::endl;
		return HBXDef::DataAloc_t::INVALID;
	}

	HBXDef::DataAloc_t BaseConjugate::SetLoadVec(HBXDef::UserCalPrec * _LoadVec, size_t _RowNum)
	{
		if (nullptr == _LoadVec)
		{
			this->genRhs(_RowNum);
			return DataAloc_t::DATAINMEM;
		}

		using namespace HBXDef;
//		void* pVoid = const_cast<void*>(_LoadVec);
//		const UserCalPrec* _tmpload = static_cast<const UserCalPrec*>(_LoadVec);
		h_vRhs.resize(_RowNum);
//		h_rhs = static_cast<UserCalPrec*>(_LoadVec);
		h_rhs = _LoadVec;
		m_nA = (int)_RowNum + 1;
		//在此需每个元素赋值，因为vector的内存可能不连续
		for (int i = 0; i < _RowNum; i++)
		{
			h_vRhs[i] = h_rhs[i];
		}
		return DataAloc_t::DATAINMEM;
	}


	bool BaseConjugate::isDevicePossible() const
	{
		return m_bGPUFlag;
	}

	size_t BaseConjugate::GetGPUDeviceQuery()
	{
		return 0;
	}

	void BaseConjugate::Reset()
	{
		h_vNoneZeroVal.clear();
		h_viColSort.clear();
		h_viNonZeroRowSort.clear();


	}

	UserStatusError_t BaseConjugate::Solve(SparseMat & _A, HBXDef::UserCalPrec & b, HBXDef::UserCalPrec & _x)
	{


		return UserStatusError_t::USER_STATUS_SUCCESS;
	}

	bool BaseConjugate::InitialDescr(cudaStream_t _stream, cusparseMatrixType_t _MatrixType)
	{
		m_stream = _stream;
		/* 创建CUBLAS对象 */
		_cublasError_id = cublasCreate(&cublasHandle);
		if (_cublasError_id != CUBLAS_STATUS_SUCCESS)
		{
			printf("cublasCreate returned %d\n-> %s\n", (int)_cuError_id, cudaGetErrorString(_cuError_id));
			exit(EXIT_FAILURE);
		}
		int version;
		cublasGetVersion(cublasHandle, &version);
		std::cout << "当前cublas版本为:" << version << std::endl;
		/* 创建CUSPARSE对象 */
		_cusparseError_id = cusparseCreate(&cusparseHandle);
		if (_cusparseError_id != CUSPARSE_STATUS_SUCCESS)
		{
			printf("cusparseCreate returned %d\n-> %s\n", (int)_cuError_id, cudaGetErrorString(_cuError_id));
			exit(EXIT_FAILURE);
		}
		//针对cusparse和cublas对象设定流
		if (m_stream)
		{
			checkCudaErrors(cudaStreamCreate(&m_stream));
			if (cublasSetStream(cublasHandle, m_stream) != CUBLAS_STATUS_SUCCESS)
			{
				fprintf(stderr, "!!!! cannot set CUBLAS stream\n");
				return false;
			}
			if (cusparseSetStream(cusparseHandle, m_stream) != CUSPARSE_STATUS_SUCCESS)
			{
				fprintf(stderr, "!!!! cannot set CUSPARSE stream\n");
				return false;
			}
		}

		/* A矩阵的描述器*/
		_cusparseError_id = cusparseCreateMatDescr(&Matdescr);
		if (_cusparseError_id != CUSPARSE_STATUS_SUCCESS)
		{
			printf("cusparseCreateMatDescr returned %d\n-> %s\n", (int)_cuError_id, cudaGetErrorString(_cuError_id));
			exit(EXIT_FAILURE);
		}
		/* 定义矩阵属性*/
		_cusparseError_id = cusparseSetMatType(Matdescr, _MatrixType);
		if (_cusparseError_id != CUSPARSE_STATUS_SUCCESS)
		{
			printf("cusparseSetMatType returned %d\n-> %s\n", (int)_cuError_id, cudaGetErrorString(_cuError_id));
			exit(EXIT_FAILURE);
		}
		_cusparseError_id = cusparseSetMatIndexBase(Matdescr, m_CSRIndexBase);
		if (_cusparseError_id != CUSPARSE_STATUS_SUCCESS)
		{
			printf("cusparseSetMatIndexBase returned %d\n-> %s\n", (int)_cuError_id, cudaGetErrorString(_cuError_id));
			exit(EXIT_FAILURE);
		}
		return true;
	}

	void BaseConjugate::FreeGPUResource()
	{
#pragma region 裸指针版
		if (false == m_bCudaFree)
		{
			/* 释放设备内存 */
			_cusparseError_id = cusparseDestroy(cusparseHandle);
			_cublasError_id = cublasDestroy(cublasHandle);
			_cuError_id = cudaFree(d_NoneZeroVal);
			_cuError_id = cudaFree(d_iColIndex);
			_cuError_id = cudaFree(d_iRowSort);
			_cuError_id = cudaFree(d_x);
			_cuError_id = cudaFree(d_r);

			_cuError_id = cudaDeviceReset();	//重置GPU设备
			m_bCudaFree = true;
		}
		else return;
#pragma endregion
	}



}


