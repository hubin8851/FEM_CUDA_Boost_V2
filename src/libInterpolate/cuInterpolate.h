#pragma once
#include <HbxDefMacro.h>
#include <CuDefMacro.h>

namespace HBXDef
{
	template<unsigned int T> class cuTable;
	template<unsigned int T> class cuInterval;

}




//插值计算CUDA版本，返回花费的时间
template< unsigned int T>
float cuInterpolate(HBXDef::cuTable<T>* _table,
	HBXDef::cuInterval<T>* _posIn,
	HBXDef::UserCalPrec* _DataOut);


#ifdef __cplusplus

#endif
#ifdef __cplusplus

#endif


//模板函数/类不能导出到动态链接库（DLL） 
//模板函数在声明的时候， 其实并不存在，函数地址也就无从谈起了，而导出到动态链接库的函数都需要有地址，也就是说——函数模板不具备导出的基本条件。
//函数模板在调用时后，有了具体的实现，这个时候才有了地址。
template __declspec(dllexport) float cuInterpolate(HBXDef::cuTable<2>* _table,
	HBXDef::cuInterval<2>* _posIn,
	HBXDef::UserCalPrec* _DataOut);

template __declspec(dllexport) float cuInterpolate(HBXDef::cuTable<3>* _table,
	HBXDef::cuInterval<3>* _posIn,
  	HBXDef::UserCalPrec* _DataOut);
