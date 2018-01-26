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
	//工厂模式中的第三种，参见
	//http://blog.csdn.net/wuzhekai1985/article/details/6660462
	//类工厂 class 存储所有的可能出现的产物，并可通过名字索引
	class BOOST_SYMBOL_EXPORT ClassFactory
	{
	private:
		//单元种类列表，从xml里读取
		std::map<std::string, BaseElem*(*)(Engng* , Domain*, int _id)>	ElemList;
		//材料种类列表，从xml里读取
		std::map<std::string, BaseMaterial*(*)(Domain*, int)>	MaterialList;
		//截面种类列表，从xml里读取
		std::map<std::string, BaseSection*(*)(Domain*, int)>	SecList;
		//边界种类列表
		std::map<std::string, BaseBoundary*(*)(Domain*, int)>	LoadList;
		//引擎种类列表
		std::map<std::string, Engng*(*)(Engng*, int)>	EngngList;
		//算法种类列表
		std::map<std::string, BaseNumMethod*(*)(Domain*, Engng*, int)>	NumericalMethodList;

	protected:
	public:
		ClassFactory();
		~ClassFactory();

#pragma region 工厂函数
		//创建一个新单元
		BaseElem* CreateElem(const char* _name, Engng* _eg, Domain* _dm, int _id);
		//注册一个新的单元，传入名称和构造函数对象
		bool RegistElem(const char* _name, BaseElem*(*creator)(Engng*, Domain*, int));

		//移除一个已有单元
		bool RemoveElem(const char* _name);

		//创建一个新材料
		BaseMaterial* CreateMaterial(const char* _name, Domain* _dm, int _id);
		//注册一个新的材料，传入名称和构造函数对象
		bool RegistMaterial(const char* _name, BaseMaterial *(*creator)(Domain*, int));

		//创建一个新截面
		BaseSection* CreateSection(const char* _name, Domain* _dm, int _id);
		//注册一个新的材料，传入名称和构造函数对象
		bool RegistSection(const char* _name, BaseSection *(*creator)(Domain*, int));

		//创建一个新边界
		BaseBoundary* CreateLoad(const char* _name, Domain* _dm, int _id);
		//注册一个新的边界条件
		bool RegistLoad(const char* _name, BaseBoundary *(*creator)(Domain*, int));

		//创建一个新引擎
		Engng* CreateEngng(const char* _name, Engng * _master, int _id);
		//注册一个新引擎，传入名称和构造函数对象
		bool RegistEngng(const char* _name, Engng*(*creator)(Engng*, int));

		//创建一个新算法
		BaseNumMethod* CreateNumMethod(const char * _name, Domain * _dm, Engng * _master, int _id);
		//注册一个新引擎，传入名称和构造函数对象
		bool RegistNumMethod(const char * _name, BaseNumMethod *(*creator)(Domain *, Engng *, int));

#pragma endregion 工厂函数

	};

	extern ClassFactory& classFactory;

	//没有弄懂为什么不放在类里面作为一个静态成员...
	BOOST_SYMBOL_EXPORT ClassFactory& GetClassFactory();

}


