#include "BaseReader.h"

namespace HBXFEMDef
{
	BaseReader::BaseReader()
	{
		std::cout << "create BaseReader!" << std::endl;
	}

 	bool BaseReader::SetSourceFilePath(const std::string _SourceFile, boost::filesystem::path _savepath)
 	{
 		m_path = _savepath;
 		m_SrcFileName = _SourceFile;
 		return true;
 	}
}


