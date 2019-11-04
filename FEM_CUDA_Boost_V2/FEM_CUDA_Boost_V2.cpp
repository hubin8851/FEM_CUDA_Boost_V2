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
// Ψһ��Ӧ�ó������


using namespace std;

int main(int argc, char **argv)
{	
	if (0)//�����ļ���ȡ
	{
		ReaderTest();
	}
	if (0)//��ֵ�������
	{
//		InterpolateTest();	
//		MultiInterpolateTest();
	}

	if (1)//����������
	{
		NumericalCalTest(argc, argv);
	}

	if(0)//VTK����
	{
		vtktest();
	}

	if (0)//��Ԫ����������
	{
		ElementMatricCalTest();
//		EngngTest();
	}



    return 0;
}


