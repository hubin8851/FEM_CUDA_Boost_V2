#pragma once
#include <ExportDef.h>
#include <map>
#include <string>



namespace HBXFEMDef
{
	class Domain;
	class BaseMaterial;
	class BaseElem;
	class BaseSection;
	class BaseBoundary;
	class Engng;
	class StructEngng;
	class BaseNumMethod;
	class BaseComponent;


	typedef void* (*AnyCreator)(int, Domain);

	//����������������ʵ����Ϣ���ƣ�Դ��WestWorldWithMessage��
	//http://blog.csdn.net/shanyongxu/article/details/48024271
	typedef std::map<unsigned int, BaseComponent*> EntityMap;


#define REGIST_ENGNG(class) static bool __dummy_ ## class = GetClassFactory().RegistEngng( _EX_ ## class ## _Name, EngngCreator<class> );
//#define REGIST_ENGNG(class) static bool = GetClassFactory.RegistEngng( _EX_ ## class ## _Name, EngngCreator<class> )
	//����ģʽ�еĵ����֣��μ�
	//http://blog.csdn.net/wuzhekai1985/article/details/6660462
	//�๤�� class �洢���еĿ��ܳ��ֵĲ������ͨ����������
	class BOOST_SYMBOL_EXPORT ClassFactory
	{
	private:
		//��Ԫ�����б���xml���ȡ
		std::map<std::string, BaseElem*(*)(void)>	ElemList;
		//���������б���xml���ȡ
		std::map<std::string, BaseMaterial*(*)(int, Domain*)>	MaterialList;
		//���������б���xml���ȡ
		std::map<std::string, BaseSection*(*)(int, Domain*)>	SecList;
		//�߽������б�
		std::map<std::string, BaseBoundary*(*)(int, Domain*)>	LoadList;
		//���������б�
		std::map<std::string, Engng*(*)(int, Engng*)>	EngngList;
		//�㷨�����б�
		std::map<std::string, BaseNumMethod*(*)(Domain*, Engng*)>	NumericalMethodList;

		EntityMap	m_EntityMap;
	protected:
	public:
		ClassFactory();
		~ClassFactory();

#pragma region ��������
		//����һ���µ�Ԫ
		BaseElem* CreateElem(const char* _name, int _n, Domain* _dm);
		//ע��һ���µĵ�Ԫ���������ƺ͹��캯������
		bool RegistElem(const char* _name, BaseElem*(*creator)(void));

		//�Ƴ�һ�����е�Ԫ
		bool RemoveElem(const char* _name, BaseElem*(*creator)(void));

		//����һ���²���
		BaseMaterial* CreateMaterial(const char* _name, int _n, Domain* _dm);
		//ע��һ���µĲ��ϣ��������ƺ͹��캯������
		bool RegistMaterial(const char* _name, BaseMaterial*(*creator)(int, Domain*));

		//����һ���½���
		BaseSection* CreateSection(const char* _name, int _n, Domain* _dm);
		//ע��һ���µĲ��ϣ��������ƺ͹��캯������
		bool RegistSection(const char* _name, BaseSection*(*creator)(int, Domain*));

		//����һ���±߽�
		BaseBoundary* CreateLoad(const char* _name, int _n, Domain* _dm);
		//ע��һ���µı߽�����
		bool RegistLoad(const char* _name, BaseBoundary*(*creator)(int, Domain*));

		//����һ��������
		Engng* CreateEngng(const char* _name, int _n, Engng* _master);
		//ע��һ�������棬�������ƺ͹��캯������
		bool RegistEngng(const char* _name, Engng*(*creator)(int, Engng*));

		//����һ�����㷨
		BaseNumMethod* CreateNumMethod(const char* _name, Domain* _dm, Engng* _master);
		//ע��һ�������棬�������ƺ͹��캯������
		bool RegistNumMethod(const char* _name, BaseNumMethod*(*creator)(Domain*, Engng*));

#pragma endregion ��������

		void RegistEntity(BaseComponent* pEntity);

		BaseComponent* GetEntityFromID(unsigned int _id) const;

		void RemoveEntity(BaseComponent* pEntity);
	};

	extern ClassFactory& classFactory;

	//û��Ū��Ϊʲô��������������Ϊһ����̬��Ա...
	BOOST_SYMBOL_EXPORT ClassFactory& GetClassFactory();

}


