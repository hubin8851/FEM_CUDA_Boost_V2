// FEM_CUDA_Boost_V2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "FEM_CUDA_Boost_V2.h"
#include <HBXPreDef.h>
#include "vtktest.h"
#include "regextest.h"
#include "IntervalTest.h"
#include "EngngTest.h"
#include "ReaderTest.h"
#include "ElementMatricTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern "C" CUT_THREADPROC InterpolateTest(void* void_arg);
CUT_THREADPROC AsynInterpolateTest(void* void_arg);
extern "C" float MultiInterpolateTest(void* void_arg);
extern "C" void EngngTest(void* void_arg);
extern "C" void ReaderTest(void* void_arg);
extern "C" void ElementMatricCalTest(void* void_arg);
// Ψһ��Ӧ�ó������


using namespace std;

int main()
{
	if (0)
	{
//		InterpolateTest();	
//		MultiInterpolateTest();
	}

	if (0)
	{
		ReaderTest();
	}
	if(1)
	{
		vtktest();
	}

	if (0)
	{
		ElementMatricCalTest();
//		EngngTest();
	}



    return 0;
}


