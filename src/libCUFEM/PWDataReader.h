#pragma once

#include <HBXPreDef.h>
#include <libCUFEM\BaseReader.h>
#include <libCUFEM\BaseNode.h>
#include <libCUFEM\BaseElement.h>
#include <libCUFEM\domain.h>
#include <libCUFEM\AttriRecord.h>



namespace HBXFEMDef
{

	class AttriRecord;
// 	class Domain;
// 	struct Node;
// 	struct BaseElem;
// 	class CSet;

	class BOOST_SYMBOL_EXPORT PWDataReader:
		public BaseReader
	{
	private:
		typedef PWDataReader _SameClass;

// 		std::string	m_SrcFileName;	//源文件名称
// 		boost::filesystem::path	m_path;	//源文件路径

	protected:
		InputFileResult ReadpwFile();	//读取pointwise到处的点集合，为VTX做准备
	public:
		//默认构造函数
		PWDataReader();
		//拷贝构造函数
//		PWDataReader(const _SameClass&) {};
		~PWDataReader(){};

		//设置索引路径，会根据当前索引路径下所有文件夹索引，搜索到的第一个当前文件读入
//		bool SetSourceFilePath(const std::string _SourceFile, boost::filesystem::path _savepath = "");

		bool SetInputData();

		//强制断开所有数据连接
		void terminate();

		const char* GetSourceName() const;

		//boost.dll输出的某一种方式中使用，弃坑。
		static std::size_t size();

// 		static boost::shared_ptr<PWDataReader> InstancePWReader()
// 		{
// 			return boost::make_shared<PWDataReader>(new PWDataReader());
// 		}

	};

	CUFEM_EXPORT BaseReader* InstancePWReader();

// 	extern "C" BOOST_SYMBOL_EXPORT PWDataReader PWReader;
// 	PWDataReader PWReader;
//	CUFEM_EXPORT PWDataReader* _theelement = nullptr;
//	CUFEM_EXPORT PWDataReader* allocReader();
	

}

//  	BOOST_DLL_ALIAS(
//  		HBXFEMDef::PWDataReader,
//  		MyPWReader
//  	)