#ifdef USESTDAFX
#include "stdafx.h"
#endif

#include <HBXPreDef.h>
#include <BoostPreDef.h>
#include <libCUFEM\domain.h>
#include <libCUFEM\Set.h>
#include <libCUFEM\BaseBoundary.h>
#include "PWDataReader.h"


namespace HBXFEMDef
{


	InputFileResult PWDataReader::ReadpwFile()
	{
		using namespace std;
		using namespace boost;
		using namespace HBXDef;
		using namespace HBXFEMDef;

		std::string stringLine;		//��ȡ�ĵ�ǰ��
		vector<string> vstrLine;	//�ָ��ĵ�ǰ��
		vector<UserCalPrec>	vPrecLine;	//�ָ��ǰ�е�����
		unsigned int _ElmtNum, _NodeNum;	//ÿ�����ڵĵ�Ԫ�ͽڵ�����
		int	 marknumber;			//�������͵��ж�
		int _markloop;				//ѭ����ǣ����ܲ����õ�
		ControlMark_t ipControlMark;//���������
		std::string _SetName;		//��������
		std::string	_tmpName;		//·����ʱ����
		xpressive::cregex	tmpreg2 = xpressive::cregex::compile("\\w+[=]?\\w+");
		xpressive::cregex	tmpreg = xpressive::cregex::compile("([-]?\\d[.]\\d{9})[E](.)(\\d{3})");	//�ڶ�ȡpw�ļ�ʱ�������������ʽ
		xpressive::cmatch	what;

		_tmpName = m_path.append("\\");
		_tmpName.append(m_SrcFileName);
		std::ifstream inFile;
		inFile.open(_tmpName);
		if (inFile.bad())
		{
			std::cerr << "Error1�� Can not open input data file" << std::endl;
			exit(1);
		}

		ipControlMark = RESET;
		std::unique_ptr< Domain >	_tmpZone;
		std::vector< Node >	_tmpvNode;
		std::unique_ptr< BaseElem > _tmpElem;

		while (!inFile.eof())
		{
			getline(inFile, stringLine);

		ZoneLoop:
			// ����ָʽΪӢ�Ķ��ţ����Ķ��źͿո񣬹���һ���ִ�����  
			boost::char_separator<char> sep(" ,��*");
			typedef boost::tokenizer<boost::char_separator<char> >  CustonTokenizer;
			CustonTokenizer tok(stringLine, sep);
			// ����ָ���
			vstrLine.clear();
			for (CustonTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg)
			{
				vstrLine.push_back(*beg);
			}
			if (vstrLine.empty() || "" == vstrLine[0])
			{
				_markloop = 0;	//�ڴ�ע�⣬Ŀǰ����ȷ���Ƿ�ʹ��markloop
				continue;
			}
			if (iequals("ZONE", vstrLine[0]))	//�ж�Ϊpw��ĳ����
			{
				ierase_first(vstrLine[1], "T=");
				_tmpName = vstrLine[1];
				_markloop = 10;
				ipControlMark = ZONE;
			}
			switch (ipControlMark)
			{
			case ControlMark_t::ZONE://�Ѿ�������ZONE
				_tmpZone = std::make_unique<Domain>(_tmpName);
				while (true)
				{
					CustonTokenizer tok(stringLine, sep);	//����node���Ō�string�M�зָ�
															// ����ָ���
					vstrLine.clear();
					for (CustonTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg)
					{
						vstrLine.push_back(*beg);
					}
					if (icontains(stringLine, "nodes="))//��ǰ��Ϊ��ǰZONE���ڽڵ�͵�Ԫ��Ϣ
					{
						ierase_first(vstrLine[0], "nodes=");
						_NodeNum = lexical_cast<unsigned int>(vstrLine[0]);	//��ǰ��ڵ���Ŀ
						ierase_first(vstrLine[1], "elements=");
						_ElmtNum = lexical_cast<unsigned int>(vstrLine[1]);	//��ǰ��ڵ���Ŀ
						ierase_first(vstrLine[2], "ZoneType=");
						//_tmpZone->ResetType(vstrLine[2]);
						getline(inFile, stringLine);
						continue;
					}
					else if (xpressive::regex_match(vstrLine[0].c_str(), what, tmpreg))//��ǰΪ������Ϣ������loop
					{
						for (size_t i = 0; i < _NodeNum; i++)
						{
							CustonTokenizer tok(stringLine, sep);
							// ����ָ���
							vstrLine.clear();
							vPrecLine.clear();
							for (CustonTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg)
							{
								//vstrLine.push_back(*beg);
								vPrecLine.emplace_back(lexical_cast<UserCalPrec>(*beg));
							}
							_tmpvNode.emplace_back(Node(lexical_cast<UserCalPrec>(vstrLine[0]),
								lexical_cast<UserCalPrec>(vstrLine[1]),
								lexical_cast<UserCalPrec>(vstrLine[2])) );
							getline(inFile, stringLine);
						}
						for (size_t i = 0; i < _ElmtNum; i++)
						{
							CustonTokenizer tok(stringLine, sep);
							// ����ָ���
							vPrecLine.clear();
							vstrLine.clear();
							for (CustonTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg)
							{
								vstrLine.push_back(*beg);
								vPrecLine.push_back(lexical_cast<UserCalPrec>(*beg));
							}
							//_tmpElem = std::make_unique<BaseElem>(vPrecLine.data(), vPrecLine.size(), classType::ELEMT);
							//_tmpZone->_vElmt.push_back( std::move(*_tmpElem) );
							getline(inFile, stringLine);
						}
						break;
					}
					else getline(inFile, stringLine);
				}
				//m_ZoneInMap.insert(make_pair(_tmpName, _tmpZone));
				goto ZoneLoop;
			default:
				break;
			}

		}

		return InputFileResult::IRRT_OK;
	}

	PWDataReader::PWDataReader():BaseReader()
	{
		std::cout << "create PWReader!" << std::endl;
	}


	bool PWDataReader::SetInputData()
	{
		namespace fs = boost::filesystem;
		using namespace HBXDef;
		if (fs::extension(m_SrcFileName) == ".pw")
		{
			CheckUserDefErrors(ReadpwFile());
			return true;
		}

		return false;
	}

	void PWDataReader::terminate()
	{

	}

	std::size_t PWDataReader::size()
	{
		return sizeof(PWDataReader);
	}

	CUFEM_API  BaseReader* InstancePWReader()
	{
		return (BaseReader*)new PWDataReader();
	}

}

