#pragma once
#include <string>
#include <map>
//2015-1-28, HBX, 定义单元类型宏//
//2015-9-30,HBX,吐血修改...囧...
//2017-9-25,HBX,吐什么...不知道了...
//2017-9-26,HBX,改回普通类，怂了。
#include <boost/dll.hpp>

namespace HBXFEMDef
{


	//单元属性结构体，包括单元编号，平面（三维）应力、应变问题，便于程序修改
	struct BOOST_SYMBOL_EXPORT ElemProperty
	{
		std::string	classname;		//对应的单元类名称
		unsigned int	_type;			//单元类型的编号
		char			StressOrStrain;	//应力、应变问题
	};

	//为了程序的拓展性，可从XML里读取每种单元对应的属性
	typedef std::map < std::string, ElemProperty >	_EltPtyInMap;
	typedef _EltPtyInMap::iterator					_EltPtyMapIter;

}