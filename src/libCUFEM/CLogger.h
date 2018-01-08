#pragma once
#include <HBXPreDef.h>
#include <ExportDef.h>

/************************************************************************/
/*          boost-log ͷ�ļ�                                         */
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
	//�����Լ��ĵȼ�
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
		boost::filesystem::path m_Filepath;	//Դ�ļ�·��
		std::string	 m_SrcFileName;			//Դ�ļ�����
		static size_t m_szMinFreeSpace;
		static size_t m_szRotationSize;
		static bool m_bAutoFlush;
	protected:

	public:
		CLogging();
		~CLogging();
		//����ʹ��log
		static void Start();
		//ֹͣʹ��log
		static void Stop();

		//���ü�¼��־�ļ���·��
		void ResetLogFilePath(std::string strPath);
		//������С�ռ�,��M����
		void ResetMinFreeSpace(size_t _rhsSz);
		//����circle-buffer�Ĵ�С����M����
		void ResetRotationSize(size_t _rhsSz);

		//core����ĺ���
		void InitGlobalAttr();
		void InitThreadAttr(); 
		void InitSourceAttr();
		 
		//sink����ĺ���
		//��־���صȼ��������
		static void SetFilterTrace();
		static void SetFilterWarning();
		static void SetFilterError();

		//����sink�������ʽ
		void SetSinkFormat();
	



	};


}
