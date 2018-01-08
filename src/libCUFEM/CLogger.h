#pragma once
#include <HBXPreDef.h>
#include <ExportDef.h>

/************************************************************************/
/*          boost-log 头文件                                         */
/************************************************************************/
#include <cassert>  
#include <iostream>  
#include <fstream>  
#include <boost/locale/generator.hpp>  
#include <boost/date_time/posix_time/posix_time_types.hpp>  

#include <boost/log/common.hpp>  
#include <boost/log/expressions.hpp>  
#include <boost/log/utility/setup/file.hpp>  
#include <boost/log/utility/setup/console.hpp>  
#include <boost/log/utility/setup/common_attributes.hpp>  
#include <boost/log/sources/logger.hpp>  
#include <boost/log/support/date_time.hpp>  

#include <boost/filesystem.hpp>   
#include <boost/log/detail/thread_id.hpp>   
#include <boost/log/sources/global_logger_storage.hpp> 




namespace HBXFEMDef
{
	//定义自己的等级
	enum MySeverity_level
	{
		trace,
		notification,
		warning,
		error,
		critical
	};

	template< typename CharT, typename TraitsT >
	inline std::basic_ostream< CharT, TraitsT >& operator<< (
		std::basic_ostream< CharT, TraitsT >& strm, MySeverity_level lvl)
	{
		static const char* const str[] =
		{
			"trace",
			"notification",
			"warning",
			"error",
			"critical"
		};
		if (static_cast<std::size_t>(lvl) < (sizeof(str) / sizeof(*str)))
			strm << str[lvl];
		else
			strm << static_cast<int>(lvl);
		return strm;
	}


	BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
	BOOST_LOG_ATTRIBUTE_KEYWORD(_severity, "Severity", MySeverity_level)
	BOOST_LOG_ATTRIBUTE_KEYWORD(_tag_attr, "tag", std::string)
	BOOST_LOG_ATTRIBUTE_KEYWORD(_timestamp, "TimeStamp", boost::posix_time::ptime)

	namespace logging = boost::log;
	namespace src = boost::log::sources;
	namespace expr = boost::log::expressions;
	namespace sinks = boost::log::sinks;
	namespace keywords = boost::log::keywords;
	namespace attrs = boost::log::attributes;

	class CLogging
	{
		typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;
	private:
		boost::filesystem::path m_Filepath;	//源文件路径
		std::string	 m_SrcFileName;			//源文件名称
		static size_t m_szMinFreeSpace;
		static size_t m_szRotationSize;
		static bool m_bAutoFlush;
	protected:

	public:
		CLogging();
		~CLogging();
		//允许使用log
		static void Start();
		//停止使用log
		static void Stop();

		//设置记录日志文件的路径
		void ResetLogFilePath(std::string strPath);
		//设置最小空间,以M计算
		void ResetMinFreeSpace(size_t _rhsSz);
		//设置circle-buffer的大小，以M计算
		void ResetRotationSize(size_t _rhsSz);

		//core方面的函数
		void InitGlobalAttr();
		void InitThreadAttr(); 
		void InitSourceAttr();
		 
		//sink方面的函数
		//日志严重等级过滤输出
		static void SetFilterTrace();
		static void SetFilterWarning();
		static void SetFilterError();

		//设置sink的输出格式
		void SetSinkFormat();
	



	};


}
