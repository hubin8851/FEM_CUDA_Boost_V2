#include "UFgetMatReader.h"

namespace HBXFEMDef
{
	::ImpMatError_t UFgetMatReader::ReadObject(const mxArray * const _Array)
	{
		//获取当前块内的项目数
		int _nObj = mxGetNumberOfFields(_Array);
		if ( 0 != _nObj)
		{
			//遍历块内项目
			for (int i = 0; i < _nObj; i++)
			{
				mxChar *_tmpchar;
				double *_tmpd;
				mxArray *innerArray = mxGetFieldByNumber_730(_Array, 0, i);
				const char *_fieldName = mxGetFieldNameByNumber(_Array, i);
				mxClassID _tmpid = mxGetClassID(innerArray);

				switch (_tmpid)
				{
				case mxCHAR_CLASS:
					_tmpchar = mxGetChars(innerArray);
					break;
				case mxDOUBLE_CLASS:
					if (mxIsSparse(innerArray))		//获取稀疏矩阵A
					{
						std::cout << "有稀疏矩阵" << std::endl;
						_mxArray_Map.insert(std::make_pair(_fieldName, innerArray));
						ReadSparseMat(innerArray);
					}
					else if (0 == std::strcmp(_fieldName, "b"))	//获取右端向量
					{
						std::cout << "有等式右端向量" << std::endl;
						_mxArray_Map.insert(std::make_pair(_fieldName, innerArray));
						ReadRhs(innerArray);
					}
					else if (0 == std::strcmp(_fieldName, "id"))	//获取ID号
					{
						_mxArray_Map.insert(std::make_pair(_fieldName, innerArray));
						m_id = *mxGetPr(innerArray);
					}
					_tmpd = mxGetPr(innerArray);
					break;
				case mxSTRUCT_CLASS:	//aux的情形
					//待定
					break;
				case mxOBJECT_CLASS:
					break;
				default:
					break;
				}

				ReadObject(innerArray);
			}
			return ::ImpMatError::IMPSUCCESS;
		}


		return ::ImpMatError_t::MATFILENULL;
	}

	::ImpMatError_t UFgetMatReader::ReadMsg(const mxArray * const _Array)
	{
		using namespace boost;
		int _nObj = mxGetNumberOfFields(_Array);
		if (0 != _nObj)
		{
			for (int i = 0; i < _nObj; i++)
			{
				mxChar *_tmpchar;
				double* _tmpd, _tmpTimeStamp;
				mxArray *innerArray = mxGetFieldByNumber_730(_Array, 0, i);
				const char *_fieldName = mxGetFieldNameByNumber(_Array, i);
				mxClassID _tmpid = mxGetClassID(innerArray);

				if (iequals("LastRevisionDate", _fieldName))
				{
					_tmpchar = mxGetChars(innerArray);
				}
				else if (iequals("DownloadTimeStamp", _fieldName))
				{
					_tmpTimeStamp = *mxGetPr(innerArray);
				}
				else if (iequals("group", _fieldName))
				{
					ReadCell(innerArray);
				}

			}
		}
		return ::ImpMatError_t::IMPSUCCESS;
	}

	::ImpMatError_t UFgetMatReader::ReadSparseMat(const mxArray * const _Array)
	{
		m_RowNum = mxGetM(_Array);
		m_ColNum = mxGetN(_Array);
		if (m_RowNum != m_ColNum)
		{
			std::cout << "矩阵不为方阵..."<<std::endl;
		}
//		std::cout << "矩阵为square，对全局变量g_MatRowNum赋值..." << std::endl;
//		std::cout << "当前矩阵维度为" << m_RowNum << std::endl;
		//以下确定维度
//		g_MatRowNum = (int)m_RowNum;
		m_MatMsg._nA = (int)(m_RowNum)+1;
		m_MatMsg._nnzA = (int)mxGetNzmax(_Array) - 1;
		m_MatMsg.h_NoneZeroVal = mxGetPr(_Array);
		m_MatMsg.h_iColSort = mxGetIr_700(_Array); //rowsort = nna_z,用的老版的mxGetIr
		m_MatMsg.h_iNonZeroRowSort = mxGetJc_700(_Array); //colsort = N+1,用的老版

//		SetStiffMat(m_MatMsg.h_NoneZeroVal, m_MatMsg.h_iColSort, m_MatMsg.h_iNonZeroRowSort, m_nnzA, m_nA);
		return ::ImpMatError_t::IMPSUCCESS;
	}

	::ImpMatError_t UFgetMatReader::ReadRhs(const mxArray * const _Array)
	{
		m_MatMsg.h_rhs = mxGetPr(_Array);
		return ::ImpMatError_t::IMPSUCCESS;
	}

	::ImpMatError_t UFgetMatReader::ReadCell(const mxArray * const _Array)
	{
		return ::ImpMatError_t();
	}



	UFgetMatReader::UFgetMatReader(const char * _matname, const char* _searchpath)
	{
		this->SetSourceFilePath(_matname, _searchpath);

		std::cout << "create UFgeMatReader!" << std::endl;

	}

	UFgetMatReader::~UFgetMatReader(void)
	{
	}

	InputFileResult UFgetMatReader::SetInputData()
	{
		int _ndir = -1;	//根目录下mxArray数目
		int _nfield = -1;	//mxArray下file数目

		std::string _tmpstr = BaseReader::m_path + "\\" + BaseReader::m_SrcFileName;

		m_pMatFile = matOpen(_tmpstr.c_str(), "r" );
		if (nullptr == m_pMatFile)
		{
			std::cerr << "当前路径索引不正确未找到mat文件" << std::endl;
			return InputFileResult::IRRT_NOTFOUND;
		}
		const char** dir = (const char **)matGetDir(m_pMatFile, &_ndir);
		//这里逻辑有些问题，应该用do while
		if ( nullptr == dir )	//判定是否读取正常
		{
			if ( _ndir < 0 )
			{
				std::cerr << "读取过程发生错误" << std::endl;
				return InputFileResult::IRRT_BAD_FORMAT;
			}
			else
			{
				std::cerr << " 当前mat下无任何矩阵 " << std::endl;
				return InputFileResult::IRRT_BAD_FORMAT;
			}
		}
		//判断是否为mat数据
		else if (boost::iequals("problem", dir[0]))
		{
			for (int i = 0; i < _ndir; i++)
			{
				m_pMxArray = matGetVariable(m_pMatFile, dir[i]);
				ReadObject(m_pMxArray);
			}
		}
		//判断是否为UFget数据库索引文件
		else if (boost::iequals("UF_Index", dir[0]))
		{
			for (int i = 0; i < _ndir; i++)
			{
				m_pMxArray = matGetVariable(m_pMatFile, dir[i]);
				ReadMsg(m_pMxArray);
			}
		}
		return InputFileResult::IRRT_OK;
	}

	HBXDef::_CSRInput<HBXDef::UserCalPrec>* UFgetMatReader::GetStiffMat(bool _bSv)
	{
		return &this->m_MatMsg;
	}

	void * UFgetMatReader::GetRhsVec(bool _bSv)
	{
		return nullptr;
	}

	int & UFgetMatReader::GetNoneZeroLgt()
	{
		// TODO: 在此处插入 return 语句
		return m_MatMsg._nnzA;
	}

	int & UFgetMatReader::GetnALgt()
	{
		// TODO: 在此处插入 return 语句
		return m_MatMsg._nA;
	}

	size_t & UFgetMatReader::GetID()
	{
		// TODO: 在此处插入 return 语句
		return this->m_id;
	}


	CUFEM_API BaseReader * InstanceUFgetReader(const char* _name, const char* _path)
	{
		return (BaseReader*)new UFgetMatReader(_name, _path);
	}

}

