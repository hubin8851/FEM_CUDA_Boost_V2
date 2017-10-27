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
		//单元向量容器
		std::vector<BaseElem<_T>>* p_vElmt;
		//材料向量容器
		std::vector<BaseMaterial<_T>>* p_vMat;
		//界面向量容器
		std::vector<BaseSection<_T>>* p_vSection;
		//载荷向量容器
		std::vector<BaseLoad<_T>>* p_vLoad;

		//每个节点默认的自由度
		int numberOfDefaultDofsPerNode;

		//
		/// Domain number
		int number;
		/// Domain serial (version) number。用以自适应的计算？
		int serialNumber;

		//分析类型
		AnylsType_t _AnylsType;

		//当前的引擎。
		std::shared_ptr<Engng<_T>> _myEngng;

	protected:
	public:
		_Domain( int _n,  );
		~_Domain();
	
	};


}