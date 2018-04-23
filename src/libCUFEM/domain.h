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

	//����Ϣ��������Ԫ���ڵ㣬���ϣ���������ԣ������Ϊһ��part�ġ���������Ϣ����������ԡ�
	//���ṩ����������������������
	class CUFEM_API Domain
	{
	public:
		std::string _myName;
		std::string _myType;

		//std::vector<FemComponent<_T>>* p_vFemCmpnn;
		std::vector<Node>		_vNode;
		//��Ԫ�ڽڵ���������
		std::vector<NSortMat<HBXDef::UserCalPrec>>	_vNSortArray;
		//��Ԫ��������
		std::vector<BaseElem > _vElmt;
		//������������
		std::vector<BaseMaterial> _vMat;
		//������������
		std::vector<BaseSection> _vSection;
		//�غ���������
		std::vector<BaseBoundary> _vLoad;
		//��������
		std::vector<Set>	_vSet;

		//ÿ���ڵ�Ĭ�ϵ����ɶ�
		int numberOfDefaultDofsPerNode;

		// Domain number
		int iMyNum;
		/// Domain serial (version) number����������Ӧ�ļ��㣿
		int iMySerialNumber;


		//��ǰ�����档
		Engng* _myEngng;

	protected:
	public:
		Domain() {};
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
		~Domain() {};

		UserStatusError_t InstanceSelf(InputRecord* _ir);

		//���õ�ǰ���������
// 		void ResetType(std::string _type)
// 		{
// 			_myType = _type;
// 		}

		//�������õ�ǰdomian�ı��
		void ResetNum(int _n) { this->iMyNum = _n; };
		//���ص�ǰdomaian�ı��
		int GetNum(int _sn) { return this->iMySerialNumber = _sn; };
		//�������õ�ǰdomian������Ӧ�������к�
		void  ResetSerialNum(int _n) { this->iMyNum; };

		BaseBoundary* GetBoundry(int _n);
		std::vector<BaseBoundary>& GetBoundry();

	};


	typedef std::map< std::string, Domain* >	_DomainInMap;
	typedef std::pair< std::string, Domain* >	_DomainInpair;
	typedef std::map< std::string, Domain* >::iterator	_DomainIter;


}