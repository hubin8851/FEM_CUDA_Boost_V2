#ifdef USESTDAFX
#include "stdafx.h"
#endif

#include "EltPropReader.h"
#include <SearchFile.h>
namespace HBXFEMDef
{

	EltPropReader::EltPropReader(const std::string & _SourceFile, boost::filesystem::path _savepath)
	{
		this->SetSourceFilePath(_SourceFile, _savepath);
	}

	HBXFEMDef::_EltPtyInMap* const EltPropReader::GetPtyMap() 
	{
		// TODO: �ڴ˴����� return ���
		return &this->m_ElemtPtyInMap;
	}


	InputFileResult EltPropReader::ReadElemtProperty()
	{
		using namespace std;
		using namespace boost;
		using boost::property_tree::ptree;

		size_t	_iCount = 0;	//��Ԫ�ܺͼ�����
		ptree	pt;
		ptree	root;
		HBXFEMDef::ElemtProperty	_tmpElPro;	//��Ԫ���Խṹ����ʱ����
		string	_ElemtName;//��Ԫ���ƣ���ʱ����

		boost::optional<boost::filesystem::path> r = HBXDef::find_file(m_path, m_SrcFileName);
		if (!r)
		{
			std::cerr << "boost������ǰ·��" << m_path << "���޸��ļ�" << std::endl;
			return InputFileResult::IRRT_NOTFOUND;
		}
		else
		{
			m_path = *r;
			std::cout << "��ǰ�ļ�����·��" << *r << std::endl;
		}

		TiXmlDocument myDoc( r->string().c_str() );
		bool loadOk = myDoc.LoadFile();
		if ( !loadOk )
		{
			cout << "�޷������ļ�.Error:" << myDoc.ErrorDesc() << endl;
			return InputFileResult::IRRT_BAD_FORMAT;
		}
		TiXmlElement *RootElement = myDoc.RootElement();  //��Ԫ��, Info  
		cout << "[root name]" << RootElement->Value() << "\n";

		TiXmlElement *pEle = RootElement;
		//�����ý��  
		for (TiXmlElement* Element = pEle->FirstChildElement();//��һ����Ԫ��  
			Element != NULL;
			Element = Element->NextSiblingElement())//��һ���ֵ�Ԫ��  
		{
			TiXmlAttribute *pAttr = Element->FirstAttribute();//��һ������
			//_tmpElPro._Type = lexical_cast<unsigned int>(pAttr->Value());
			while (NULL != pAttr) //�����������  
			{
				std::string _tmpstr = pAttr->Name();//��ȡ��Ԫ����
				if ( iequals(_tmpstr, "name") )//��ȡ��Ԫ��
				{
					strcpy_s( _tmpElPro._Name, pAttr->Value() );
				}
				if (iequals(_tmpstr, "type"))//��ȡ���ͺ�
				{
					_tmpElPro._Type = lexical_cast<unsigned int>(pAttr->Value());
				}
				if (iequals(_tmpstr, "classname"))//��ȡ��Ӧ������
				{
					strcpy_s(_tmpElPro._ClassName, pAttr->Value());
				}
				if (iequals(_tmpstr, "nodenum"))//��ȡ��Ԫ�ڵ���
				{
					_tmpElPro._NNum = lexical_cast<unsigned short>(pAttr->Value());
				}
		
				pAttr = pAttr->Next();
			}
			m_ElemtPtyInMap.emplace(make_pair(_tmpElPro._Name, _tmpElPro));
			delete pAttr;
		}

		
		return InputFileResult_t::IRRT_OK;
	}

	bool EltPropReader::SetInputData()
	{
		namespace fs = boost::filesystem;
		using namespace HBXDef;
		if (fs::extension(m_SrcFileName) == ".xml")
		{
			CheckUserDefErrors(ReadElemtProperty());
			return true;
		}

		return false;
	}

	void EltPropReader::terminate()
	{
	}

	CUFEM_EXPORT BaseReader * InstanceElemtPropReader()
	{
		return (BaseReader*)new EltPropReader();
	}

}


