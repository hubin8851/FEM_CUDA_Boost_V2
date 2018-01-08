#include "MyFormatLog.h"


namespace HBXDef
{
	void MyFormatLog::consume( logging::record_view const & rec, string_type const & command_line )
	{
		//system函数的定义参见http://www.cplusplus.com/reference/cstdlib/system/?kw=system
		std::system( command_line.c_str() );
	}

	std::string severity_level_as_urgency(logging::value_ref<logging::trivial::severity_level, logging::trivial::tag::severity> const & level)
	{
		if (!level || level.get() == logging::trivial::info)
		{
			return "normal";
		}
		logging::trivial::severity_level lvl = level.get();
		if (lvl < logging::trivial::info)
		{
			return "low";
		}
		else
			return "critical";
	}

	void InstanceFormatLog(const char * _fileName, boost::filesystem::path _path)
	{
		boost::shared_ptr< logging::core > core = logging::core::get();
		boost::shared_ptr< MyFormat_sink > MyFormatSink(new MyFormat_sink());

		const std::pair< const char*, const char* > shell_decorations[] =
		{
			std::pair< const char*, const char* >("\"", "\\\""),
			std::pair< const char*, const char* >("$", "\\$"),
			std::pair< const char*, const char* >("!", "\\!")
		};

		// Make the formatter generate the command line for notify-send
		MyFormatSink->set_formatter
		(
			expr::stream << "notify-send -t 2000 -u "
			<< boost::phoenix::bind(&severity_level_as_urgency, logging::trivial::severity.or_none())
			<< expr::if_(expr::has_attr(process_name))
			[
				expr::stream << " -a '" << process_name << "'"
			]
		<< expr::if_(expr::has_attr(caption))
			[
				expr::stream << " \"" << expr::char_decor(shell_decorations)[expr::stream << caption] << "\""
			]
		<< " \"" << expr::char_decor(shell_decorations)[expr::stream << expr::message] << "\""
			);

		core->add_sink( MyFormatSink );
		core->add_global_attribute("ProcessName", attrs::current_process_name());
		logging::add_common_attributes();
	}

	void FormatLog_Test()
	{
//		src::severity_channel_logger< severity_level > lg(keywords::channel = "net");
//		BOOST_LOG(lg_mt) << logging::add_value("Connected", true);
		BOOST_LOG_TRIVIAL(debug) << "Hello, it's a simple notification";
		BOOST_LOG_TRIVIAL(info) << logging::add_value(caption, "Caption text") << "And this notification has caption as well";
	}

}


