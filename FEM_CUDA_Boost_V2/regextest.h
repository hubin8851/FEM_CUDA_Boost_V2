#pragma once
#include <regex>

//正则表达式test
void RegexTest()
{
	std::string  stringLine = "      1,   31054.8535,  -2140.90234,   8901.31738";
	std::string  stringLine2 = "*Part, name=spaceshuttle2005";
	std::regex	IfNumRule("((\\s+[-]\\d+(\\.\\d+)?)[\,])+");	//在读取pw文件时，所需的正则表达式
	std::regex  IfNotNum("\\*+.*");	//正则表达式，以*起始的字符串
	std::regex	NumRule("\\s*\\d+((\.\\d*)?)");
	std::regex  Rule3("\\s*\\d+(\\.\\d+)?(,|\\s)?");//匹配浮点数
	std::smatch  o;

	//	bool _rlt2 = std::regex_match(stringLine2, Rule3);
	int i = 0;
	std::vector<float> vFloat;
	std::sregex_iterator end_iter;

	bool _rlt = std::regex_match(stringLine2, IfNotNum);

	if (!std::regex_match(stringLine, IfNotNum))
	{
		auto begin_iter = std::sregex_iterator(stringLine.begin(), stringLine.end(), Rule3);
		for (auto it = begin_iter; it != end_iter; it++)
		{
			vFloat.emplace_back(std::move(std::stof(it->str())));
		}
	}

}