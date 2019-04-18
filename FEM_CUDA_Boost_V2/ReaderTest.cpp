#include "stdafx.h"
#include <..\src\libReader\UFgetMatReader.h>
#include <..\src\libReader\InpDataReader.h>
#include "ReaderTest.h"


#include <metis.h> 
#include "..\programs\metisbin.h"






void ReaderTest(void * void_arg)
{
	if (0)
	{
//		using namespace HBXFEMDef;
//		UFgetMatReader  g_UFgetMatReader("UF_Index.mat", "G:\\SuiteSparse-2.1.1\\UFget\\mat");
//		bool _res = g_UFgetMatReader.SetInputData();
	}
	
	if (1)
	{
		using namespace HBXFEMDef;
		InpDataReader g_InpDataReader("B31BEAMTEST.inp", "F:\\data_from_HBX_phd\\database\\B31Test\\");
		g_InpDataReader.SetEltPropFilePath("EltProperty.xml", "F:\\data_from_HBX_phd\\vs2015\\FEM_CUDA_Boost_V2\\");
		g_InpDataReader.SetInputData();
		InputRecord* g_Record = g_InpDataReader.GetInputRecord();


		mesh_t* mesh;		//mesh文件
		graph_t* graph;		//生成的无向图
		params_t* params;	//参数宏命令

		int status = 0;	//状态值，当前选用

	}

	

}

