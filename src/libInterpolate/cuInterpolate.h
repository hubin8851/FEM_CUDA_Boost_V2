#pragma once
#include <HbxDefMacro.h>
#include <CuDefMacro.h>


namespace HBXDef
{
	template<unsigned int T> class cuTable;
	template<unsigned int T> class cuLocation;
}




//��ֵ����CUDA�汾�����ػ��ѵ�ʱ��
template< unsigned int T>
float cuInterpolate(HBXDef::cuTable<T>* _table,
	HBXDef::cuLocation<T>* _posIn,
	HBXDef::UserCalPrec* _DataOut,
	unsigned int N = MAX_GPUITER);

//CUDA��ֵ����У�����
template< unsigned int T>
float Interpolate_cpu(HBXDef::cuTable<T>* _table,
	HBXDef::cuLocation<T>* h_loc,
	HBXDef::UserCalPrec* h_result,
	unsigned int _num);

#ifdef __cplusplus

#endif
#ifdef __cplusplus

#endif


//ģ�庯��/�಻�ܵ�������̬���ӿ⣨DLL�� 
//ģ�庯����������ʱ�� ��ʵ�������ڣ�������ַҲ���޴�̸���ˣ�����������̬���ӿ�ĺ�������Ҫ�е�ַ��Ҳ����˵��������ģ�岻�߱������Ļ���������
//����ģ���ڵ���ʱ�����˾����ʵ�֣����ʱ������˵�ַ��
template __declspec(dllexport) float cuInterpolate(HBXDef::cuTable<2>* _table,
	HBXDef::cuLocation<2>* _posIn,
	HBXDef::UserCalPrec* _DataOut,
	unsigned int N);

template __declspec(dllexport) float cuInterpolate(HBXDef::cuTable<3>* _table,
	HBXDef::cuLocation<3>* _posIn,
	HBXDef::UserCalPrec* _DataOut,
	unsigned int N);


template __declspec(dllexport) float Interpolate_cpu(HBXDef::cuTable<2>* _table,
	HBXDef::cuLocation<2>* h_loc,
	HBXDef::UserCalPrec* h_result,
	unsigned int _num);


template __declspec(dllexport) float Interpolate_cpu(HBXDef::cuTable<3>* _table,
	HBXDef::cuLocation<3>* h_loc,
	HBXDef::UserCalPrec* h_result,
	unsigned int _num);