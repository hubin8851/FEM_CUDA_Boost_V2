#pragma once

#include <ExportDef.h>
#include <HBXPreDef.h>
#include <BoostPreDef.h>
#include <libCUFEM\BaseReader.h>


#pragma warning(disable: 4996)

namespace HBXFEMDef
{

	//Matrix Market�����SparseMat�Ķ�ȡ
	class CUFEM_API MMSpMatReader :
		public BaseReader
	{
	private:
		char m_ElemetType;
		bool MatSaveFormat;
		HBXDef::_CSRInput<HBXDef::UserCalPrec> m_MatMsg;

	protected:
	public:
		MMSpMatReader(	const char* _matname = "bcsstk01.mtx", 
						const char* _searchpath = "E:\\Phd_Hu_Data\\matrix market",
						bool _csrFormat = true);
		virtual ~MMSpMatReader(void);

		//���mtx�ļ��ĵ��룬�Ծ���ά�Ƚ���У�鲢����ϡ�����ṹ����
		InputFileResult SetInputData();

		//ǿ�ƶϿ�������������
		void terminate() {};

		//��ȡCSR��ʽϡ�����
		HBXDef::_CSRInput<HBXDef::UserCalPrec>* GetStiffMat(bool _bSv = false);

		//���nnA�ĳ���
		int& GetNoneZeroLgt();
		//���rowsort�ĳ��ȣ���rownum+1
		int& GetnALgt();
	};

	CUFEM_API BaseReader* InstanceMMSpMatReader(const char* _name = "", const char* _path = "");



}
