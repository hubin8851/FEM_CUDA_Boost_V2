#pragma once
#include <HBXPreDef.h>
#include <ExportDef.h>
#include <tinyxml\tinyxml.h>
#include <tinyxml\tinyxml2.h>
#include <libCUFEM\BaseReader.h>
#include <libCUFEM\ElemtProp.h>



namespace HBXFEMDef
{
	class CUFEM_API EltPropReader:
		public BaseReader
	{
	private:
		HBXFEMDef::_EltPtyInMap	m_ElemtPtyInMap;
	protected:
		InputFileResult	ReadElemtProperty();		//获取所有的表号编码
	public:
		EltPropReader(const std::string& _SourceFile = "EltProperty.xml",
							std::string _savepath = "..\\");	//构造函数,默认路径为根目录

		HBXFEMDef::_EltPtyInMap* const GetPtyMap();

		//设置索引路径，会根据当前索引路径下所有文件夹索引，搜索到的第一个当前文件读入
		bool SetSourceFilePath(const std::string _SourceFile, std::string _savepath);

		InputFileResult SetInputData();

		//强制断开所有数据连接
		void terminate();
	};


	CUFEM_API BaseReader* InstanceElemtPropReader();
}
