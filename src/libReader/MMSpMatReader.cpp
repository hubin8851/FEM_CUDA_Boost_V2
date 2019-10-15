#include "MMSpMatReader.h"
#include <mmio.h>
#include <mmio_wrapper.h>

namespace HBXFEMDef
{
	MMSpMatReader::MMSpMatReader(	const char * _matname, 
									const char* _searchpath,
									bool _csrFormat):MatSaveType(_csrFormat)
	{
		this->SetSourceFilePath(_matname, _searchpath);

		std::cout << "create MMSpMatReader!" << std::endl;

	}

	MMSpMatReader::~MMSpMatReader(void)
	{

	}

	InputFileResult MMSpMatReader::SetInputData()
	{
		//四位的结构体，表示不同的矩阵类型，比如矩阵的对称性、元素为实数亦或复数
		MM_typecode matcode;
		int error;
		int m, n, nnz;
		int    *trow, *tcol;
		int i, j;
		int count;
		double *tval;

		int    *tempRowInd, *tempColInd;
		double *tempVal;

		struct cooFormat *work;

		std::string _tmpstr = BaseReader::m_path + "\\" + BaseReader::m_SrcFileName;
		char* chr = const_cast<char*>(_tmpstr.c_str());

		/* read the matrix */
		error = mm_read_mtx_crd( chr, &m, &n, &nnz, &trow, &tcol, &tval, &matcode);
		if (error) {
			fprintf(stderr, "!!!! can not open file: '%s'\n", chr);
			return InputFileResult::IRRT_NOTFOUND;
		}

		/* start error checking */
		if (mm_is_complex(matcode) && ((m_ElemetType != 'z') && (m_ElemetType != 'c'))) {
			fprintf(stderr, "!!!! complex matrix requires type 'z' or 'c'\n");
			return InputFileResult::IRRT_BAD_FORMAT;
		}

		//判断矩阵格式是否为稠密、Array形式的矩阵
		if (mm_is_dense(matcode) || mm_is_array(matcode) || mm_is_pattern(matcode) /*|| mm_is_integer(matcode)*/) {
			fprintf(stderr, "!!!! dense, array, pattern and integer matrices are not supported\n");
			return InputFileResult::IRRT_BAD_FORMAT;
		}

		if ( mm_is_symmetric(matcode) || mm_is_hermitian(matcode) || mm_is_skew(matcode) )
		{
			//统计非对角元元素
			count = 0;
			for (i = 0; i < nnz; i++) {
				if (trow[i] != tcol[i]) {//如果行号和列好不一，+1
					count++;
				}
			}
			//为对称阵分配地址空间
			tempRowInd = (int *)malloc((nnz + count) * sizeof(int));
			tempColInd = (int *)malloc((nnz + count) * sizeof(int));
			if (mm_is_real(matcode) || mm_is_integer(matcode)) {
				tempVal = (double *)malloc((nnz + count) * sizeof(double));
			}
			else {
				tempVal = (double *)malloc(2 * (nnz + count) * sizeof(double));
			}

			//copy the elements regular and transposed locations
			for (j = 0, i = 0; i < nnz; i++) 
			{
				tempRowInd[j] = trow[i];
				tempColInd[j] = tcol[i];
				if (mm_is_real(matcode) || mm_is_integer(matcode)) {
					tempVal[j] = tval[i];
				}
				else {
					tempVal[2 * j] = tval[2 * i];
					tempVal[2 * j + 1] = tval[2 * i + 1];
				}
				j++;
				if (trow[i] != tcol[i]) {
					tempRowInd[j] = tcol[i];
					tempColInd[j] = trow[i];
					if (mm_is_real(matcode) || mm_is_integer(matcode)) {
						if (mm_is_skew(matcode)) {
							tempVal[j] = -tval[i];
						}
						else {
							tempVal[j] = tval[i];
						}
					}
					else {
						if (mm_is_hermitian(matcode)) {
							tempVal[2 * j] = tval[2 * i];
							tempVal[2 * j + 1] = -tval[2 * i + 1];
						}
						else {
							tempVal[2 * j] = tval[2 * i];
							tempVal[2 * j + 1] = tval[2 * i + 1];
						}
					}
					j++;
				}
			}
			nnz += count;
			//free temporary storage
			free(trow);
			free(tcol);
			free(tval);
		}
		else 
		{
			tempRowInd = trow;
			tempColInd = tcol;
			tempVal = tval;
		}

		// life time of (trow, tcol, tval) is over.
	// please use COO format (tempRowInd, tempColInd, tempVal)

// use qsort to sort COO format 
		work = (struct cooFormat *)malloc(sizeof(struct cooFormat)*nnz);
		if (NULL == work) {
			fprintf(stderr, "!!!! allocation error, malloc failed\n");
			return InputFileResult::IRRT_BAD_FORMAT;
		}
		for (i = 0; i < nnz; i++) 
		{
			work[i].i = tempRowInd[i];
			work[i].j = tempColInd[i];
			work[i].p = i; // permutation is identity
		}

		if (csrFormat) {
			/* create row-major ordering of indices (sorted by row and within each row by column) */
			qsort(work, nnz, sizeof(struct cooFormat), (FUNPTR)fptr_array[0]);
		}
		else {
			/* create column-major ordering of indices (sorted by column and within each column by row) */
			qsort(work, nnz, sizeof(struct cooFormat), (FUNPTR)fptr_array[1]);

		}
	}


	HBXDef::_CSRInput<HBXDef::UserCalPrec>* MMSpMatReader::GetStiffMat(bool _bSv)
	{
		return &this->m_MatMsg;
	}


	int & MMSpMatReader::GetNoneZeroLgt()
	{
		// TODO: 在此处插入 return 语句
		return m_MatMsg._nnzA;
	}

	int & MMSpMatReader::GetnALgt()
	{
		// TODO: 在此处插入 return 语句
		return m_MatMsg._nA;
	}

	size_t & MMSpMatReader::GetID()
	{
		// TODO: 在此处插入 return 语句
		return this->m_id;
	}


	CUFEM_API BaseReader * InstanceMMSpMatReader(const char* _name, const char* _path)
	{
		return (BaseReader*)new MMSpMatReader(_name, _path);
	}
}


