#include "stdafx.h"
#include <..\src\libReader\MMSpMatReader.h>
#include <..\src\libReader\UFgetMatReader.h>
#include <..\src\libReader\InpDataReader.h>
#include "ReaderTest.h"


#include <metis.h> 
#include "..\programs\metisbin.h"






void ReaderTest(void * void_arg)
{
	if (1)//MTX读取器
	{
		using namespace HBXFEMDef;
		MMSpMatReader g_MMSpMatReader("bcsstk01.mtx");
		g_MMSpMatReader.SetInputData();
	}


	if (0)//UFGET读取器测试
	{
		using namespace HBXFEMDef;
		UFgetMatReader  g_UFgetMatReader("bcsstk03.mat", "G:\\SuiteSparse-2.1.1\\UFget\\mat\\HB");
		bool _res = g_UFgetMatReader.SetInputData();
		HBXDef::_CSRInput<HBXDef::UserCalPrec>* _tmpCSR = g_UFgetMatReader.GetStiffMat();


	}
	
	if (0)
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

