#pragma once

#include <HBXPreDef.h>
#include <map>
#include <string>

namespace HBXFEMDef
{
	//��Ԫ���Խṹ�壬������Ԫ��ţ�ƽ�棨��ά��Ӧ����Ӧ�����⣬���ڳ����޸�
	struct BOOST_SYMBOL_EXPORT ElemProperty
	{
		char	_Name[64];			//��Ӧ�ĵ�Ԫ������
		char	_ClassName[64];
		unsigned int		_Type;	//��Ԫ���͵ı��	
		unsigned char _NNum;		//�ڵ���
		char	_StressOrStrain;	//Ӧ����Ӧ������
	};



	//Ϊ�˳������չ�ԣ��ɴ�XML���ȡÿ�ֵ�Ԫ��Ӧ������
	typedef std::map < std::string, ElemProperty >	_EltPtyInMap;
	typedef _EltPtyInMap::iterator					_EltPtyMapIter;
}

