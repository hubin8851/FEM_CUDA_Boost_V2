#pragma once

#include <map>
#include <vector>
#include <ExportDef.h>
#include <HbxDefMacro.h>
#include <HBXDefStruct.h>
#include <libCUFEM\BaseNode.h>
#include <libCUFEM\BaseElement.h>
#include <libCUFEM\BaseSection.h>
#include <libCUFEM\BaseMaterial.h>
#include <libCUFEM\BaseBoundary.h>
#include <libCUFEM\Set.h>
#include <libCUFEM\InputRecord.h>

namespace HBXFEMDef 
{
	class FemComponent;
	struct Node;
	class BaseElem;
	class BaseMaterial;
	class BaseSection;
	class BaseBoundary;
	class MetaStep;
	class Engng;
	class Set;
	template<typename _T> class NSortMat;

	//域信息，包括单元，节点，材料，截面等属性，简单理解为一个part的、除解算信息外的所有属性。
	//其提供给解算器待计算的相关数据
	class CUFEM_API Domain
	{
	public:
		std::string _myName;
		std::string _myType;

		//std::vector<FemComponent<_T>>* p_vFemCmpnn;
		std::vector<Node>		_vNode;
		//单元内节点数组容器
		std::vector<NSortMat<HBXDef::UserCalPrec>>	_vNSortArray;
		//单元向量容器
		std::vector<BaseElem > _vElmt;
		//材料向量容器
		std::vector<BaseMaterial> _vMat;
		//截面向量容器
		std::vector<BaseSection> _vSection;
		//载荷向量容器
		std::vector<BaseBoundary> _vLoad;
		//集合容器
		std::vector<Set>	_vSet;

		//每个节点默认的自由度
		int numberOfDefaultDofsPerNode;

		// Domain number
		int iMyNum;
		/// Domain serial (version) number。用以自适应的计算？
		int iMySerialNumber;


		//当前的引擎。
		Engng* _myEngng;

	protected:
	public:
		Domain() {};
		//@：_name:当前域名称
		Domain(std::string _name) :_myName(_name)
		{
			_vElmt.clear();
		};
		//@_n:当前domain的序号
		//@_serNum:用以自适应计算的序列号
		//@_pEngng:从属的驱动器
		Domain(int _n, int serNum, Engng* _pEngng)
		{
			_vSet.clear();
		};
		~Domain() {};

		UserStatusError_t InstanceSelf(InputRecord* _ir);

		//设置当前域计算类型
// 		void ResetType(std::string _type)
// 		{
// 			_myType = _type;
// 		}

		//重新设置当前domian的编号
		void ResetNum(int _n) { this->iMyNum = _n; };
		//返回当前domaian的编号
		int GetNum(int _sn) { return this->iMySerialNumber = _sn; };
		//重新设置当前domian的自适应计算序列号
		void  ResetSerialNum(int _n) { this->iMyNum; };

		BaseBoundary* GetBoundry(int _n);
		std::vector<BaseBoundary>& GetBoundry();

	};


	typedef std::map< std::string, Domain* >	_DomainInMap;
	typedef std::pair< std::string, Domain* >	_DomainInpair;
	typedef std::map< std::string, Domain* >::iterator	_DomainIter;


}