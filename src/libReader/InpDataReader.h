#pragma once

#include <HBXPreDef.h>
#include <libCUFEM\BaseNode.h>
#include <libCUFEM\BaseElement.h>
#include <libCUFEM\domain.h>
#include <libCUFEM\AttriRecord.h>
#include <libCUFEM\BaseReader.h>
#include <libCUFEM\Set.h>
#include <libCUFEM\InputRecord.h>
#include <libReader\EltPropReader.h>
#include <libReader\DynamicInputRecord.h>


#pragma warning(disable: 4996)

namespace HBXFEMDef
{

	class AttriRecord;
	class Domain;
	class InputRecord;
	class BaseElem;


	class BOOST_SYMBOL_EXPORT InpDataReader :
		public BaseReader
	{
	private:
		typedef InpDataReader _SameClass;

		HBXFEMDef::EltPropReader	m_EltProp;

	protected:
		void GetSecMesg(std::vector<std::string>& _vecIn, std::string& _elset,
			std::string& _mat,
			std::string& _secshape);

		InputFileResult ReadInpFile();	//��ȡpointwise�����ĵ㼯�ϣ�ΪVTX��׼��
	public:
		//Ĭ�Ϲ��캯��
		InpDataReader(const std::string _SourceFile = "", 
						boost::filesystem::path _savepath = "F:\\data from HBX_phd\\vs2015");
		//�������캯��
		//		PWDataReader(const _SameClass&) {};
		~InpDataReader() {};

		//��������·��������ݵ�ǰ����·���������ļ����������������ĵ�һ����ǰ�ļ�����
		//		bool SetSourceFilePath(const std::string _SourceFile, boost::filesystem::path _savepath = "");

		bool SetInputData();

		//ǿ�ƶϿ�������������
		void terminate();


	};

	CUFEM_EXPORT BaseReader* InstanceInpReader();


}
