#include <HBXPreDef.h>
#include "InpDataReader.h"
#include <libCUFEM\ClassFactory.h>
#include <libReader\DynamicInputRecord.h>
#include <regex>
namespace HBXFEMDef
{

	InputFileResult InpDataReader::ReadInpFile()
	{
		using namespace std;
		using namespace boost;
		using namespace HBXDef;
		using namespace HBXFEMDef;
		int		_Dim;
		unsigned short _stressFlag;
		unsigned short _nodenum;

		std::string stringLine;		//读取的当前行
		vector<string> vstrLine;	//分割后的当前行
		vector<HBXDef::UserCalPrec>	vPrecLine;	//分割后当前行的数据
		unsigned int _ElmtNum, _NodeNum;	//每个域内的单元和节点数量
		int	 marknumber;			//数据类型的判定
		int _markloop;				//循环标记，可能不会用到
		HBXDef::ControlMark_t ipControlMark;//控制流标记
		std::string _tmpName;		//临时变量名，记录名称
		boost::filesystem::path _tmppath;		//路径临时变量
		std::regex	tmpreg2("\\w+[=]?\\w+");
		std::regex	tmpreg("\\d*[,]");	//在读取pw文件时，所需的正则表达式
		std::smatch	what;

		_tmppath = m_path.append(m_SrcFileName);
		std::ifstream inFile;
		inFile.open(_tmppath.c_str());
		if (inFile.bad())
		{
			std::cerr << "Error1： Can not open input data file" << std::endl;
			return InputFileResult::IRRT_BAD_FORMAT;
		}

		//可能出现的某几种类型的指针
		Domain*	_tmpZone;
		std::vector<Node>	_tmpVNode;
		std::vector<vector<HBXDef::UserReadPrec>> _tmpVElem;
		BaseMaterial* _pMaterial;
		BaseSection* _pSection;
		BaseBoundary* _pBoundary;
		Set*	_tmpSet;

		ipControlMark = HBXDef::RESET;
		while (!inFile.eof())
		{
			getline(inFile, stringLine);
			// 定义分割方式为英文逗号，中文逗号和空格，构造一个分词器，  
			boost::char_separator<char> sep(" ,，*");
			typedef boost::tokenizer<boost::char_separator<char> >  CustonTokenizer;
			CustonTokenizer tok(stringLine, sep);
			if ( std::regex_match(stringLine, tmpreg) )	//判断，若为数字直接swith
			{
				marknumber = _markloop;
				vPrecLine.clear();
				std::sregex_iterator it(stringLine.begin(), stringLine.end(), tmpreg);
				std::sregex_iterator end;
				for (; it != end; ++it)
				{
					vPrecLine.emplace_back(std::move( std::stoi(it->str()) ) );
				}
			}
			// 			else
			// 			{
			// 				// 输出分割结果
			// 				vstrLine.clear();
			// 				for (CustonTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg)
			// 				{
			// 					vstrLine.push_back(*beg);
			// 				}
			// 			}
			// 
			// 			marknumber = 0;
			// 			if (vstrLine.empty() || "" == vstrLine[0])
			// 			{
			// 				_markloop = 0;
			// 				continue;
			// 			}
			// 			else if (boost::iequals("instance", vstrLine[0]))//判断是否进入实例
			// 			{
			// 				for (auto _str = vstrLine.begin(); _str != vstrLine.end(); _str++)
			// 				{
			// 					if (boost::icontains("name=", _str))
			// 					{
			// 						boost::ierase_first(_str, "name=");
			// 					}
			// 				}
			// 				continue;
			// 			}
			// 			else if (boost::iequals("Node", vstrLine[0]))//判断是否进入节点段
			// 			{
			// 				_markloop = 10;
			// 				ipControlMark = NODE;
			// //				_tmpVNode = std::make_unique<std::vector<Node>>();
			// 			}
			// 			else if (boost::iequals("Element", vstrLine[0]) && icontains(stringLine, "type="))//判断是否进入单元段
			// 			{
			// 				boost::ierase_first(vstrLine[1], "type=");
			// 				_tmpName = vstrLine[1];	//获的单元的名称
			// 				_markloop = 20;
			// 				ipControlMark = ELEMENT;
			// //				_tmpVElem = std::make_unique<std::vector<vector<HBXDef::UserReadPrec>>>();
			// 			}
			// 
			// 			//华丽的分隔符，goto位置。。。
			// 			ReadDigtal:
			// 			switch (marknumber)
			// 			{
			// 			case ControlMark_t::NODE:
			// // 				_tmpVNode->emplace_back(Node(lexical_cast<HBXDef::UserReadPrec>(vstrLine[0]), 
			// // 									lexical_cast<HBXDef::UserReadPrec>(vstrLine[1]),
			// // 									lexical_cast<HBXDef::UserReadPrec>(vstrLine[2])) );
			// 				break;
			// 			case ControlMark_t::ELEMENT:
			// 
			// 				break;
			// 			default:
			// 				break;
			// 			}
			// 		}
		}
		return InputFileResult::IRRT_BAD_FORMAT;
	}

	InpDataReader::InpDataReader()
	{
		std::cout << "create InpDataReader!" << std::endl;
	}

	bool InpDataReader::SetInputData()
	{
		namespace fs = boost::filesystem;
		using namespace HBXDef;
		if (fs::extension(m_SrcFileName) == ".inp")
		{
			CheckUserDefErrors(ReadInpFile());
			return true;
		}

		return false;
	}

	void InpDataReader::terminate()
	{
	}

	CUFEM_EXPORT  BaseReader* InstanceInpReader()
	{
		return (BaseReader*)new InpDataReader();
	}

}

