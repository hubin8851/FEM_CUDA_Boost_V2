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
		using namespace HBXDef;
		using namespace HBXFEMDef;
		int		_Dim;
		unsigned short _stressFlag;
		unsigned short _nodenum;

		std::string stringLine;		//��ȡ�ĵ�ǰ��
		vector<string> vstrLine;	//�ָ��ĵ�ǰ��
		vector<HBXDef::UserCalPrec>	vFloat;	//�ָ��ǰ�е�����
		unsigned int _ElmtNum, _NodeNum;	//ÿ�����ڵĵ�Ԫ�ͽڵ�����
		int	 marknumber;			//�������͵��ж�
		int _markloop;				//ѭ����ǣ����ܲ����õ�
		HBXDef::ControlMark_t ipControlMark;//���������
		std::string _tmpName, _SetName, _SectionName, _MatName;
		std::string _Elset, _Nset;	//��Ԫ���ϣ��ڵ㼯��
		std::string _InstanceName, _partName;	//��ʱ����������¼����
		boost::filesystem::path _tmppath;		//·����ʱ����
		std::regex	RegRule2("\\w+[=]?\\w+");
		std::regex	IfNumRule("((\\s*\\d+(\.?)\\d*)(\,)?)*$");	//�ڶ�ȡpw�ļ�ʱ�������������ʽ
		std::regex	NumRule("\\s*\\d+((\.\\d*)?)");
		//std::regex	NumRule("^\\s*\\d+");
		std::smatch	what;

		_tmppath = m_path.append(m_SrcFileName);
		std::ifstream inFile;
		inFile.open(_tmppath.c_str());
		if (inFile.bad())
		{
			std::cerr << "Error1�� Can not open input data file" << std::endl;
			return InputFileResult::IRRT_BAD_FORMAT;
		}

		//���ܳ��ֵ�ĳ�������͵�ָ��
		std::unique_ptr< Domain >	_tmpZone;
		std::vector<Node>	_tmpVNode;
//		std::vector<vector<HBXDef::UserReadPrec>> _tmpVElem;
		BaseMaterial* _pMaterial;
		BaseSection* _pSection;
		BaseBoundary* _pBoundary;
		Set*	_tmpSet;

		ipControlMark = HBXDef::RESET;
		while (!inFile.eof())
		{
			getline(inFile, stringLine);
			// ����ָʽΪӢ�Ķ��ţ����Ķ��źͿո񣬹���һ���ִ�����  
			boost::char_separator<char> sep(" ,��*");
			typedef boost::tokenizer<boost::char_separator<char> >  CustonTokenizer;
			CustonTokenizer tok(stringLine, sep);

			if ( std::regex_match(stringLine, IfNumRule) )	//�жϣ���Ϊ����ֱ��swith
			{
				//sregex_iterator�÷��μ�
				//http://blog.csdn.net/caroline_wendy/article/details/17319899
				vFloat.clear();
				auto begin_iter = std::sregex_iterator(stringLine.begin(), stringLine.end(), NumRule);
				std::sregex_iterator end_iter;
				for (auto it=begin_iter; it!=end_iter; it++)
				{
					vFloat.emplace_back(std::move( std::stoi(it->str()) ) );
				}
				marknumber = _markloop;
				goto ReadDigtal;
			}
			else
			{
				// ����ָ���
				vstrLine.clear();
				for (CustonTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg)
				{
					vstrLine.push_back(*beg);
				}
 			}		
			marknumber = 0;
			if (vstrLine.empty() || "" == vstrLine[0])
			{
				_markloop = 0;
				continue;
			}
			else if (boost::iequals("instance", vstrLine[0]))//�ж��Ƿ����ʵ��
			{
				for (auto _str = vstrLine.begin(); _str != vstrLine.end(); _str++)
				{
					if (boost::icontains("name=", _str))
					{
						boost::ierase_first(_str, "name=");
						_InstanceName = _str->c_str();
						_tmpZone = std::make_unique<Domain>(_InstanceName);
					}
					else if (boost::icontains("part=", _str))
					{
						boost::ierase_first(_str, "part=");
						_partName = _str->c_str();
					}
				}
				continue;
 			}
 			else if (boost::iequals("Node", vstrLine[0]))//�ж��Ƿ����ڵ��
 			{
 				_markloop = 10;
 				ipControlMark = NODE;
 //				_tmpVNode = std::make_unique<std::vector<Node>>();
 			}
 			else if (boost::iequals("Element", vstrLine[0]) && boost::icontains(stringLine, "type="))//�ж��Ƿ���뵥Ԫ��
 			{
 				boost::ierase_first(vstrLine[1], "type=");
 				_tmpName = vstrLine[1];	//��ĵ�Ԫ������
 				_markloop = 20;
 				ipControlMark = ELEMENT;
 //				_tmpVElem = std::make_unique<std::vector<vector<HBXDef::UserReadPrec>>>();
				continue;
 			}
			//��ȡ����������ơ����ü���
			else if (boost::iequals("section:", vstrLine[0]))
			{
				_SectionName = vstrLine[1];
				_markloop = 0;
				ipControlMark = SECTION;
			}
			else if (ipControlMark == SECTION && boost::iequals("Section", vstrLine[1]))
			{
				boost::ierase_first(vstrLine[2], "elset=");
				_Elset = vstrLine[2];
				boost::ierase_first(vstrLine[3], "material=");
				_MatName = vstrLine[3];
				_markloop = 42;
			}
			//��ȡ����������ơ����ü���
			else if (boost::iequals("Material", vstrLine[0]))
			{
				boost::ierase_first(vstrLine[1], "name=");
				_MatName = vstrLine[1];
				_markloop = 0;
				ipControlMark = MATERIAL;
			}
			else if (ipControlMark == MATERIAL && boost::iequals("Density", vstrLine[0])) _markloop = ControlMark_t::DENSITY;
			else if (ipControlMark == MATERIAL && boost::iequals("Elastic", vstrLine[0])) _markloop = ControlMark_t::ELASTIC;
			//��ȡ�����������
			else if (boost::iequals("Elset", vstrLine[0]))
			{
				boost::ierase_first(vstrLine[1], "elset=");
				_SetName = vstrLine[1];
				_markloop = 51;
				ipControlMark = ELSET;
			}
			else if (boost::iequals("Nset", vstrLine[0]))
			{
				boost::ierase_first(vstrLine[1], "nset=");
				_SetName = vstrLine[1];
				_markloop = 52;
				ipControlMark = NSET;
			}
 			//�����ķָ�����gotoλ�á�����
 			ReadDigtal:
 			switch (marknumber)
 			{
 			case ControlMark_t::NODE:
  				_tmpVNode.emplace_back(Node(stof(vstrLine[0]), 
										stof(vstrLine[1]),
										stof(vstrLine[2])) );
 				break;
 			case ControlMark_t::ELEMENT:
 
 				break;
 			default:
 				break;
 			}
		}
		return InputFileResult::IRRT_OK;
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

