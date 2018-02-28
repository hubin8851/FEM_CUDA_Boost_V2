#pragma once

#include <HBXPreDef.h>
#include <map>
#include <string>

namespace HBXFEMDef
{
	//单元属性结构体，包括单元编号，平面（三维）应力、应变问题，便于程序修改
	struct BOOST_SYMBOL_EXPORT ElemProperty
	{
		char	_Name[64];			//对应的单元类名称
		char	_ClassName[64];
		unsigned int		_Type;	//单元类型的编号	
		unsigned char _NNum;		//节点数
		char	_StressOrStrain;	//应力、应变问题
	};



	//为了程序的拓展性，可从XML里读取每种单元对应的属性
	typedef std::map < std::string, ElemProperty >	_EltPtyInMap;
	typedef _EltPtyInMap::iterator					_EltPtyMapIter;
}

