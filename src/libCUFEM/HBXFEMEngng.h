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
		TOTALL,	//ȫ����������
		UPDATEL	//������������
	}nlinearFormul_t;


}
