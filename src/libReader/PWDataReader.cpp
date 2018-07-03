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

		std::string stringLine;		//读取的当前行
		vector<string> vstrLine;	//分割后的当前行
		vector<UserCalPrec>	vPrecLine;	//分割后当前行的数据
		unsigned int _ElmtNum, _NodeNum;	//每个域内的单元和节点数量
		int	 marknumber;			//数据类型的判定
		int _markloop;				//循环标记，可能不会用到
		ControlMark_t ipControlMark;//控制流标记
		std::string _SetName;		//集合名称
		std::string	_tmpName;		//路径临时变量
		xpressive::cregex	tmpreg2 = xpressive::cregex::compile("\\w+[=]?\\w+");
		xpressive::cregex	tmpreg = xpressive::cregex::compile("([-]?\\d[.]\\d{9})[E](.)(\\d{3})");	//在读取pw文件时，所需的正则表达式
		xpressive::cmatch	what;

		_tmpName = m_path.append("\\");
		_tmpName.append(m_SrcFileName);
		std::ifstream inFile;
		inFile.open(_tmpName);
		if (inFile.bad())
		{
			std::cerr << "Error1： Can not open input data file" << std::endl;
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
			// 定义分割方式为英文逗号，中文逗号和空格，构造一个分词器，  
			boost::char_separator<char> sep(" ,，*");
			typedef boost::tokenizer<boost::char_separator<char> >  CustonTokenizer;
			CustonTokenizer tok(stringLine, sep);
			// 输出分割结果
			vstrLine.clear();
			for (CustonTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg)
			{
				vstrLine.push_back(*beg);
			}
			if (vstrLine.empty() || "" == vstrLine[0])
			{
				_markloop = 0;	//在此注意，目前尚无确定是否使用markloop
				continue;
			}
			if (iequals("ZONE", vstrLine[0]))	//判断为pw的某个域
			{
				ierase_first(vstrLine[1], "T=");
				_tmpName = vstrLine[1];
				_markloop = 10;
				ipControlMark = ZONE;
			}
			switch (ipControlMark)
			{
			case ControlMark_t::ZONE://已经进入了ZONE
				_tmpZone = std::make_unique<Domain>(_tmpName);
				while (true)
				{
					CustonTokenizer tok(stringLine, sep);	//若為node，才對string進行分割
															// 输出分割结果
					vstrLine.clear();
					for (CustonTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg)
					{
						vstrLine.push_back(*beg);
					}
					if (icontains(stringLine, "nodes="))//当前行为当前ZONE域内节点和单元信息
					{
						ierase_first(vstrLine[0], "nodes=");
						_NodeNum = lexical_cast<unsigned int>(vstrLine[0]);	//当前域节点数目
						ierase_first(vstrLine[1], "elements=");
						_ElmtNum = lexical_cast<unsigned int>(vstrLine[1]);	//当前域节点数目
						ierase_first(vstrLine[2], "ZoneType=");
						//_tmpZone->ResetType(vstrLine[2]);
						getline(inFile, stringLine);
						continue;
					}
					else if (xpressive::regex_match(vstrLine[0].c_str(), what, tmpreg))//当前为数据信息，则作loop
					{
						for (size_t i = 0; i < _NodeNum; i++)
						{
							CustonTokenizer tok(stringLine, sep);
							// 输出分割结果
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
							// 输出分割结果
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

