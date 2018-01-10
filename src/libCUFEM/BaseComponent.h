#pragma once

//�����״̬�����ṩ�������״̬��ʽ��ÿ�����״̬��
//1.Ψһ��ʶid
//2.����״̬�ص�
//3.�˳�״̬�ص�
//4.ÿ֡���ô����߼��Ļص�
//5.����������Ϣ�Ļص�
namespace HBXFEMDef
{
	class Package;


	class BaseComponent
	{
	private:
		void* _data;

		//ӳ����Ӧ��ID
		unsigned int m_MyID;
		unsigned int m_iNextID;

	public:
		//�ú������������Ա�֤ID���õ���ȷ�ԡ��䱣֤�����㷨��ֵ���ڻ��ߵ���ID���Ա�֤map��״ͼ�ڵ�ֵ����ͻ
		void ResetID(unsigned int _id);

		unsigned int GetID() const { return m_MyID; };

		virtual const char* GetClassName() const = 0;

		BaseComponent(unsigned int _id)
		{
			ResetID(_id);
		}
		virtual ~BaseComponent() {};

		//���е������ͨ���ú���ˢ������
		virtual void  Update() = 0;

		//���ݽ����Ľӿڡ�ʹ��messageDispatcher�෢��
		virtual bool HandleMessage(const Package& msg) = 0;

		//
//		void(*OnEnter)(BaseComponent* _cpn);

		//
//		void(*OnExit)(BaseComponent* _cpn);

//		void(*Update)(BaseComponent* _cpn);

		//�����յ����ú󷵻ز���ֵ
//		bool(*OnMessage)(BaseComponent* _cpt, void* sender, int subject, void* data);


	};

}

