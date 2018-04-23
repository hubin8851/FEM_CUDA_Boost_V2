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
							std::string _savepath = "F:\\data from HBX_phd\\VS2015");	//构造函数

		HBXFEMDef::_EltPtyInMap* const GetPtyMap();

		bool SetInputData();

		//强制断开所有数据连接
		void terminate();
	};


	CUFEM_API BaseReader* InstanceElemtPropReader();
}
