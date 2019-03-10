#pragma once

#include <ExportDef.h>
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


	class CUFEM_API InpDataReader :
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
		InpDataReader(	const std::string _SourceFile = "", 
						std::string _savepath = "F:\\data from HBX_phd\\vs2015");
		//�������캯��
		//		PWDataReader(const _SameClass&) {};
		~InpDataReader() {};

		//���õ�Ԫ����xml����
		bool SetEltPropFilePath(const std::string _SourceFile, boost::filesystem::path _savepath = "");

		InputFileResult SetInputData();

		//ǿ�ƶϿ�������������
		void terminate();


	};

	CUFEM_API BaseReader* InstanceInpReader(const std::string _SourceFile = "",
											std::string _savepath = "F:\\data from HBX_phd\\vs2015");


}
