#include <HBXPreDef.h>
#include "InpDataReader.h"
#include <HBXFEMDefStruct.h>
#include <libCUFEM\MatArray.h>
#include <libCUFEM\ClassFactory.h>
#include <libReader\DynamicInputRecord.h>
#include <regex>
namespace HBXFEMDef
{
	void InpDataReader::GetSecMesg(std::vector<std::string>& _vecIn, 
		std::string & _elset, 
		std::string & _mat, 
		std::string & _secshape)
	{
		for (auto _iter = _vecIn.begin(); _iter != _vecIn.end(); _iter++)
		{
			if (boost::icontains(*_iter, "elset="))
			{
				boost::ierase_first(*_iter, "elset=");
				_elset = *_iter;
				continue;
			}
			if (boost::icontains(*_iter, "material="))
			{
				boost::ierase_first(*_iter, "material=");
				_mat = *_iter;
				continue;
			}
			if (boost::icontains(*_iter, "section="))
			{
				boost::ierase_first(*_iter, "section=");
				_secshape = *_iter;
				continue;
			}

		}
		return;
	}
	
	
	InputFileResult InpDataReader::ReadInpFile()
	{
		using namespace std;
		using namespace HBXDef;
//		using namespace HBXFEMDef;

		double	rdens = 0.0;//�ض�
		int		_Dim;
		unsigned short _stressFlag;
		unsigned short _nodenum;

		std::string stringLine;		//��ȡ�ĵ�ǰ��
		vector<string> vstrLine;	//�ָ��ĵ�ǰ��
		vector<HBXDef::UserReadPrec>	vFloat;	//�ָ��ǰ�е�����
		unsigned int _ElmtNum, _NodeNum;	//ÿ�����ڵĵ�Ԫ�ͽڵ�����
		int	 marknumber;			//�������͵��ж�
		int _markloop;				//ѭ����ǣ����ܲ����õ�
		HBXDef::ControlMark_t ipControlMark;//���������
		std::string _ElemtName, _SetName, _SectionName, _MatName, _SecShape;
		std::string _Elset, _Nset;	//��Ԫ���ϣ��ڵ㼯��
		std::string _InstanceName, _partName;	//��ʱ����������¼����
		boost::filesystem::path _tmppath;		//·����ʱ����
		std::regex	RegRule2("\\w+[=]?\\w+");
		std::regex	IfNumRule("((\\s*\\d+(\\.\\d+)?)(\,|$)?)*");	//�ڶ�ȡpw�ļ�ʱ�������������ʽ
		std::regex  IfNotNum("\\*+.*");	//������ʽ����*��ʼ���ַ���
		std::regex  NumRule("\\s*\\d+(\\.\\d+)?(,|\\s)?");//ƥ�両����
		std::smatch	what;
		std::sregex_iterator end_iter;

		_tmppath = m_path.append(m_SrcFileName);
		std::ifstream inFile;
		inFile.open(_tmppath.c_str());
		if (inFile.bad())
		{
			std::cerr << "Error1�� Can not open input data file" << std::endl;
			return InputFileResult::IRRT_BAD_FORMAT;
		}

		//���ܳ��ֵ�ĳ�������͵�ָ��
		std::shared_ptr< Domain >	_tmpZone;
		std::shared_ptr< std::vector<Node> >	_tmpVNode;
		std::shared_ptr<HBXFEMDef::MatArray<HBXFEMDef::UserReadPrec>> _tmpElem;
		std::shared_ptr<HBXFEMDef::_Material<HBXFEMDef::UserReadPrec>> _pMaterial;
		std::shared_ptr<HBXFEMDef::_Section<HBXFEMDef::UserReadPrec>> _pSection;
		HBXFEMDef::BaseBoundary* _pBoundary;
		std::shared_ptr<HBXFEMDef::Set>	_pSet;

		ipControlMark = HBXDef::RESET;
		while (!inFile.eof())
		{
			getline(inFile, stringLine);
			// ����ָʽΪӢ�Ķ��ţ����Ķ��źͿո񣬹���һ���ִ�����  
			boost::char_separator<char> sep(" ,��*");
			typedef boost::tokenizer<boost::char_separator<char> >  CustonTokenizer;
			CustonTokenizer tok(stringLine, sep);

			if (!std::regex_match(stringLine, IfNotNum))	//�жϣ���Ϊ����ֱ��swith
			{
				//sregex_iterator�÷��μ�
				// 
				/* http://blog.csdn.net/caroline_wendy/article/details/17319899 */
				/* http://bbs.csdn.net/topics/392018932 */
				vFloat.clear();
//				boost::split( vstrLine, stringLine, boost::is_any_of(",") );
// 				for (auto i = 0; i < vstrLine.size(); i++)
// 				{
// 					vFloat.push_back( boost::lexical_cast<HBXFEMDef::UserReadPrec>(vstrLine[i]) );
// 				}
				if ( ELEMENT == _markloop)
				{
					for (CustonTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg)
					{
						vFloat.push_back(boost::lexical_cast<HBXFEMDef::UserReadPrec>(*beg)-1);
					}
				}
				else
				{
					for (CustonTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg)
					{
						vFloat.push_back(boost::lexical_cast<HBXFEMDef::UserReadPrec>(*beg));
					}
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
				goto ReadData;
			}
		

			//�����ķָ�����gotoλ�á�����
			ReadData:
			marknumber = 0;
			if (vstrLine.empty() || "" == vstrLine[0])
			{
				_markloop = 0;
				ipControlMark = RESET;
				continue;
			}
			else if (boost::iequals("instance", vstrLine[0]))//�ж��Ƿ����ʵ��
			{
				if (boost::icontains( vstrLine[1], "name=" ))
				{
					boost::ierase_first(vstrLine[1], "name=");
					_InstanceName = vstrLine[1];
					_tmpZone = std::make_unique<Domain>(_InstanceName);
				}
				if (boost::icontains( vstrLine[2], "part=" ))
				{
					boost::ierase_first(vstrLine[2], "part=");
					_partName = vstrLine[2];
				}
				continue;
 			}
 			else if (boost::iequals("Node", vstrLine[0]))//�ж��Ƿ����ڵ��
 			{
 				_markloop = 10;
 				ipControlMark = NODE;
 				_tmpVNode = std::make_shared<std::vector<Node>>();
				m_DynRecord.SetField(*_tmpVNode, "Node");
 			}
 			else if (boost::iequals( vstrLine[0], "Element" ) && boost::icontains( stringLine, "type=" ))//�ж��Ƿ���뵥Ԫ��
 			{
 				boost::ierase_first( vstrLine[1], "type=" );
				_ElemtName = vstrLine[1];	//��ĵ�Ԫ�����Ʋ������õ�Ԫ���������
				auto _iter = m_EltProp.GetPtyMap()->find(_ElemtName);
				_tmpElem = std::make_shared< HBXFEMDef::MatArray<HBXFEMDef::UserReadPrec>>((unsigned short)(_iter->second._NNum));
				m_DynRecord.SetField(_tmpElem, _ElemtName.c_str());

 				_markloop = 20;
 				ipControlMark = ELEMENT;
				continue;
 			}
			//��ȡ����������ơ����ü���
			else if (boost::iequals("section:", vstrLine[0]))
			{
				_SectionName = vstrLine[1];
				_markloop = 0;
				ipControlMark = SECTION;
				continue;
			}
			else if (ipControlMark == SECTION && boost::iequals("Section", vstrLine[1]))
			{
				GetSecMesg(vstrLine, _Elset, _MatName, _SecShape);
				_markloop = SECTION;
			}
			//��ȡ����������ơ����ü���
			else if (boost::iequals("Material", vstrLine[0]))
			{
				boost::ierase_first(vstrLine[1], "name=");
				_MatName = vstrLine[1];
				_markloop = 0;
				ipControlMark = MATERIAL;
				continue;
			}
			else if (ipControlMark == MATERIAL && boost::iequals("Density", vstrLine[0])) _markloop = ControlMark_t::DENSITY;
			else if (ipControlMark == MATERIAL && boost::iequals("Elastic", vstrLine[0])) _markloop = ControlMark_t::ELASTIC;
			else if (ipControlMark == MATERIAL && boost::iequals("Plastic", vstrLine[0])) _markloop = ControlMark_t::PLASTIC;
			//��ȡ�����������
			else if (boost::iequals("Elset", vstrLine[0]))
			{
				boost::ierase_first(vstrLine[1], "elset=");
				_SetName = vstrLine[1];
				_markloop = ELSET;
				continue;
			}
			else if (boost::iequals("Nset", vstrLine[0]))
			{
				boost::ierase_first(vstrLine[1], "nset=");
				_SetName = vstrLine[1];
				_markloop = NSET;
				continue;
			}
 			//�����ķָ�����gotoλ�á�����
 			ReadDigtal:
 			switch (marknumber)
 			{
 			case ControlMark_t::NODE:
				if (3 == vFloat.size())//�ڵ������Ϊ��ά����
				{
					vFloat.emplace_back(0);//Z����������
				}
  				_tmpVNode->emplace_back( Node(vFloat[1],
										vFloat[2],
										vFloat[3]) );
 				break;
 			case ControlMark_t::ELEMENT:
				vFloat.erase(vFloat.begin());
				_tmpElem->emplace_back(vFloat);
 				break;
			case ControlMark_t::ZONE:
			case ControlMark_t::NSET:
				_pSet = std::make_shared< HBXFEMDef::Set >(vFloat, false);
				m_DynRecord.SetField(_pSet, _SetName.c_str());
				break;
			case ControlMark_t::ELSET:
				_pSet = std::make_shared< HBXFEMDef::Set >(vFloat, true);
				m_DynRecord.SetField(_pSet, _SetName.c_str());
				break;
			case ControlMark_t::SECTION:
				_pSection = std::make_shared<HBXFEMDef::_Section<HBXFEMDef::UserReadPrec>>( _SectionName.c_str(),
								_Elset.c_str(), 
								_MatName.c_str(),
								vFloat[0],
								vFloat[1]);
				m_DynRecord.SetField(_pSection, _SectionName.c_str());
				break;
			case ControlMark_t::DENSITY:
				rdens = vFloat[0] * 9.8f;	//�ض� = �ܶ�*9.8
				break;
			case ControlMark_t::ELASTIC:
				_pMaterial = std::make_shared<HBXFEMDef::_Material<HBXFEMDef::UserReadPrec>>(vFloat[0],
					vFloat[1],
					rdens,
					_MatName.c_str());
				m_DynRecord.SetField(_pMaterial, _MatName.c_str());
			case ControlMark_t::PLASTIC:
				_pMaterial = std::make_shared<HBXFEMDef::_Material<HBXFEMDef::UserReadPrec>>(vFloat[0],
					vFloat[1],
					rdens,
					_MatName.c_str());
				m_DynRecord.SetField(_pMaterial, _MatName.c_str());
				break;
 			default:
 				break;
 			}
		}
		return InputFileResult::IRRT_OK;
	}

	InpDataReader::InpDataReader(const std::string _SourceFile, 
								boost::filesystem::path _savepath ):m_EltProp("EltProperty.xml", _savepath)
	{
		std::cout << "create InpDataReader!" << std::endl;
		m_EltProp.SetInputData();
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

