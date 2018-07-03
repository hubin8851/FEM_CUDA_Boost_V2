#pragma once

#include <ExportDef.h>
#include <HbxDefMacro.h>
#include <libCUFEM\BaseComponent.h>

namespace HBXFEMDef
{
	class Engng;
	class Domain;
	class BaseComponent;

	//��ֵ���㷽���Ļ��ࡣ
	//����ض���ʵ����ѡ��һϵ�в�ͬ����ֵ���������������Բ�ͬ������������ͬ�ĺ����塣
	//�����麯�����������Ľӿ�ͨ����ز����õ����㷨�����㷨״̬ö��ֵ��
	//����ͨ�����εķ�ʽ�������ࡣÿһ���ض�������ʵ��������Ӧ�����Է��̲��ִ�����Ӧ����ֵ���������
	//�������Խ��㷨ʵ�ֺ����ݰ���
	class CUFEM_API BaseNumMethod:BaseComponent
	{
	private:
	protected:
		Domain*	MyDomain;
		Engng*	MyEngng;


	public:
		BaseNumMethod(Domain* _dm, Engng* _eng, int _id = classType::NUMERICALMETHOD);

		virtual ~BaseNumMethod();

		//���ص�ǰ�㷨��ӵ����
		Engng*	GetEngng() { return MyEngng; };

		//���ļ��ж�ȡ����
		virtual InputFileResult InitFrom() { return InputFileResult::IRRT_OK; };

		virtual void Reset();

		virtual void ResetDomain(Domain* _dm) { MyDomain = _dm; };

		virtual const char* GetClassName()const { return typeid(BaseNumMethod).name(); };

		virtual UserStatusError_t Solve( SparseMat& _A, HBXDef::UserCalPrec& b, HBXDef::UserCalPrec& _x ) = 0;
	};

	template<typename T> BaseNumMethod* NumerialSolverCreator() { return new T(); };
	template<typename T> BaseNumMethod* NumerialSolverCreator(Domain *_dm, Engng* _eg) { return new T(_dm, _eg); };

}
