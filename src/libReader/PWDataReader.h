#pragma once

/************************************************************************/
/*     PW文件的数据读取类                                               */
/************************************************************************/

#include <HBXPreDef.h>
#include <ExportDef.h>
#include <libCUFEM\BaseNode.h>
#include <libCUFEM\BaseElement.h>
#include <libCUFEM\domain.h>
#include <libCUFEM\AttriRecord.h>
#include <libCUFEM\BaseSlnRecord.h>
#include <libCUFEM\BaseReader.h>
#include <MyTimer.h>


namespace HBXFEMDef
{

	class AttriRecord;
// 	class Domain;
// 	struct Node;
// 	struct BaseElem;
// 	class CSet;

	class CUFEM_API PWDataReader:
		public BaseReader
	{
	private:
		typedef PWDataReader _SameClass;

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

		InputFileResult SetInputData();

		//强制断开所有数据连接
		void terminate();

		//boost.dll输出的某一种方式中使用，弃坑。
		static std::size_t size();

// 		static boost::shared_ptr<PWDataReader> InstancePWReader()
// 		{
// 			return boost::make_shared<PWDataReader>(new PWDataReader());
// 		}

	};

	CUFEM_API BaseReader* InstancePWReader();

// 	extern "C" BOOST_SYMBOL_EXPORT PWDataReader PWReader;
// 	PWDataReader PWReader;
//	CUFEM_EXPORT PWDataReader* _theelement = nullptr;
//	CUFEM_EXPORT PWDataReader* allocReader();
	

}

//  	BOOST_DLL_ALIAS(
//  		HBXFEMDef::PWDataReader,
//  		MyPWReader
//  	)