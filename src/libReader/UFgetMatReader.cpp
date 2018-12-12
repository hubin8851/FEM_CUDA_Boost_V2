#include "UFgetMatReader.h"

namespace HBXFEMDef
{
	::ImpMatError_t UFgeMatReader::ReadObject(const mxArray * const _Array)
	{
		//��ȡ��ǰ���ڵ���Ŀ��
		int _nObj = mxGetNumberOfFields(_Array);
		if ( 0 != _nObj)
		{
			//����������Ŀ
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
					if (mxIsSparse(innerArray))		//��ȡϡ�����A
					{
						std::cout << "��ϡ�����" << std::endl;
						_mxArray_Map.insert(std::make_pair(_fieldName, innerArray));
						ReadSparseMat(innerArray);
					}
					else if (0 == std::strcmp(_fieldName, "b"))	//��ȡ�Ҷ�����
					{
						std::cout << "�е�ʽ�Ҷ�����" << std::endl;
						_mxArray_Map.insert(std::make_pair(_fieldName, innerArray));
						ReadRhs(innerArray);
					}
					else if (0 == std::strcmp(_fieldName, "id"))	//��ȡID��
					{
						_mxArray_Map.insert(std::make_pair(_fieldName, innerArray));
						m_id = *mxGetPr(innerArray);
					}
					_tmpd = mxGetPr(innerArray);
					break;
				case mxSTRUCT_CLASS:	//aux������
					//����
					break;
				case mxOBJECT_CLASS:
					break;
				default:
					break;
				}

				ReadObject(innerArray);
			}
		}


		return ::ImpMatError_t::IMPSUCCESS;
	}

	::ImpMatError_t UFgeMatReader::ReadMsg(const mxArray * const _Array)
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

	::ImpMatError_t UFgeMatReader::ReadSparseMat(const mxArray * const _Array)
	{
		m_RowNum = mxGetM(_Array);
		m_ColNum = mxGetN(_Array);
		if (m_RowNum != m_ColNum)
		{
			std::cout << "����Ϊ����..."<<std::endl;
		}
//		std::cout << "����Ϊsquare����ȫ�ֱ���g_MatRowNum��ֵ..." << std::endl;
//		std::cout << "��ǰ����ά��Ϊ" << m_RowNum << std::endl;
		//����ȷ��ά��
//		g_MatRowNum = (int)m_RowNum;
		m_MatMsg._nA = (int)(m_RowNum)+1;
		m_MatMsg._nnzA = (int)mxGetNzmax(_Array) - 1;
		m_MatMsg.h_NoneZeroVal = mxGetPr(_Array);
		m_MatMsg.h_iColSort = mxGetIr(_Array); //rowsort = nna_z
		m_MatMsg.h_iNonZeroRowSort = mxGetJc(_Array); //colsort = N+1

//		SetStiffMat(m_MatMsg.h_NoneZeroVal, m_MatMsg.h_iColSort, m_MatMsg.h_iNonZeroRowSort, m_nnzA, m_nA);
		return ::ImpMatError_t::IMPSUCCESS;
	}



	bool UFgeMatReader::SetInputData()
	{
		int _ndir = -1;	//��Ŀ¼��mxArray��Ŀ
		int _nfield = -1;	//mxArray��file��Ŀ

		m_pMatFile = matOpen( BaseReader::m_path.c_str(), "r" );
		if (nullptr == m_pMatFile)
		{
			std::cerr << "��ǰ·����������ȷδ�ҵ�mat�ļ�" << std::endl;
			return false;
		}
		const char** dir = (const char **)matGetDir(m_pMatFile, &_ndir);
		//�����߼���Щ���⣬Ӧ����do while
		if ( nullptr == dir )	//�ж��Ƿ��ȡ����
		{
			if ( _ndir < 0 )
			{
				std::cerr << "��ȡ���̷�������" << std::endl;
				return ImpMatError_t::MATFILENULL;
			}
			else
			{
				std::cerr << " ��ǰmat�����κξ��� " << std::endl;
			}
		}
		//�ж��Ƿ�Ϊmat����
		else if (boost::iequals("problem", dir[0]))
		{
			for (int i = 0; i < _ndir; i++)
			{
				m_pMxArray = matGetVariable(m_pMatFile, dir[i]);
				ReadObject(m_pMxArray);
			}
		}
		//�ж��Ƿ�ΪUFget���ݿ������ļ�
		else if (boost::iequals("UF_Index", dir[0]))
		{
			for (int i = 0; i < _ndir; i++)
			{
				m_pMxArray = matGetVariable(m_pMatFile, dir[i]);
				ReadMsg(m_pMxArray);
			}
		}


		return true;
	}

}
