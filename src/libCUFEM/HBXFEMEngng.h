#pragma once

namespace HBXFEMDef
{
	typedef enum problemScale
	{
		macroScale,
		microScale
	}problemScale_t;

	typedef enum nlinearFormul
	{
		UNKNOWN,
		TOTALL,	//全局拉格朗日
		UPDATEL	//更新拉格朗日
	}nlinearFormul_t;


}
