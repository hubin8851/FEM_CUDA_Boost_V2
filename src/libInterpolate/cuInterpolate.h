#pragma once
#include <HbxDefMacro.h>
#include <CuDefMacro.h>

namespace HBXDef
{
	template<unsigned int T> class cuTable;
	template<unsigned int T> class cuInterval;

}




//��ֵ����CUDA�汾�����ػ��ѵ�ʱ��
template< unsigned int T>
float cuInterpolate(HBXDef::cuTable<T>* _table,
	HBXDef::cuInterval<T>* _posIn,
	HBXDef::UserCalPrec* _DataOut);


#ifdef __cplusplus

#endif
#ifdef __cplusplus

#endif


//ģ�庯��/�಻�ܵ�������̬���ӿ⣨DLL�� 
//ģ�庯����������ʱ�� ��ʵ�������ڣ�������ַҲ���޴�̸���ˣ�����������̬���ӿ�ĺ�������Ҫ�е�ַ��Ҳ����˵��������ģ�岻�߱������Ļ���������
//����ģ���ڵ���ʱ�����˾����ʵ�֣����ʱ������˵�ַ��
template __declspec(dllexport) float cuInterpolate(HBXDef::cuTable<2>* _table,
	HBXDef::cuInterval<2>* _posIn,
	HBXDef::UserCalPrec* _DataOut);

template __declspec(dllexport) float cuInterpolate(HBXDef::cuTable<3>* _table,
	HBXDef::cuInterval<3>* _posIn,
  	HBXDef::UserCalPrec* _DataOut);
