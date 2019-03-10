#pragma once

/************************************************************************/
/*     PW�ļ������ݶ�ȡ��                                               */
/************************************************************************/

#include <HBXPreDef.h>
#include <ExportDef.h>
#include <libCUFEM\BaseNode.h>
#include <libCUFEM\BaseElement.h>
#include <libCUFEM\domain.h>
#include <libCUFEM\AttriRecord.h>
#include <libCUFEM\BaseSlnRecord.h>
#include <libCUFEM\BaseReader.h>
#include <MyTimer.h>


namespace HBXFEMDef
{

	class AttriRecord;
// 	class Domain;
// 	struct Node;
// 	struct BaseElem;
// 	class CSet;

	class CUFEM_API PWDataReader:
		public BaseReader
	{
	private:
		typedef PWDataReader _SameClass;

	protected:
		InputFileResult ReadpwFile();	//��ȡpointwise�����ĵ㼯�ϣ�ΪVTX��׼��
	public:
		//Ĭ�Ϲ��캯��
		PWDataReader();
		//�������캯��
//		PWDataReader(const _SameClass&) {};
		~PWDataReader(){};

		//��������·��������ݵ�ǰ����·���������ļ����������������ĵ�һ����ǰ�ļ�����
//		bool SetSourceFilePath(const std::string _SourceFile, boost::filesystem::path _savepath = "");

		InputFileResult SetInputData();

		//ǿ�ƶϿ�������������
		void terminate();

		//boost.dll�����ĳһ�ַ�ʽ��ʹ�ã����ӡ�
		static std::size_t size();

// 		static boost::shared_ptr<PWDataReader> InstancePWReader()
// 		{
// 			return boost::make_shared<PWDataReader>(new PWDataReader());
// 		}

	};

	CUFEM_API BaseReader* InstancePWReader();

// 	extern "C" BOOST_SYMBOL_EXPORT PWDataReader PWReader;
// 	PWDataReader PWReader;
//	CUFEM_EXPORT PWDataReader* _theelement = nullptr;
//	CUFEM_EXPORT PWDataReader* allocReader();
	

}

//  	BOOST_DLL_ALIAS(
//  		HBXFEMDef::PWDataReader,
//  		MyPWReader
//  	)