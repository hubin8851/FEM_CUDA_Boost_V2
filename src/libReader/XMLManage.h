#pragma once
#include <BoostPreDef.h>
#include <tinyxml\tinyxml.h>
#include <tinyxml\tinystr.h>
#include "HbxCudaDef\AeroTable.h"

using namespace std;


class CXMLManage
{
public:
	CXMLManage( const std::string& _SourceFile = "EltProperty.xml", boost::filesystem::path _savepath = "F:\\data from HBX_phd\\VS2010\\FEMCal_CUDA_MPI_Boost\\data\\DataIn" );	//构造函数
	int	TestXML(const char* _filename);
	bool	SetSourceFilePath( const std::string& _SourceFile = "EltProperty.xml", boost::filesystem::path _savepath = "F:\\data from HBX_phd\\VS2010\\FEMCal_CUDA_MPI_Boost\\data\\DataIn" );
	void	ReadElemtProperty();		//获取所有的表号编码
	//HBXFEMDef::_EltPtyInMap&	GetElemtPtyInMap();		//获取[单元名称-(属性)]-对象名称的映射

	//读取气动力参数表相关函数
	void	ReadAeroCoef( const char* _filename );				
	bool	ReadTableAttri( TiXmlElement* _inputT, HBXDef::_AEROTABLE* _outTable );
	bool	ReadBlock( TiXmlElement* _inputT, HBXDef::_AEROBLOCK* _outBlock  );
	bool	ReadBlockAttri( TiXmlElement* _inputT, HBXDef::_AEROBLOCK* _outBlock );
	//@_inputT：输入的xml树形结构的某个节点
	//@_outBlock：输出的_AeroBlock指针
	//@_idx：_AeroBlock下的维度表的索引号
	//返回值：当前维度下的插值点个数
	size_t	ReadDemtion( TiXmlElement* _inputT, HBXDef::_AEROBLOCK* _outBlock, size_t _idx );
	bool	ReadInterpolationData( TiXmlElement* _inputT, HBXDef::_AEROBLOCK* _outBlock, size_t _lgt );

	HBXDef::_AEROTABLE&	GetAeroTable( std::string _str );	//获取气动数据表


	~CXMLManage(void);

private:
	boost::filesystem::path	m_path;
	std::string		m_SrcFileName;

	//HBXFEMDef::_EltPtyInMap	m_ElemtPtyInMap;
	//为了程序的拓展性，可从XML里读取每种单元对应的属性
	typedef std::map< std::string, boost::shared_ptr<HBXDef::_AEROTABLE> > _AeroTableInMap;
	typedef _AeroTableInMap::iterator _AeroTableMapIter;

	_AeroTableInMap	m_AeroTableInMap;
};

