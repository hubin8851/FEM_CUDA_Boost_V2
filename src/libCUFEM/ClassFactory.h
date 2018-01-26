#pragma once
#include <HBXPreDef.h>



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

//	typedef bimap< tagged<int, struct id >, tagged< std::string, struct name >> EntityMap;
//	typedef std::map< bimap<tagged<int, struct id >, tagged< std::string, struct name >>, BaseComponent*> EntityMap;

#define REGIST_ENGNG(class) static bool __dummy_ ## class = GetClassFactory().RegistEngng( _EX_ ## class ## _Name, EngngCreator<class> );
#define REGIST_ELEMT(class) static bool __dummy_ ## class = GetClassFactory().RegistElem( _EX_ ## class ## _Name, ElemtCreator<class> );
	//����ģʽ�еĵ����֣��μ�
	//http://blog.csdn.net/wuzhekai1985/article/details/6660462
	//�๤�� class �洢���еĿ��ܳ��ֵĲ������ͨ����������
	class BOOST_SYMBOL_EXPORT ClassFactory
	{
	private:
		//��Ԫ�����б���xml���ȡ
		std::map<std::string, BaseElem*(*)(Engng* , Domain*, int _id)>	ElemList;
		//���������б���xml���ȡ
		std::map<std::string, BaseMaterial*(*)(Domain*, int)>	MaterialList;
		//���������б���xml���ȡ
		std::map<std::string, BaseSection*(*)(Domain*, int)>	SecList;
		//�߽������б�
		std::map<std::string, BaseBoundary*(*)(Domain*, int)>	LoadList;
		//���������б�
		std::map<std::string, Engng*(*)(Engng*, int)>	EngngList;
		//�㷨�����б�
		std::map<std::string, BaseNumMethod*(*)(Domain*, Engng*, int)>	NumericalMethodList;

	protected:
	public:
		ClassFactory();
		~ClassFactory();

#pragma region ��������
		//����һ���µ�Ԫ
		BaseElem* CreateElem(const char* _name, Engng* _eg, Domain* _dm, int _id);
		//ע��һ���µĵ�Ԫ���������ƺ͹��캯������
		bool RegistElem(const char* _name, BaseElem*(*creator)(Engng*, Domain*, int));

		//�Ƴ�һ�����е�Ԫ
		bool RemoveElem(const char* _name);

		//����һ���²���
		BaseMaterial* CreateMaterial(const char* _name, Domain* _dm, int _id);
		//ע��һ���µĲ��ϣ��������ƺ͹��캯������
		bool RegistMaterial(const char* _name, BaseMaterial *(*creator)(Domain*, int));

		//����һ���½���
		BaseSection* CreateSection(const char* _name, Domain* _dm, int _id);
		//ע��һ���µĲ��ϣ��������ƺ͹��캯������
		bool RegistSection(const char* _name, BaseSection *(*creator)(Domain*, int));

		//����һ���±߽�
		BaseBoundary* CreateLoad(const char* _name, Domain* _dm, int _id);
		//ע��һ���µı߽�����
		bool RegistLoad(const char* _name, BaseBoundary *(*creator)(Domain*, int));

		//����һ��������
		Engng* CreateEngng(const char* _name, Engng * _master, int _id);
		//ע��һ�������棬�������ƺ͹��캯������
		bool RegistEngng(const char* _name, Engng*(*creator)(Engng*, int));

		//����һ�����㷨
		BaseNumMethod* CreateNumMethod(const char * _name, Domain * _dm, Engng * _master, int _id);
		//ע��һ�������棬�������ƺ͹��캯������
		bool RegistNumMethod(const char * _name, BaseNumMethod *(*creator)(Domain *, Engng *, int));

#pragma endregion ��������

	};

	extern ClassFactory& classFactory;

	//û��Ū��Ϊʲô��������������Ϊһ����̬��Ա...
	BOOST_SYMBOL_EXPORT ClassFactory& GetClassFactory();

}


