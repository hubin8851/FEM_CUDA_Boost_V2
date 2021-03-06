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

#define _IF_Domain_Type "domain"
#define _IF_Domain_nnode "nnode"
#define _IF_Domain_nelt "nelt"
#define _IF_Domain_nsect "nsect"
#define _IF_Domain_nmat "nmat"
#define _IF_Domain_nset "nset"

	//域信息，包括单元，节点，材料，截面等属性，简单理解为一个part的、除解算信息外的所有属性。
	//其提供给解算器待计算的相关数据
	class CUFEM_API Domain
	{
	public:
		std::string _myName;
		AnylsType _myType;

		//空间维度
		int _nsd;
		//每个节点默认的自由度
		int numberOfDefaultDofsPerNode;
		//对称标志位
		bool _Axisymm;

		//std::vector<FemComponent<_T>>* p_vFemCmpnn;
		//每个域内节点只有一组
		std::shared_ptr< std::vector<Node> >	_pNode;
		//单元内节点数组容器
//		std::map< BaseElem, HBXFEMDef::NSortMat<HBXDef::UserReadPrec>* >*	_pNSortArray;
		//单元向量容器
		std::vector< BaseElem > _vElmt;
		//材料向量容器
		std::vector<BaseMaterial > _vMat;
		//截面向量容器
		std::vector<BaseSection > _vSection;
		//载荷向量容器
		std::vector<BaseBoundary> _vLoad;
		//集合容器
		std::vector<Set>	_vSet;

		// Domain number
		int iMyNum;
		/// Domain serial (version) number。用以自适应的计算？
		int iMySerialNumber;


		//当前的引擎。
		Engng* _myEngng;

	protected:

	private:
		//依据单元名称解析默认的自由度
		void GetDomainDofsDefaults( const char* );
	public:
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

		Domain(const Domain& src) ;
		Domain &operator = (const Domain &src) ;

		~Domain() {};

		InputFileResult_t InstanceSelf(InputRecord* _dr);

		//设置当前域计算类型
// 		void ResetType(std::string _type)
// 		{
// 			_myType = _type;
// 		}

		//重新设置当前domian的编号
		void ResetNum(int _n) { this->iMyNum = _n; };
		//返回当前domaian的编号
		int GetNum(int _sn) { return this->iMySerialNumber = _sn; };
		//获取单元向量指针
		std::vector< BaseElem >& GetElemList()
		{
			return this->_vElmt;
		}

		BaseElem* GetElem(int _n)
		{
			return &this->_vElmt[_n];
		}

		//返回域中的单元数量
		int GetNumOfElem() const 
		{
			return this->_vElmt.size();
		};
		//获取当前域的方程维度，维度公式为 dofpernode * nodenum
		int GetNumOfEquations() const {
			return this->_pNode->size()*_nsd;
		}

		//重新设置当前domian的自适应计算序列号
		void  ResetSerialNum(int _n) { this->iMyNum; };

		BaseBoundary* GetBoundry(int _n);
		std::vector<BaseBoundary>& GetBoundry();

	};


	typedef std::map< std::string, Domain* >	_DomainInMap;
	typedef std::pair< std::string, Domain* >	_DomainInpair;
	typedef std::map< std::string, Domain* >::iterator	_DomainIter;


}