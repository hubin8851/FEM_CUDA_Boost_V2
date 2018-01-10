#include "ClassFactory.h"
#include <HbxDefMacro.h>
#include <boost\algorithm\string.hpp>
#include <boost\bind.hpp>
#include <libCUFEM\CuFemCmpnt.h>
#include <libCUFEM\BaseComponent.h>
#include <list>
#include <cctype>

namespace HBXFEMDef
{
	using namespace std;

	BOOST_SYMBOL_EXPORT ClassFactory& GetClassFactory()
	{
		// TODO: �ڴ˴����� return ���
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

	//����ĳ��ĺ꣬���б���������ͨ����Ԫ��������ֵ���ָ��,���Ĭ�Ϲ��캯��
#define CF_CREATE(MyMap, ...) \
		auto creator = MyMap.find(conv2lower(_name));\
		return creator != MyMap.end() ? creator->second(__VA_ARGS__) : nullptr;

#define CF_STORE(MyMap) \
		MyMap[ conv2lower(_name) ] = creator;\
		return true;

#ifndef CF_REMOVE(MyMap)
#define CF_REMOVE(MyMap) \
		auto mapsize = MyMap.size(); \
		auto remover = MyMap.erase(_name);\
		return remover == mapsize ? true: false;
#endif

	//����ĳ��ĺ꣬���б���������ͨ����Ԫ��������ֵ���ָ��,���oofem�и���Ĺ��캯��
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

#ifndef CF_REMOVE(MyMap)
#define CF_REMOVE(MyMap) \
		auto mapsize = MyMap.size(); \
		auto remover = MyMap.erase(_name);\
		return remover == mapsize ? true: false;
#endif
	

	ClassFactory::ClassFactory()
	{
		
	}


	ClassFactory::~ClassFactory()
	{
	}

	BaseElem * ClassFactory::CreateElem(const char * _name, int _n, Domain * _dm)
	{
		//���桢���ϡ���Ԫ����Ĺ��캯��δ��
		CF_CREATE(ElemList)
	}

	bool ClassFactory::RegistElem(const char* _name, BaseElem *(*creator)(void))
	{
		CF_STORE(ElemList);
	}

	bool ClassFactory::RemoveElem(const char * _name, BaseElem *(*creator)(void))
	{
		CF_REMOVE(ElemList);
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

	BaseNumMethod * ClassFactory::CreateNumMethod(const char * _name, Domain * _dm, Engng * _master)
	{
		CF_CREATE(NumericalMethodList, _dm, _master);
	}

	bool ClassFactory::RegistNumMethod(const char * _name, BaseNumMethod *(*creator)(Domain *, Engng *))
	{
		CF_STORE(NumericalMethodList);
	}

	void ClassFactory::RegistEntity(BaseComponent * pEntity)
	{
		m_EntityMap.insert(std::make_pair(m_EntityMap.size(), pEntity ));
	}

	BaseComponent * ClassFactory::GetEntityFromID(unsigned int _id) const
	{
		EntityMap::const_iterator _iter = m_EntityMap.find(_id);

		Assert( (_iter != m_EntityMap.end()) && "<EntityManager::GetEntityFromID>: invalid ID" );
		_iter->second->ResetID(_iter->first + _iter->second->GetID());
		return _iter->second;
	
	}

	void ClassFactory::RemoveEntity(BaseComponent * pEntity)
	{
		m_EntityMap.erase( m_EntityMap.find(pEntity->GetID()) );
	}

}


