// FEM_CUDA_Boost_V2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "FEM_CUDA_Boost_V2.h"
#include <HBXPreDef.h>
#include <libReader\PWDataReader.h>
#include <libReader\InpDataReader.h>
#include <regex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

int main()
{
    int nRetCode = 0;

	if (1)
	{
		std::string  stringLine = "      1,   31054.8535,  -2140.90234,   8901.31738";
		std::string  stringLine2 = " -2140.90234, 31054.8535";
		std::regex	IfNumRule("((\\s+[-]\\d+(\\.\\d+)?)[\,])+");	//�ڶ�ȡpw�ļ�ʱ�������������ʽ
		std::regex	NumRule("\\s*\\d+((\.\\d*)?)");
		std::regex  Rule3("\\s*\\d+(\\.\\d+)?(,|\\s)?");//ƥ�両����
		std::smatch  o;

	//	bool _rlt2 = std::regex_match(stringLine2, Rule3);
		int i = 0;
		std::vector<float> vFloat;
		std::sregex_iterator end_iter;

		bool _rlt = std::regex_match(stringLine2, IfNumRule);

		while (regex_search(stringLine2, o, Rule3))
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
		xxx->SetSourceFilePath("spaceshuttle_quadmain.inp", "C:\\Users\\hbx\\Desktop\\");
		xxx->SetInputData();
	}


#pragma region MFC���
    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // ��ʼ�� MFC ����ʧ��ʱ��ʾ����
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: ���Ĵ�������Է���������Ҫ
            wprintf(L"����: MFC ��ʼ��ʧ��\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
        }
    }
    else
    {
        // TODO: ���Ĵ�������Է���������Ҫ
        wprintf(L"����: GetModuleHandle ʧ��\n");
        nRetCode = 1;
    }
#pragma endregion
    return nRetCode;
}
