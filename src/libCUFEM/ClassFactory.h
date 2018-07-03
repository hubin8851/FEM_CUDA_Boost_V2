#pragma once

#include <map>
#include <ExportDef.h>
#include <SpMatrixType.h>
#include <SpMatrix.h>
#include <SolverType.h>

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
	class ExportModule;

	typedef void* (*AnyCreator)(int, Domain);

//	typedef bimap< tagged<int, struct id >, tagged< std::string, struct name >> EntityMap;
//	typedef std::map< bimap<tagged<int, struct id >, tagged< std::string, struct name >>, BaseComponent*> EntityMap;

#define REGIST_ENGNG(class) static bool __dummy_ ## class = GetClassFactory().RegistEngng( _EX_ ## class ## _Name, EngngCreator<class> );
#define REGIST_ELEMT(class) static bool __dummy_ ## class = GetClassFactory().RegistElem( _EX_ ## class ## _Name, ElemtCreator<class> );
#define REGIST_MATERIAL(class) static bool __dummy_ ## class = GetClassFactory().RegistMaterial( _EX_ ## class ## _Name, MaterialCreator<class> );
#define REGIST_EXPORTMODULE(class) static bool __dummy_ ## class = GetClassFactory().RegistExportModule( _EX_ ## class ## _Name, ExportModuleCreator<class> );
#define REGIST_SPARSEMAT(class) static bool __dummy__ ## class = GetClassFactory().RegistSparseMatrix( _EX_ ## class ## _Name, SparseMatCreator<class> );
#define REGIST_SOLVER(class) static bool __dummy__ ## class = GetClassFactory().RegistNumMethod(type, NumerialSolverCreator<class>);
	//����ģʽ�еĵ����֣��μ�
	//http://blog.csdn.net/wuzhekai1985/article/details/6660462
	//�๤�� class �洢���еĿ��ܳ��ֵĲ������ͨ����������
	class CUFEM_API ClassFactory
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
		//ϡ����������б�
		std::map<HBXDef::SpMatrixType_t, HBXDef::SparseMat*(*)()> SpMatrixList;
		//�㷨�����б�
		std::map<HBXDef::SolveMethod_t, BaseNumMethod*(*)(Domain*, Engng*, int)>	NumericalMethodList;
		//
		std::map<std::string, ExportModule*(*)( Engng*, int )> ModuleList;
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

		//�������/����ģ��
		ExportModule* CreateExportModule( const char* _name, Engng* _master, int _id );
		//ע�����/����ģ��
		bool RegistExportModule( const char* _name, ExportModule*(*creator)(Engng*, int) );

		HBXDef::SparseMat* CreatSparseMatrix( HBXDef::SpMatrixType_t _type);

		bool RegistSparseMatrix( HBXDef::SpMatrixType_t _type, HBXDef::SparseMat*(*creator)() );

		//����һ�����㷨
		BaseNumMethod* CreateNumMethod(HBXDef::SolveMethod_t _name, Domain * _dm, Engng * _master, int _id);
		//ע��һ�������棬�������ƺ͹��캯������
		bool RegistNumMethod(HBXDef::SolveMethod_t _name, BaseNumMethod *(*creator)(Domain *, Engng *, int));

#pragma endregion ��������

	};

	extern ClassFactory& classFactory;

	//û��Ū��Ϊʲô��������������Ϊһ����̬��Ա...
	CUFEM_API ClassFactory& GetClassFactory();

}


