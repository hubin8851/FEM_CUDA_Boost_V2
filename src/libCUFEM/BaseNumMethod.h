#pragma once

#include <HBXPreDef.h>
// #include <BoostPreDef.h>
// #include <ExportDef.h>
// #include <HbxDefMacro.h>

namespace HBXFEMDef
{
	class Engng;
	class Domain;

	//��ֵ���㷽���Ļ��ࡣ
	//����ض���ʵ����ѡ��һϵ�в�ͬ����ֵ���������������Բ�ͬ������������ͬ�ĺ����塣
	//�����麯�����������Ľӿ�ͨ����ز����õ����㷨�����㷨״̬ö��ֵ��
	//����ͨ�����εķ�ʽ�������ࡣÿһ���ض�������ʵ��������Ӧ�����Է��̲��ִ�����Ӧ����ֵ���������
	//�������Խ��㷨ʵ�ֺ����ݰ���
	class BOOST_SYMBOL_EXPORT BaseNumMethod
	{
	private:
	protected:
		Domain*	MyDomain;
		Engng*	MyEngng;


	public:
		BaseNumMethod(Domain* _dm, Engng* _eng) :MyDomain(_dm), MyEngng(_eng) {};

		virtual ~BaseNumMethod();

		//���ص�ǰ�㷨��ӵ����
		Engng*	GetEngng() { return MyEngng; };

		//���ļ��ж�ȡ����
		virtual InputFileResult InitFrom() { return InputFileResult::IRRT_OK; };

		virtual void Reset();

		virtual void ResetDomain(Domain* _dm) { MyDomain = _dm; };
	};

}
