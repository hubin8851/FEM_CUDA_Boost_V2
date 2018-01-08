#include "ClassFactory.h"
#include <boost\algorithm\string.hpp>
#include <boost\bind.hpp>
#include <libCUFEM\CuFemCmpnt.h>
#include <list>
#include <cctype>

namespace HBXFEMDef
{
	using namespace std;

	BOOST_SYMBOL_EXPORT ClassFactory& GetClassFactory()
	{
		// TODO: 在此处插入 return 语句
		static ClassFactory res;
		return res;
	}

	ClassFactory& classFactory = GetClassFactory();

	std::string conv2lower(std::string input)
	{
		for (std::size_t i = 0; i < input.size(); i++) {
			input[i] = (char)std::tolower(input[i]);
		}
		return input;
	}

	//创建某物的宏，在列表中搜索并通过三元操作返回值或空指针,针对默认构造函数
#define CF_CREATE(MyMap, ...) \
		auto creator = MyMap.find(conv2lower(_name));\
		return creator != MyMap.end() ? creator->second(__VA_ARGS__) : nullptr;
		
#define CF_STORE(MyMap) \
		MyMap[ conv2lower(_name) ] = creator;\
		return true;

	//创建某物的宏，在列表中搜索并通过三元操作返回值或空指针,针对oofem中各类的构造函数
#ifndef CF_CREATE(MyMap, ...)
#define CF_CREATE(MyMap, ...) \
		auto creator = MyMap.find(boost::to_lower_copy(_name));\
		return creator != MyMap.end() ? creator->second(__VA_ARGS__) : nullptr;
#endif

#ifndef CF_STORE(MyMap)
#define CF_STORE(MyMap) \
		MyMap[ boost::to_lower_copy(_name) ] = creator;\
		return true;
#endif



	ClassFactory::ClassFactory()
	{
	}


	ClassFactory::~ClassFactory()
	{
	}

	BaseElem * ClassFactory::CreateElem(const char * _name, int _n, Domain * _dm)
	{
		//截面、材料、单元基类的构造函数未定
		CF_CREATE(ElemList)
		return nullptr;
	}

	bool ClassFactory::RegistElem(const char* _name, BaseElem *(*creator)(void))
	{
		CF_STORE(ElemList);
		return false;
	}

	BaseMaterial * ClassFactory::CreateMaterial(const char* _name, int _n, Domain * _dm)
	{
		CF_CREATE(MaterialList, _n, _dm);
	}

	bool ClassFactory::RegistMaterial(const char* _name, BaseMaterial *(*creator)(int, Domain*))
	{
		CF_STORE(MaterialList)
	}

	BaseSection * ClassFactory::CreateSection(const char* _name, int _n, Domain * _dm)
	{
		CF_CREATE(SecList, _n, _dm);
	}

	bool ClassFactory::RegistSection(const char* _name, BaseSection *(*creator)(int, Domain*))
	{
		CF_STORE(SecList)
	}

	BaseBoundary * ClassFactory::CreateLoad(const char* _name, int _n, Domain * _dm)
	{
		CF_CREATE(LoadList, _n, _dm)
	}

	bool ClassFactory::RegistLoad(const char* _name, BaseBoundary *(*creator)(int, Domain*))
	{
		CF_STORE(LoadList)
	}

	Engng * ClassFactory::CreateEngng(const char* _name, int _n, Engng * _master)
	{
		CF_CREATE(EngngList, _n, _master);
	}

	bool ClassFactory::RegistEngng(const char* _name, Engng*(*creator)(int, Engng*))
	{
		CF_STORE(EngngList);
	}

}


