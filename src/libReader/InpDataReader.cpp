#include <HBXPreDef.h>
#include "InpDataReader.h"
#include <libCUFEM\ClassFactory.h>
#include <libReader\DynamicInputRecord.h>

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

		std::string stringLine;		//��ȡ�ĵ�ǰ��
		vector<string> vstrLine;	//�ָ��ĵ�ǰ��
		vector<UserCalPrec>	vPrecLine;	//�ָ��ǰ�е�����
		unsigned int _ElmtNum, _NodeNum;	//ÿ�����ڵĵ�Ԫ�ͽڵ�����
		int	 marknumber;			//�������͵��ж�
		int _markloop;				//ѭ����ǣ����ܲ����õ�
		ControlMark_t ipControlMark;//���������
		std::string _tmpName;		//��ʱ����������¼����
		boost::filesystem::path _tmppath;		//·����ʱ����
		xpressive::cregex	tmpreg2 = xpressive::cregex::compile("\\w+[=]?\\w+");
		xpressive::cregex	tmpreg = xpressive::cregex::compile("([-]?\\d[.]\\d{9})[E](.)(\\d{3})");	//�ڶ�ȡpw�ļ�ʱ�������������ʽ
		xpressive::cmatch	what;

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
		std::unique_ptr< std::vector<Node> >	_tmpVNode;
		std::unique_ptr< std::vector<vector<UserReadPrec>> > _tmpVElem;
		std::unique_ptr< BaseMaterial > _tmpMaterial;
		std::unique_ptr< BaseSection > _tmpSection;
		std::unique_ptr< BaseBoundary > _tmpBoundary;
		std::unique_ptr<Set>	_tmpSet;

		DynamicInputRecord* _tmpRecord;

		ipControlMark = RESET;
		while (!inFile.eof())
		{
			getline(inFile, stringLine);
			// ����ָʽΪӢ�Ķ��ţ����Ķ��źͿո񣬹���һ���ִ�����  
			boost::char_separator<char> sep(" ,��*");
			typedef boost::tokenizer<boost::char_separator<char> >  CustonTokenizer;
			CustonTokenizer tok(stringLine, sep);
			if (all(tok.begin(), is_digit()))	//�жϣ���Ϊ����ֱ��swith
			{
				marknumber = _markloop;
				vPrecLine.clear();
				for (CustonTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg)
				{
					vPrecLine.push_back(lexical_cast<UserCalPrec>(*beg));
				}
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
			else if (iequals("instance", vstrLine[0]))//�ж��Ƿ����ʵ��
			{
				for (auto _str = vstrLine.begin(); _str != vstrLine.end(); _str++)
				{
					if (icontains("name=", _str))
					{
						ierase_first(_str, "name=");
					}
				}
				continue;
			}
			else if (iequals("Node", vstrLine[0]))//�ж��Ƿ����ڵ��
			{
				_markloop = 10;
				ipControlMark = NODE;
				_tmpVNode = std::make_unique<std::vector<Node>>();
			}
			else if (iequals("Element", vstrLine[0]) && icontains(stringLine, "type="))//�ж��Ƿ���뵥Ԫ��
			{
				ierase_first(vstrLine[1], "type=");
				_tmpName = vstrLine[1];	//��ĵ�Ԫ������
				_markloop = 20;
				ipControlMark = ELEMENT;
				_tmpVElem = std::make_unique<std::vector<vector<UserReadPrec>>>();
			}
// 			else if (all(vstrLine[0].substr(0, 1), is_digit()))
// 			{
// 				marknumber = _markloop;
// 			}
			//�����ķָ�����gotoλ�á�����
			ReadDigtal:
			switch (marknumber)
			{
			case ControlMark_t::NODE:
				_tmpVNode->emplace_back(Node(lexical_cast<UserReadPrec>(vstrLine[0]), 
									lexical_cast<UserReadPrec>(vstrLine[1]), 
									lexical_cast<UserReadPrec>(vstrLine[2])) );
				break;
			case ControlMark_t::ELEMENT:
				_tmpRecord = new DynamicInputRecord;
				_tmpVElem->emplace_back(vPrecLine);
				break;
			default:
				break;
			}
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

