#ifdef USESTDAFX
#include "stdafx.h"
#endif

#include "EltPropReader.h"
#include <SearchFile.h>
namespace HBXFEMDef
{

	EltPropReader::EltPropReader(const std::string & _SourceFile, std::string _savepath)
	{
		this->SetSourceFilePath(_SourceFile, _savepath);
	}

	HBXFEMDef::_EltPtyInMap* const EltPropReader::GetPtyMap() 
	{
		// TODO: 在此处插入 return 语句
		return &this->m_ElemtPtyInMap;
	}


	InputFileResult EltPropReader::ReadElemtProperty()
	{
		using namespace std;
		using namespace boost;
		using boost::property_tree::ptree;

		size_t	_iCount = 0;	//单元总和计数器
		ptree	pt;
		ptree	root;
		HBXFEMDef::ElemtProperty	_tmpElPro;	//单元属性结构体临时变量
		string	_ElemtName;//单元名称，临时变量

		boost::filesystem::path m_root = m_path + "\\" + m_SrcFileName;

		TiXmlDocument myDoc( m_root.string().c_str() );
		bool loadOk = myDoc.LoadFile();
		if ( !loadOk )
		{
			cout << "无法加载文件.Error:" << myDoc.ErrorDesc() << endl;
			return InputFileResult::IRRT_BAD_FORMAT;
		}
		TiXmlElement *RootElement = myDoc.RootElement();  //根元素, Info  
		cout << "[root name]" << RootElement->Value() << "\n";

		TiXmlElement *pEle = RootElement;
		//遍历该结点  
		for (TiXmlElement* Element = pEle->FirstChildElement();//第一个子元素  
			Element != NULL;
			Element = Element->NextSiblingElement())//下一个兄弟元素  
		{
			TiXmlAttribute *pAttr = Element->FirstAttribute();//第一个属性
			//_tmpElPro._Type = lexical_cast<unsigned int>(pAttr->Value());
			while (NULL != pAttr) //输出所有属性  
			{
				std::string _tmpstr = pAttr->Name();//获取单元名称
				if ( iequals(_tmpstr, "name") )//获取单元名
				{
					strcpy_s( _tmpElPro._Name, pAttr->Value() );
				}
				if (iequals(_tmpstr, "type"))//获取类型号
				{
					_tmpElPro._Type = lexical_cast<unsigned int>(pAttr->Value());
				}
				if (iequals(_tmpstr, "classname"))//获取对应类名称
				{
					strcpy_s(_tmpElPro._ClassName, pAttr->Value());
				}
				if (iequals(_tmpstr, "nodenum"))//获取单元节点数
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

	CUFEM_API BaseReader * InstanceElemtPropReader()
	{
		return (BaseReader*)new EltPropReader();
	}

}


