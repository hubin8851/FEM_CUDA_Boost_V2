#include <regex>
#include "MMSpMatReader.h"
#include <mmio.h>
#include <mmio_wrapper.h>



namespace HBXFEMDef
{
	MMSpMatReader::MMSpMatReader(	const char * _matname, 
									const char* _searchpath,
									bool _csrFormat):MatSaveFormat(_csrFormat)
	{
		this->SetSourceFilePath(_matname, _searchpath);

		std::cout << "create MMSpMatReader!" << std::endl;

	}

	MMSpMatReader::~MMSpMatReader(void)
	{

	}

	InputFileResult MMSpMatReader::SetInputData()
	{
		int rowsA = 0; // number of rows of A
		int colsA = 0; // number of columns of A
		int nnzA = 0; // number of nonzeros of A
		int baseA = 0; // base index in CSR format

		// CSR(A) from I/O
		int *h_csrRowPtrA = NULL; // <int> n+1 
		int *h_csrColIndA = NULL; // <int> nnzA 
		double *h_csrValA = NULL; // <double> nnzA 

		std::string _tmpstr = BaseReader::m_path + "\\" + BaseReader::m_SrcFileName;
		char* chr = const_cast<char*>(_tmpstr.c_str());

		
		if (!this->m_SrcFileName.empty())
		{
			if (loadMMSparseMatrix<HBXDef::UserReadPrec>(chr, 'd', true, &rowsA, &colsA,
				&nnzA, &h_csrValA, &h_csrRowPtrA, &h_csrColIndA, true))
			{
				return InputFileResult::IRRT_BAD_FORMAT;
			}
			baseA = h_csrRowPtrA[0]; // baseA = {0,1}
		}

		if (rowsA != colsA)
		{
			fprintf(stderr, "Error: only support square matrix\n");
			return InputFileResult::IRRT_BAD_FORMAT;
		}

		printf("WARNING: cusolverRf only works for base-0 \n");
		if (baseA)
		{
			for (int i = 0; i <= rowsA; i++)
			{
				h_csrRowPtrA[i]--;
			}
			for (int i = 0; i < nnzA; i++)
			{
				h_csrColIndA[i]--;
			}
			baseA = 0;
		}

		this->m_MatMsg.h_iColSort = h_csrColIndA;
		this->m_MatMsg.h_iNonZeroRowSort = h_csrRowPtrA;
		this->m_MatMsg.h_NoneZeroVal = h_csrValA;
		this->m_MatMsg._nA = rowsA+1;
		this->m_MatMsg._nnzA = nnzA;

		printf("sparse matrix A is %d x %d with %d nonzeros, base=%d\n", rowsA, colsA, nnzA, baseA);
		return InputFileResult::IRRT_OK;
	}


	InputFileResult MMSpMatReader::SetRhsData(	const char* _matname,
												const char* _searchpath)
	{
		std::string stringLine;		//读取的当前行
		int i = 0;	//计数器，表示当前的列数

		std::string _tmpstr(_searchpath);
		_tmpstr.append("\\");
		_tmpstr.append(_matname);
		char* chr = const_cast<char*>(_tmpstr.c_str());

		std::ifstream inFile;
		inFile.open(chr);
		if (inFile.bad() || !inFile.is_open())
		{
			std::cerr << "Error1： Can not open input data file" << std::endl;
			return InputFileResult::IRRT_BAD_FORMAT;
		}

		while (!inFile.eof())
		{
			std::regex	annotRule("^%\\w+");
			bool  getnum  = false;

			getline(inFile, stringLine);
			std::stringstream ss(stringLine);
			HBXDef::UserCalPrec _tmpNum;

			if (std::regex_match(stringLine, annotRule))	//判断，若为%开头直接跳过
				continue;

			if (!getnum)
			{				
				std::vector<int> props;
				int x;
				while (ss >> x)
				{
					props.push_back(x);
				}
				m_MatMsg.h_rhs = (HBXDef::UserCalPrec*)malloc( sizeof(HBXDef::UserCalPrec)* props[0] );
				getnum = true;
				continue;
			}
			ss >> _tmpNum;
			m_MatMsg.h_rhs[i] = _tmpNum;	//这里没检测容易越界，然后报错像CXK
			i++;
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



	CUFEM_API BaseReader * InstanceMMSpMatReader(const char* _name, const char* _path)
	{
		return (BaseReader*)new MMSpMatReader(_name, _path);
	}
}


