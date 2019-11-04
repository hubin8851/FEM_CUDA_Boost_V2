// FEM_CUDA_Boost_V2.cpp : 定义控制台应用程序的入口点。
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
#include "NumericalCalTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern "C" CUT_THREADPROC InterpolateTest(void* void_arg);
CUT_THREADPROC AsynInterpolateTest(void* void_arg);
extern "C" float MultiInterpolateTest(void* void_arg);
extern "C" void EngngTest(void* void_arg);
extern "C" void ReaderTest(void* void_arg);
extern "C" void ElementMatricCalTest(void* void_arg);
extern "C" void NumericalCalTest(int argc, char **argv);
// 唯一的应用程序对象


using namespace std;

int main(int argc, char **argv)
{	
	if (0)//测试文件读取
	{
		ReaderTest();
	}
	if (0)//插值计算测试
	{
//		InterpolateTest();	
//		MultiInterpolateTest();
	}

	if (1)//迭代法测试
	{
		NumericalCalTest(argc, argv);
	}

	if(0)//VTK测试
	{
		vtktest();
	}

	if (0)//单元矩阵计算测试
	{
		ElementMatricCalTest();
//		EngngTest();
	}



    return 0;
}


