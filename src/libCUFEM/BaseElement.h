#pragma once

#include <HbxDefMacro.h>
#include <HBXDefStruct.h>
#include <ExportDef.h>
#include <libCUFEM\InputRecord.h>
#include <libCUFEM\BaseComponent.h>


namespace HBXFEMDef
{

#define _EX_Element_nodes "nodes"
#define _EX_Element_crosssect "crosssect"


	template <class Entity_type> class State;
	template <class Entity_type> class StateMachine;

	using namespace HBXDef;
	template<typename _T> struct _Nodes;

	class Domain;
	class Engng;
	class BaseComponent;
// 	template<typename _T>
// 	struct _BaseElem
// 	{
// 		size_t* _iNode;	//��Ԫ�ڽڵ��ţ��ڼ̳к�����������ڴ����
// 		size_t _iMat;	//��Ԫ��������
// 
// 	};

	//��Ԫ����ʵ�ֽӿڣ���������ɸ��ֵ�Ԫ����������㷨��䡣
	class CUFEM_API BaseElem:public BaseComponent
	{
	private:
	protected:
		StateMachine<BaseElem>* m_pStateMachine;

		bool m_bFresh;//ˢ�±�־λ��������Ҫ����������󡢸նȾ����ˢ��

		Domain*	MyDomain;
		Engng*	MyEngng;

	public:

		size_t* _iNode;	//��Ԫ�ڽڵ��ţ��ڼ̳к�����������ڴ����
		size_t _iMat;	//��Ԫ��������

		//����ǰ��id���ں�������µĵ�Ԫ���ͣ�ֻ��Ҫ��classtype.hͷ�ļ�����Ӻ�������ģ�鼴��
		//��Ԫ�����ڵ�������У����ݿ���û�У���Ԫ������Ĭ��ֵ
		BaseElem(Engng* _engng, Domain* _dm, int _id = classType::ELEMT);

//		BaseElem(HBXDef::UserCalPrec* _begin, size_t _lgth, int _id);


		virtual ~BaseElem();

		//��ʼ���������ݣ���ȡ���ϵ��������
		virtual InputFileResult_t InitFrom(InputRecord * _dr);

		//�˺�������ˢ�µ�ǰ��Ԫ��״̬
		virtual void Update();

		virtual bool HandleMessage(const Package& msg);

		StateMachine<BaseElem>* GetFSM() const { return m_pStateMachine; };

		//������
		void ResetDomain(Domain* _dm) { this->MyDomain = _dm; };
		Domain* GetDomain()const { return this->MyDomain; };
		
		void ResetEngng(Engng* _engng) { this->MyEngng = _engng; };
		Engng* GetEngng()const { return this->MyEngng; };

		const char* GetClassName() const { return "BaseElement"; };

		void SetFreshFlag() { m_bFresh = true; };//������Ҫ���±�־λΪtrue

		void ResetFreshFlag() { m_bFresh = false; };//������Ҫ���±�־λΪfalse

		bool isNeedFresh() const { return m_bFresh; };
	};

	//������classfactory��ע��
	template<typename T> BaseElem* ElemtCreator(Engng * _master, Domain* _dm, int _id) { return (new T(_master, _dm, _id)); }
}
