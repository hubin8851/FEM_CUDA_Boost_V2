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

	//����Ϣ��������Ԫ���ڵ㣬���ϣ���������ԣ������Ϊһ��part�ġ���������Ϣ����������ԡ�
	//���ṩ����������������������
	class CUFEM_API Domain
	{
	public:
		std::string _myName;
		AnylsType _myType;

		//�ռ�ά��
		int _nsd;
		//ÿ���ڵ�Ĭ�ϵ����ɶ�
		int numberOfDefaultDofsPerNode;
		//�ԳƱ�־λ
		bool _Axisymm;

		//std::vector<FemComponent<_T>>* p_vFemCmpnn;
		//ÿ�����ڽڵ�ֻ��һ��
		std::shared_ptr< std::vector<Node> >	_pNode;
		//��Ԫ�ڽڵ���������
//		std::map< BaseElem, HBXFEMDef::NSortMat<HBXDef::UserReadPrec>* >*	_pNSortArray;
		//��Ԫ��������
		std::vector< BaseElem > _vElmt;
		//������������
		std::vector<BaseMaterial > _vMat;
		//������������
		std::vector<BaseSection > _vSection;
		//�غ���������
		std::vector<BaseBoundary> _vLoad;
		//��������
		std::vector<Set>	_vSet;

		// Domain number
		int iMyNum;
		/// Domain serial (version) number����������Ӧ�ļ��㣿
		int iMySerialNumber;


		//��ǰ�����档
		Engng* _myEngng;

	protected:

	private:
		//���ݵ�Ԫ���ƽ���Ĭ�ϵ����ɶ�
		void GetDomainDofsDefaults( const char* );
	public:
		//@��_name:��ǰ������
		Domain(std::string _name) :_myName(_name)
		{
			_vElmt.clear();
		};
		//@_n:��ǰdomain�����
		//@_serNum:��������Ӧ��������к�
		//@_pEngng:������������
		Domain(int _n, int serNum, Engng* _pEngng)
		{
			_vSet.clear();
		};

		Domain(const Domain& src) ;
		Domain &operator = (const Domain &src) ;

		~Domain() {};

		InputFileResult_t InstanceSelf(InputRecord* _dr);

		//���õ�ǰ���������
// 		void ResetType(std::string _type)
// 		{
// 			_myType = _type;
// 		}

		//�������õ�ǰdomian�ı��
		void ResetNum(int _n) { this->iMyNum = _n; };
		//���ص�ǰdomaian�ı��
		int GetNum(int _sn) { return this->iMySerialNumber = _sn; };
		//��ȡ��Ԫ����ָ��
		std::vector< BaseElem >& GetElemList()
		{
			return this->_vElmt;
		}

		BaseElem* GetElem(int _n)
		{
			return &this->_vElmt[_n];
		}

		//�������еĵ�Ԫ����
		int GetNumOfElem() const 
		{
			return this->_vElmt.size();
		};
		//��ȡ��ǰ��ķ���ά�ȣ�ά�ȹ�ʽΪ dofpernode * nodenum
		int GetNumOfEquations() const {
			return this->_pNode->size()*_nsd;
		}

		//�������õ�ǰdomian������Ӧ�������к�
		void  ResetSerialNum(int _n) { this->iMyNum; };

		BaseBoundary* GetBoundry(int _n);
		std::vector<BaseBoundary>& GetBoundry();

	};


	typedef std::map< std::string, Domain* >	_DomainInMap;
	typedef std::pair< std::string, Domain* >	_DomainInpair;
	typedef std::map< std::string, Domain* >::iterator	_DomainIter;


}