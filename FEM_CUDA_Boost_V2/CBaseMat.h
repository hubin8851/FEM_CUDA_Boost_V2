#pragma once

#include <HBXPreDef.h>

extern HBXDef::PrecsType_t	g_PrecsType;	//全局变量，设置数据读取和计算的精度(float or double)

namespace HBXFEMDef
{
	using namespace boost;
	using namespace HBXDef;
	//材料定义结构体
	template<typename _T>
	class _BaseMaterial
	{
	public:
		_BaseMaterial(_T _E = 0, _T _mu = 0, _T _Gama = 0, const char *ch = nullptr) :dE(_E), dMu(_mu), dMaGama(_Gama)
		{
			strcpy(this->chname, ch);
			dG = dE / (2 * (1 + dMu));
		}

		_T dE;	//弹性模量
		_T dMu;	//泊松比
		_T dG;	//剪切模量
		_T dAlpha;	//线膨胀系数
		_T dMaGama; //重度
		char   chname[40];//材料名称字符数组
	};



}
