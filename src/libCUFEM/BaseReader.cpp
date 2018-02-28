#include "BaseReader.h"
#include <libCUFEM\BaseSlnRecord.h>
namespace HBXFEMDef
{
	BaseReader::BaseReader()
	{
//		std::cout << "create BaseReader!" << std::endl; //验证多态性用
		m_SlnRecord = new BaseSlnRecord;
	}

 	bool BaseReader::SetSourceFilePath(const std::string _SourceFile, boost::filesystem::path _savepath)
 	{
 		m_path = _savepath;
 		m_SrcFileName = _SourceFile;
 		return true;
 	}

	BaseSlnRecord* BaseReader::GetSlnRecord()
	{
		return this->m_SlnRecord;
	}
}


