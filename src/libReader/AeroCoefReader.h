#pragma once

/************************************************************************/
/*     气动系数数据读取类，从xml表中读取                                */
/************************************************************************/

#include <HBXPreDef.h>
#include <tinyxml\tinyxml.h>
#include <tinyxml\tinyxml2.h>
#include <libCUFEM\BaseReader.h>
#include <libCUFEM\ElemtProp.h>
#include <HbxXmlDef\AeroTable.h>

namespace HBXFEMDef
{
	class InputRecord;


	class CUFEM_API AeroCoefReader
		:public BaseReader
	{
	private:
		//m_record已经在基类里了。在初始化时将record转化成启动数据表记录集
	protected:
		//读取气动力参数表相关函数
		InputFileResult	ReadAeroCoef();
		//读取表属性
		bool	ReadTableAttri(TiXmlElement* _inputT, HBXDef::_AEROTABLE* _outTable);
		//读取块内容
		bool	ReadBlock(TiXmlElement* _inputT, HBXDef::_AEROBLOCK* _outBlock);
		//读取块属性
		bool	ReadBlockAttri(TiXmlElement* _inputT, HBXDef::_AEROBLOCK* _outBlock);

		//@_inputT：输入的xml树形结构的某个节点
		//@_outBlock：输出的_AeroBlock指针
		//@_idx：_AeroBlock下的维度表的索引号
		//返回值：当前维度下的插值点个数
		size_t	ReadDemtion(TiXmlElement* _inputT, HBXDef::_AEROBLOCK* _outBlock, size_t _idx,
			std::vector<HBXDef::UserCalPrec>& vecOut);
		//读取插值点数据
		bool	ReadInterpolationData(TiXmlElement* _inputT, HBXDef::_AEROBLOCK* _outBlock, size_t _lgt);

	public:
		AeroCoefReader(const std::string& _SourceFile = "EltProperty.xml",
			std::string _savepath = "F:\\data from HBX_phd\\VS2015");	//构造函数

		//自身额外添加的函数，假装减少一次指针的指向
		HBXDef::_AEROTABLE*	GetAeroTable(const char* _str);	//获取气动数据表

		//通用接口
		bool SetInputData();

		//强制断开所有数据连接
		void terminate();
	};


	CUFEM_API BaseReader* InstanceAeroCoefReader();
}



