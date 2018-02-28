#pragma once
#include <HBXPreDef.h>
#include <tinyxml\tinyxml.h>
#include <tinyxml\tinyxml2.h>
#include <libCUFEM\BaseReader.h>
#include <libCUFEM\ElemtProp.h>

using namespace std;


namespace HBXFEMDef
{
	class BOOST_SYMBOL_EXPORT EltPropReader:
		public BaseReader
	{
	private:
		HBXFEMDef::_EltPtyInMap	m_ElemtPtyInMap;
	protected:
		InputFileResult	ReadElemtProperty();		//��ȡ���еı�ű���
	public:
		EltPropReader(const std::string& _SourceFile = "EltProperty.xml",
							boost::filesystem::path _savepath = "F:\\data from HBX_phd\\VS2015");	//���캯��

		bool SetInputData();

		//ǿ�ƶϿ�������������
		void terminate();
	};


	CUFEM_EXPORT BaseReader* InstanceElemtPropReader();
}
