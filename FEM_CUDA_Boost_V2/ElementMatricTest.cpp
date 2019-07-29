#include "stdafx.h"
#include <chrono>
#include <..\src\libReader\UFgetMatReader.h>
#include <..\src\libReader\InpDataReader.h>
#include <..\src\libElement\Beam21.h>
#include <..\src\libElement\Beam31.h>
#include <..\src\libElement\S4R.h>
#include "MyTimer.h"
#include "ElementMatricTest.h"


size_t g_MultiFactor = 100;

extern "C"
void ElementMatricCalTest(void * void_arg)
{
	using namespace HBXFEMDef;
	InpDataReader g_InpDataReader("B31BEAMTEST.inp", "F:\\data_from_HBX_phd\\database\\B31Test\\");
	g_InpDataReader.SetEltPropFilePath("EltProperty.xml", "F:\\data_from_HBX_phd\\vs2015\\FEM_CUDA_Boost_V2\\");
	g_InpDataReader.SetInputData();
	InputRecord* g_Record = g_InpDataReader.GetInputRecord();
	std::vector<HBXFEMDef::Node> g_Node;
	g_Record->GiveField(g_Node,"Node");
	int _lgth =g_Node.size();
	Beam21Elemt g_B21(nullptr, nullptr,0);
	
	auto start = std::chrono::system_clock::now();
	for (size_t i = 0; i < g_MultiFactor; i++)
	{
		for (int i = 0; i < _lgth - 1; i++)
		{
			g_B21.StiffMatCal(1, 1, g_Node[i]._X, g_Node[i]._Y,
				g_Node[i + 1]._X, g_Node[i + 1]._Y, 0);
		}
	}
	auto end = std::chrono::system_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	cout << "»¨·ÑÁË"
		<< double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den << "Ãë" << endl;
}
