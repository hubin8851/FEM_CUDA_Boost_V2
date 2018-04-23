#ifdef USESTDAFX	//ʹ��Ԥ����ͷ�ļ��� 
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

		std::string _tmppath;		//·����ʱ����
		_tmppath = m_path.append(m_SrcFileName);

		TiXmlDocument _tmpdoc(_tmppath.c_str());
		TiXmlElement*	_tmpElmt = nullptr;
		std::shared_ptr<HBXDef::_AEROTABLE> _tmpTable = std::make_shared<HBXDef::_AEROTABLE>();

		bool	_loadOkey = _tmpdoc.LoadFile();
		if (!_loadOkey)
		{
			std::cerr << "����XML�ļ����󣬸��ļ�·��Ϊ" << m_path << std::endl;
			std::cerr << "��ǰ������Ϊ" << _tmpdoc.ErrorDesc() << std::endl;
			return InputFileResult::IRRT_BAD_FORMAT;
		}
		_tmpElmt = _tmpdoc.RootElement();
		std::string _tmpstr(_tmpElmt->FirstAttribute()->Value());
		boost::to_lower(_tmpstr);

		if (!ReadTableAttri(_tmpElmt, _tmpTable.get()))
		{
			std::cerr << "�����ͷ���Գ���..." << std::endl;
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
						std::cerr << "����BLOCK���Դ���" << std::endl;
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
			std::cerr << "����TinyXml�ı�ָ�����..." << std::endl;
			return false;
		}
		TiXmlAttribute* _TmpAttri = _inputT->FirstAttribute();
		while (_TmpAttri)//��block��ѭ��
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
			std::cerr << "����TinyXml�ı�ָ�����..." << std::endl;
			return false;
		}
		if (!ReadBlockAttri(_inputT, _outBlock))
		{
			std::cerr << "����TinyXml�Ŀ����Դ���..." << std::endl;
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
					std::cerr << "����TinyXml��ά�����Դ���..." << std::endl;
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
			_outBlock->_data = new HBXDef::UserCalPrec[_tmpdataNum];	//�ڴ�ʱ���в�����֪
			ReadInterpolationData(_tmpNode, _outBlock, _tmpdataNum);
		}
		return true;
	}

	bool AeroCoefReader::ReadBlockAttri(TiXmlElement * _inputT, HBXDef::_AEROBLOCK * _outBlock)
	{
		using namespace boost;
		if (nullptr == _inputT)
		{
			std::cerr << "����TinyXml�Ŀ�����ָ�����..." << std::endl;
			return false;
		}
		TiXmlAttribute* _TmpAttri = _inputT->FirstAttribute();
		while (_TmpAttri)//��block�ڵ�������ѭ��
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
				if (_outBlock->_dim > 0 && _outBlock->_dim < 100)	//���������Χ����ô��...
				{
					_outBlock->_numperdim = new unsigned int[_outBlock->_dim];
//					_outBlock->_corddata = new HBXDef::UserCalPrec*[_outBlock->_dim];//�Զ�ά��������ά�ȷ���ռ�
					 //_outBlock->_beg = new unsigned int[_outBlock->_dim];
				}
				else
				{
					std::cout << "��ǰ����ά�Ȳ�����Ҫ��..." << std::endl;
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
		HBXDef::IFNULL(_inputT, "����TinyXml�ı�ָ�����...");

		TiXmlAttribute* _TmpAttri = _inputT->FirstAttribute();
		std::string _strText;	//�4G���㹻

		while (_TmpAttri)
		{
			if (iequals(_TmpAttri->Name(), "name"))
			{
				//�˴�����
			}
			if (iequals(_TmpAttri->Name(), "demention"))
			{
				_outBlock->_numperdim[_idx] = _TmpAttri->IntValue();
				//Ϊ��ǰά�Ȳ�ֵ����������ڴ�
// #ifdef DEBUG
// 				_outBlock->_corddata[_idx] = new HBXDef::UserCalPrec[_TmpAttri->IntValue()];
// #endif // DEBUG
// 				_outBlock->_corddata[_idx] = new HBXDef::UserCalPrec[_TmpAttri->IntValue()];
			}
			_TmpAttri = _TmpAttri->Next();
		}
		_strText = _inputT->GetText();

		// ����ָʽΪӢ�Ķ��ţ����Ķ��źͿո񣬹���һ���ִ�����  
		boost::char_separator<char> sep(" ,��*");
		typedef boost::tokenizer<boost::char_separator<char> >  CustonTokenizer;
		CustonTokenizer tok(_strText, sep);
		// ����ָ���
		size_t _idy = 0;
		for (CustonTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg, _idy++)
		{
			vecOut.push_back(lexical_cast<HBXDef::UserCalPrec>(*beg));	//��֤��
//			_outBlock->_corddata[_idx][_idy] = lexical_cast<double>(*beg);
		}
// 		if (vecOut.size() != _outBlock->_numperdim[_idx])//�ж������ڵĲ�����ʵ�����ݸ����Ƿ����
// 		{
// 			std::cerr << "ά�����������ֵ����������..." << std::endl;
// 			return 0;
// 		}

		return _outBlock->_numperdim[_idx];
	}

	bool AeroCoefReader::ReadInterpolationData(TiXmlElement * _inputT, HBXDef::_AEROBLOCK * _outBlock, size_t _lgt)
	{
		using namespace boost;
		HBXDef::IFNULL(_inputT, "��ֵ���ݱ������...")

		std::string _strText;
		std::vector<std::string> vstrText;

		_strText = _inputT->GetText();
		HBXDef::IFNULL(_strText.c_str(), "��ǰ��������Ϊ��")
			// ����ָʽΪӢ�Ķ��ţ����Ķ��źͿո񣬹���һ���ִ�����  
			boost::char_separator<char> sep(" ,��*");
		typedef boost::tokenizer<boost::char_separator<char> >  CustonTokenizer;
		CustonTokenizer tok(_strText, sep);
		// ����ָ���
		vstrText.clear();
		size_t _idx = 0;
		for (CustonTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg, _idx++)
		{
			vstrText.push_back(*beg);	//��֤��
			_outBlock->_data[_idx] = lexical_cast<double>(*beg);
		}
		if (vstrText.size() != _lgt)//�ж������ڵĲ�����ʵ�����ݸ����Ƿ����
		{
			std::cerr << "ά�����������ֵ����������..." << std::endl;
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

	//��ȡ�������ݱ�
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

