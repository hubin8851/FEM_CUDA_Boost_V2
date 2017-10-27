#pragma once

#include <BoostPreDef.h>
#include <HBXPreDef.h>
#include <HbxDefMacro.h>
#include <HBXDefStruct.h>

namespace HBXFEMDef 
{
	template<typename _T> class FemComponent;
	template<typename _T> struct Node;
	template<typename _T> class BaseElem;
	template<typename _T> class BaseMaterial;
	template<typename _T> class BaseSection;
	template<typename _T> class BaseLoad;
	template<typename _T> class Engng;


	using namespace std;
	using namespace boost;

	template<typename _T>
	class _Domain
	{
	private:
		std::string _myName;
		std::string _myType;

		//std::vector<FemComponent<_T>>* p_vFemCmpnn;
		//std::vector<Node<_T>>*	p_vNode;
		//��Ԫ��������
		std::vector<BaseElem<_T>>* p_vElmt;
		//������������
		std::vector<BaseMaterial<_T>>* p_vMat;
		//������������
		std::vector<BaseSection<_T>>* p_vSection;
		//�غ���������
		std::vector<BaseLoad<_T>>* p_vLoad;

		//ÿ���ڵ�Ĭ�ϵ����ɶ�
		int numberOfDefaultDofsPerNode;

		//
		/// Domain number
		int number;
		/// Domain serial (version) number����������Ӧ�ļ��㣿
		int serialNumber;

		//��������
		AnylsType_t _AnylsType;

		//��ǰ�����档
		std::shared_ptr<Engng<_T>> _myEngng;

	protected:
	public:
		_Domain( int _n,  );
		~_Domain();
	
	};


}