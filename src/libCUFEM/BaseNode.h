#pragma once
#include <boost/dll.hpp>
#include <ExportDef.h>


namespace HBXFEMDef
{
	//��xyz�������������ֱ�洢��������ʽ������GPU������������
	template<typename _T>
	struct BOOST_SYMBOL_EXPORT _Nodes
	{
		_T* _X;
		_T* _Y;
		_T* _Z;
	};

	//�����ڵ㣬����xyz�����������
// 	template<typename _T>
// 	struct _Node
// 	{
// 		_T _X;
// 		_T _Y;
// 		_T _Z;
// 	};

	struct BOOST_SYMBOL_EXPORT Node
	{
		double _X;
		double _Y;
		double _Z;
	};

}

