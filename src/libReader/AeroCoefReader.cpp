#ifdef USESTDAFX	//使用预编译头文件宏 
#include "stdafx.h"
#endif

#include "AeroCoefReader.h"
#include <libReader\AeroTableRecord.h>

namespace HBXFEMDef
{
	InputFileResult AeroCoefReader::ReadAeroCoef()
	{
		using namespace std;
		using namespace boost;
		using boost::property_tree::ptree;

		std::string _tmppath;		//路径临时变量
		_tmppath = m_path.append(m_SrcFileName);

		TiXmlDocument _tmpdoc(_tmppath.c_str());
		TiXmlElement*	_tmpElmt = nullptr;
		std::shared_ptr<HBXDef::_AEROTABLE> _tmpTable = std::make_shared<HBXDef::_AEROTABLE>();

		bool	_loadOkey = _tmpdoc.LoadFile();
		if (!_loadOkey)
		{
			std::cerr << "载入XML文件错误，该文件路径为" << m_path << std::endl;
			std::cerr << "当前错误码为" << _tmpdoc.ErrorDesc() << std::endl;
			return InputFileResult::IRRT_BAD_FORMAT;
		}
		_tmpElmt = _tmpdoc.RootElement();
		std::string _tmpstr(_tmpElmt->FirstAttribute()->Value());
		boost::to_lower(_tmpstr);

		if (!ReadTableAttri(_tmpElmt, _tmpTable.get()))
		{
			std::cerr << "载入表头属性出错..." << std::endl;
			return InputFileResult::IRRT_BAD_FORMAT;
		}

		TiXmlElement* _blocks = _tmpElmt->FirstChildElement();
		for (size_t i = 0; i < _tmpTable->_blocknum; i++)
		{
			if (iequals(_blocks->Value(), "block"))
			{
				for (size_t j = 0; j < _tmpTable->_blocknum; j++)
				{
					if (!ReadBlock(_blocks, _tmpTable->_blocks + j))
					{
						std::cerr << "读入BLOCK属性错误" << std::endl;
						exit(1);
					}
				}
			}
		}

		m_Record->SetField(_tmpTable, _tmpstr.c_str());

		return InputFileResult::IRRT_OK;
	}

	bool AeroCoefReader::ReadTableAttri(TiXmlElement * _inputT, HBXDef::_AEROTABLE * _outTable)
	{
		using namespace boost;
		if (nullptr == _inputT)
		{
			std::cerr << "读入TinyXml的表指针错误..." << std::endl;
			return false;
		}
		TiXmlAttribute* _TmpAttri = _inputT->FirstAttribute();
		while (_TmpAttri)//对block做循环
		{
			std::string _name = _TmpAttri->Name();
			if (iequals(_name, "name"))
			{
#ifdef WIN32
				strcpy_s(_outTable->_name, _TmpAttri->Value());
#else
				strcpy(_outTable->_name, _TmpAttri->Value());
#endif // !WIN32
			}
			if (iequals(_name, "block"))
			{
				_outTable->_blocknum = lexical_cast<int>(_TmpAttri->Value());
				_outTable->_blocks = new HBXDef::_AeroBlock[_outTable->_blocknum];
			}

			_TmpAttri = _TmpAttri->Next();
		}
		delete _TmpAttri;
		return true;
	}

	bool AeroCoefReader::ReadBlock(TiXmlElement * _inputT, HBXDef::_AEROBLOCK * _outBlock)
	{
		using namespace boost;
		if (nullptr == _inputT)
		{
			std::cerr << "读入TinyXml的表指针错误..." << std::endl;
			return false;
		}
		if (!ReadBlockAttri(_inputT, _outBlock))
		{
			std::cerr << "读入TinyXml的块属性错误..." << std::endl;
			return false;
		}
		TiXmlElement* _tmpNode = _inputT->FirstChildElement();

		size_t _tmpdataNum = 1;
		size_t _tmpcordNum = 0;
		std::vector<HBXDef::UserCalPrec> _dimVec;
		if (iequals(_tmpNode->Value(), "demtion"))
		{
			for (size_t i = 0; i < _outBlock->_dim; i++)
			{
				size_t _tmpNum = ReadDemtion(_tmpNode, _outBlock, i, _dimVec);
				_tmpdataNum *= _tmpNum;
				_tmpcordNum += _tmpNum;
				if (_tmpdataNum < 0)
				{
					std::cerr << "读入TinyXml的维度属性错误..." << std::endl;
					return false;
				}
				_tmpNode = _tmpNode->NextSiblingElement();
			}
			_outBlock->_corddata = new HBXDef::UserCalPrec[_tmpcordNum];
			memcpy(_outBlock->_corddata, _dimVec.data(), _dimVec.size() * sizeof(HBXDef::UserCalPrec));
		}
		//TiXmlNode*  _tmpNode = _tmpNode;
		if (equals(_tmpNode->Value(), "Data"))
		{
			_outBlock->_data = new HBXDef::UserCalPrec[_tmpdataNum];	//在此时所有参数已知
			ReadInterpolationData(_tmpNode, _outBlock, _tmpdataNum);
		}
		return true;
	}

	bool AeroCoefReader::ReadBlockAttri(TiXmlElement * _inputT, HBXDef::_AEROBLOCK * _outBlock)
	{
		using namespace boost;
		if (nullptr == _inputT)
		{
			std::cerr << "读入TinyXml的块属性指针错误..." << std::endl;
			return false;
		}
		TiXmlAttribute* _TmpAttri = _inputT->FirstAttribute();
		while (_TmpAttri)//对block内的属性做循环
		{
			std::string _name = _TmpAttri->Name();
			if (iequals(_name, "name"))
			{
#ifdef WIN32
				strcpy_s(_outBlock->_name, _TmpAttri->Value());
#else
				strcpy(_outBlock->_name, _TmpAttri->Value());
#endif // WIN32
			}
			if (iequals(_name, "demention"))
			{
				_outBlock->_dim = lexical_cast<int>(_TmpAttri->Value());
				if (_outBlock->_dim > 0 && _outBlock->_dim < 100)	//这个参数范围合适么？...
				{
					_outBlock->_numperdim = new unsigned int[_outBlock->_dim];
//					_outBlock->_corddata = new HBXDef::UserCalPrec*[_outBlock->_dim];//对二维数组的外层维度分配空间
					 //_outBlock->_beg = new unsigned int[_outBlock->_dim];
				}
				else
				{
					std::cout << "当前块内维度不满足要求..." << std::endl;
				}
			}

			_TmpAttri = _TmpAttri->Next();
		}
		delete _TmpAttri;
		return true;
	}


	size_t AeroCoefReader::ReadDemtion(TiXmlElement * _inputT, 
									HBXDef::_AEROBLOCK * _outBlock, 
									size_t _idx,
									std::vector<HBXDef::UserCalPrec>& vecOut )
	{
		using namespace boost;
		HBXDef::IFNULL(_inputT, "读入TinyXml的表指针错误...");

		TiXmlAttribute* _TmpAttri = _inputT->FirstAttribute();
		std::string _strText;	//最长4G，足够

		while (_TmpAttri)
		{
			if (iequals(_TmpAttri->Name(), "name"))
			{
				//此处留下
			}
			if (iequals(_TmpAttri->Name(), "demention"))
			{
				_outBlock->_numperdim[_idx] = _TmpAttri->IntValue();
				//为当前维度插值点数组分配内存
// #ifdef DEBUG
// 				_outBlock->_corddata[_idx] = new HBXDef::UserCalPrec[_TmpAttri->IntValue()];
// #endif // DEBUG
// 				_outBlock->_corddata[_idx] = new HBXDef::UserCalPrec[_TmpAttri->IntValue()];
			}
			_TmpAttri = _TmpAttri->Next();
		}
		_strText = _inputT->GetText();

		// 定义分割方式为英文逗号，中文逗号和空格，构造一个分词器，  
		boost::char_separator<char> sep(" ,，*");
		typedef boost::tokenizer<boost::char_separator<char> >  CustonTokenizer;
		CustonTokenizer tok(_strText, sep);
		// 输出分割结果
		size_t _idy = 0;
		for (CustonTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg, _idy++)
		{
			vecOut.push_back(lexical_cast<HBXDef::UserCalPrec>(*beg));	//验证用
//			_outBlock->_corddata[_idx][_idy] = lexical_cast<double>(*beg);
		}
// 		if (vecOut.size() != _outBlock->_numperdim[_idx])//判断属性内的参数与实际数据个数是否相符
// 		{
// 			std::cerr << "维度内属性与插值点个数不相符..." << std::endl;
// 			return 0;
// 		}

		return _outBlock->_numperdim[_idx];
	}

	bool AeroCoefReader::ReadInterpolationData(TiXmlElement * _inputT, HBXDef::_AEROBLOCK * _outBlock, size_t _lgt)
	{
		using namespace boost;
		HBXDef::IFNULL(_inputT, "插值数据表导入错误...")

		std::string _strText;
		std::vector<std::string> vstrText;

		_strText = _inputT->GetText();
		HBXDef::IFNULL(_strText.c_str(), "当前块内数据为空")
			// 定义分割方式为英文逗号，中文逗号和空格，构造一个分词器，  
			boost::char_separator<char> sep(" ,，*");
		typedef boost::tokenizer<boost::char_separator<char> >  CustonTokenizer;
		CustonTokenizer tok(_strText, sep);
		// 输出分割结果
		vstrText.clear();
		size_t _idx = 0;
		for (CustonTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg, _idx++)
		{
			vstrText.push_back(*beg);	//验证用
			_outBlock->_data[_idx] = lexical_cast<double>(*beg);
		}
		if (vstrText.size() != _lgt)//判断属性内的参数与实际数据个数是否相符
		{
			std::cerr << "维度内属性与插值点个数不相符..." << std::endl;
			return false;
		}

		return true;
	}

	AeroCoefReader::AeroCoefReader(const std::string & _SourceFile, std::string _savepath)
	{
		this->SetSourceFilePath(_SourceFile, _savepath);

		std::cout << "create AeroCoefReader!" << std::endl;

		m_Record = new AeroTableRecord();
	}

	//获取气动数据表
	HBXDef::_AEROTABLE* AeroCoefReader::GetAeroTable(const char* _str)
	{
		using namespace std;

		return m_Record->GiveField( _str);
	}

	bool AeroCoefReader::SetInputData()
	{
		namespace fs = boost::filesystem;
		using namespace HBXDef;
		if (fs::extension(m_SrcFileName) == ".xml")
		{
			CheckUserDefErrors(ReadAeroCoef());
			return true;
		}

		return false;
	}

	void AeroCoefReader::terminate()
	{
	}




	CUFEM_API BaseReader * InstanceAeroCoefReader()
	{
		return (BaseReader*)new AeroCoefReader();
	}

}

