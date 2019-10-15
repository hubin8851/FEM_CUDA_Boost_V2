#pragma once

#include <ExportDef.h>
#include <HBXPreDef.h>
#include <BoostPreDef.h>
#include <libCUFEM\BaseReader.h>
#include <matlab\mat.h>


#pragma warning(disable: 4996)

namespace HBXFEMDef
{

	//UFget�Ķ�ȡ�࣬������UFget�ļ���matlab��*.mat�ļ�
	//UFgetMatReader��������m_pMatFile�ڣ�����CSR��ʽ�ľ���ṹ����ṩָ�룬����������ڴ�
	class CUFEM_API UFgetMatReader :
		public BaseReader
	{
	private:
		typedef std::map<std::string, mxArray*>::iterator	mx_Map_itr;

		//mat�ļ��У���Ŀ-���� ��ӳ���
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
		UFgetMatReader(const char* _matname = "662_bus.mat", const char* _searchpath = "C:\\Users\\hbx\\Desktop\\SuiteSparse-2.1.1\\UFget");
		virtual ~UFgetMatReader(void);

		//���inp\xml\PW�ļ��ĵ��룬���������������ܸվ����ά�ȵȲ���
		InputFileResult SetInputData();

		//ǿ�ƶϿ�������������
		void terminate() {};

		//��ȡCSR��ʽϡ�����
		HBXDef::_CSRInput<HBXDef::UserCalPrec>* GetStiffMat( bool _bSv = false );

		//���ϵͳ�����Ҷ�����
		void* GetRhsVec( bool _bSv = false );
		
		//���nnA�ĳ���
		int& GetNoneZeroLgt();
		//���rowsort�ĳ��ȣ���rownum+1
		int& GetnALgt();
		//��ȡ��ǰ�����ID�ţ���ΪĳЩ�����CSR��ʽ��rowsort������
		size_t& GetID();

	};


	CUFEM_API BaseReader* InstanceUFgetReader(const char* _name = "", const char* _path = "");
}
