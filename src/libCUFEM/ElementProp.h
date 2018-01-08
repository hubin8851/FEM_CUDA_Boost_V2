#pragma once
#include <string>
#include <map>
//2015-1-28, HBX, ���嵥Ԫ���ͺ�//
//2015-9-30,HBX,��Ѫ�޸�...��...
//2017-9-25,HBX,��ʲô...��֪����...
//2017-9-26,HBX,�Ļ���ͨ�࣬���ˡ�
#include <boost/dll.hpp>

namespace HBXFEMDef
{


	//��Ԫ���Խṹ�壬������Ԫ��ţ�ƽ�棨��ά��Ӧ����Ӧ�����⣬���ڳ����޸�
	struct BOOST_SYMBOL_EXPORT ElemProperty
	{
		std::string	classname;		//��Ӧ�ĵ�Ԫ������
		unsigned int	_type;			//��Ԫ���͵ı��
		char			StressOrStrain;	//Ӧ����Ӧ������
	};

	//Ϊ�˳������չ�ԣ��ɴ�XML���ȡÿ�ֵ�Ԫ��Ӧ������
	typedef std::map < std::string, ElemProperty >	_EltPtyInMap;
	typedef _EltPtyInMap::iterator					_EltPtyMapIter;

}