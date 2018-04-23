#pragma once
#include <ExportDef.h>
#include <string>




namespace HBXFEMDef
{
	class Domain;



	//��������Ļ��࣬�ṩ������Ľӿڣ��������Ժͷ�����ͨ�ýӿڡ�
	//��һnumber���Դ������ļ��ж�ȡ����
	//���domain���Ա�����ǰ�������Ա�ͬ����ģ��������ݽ���
	class CUFEM_API CuFemComponent
	{
	private:
	protected:
		int number;//������
		Domain *MyDomain;	//���Ա�����ǰ�������Ա�ͬ����ģ��������ݽ���
	public:
		//���캯��
		CuFemComponent(int n, Domain* _d) :number(n), MyDomain(_d) {};
		//��������
		~CuFemComponent() {};

		void ResetNumber(int _n) { this->number = _n; };
		void ResetDomain(Domain* _dm) { this->MyDomain = _dm; };

		virtual const char* GetClassName() const = 0;
		
		virtual Domain* GetDomain() const { return this->MyDomain; };

		std::string errorInfo(const char* func) const;
	};

}
