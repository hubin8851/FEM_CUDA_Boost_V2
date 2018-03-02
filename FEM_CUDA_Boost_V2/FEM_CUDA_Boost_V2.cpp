// FEM_CUDA_Boost_V2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "FEM_CUDA_Boost_V2.h"
#include <HBXPreDef.h>
#include <libCUFEM\util.h>
#include <libReader\PWDataReader.h>
#include <libReader\InpDataReader.h>
#include <regex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int main()
{
    int nRetCode = 0;

	if (0)
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

	if (1)
	{
		using namespace HBXFEMDef;
		BaseReader* xxx = InstanceInpReader();
//		xxx->SetSourceFilePath("spaceshuttle_quadmain.inp", "C:\\Users\\hbx\\Desktop\\");
		xxx->SetSourceFilePath("beam.inp", "D:\\AbaqusTemp"); 
		xxx->SetInputData();

		HBXFEMDef::Engng* em = HBXFEMDef::InstanceProblem(xxx, problemMode_t::_processor, 0);
	}


#pragma region MFC相关
    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // 初始化 MFC 并在失败时显示错误
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: 更改错误代码以符合您的需要
            wprintf(L"错误: MFC 初始化失败\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: 在此处为应用程序的行为编写代码。
        }
    }
    else
    {
        // TODO: 更改错误代码以符合您的需要
        wprintf(L"错误: GetModuleHandle 失败\n");
        nRetCode = 1;
    }
#pragma endregion
    return nRetCode;
}
