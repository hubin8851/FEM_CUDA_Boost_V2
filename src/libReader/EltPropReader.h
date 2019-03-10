#pragma once
#include <HBXPreDef.h>
#include <ExportDef.h>
#include <tinyxml\tinyxml.h>
#include <tinyxml\tinyxml2.h>
#include <libCUFEM\BaseReader.h>
#include <libCUFEM\ElemtProp.h>



namespace HBXFEMDef
{
	class CUFEM_API EltPropReader:
		public BaseReader
	{
	private:
		HBXFEMDef::_EltPtyInMap	m_ElemtPtyInMap;
	protected:
		InputFileResult	ReadElemtProperty();		//��ȡ���еı�ű���
	public:
		EltPropReader(const std::string& _SourceFile = "EltProperty.xml",
							std::string _savepath = "..\\");	//���캯��,Ĭ��·��Ϊ��Ŀ¼

		HBXFEMDef::_EltPtyInMap* const GetPtyMap();

		//��������·��������ݵ�ǰ����·���������ļ����������������ĵ�һ����ǰ�ļ�����
		bool SetSourceFilePath(const std::string _SourceFile, std::string _savepath);

		InputFileResult SetInputData();

		//ǿ�ƶϿ�������������
		void terminate();
	};


	CUFEM_API BaseReader* InstanceElemtPropReader();
}
