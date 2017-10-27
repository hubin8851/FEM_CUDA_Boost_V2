#pragma once

#include <HBXPreDef.h>

extern HBXDef::PrecsType_t	g_PrecsType;	//ȫ�ֱ������������ݶ�ȡ�ͼ���ľ���(float or double)

namespace HBXFEMDef
{
	using namespace boost;
	using namespace HBXDef;
	//���϶���ṹ��
	template<typename _T>
	class _BaseMaterial
	{
	public:
		_BaseMaterial(_T _E = 0, _T _mu = 0, _T _Gama = 0, const char *ch = nullptr) :dE(_E), dMu(_mu), dMaGama(_Gama)
		{
			strcpy(this->chname, ch);
			dG = dE / (2 * (1 + dMu));
		}

		_T dE;	//����ģ��
		_T dMu;	//���ɱ�
		_T dG;	//����ģ��
		_T dAlpha;	//������ϵ��
		_T dMaGama; //�ض�
		char   chname[40];//���������ַ�����
	};



}
