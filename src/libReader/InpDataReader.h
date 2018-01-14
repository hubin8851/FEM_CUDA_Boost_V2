#pragma once

#include <HBXPreDef.h>
#include <libCUFEM\BaseNode.h>
#include <libCUFEM\BaseElement.h>
#include <libCUFEM\domain.h>
#include <libCUFEM\AttriRecord.h>
#include <libCUFEM\BaseReader.h>
#include <libCUFEM\Set.h>
#include <libCUFEM\InputRecord.h>

#pragma warning(disable: 4996)

namespace HBXFEMDef
{

	class AttriRecord;
	class Domain;
	class InputRecord;
	struct BaseElem;

	class DynamicInputRecord;

	class BOOST_SYMBOL_EXPORT InpDataReader :
		public BaseReader
	{
	private:
		typedef InpDataReader _SameClass;

	protected:
		InputFileResult ReadInpFile();	//��ȡpointwise�����ĵ㼯�ϣ�ΪVTX��׼��
	public:
		//Ĭ�Ϲ��캯��
		InpDataReader();
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
