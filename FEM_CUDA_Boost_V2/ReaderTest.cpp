#include "stdafx.h"
#include <..\src\libReader\MMSpMatReader.h>
#include <..\src\libReader\UFgetMatReader.h>
#include <..\src\libReader\InpDataReader.h>
#include "ReaderTest.h"


#include <metis.h> 
#include "..\programs\metisbin.h"






void ReaderTest(void * void_arg)
{
	if (1)//MTX��ȡ��
	{
		using namespace HBXFEMDef;
		MMSpMatReader g_MMSpMatReader("bcsstk01.mtx");
		g_MMSpMatReader.SetInputData();
	}


	if (0)//UFGET��ȡ������
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


		mesh_t* mesh;		//mesh�ļ�
		graph_t* graph;		//���ɵ�����ͼ
		params_t* params;	//����������

		int status = 0;	//״ֵ̬����ǰѡ��

	}

	

}

