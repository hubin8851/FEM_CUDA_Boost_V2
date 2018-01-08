#pragma once
#include <../../HBXDEF/BoostPreDef.h>
#include <cstdlib>
#include <string>
#include <utility>
#include <stdexcept>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/phoenix.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/current_process_name.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/basic_sink_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/value_ref.hpp>
#include <boost/log/utility/formatting_ostream.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>


namespace HBXDef
{
	//[ example_extension_record_tagger_severity
#ifndef MYSEVERITYLEVEL
#define MYSEVERITYLEVEL
	enum MySeverity_level
	{
		normal,
		notification,
		warning,
		error,
		critical
	};
#endif // !MYSEVERITYLEVEL


	namespace logging = boost::log;
	namespace src = boost::log::sources;
	namespace expr = boost::log::expressions;
	namespace sinks = boost::log::sinks;
	namespace attrs = boost::log::attributes;
	namespace keywords = boost::log::keywords;

	class MyFormatLog :
		public sinks::basic_formatted_sink_backend<char, sinks::synchronized_feeding>
	{
	private:
	protected:
	public:
		//该函数消费从前端传来的记录
		void consume( logging::record_view const& rec, string_type const& command_line );
	};

	typedef sinks::synchronous_sink< MyFormatLog > MyFormat_sink;

	BOOST_LOG_ATTRIBUTE_KEYWORD(process_name, "ProcessName", std::string)
	BOOST_LOG_ATTRIBUTE_KEYWORD(caption, "Caption", std::string)

	//自定义警告等级格式化函数
	std::string severity_level_as_urgency(logging::value_ref<logging::trivial::severity_level, logging::trivial::tag::severity> const& level);

	//实例化日志库函数并设置格式
	void InstanceFormatLog(const char* _fileName = "MyOrdinary.log", boost::filesystem::path _path = "");

	void FormatLog_Test();
}

