#include "stdafx.h"
#include <../src/libReader/UFgetMatReader.h>
#include "ReaderTest.h"


void ReaderTest(void * void_arg)
{
	using namespace HBXFEMDef;
	UFgeMatReader  g_UFgetMatReader("UF_Index.mat", "G:\\SuiteSparse-2.1.1\\UFget\\mat");
	HBXDef::CheckUserDefErrors( g_UFgetMatReader.SetInputData() );
}
