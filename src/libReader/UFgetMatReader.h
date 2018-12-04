#pragma once

#include <ExportDef.h>
#include <HBXPreDef.h>
#include <BoostPreDef.h>
#include <libCUFEM\BaseReader.h>
#include <matlab\mat.h>


#pragma warning(disable: 4996)

namespace HBXFEMDef
{


	class CUFEM_API UFgeMatReader :
		public BaseReader
	{
	private:
		typedef std::map<std::string, mxArray*>::iterator	mx_Map_itr;

		std::map<std::string, mxArray*>	_mxArray_Map;

		MATFile*	m_pMatFile;	//�ļ�ָ��
		mxArray*	m_pMxArray;	//��������ָ��

		mwSize m_RowNum, m_ColNum;	//�����������
		size_t m_id;				//�����ڱ�׼���е�ID��

		HBXDef::_CSRInput<HBXDef::UserCalPrec> m_MatMsg;

	protected:
		//���ڶ�ȡ��������������Ϣ
		::ImpMatError_t ReadObject(const mxArray *const _Array);
		//���ڶ�ȡUF_Index�ڵ���Ϣ
		::ImpMatError_t ReadMsg(const mxArray* const _Array);
		::ImpMatError_t ReadSparseMat(const mxArray *const _Array);
		::ImpMatError_t ReadRhs(const mxArray *const _Array);
		::ImpMatError_t ReadCell(const mxArray* const _Array);

	public:
		UFgeMatReader(const char* _matname, boost::filesystem::path _searchpath = "C:\\Users\\hbx\\Desktop\\SuiteSparse-2.1.1\\UFget");
		~UFgeMatReader(void);

		//���inp\xml\PW�ļ��ĵ��룬���������������ܸվ����ά�ȵȲ���
		bool SetInputData();

		//ǿ�ƶϿ�������������
		void terminate();

		//��ȡϡ�����ģ�庯��
		void* GetStiffMat( bool _bSv = false );

		//���ϵͳ�����Ҷ�����
		void* GetRhsVec( bool _bSv = false );
		
		//���nnA�ĳ���
		int& GetNoneZeroLgt();
		//���rowsort�ĳ��ȣ���rownum+1
		int& GetnALgt();
		//��ȡ��ǰ�����ID�ţ���ΪĳЩ�����CSR��ʽ��rowsort������
		int& GetID();

	}
}
