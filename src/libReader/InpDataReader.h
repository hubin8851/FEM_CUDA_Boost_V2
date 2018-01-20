#pragma once

#include <HBXPreDef.h>
#include <libCUFEM\BaseNode.h>
#include <libCUFEM\BaseElement.h>
#include <libCUFEM\domain.h>
#include <libCUFEM\AttriRecord.h>
#include <libCUFEM\BaseReader.h>
#include <libCUFEM\Set.h>
#include <libCUFEM\InputRecord.h>

#pragma warning(disable: 4996)

namespace HBXFEMDef
{

	class AttriRecord;
	class Domain;
	class InputRecord;
	class BaseElem;

	class DynamicInputRecord;

	class BOOST_SYMBOL_EXPORT InpDataReader :
		public BaseReader
	{
	private:
		typedef InpDataReader _SameClass;

	protected:
		InputFileResult ReadInpFile();	//读取pointwise到处的点集合，为VTX做准备
	public:
		//默认构造函数
		InpDataReader();
		//拷贝构造函数
		//		PWDataReader(const _SameClass&) {};
		~InpDataReader() {};

		//设置索引路径，会根据当前索引路径下所有文件夹索引，搜索到的第一个当前文件读入
		//		bool SetSourceFilePath(const std::string _SourceFile, boost::filesystem::path _savepath = "");

		bool SetInputData();

		//强制断开所有数据连接
		void terminate();


	};

	CUFEM_EXPORT BaseReader* InstanceInpReader();


}
