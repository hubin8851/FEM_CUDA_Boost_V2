#include "CLogger.h"

namespace HBXFEMDef
{
	CLogging::CLogging()
	{
		m_szMinFreeSpace = 10 * 1024 * 1024;
		m_szRotationSize = 1 * 1024 * 1024;
		m_bAutoFlush = false;
	}

	CLogging::~CLogging()
	{
	}
	void CLogging::Start()
	{
		logging::core::get()->set_logging_enabled(true);
	}
	void CLogging::Stop()
	{
		logging::core::get()->set_logging_enabled(false);
	}
	void CLogging::ResetLogFilePath(std::string strPath)
	{
		m_Filepath = strPath;
	}
	void CLogging::ResetMinFreeSpace(size_t _rhsSz)
	{
		m_szMinFreeSpace = _rhsSz * 1024 * 1024;
	}
	void CLogging::ResetRotationSize(size_t _rhsSz)
	{
		m_szRotationSize = _rhsSz * 1024 * 1024;
	}
	void CLogging::InitGlobalAttr()
	{

	}
	void CLogging::SetFilterTrace()
	{
		logging::core::get()->set_filter(expr::attr<MySeverity_level>("Severity") >= trace);
	}
	void CLogging::SetFilterWarning()
	{
		logging::core::get()->set_filter(expr::attr<MySeverity_level>("Severity") >= warning);
	}
	void CLogging::SetFilterError()
	{
		logging::core::get()->set_filter(expr::attr<MySeverity_level>("Severity") >= error);
	}
	void CLogging::SetSinkFormat()
	{
		logging::formatter fmt = expr::stream
			<< std::setw(6) << std::setfill('0') << line_id << std::setfill(' ')
			<< ": <" << _severity << ">\t"
			<< expr::if_(expr::has_attr(_tag_attr))
			[
				expr::stream << "[" << _tag_attr << "] "
			]
		<< expr::smessage;
	}


}

