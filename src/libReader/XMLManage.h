#pragma once
#include "stdafx.h"
#include "AeroTable.h"

using namespace std;


class CXMLManage
{
public:
	CXMLManage( const std::string& _SourceFile = "EltProperty.xml", boost::filesystem::path _savepath = "F:\\data from HBX_phd\\VS2010\\FEMCal_CUDA_MPI_Boost\\data\\DataIn" );	//���캯��
	int	TestXML(const char* _filename);
	bool	SetSourceFilePath( const std::string& _SourceFile = "EltProperty.xml", boost::filesystem::path _savepath = "F:\\data from HBX_phd\\VS2010\\FEMCal_CUDA_MPI_Boost\\data\\DataIn" );
	void	ReadElemtProperty();		//��ȡ���еı��ű���
	//HBXFEMDef::_EltPtyInMap&	GetElemtPtyInMap();		//��ȡ[��Ԫ����-(����)]-�������Ƶ�ӳ��

	//��ȡ��������������غ���
	void	ReadAeroCoef( const char* _filename );				
	bool	ReadTableAttri( TiXmlElement* _inputT, HBXDef::_AEROTABLE* _outTable );
	bool	ReadBlock( TiXmlElement* _inputT, HBXDef::_AEROBLOCK* _outBlock  );
	bool	ReadBlockAttri( TiXmlElement* _inputT, HBXDef::_AEROBLOCK* _outBlock );
	//@_inputT�������xml���νṹ��ĳ���ڵ�
	//@_outBlock�������_AeroBlockָ��
	//@_idx��_AeroBlock�µ�ά�ȱ���������
	//����ֵ����ǰά���µĲ�ֵ�����
	size_t	ReadDemtion( TiXmlElement* _inputT, HBXDef::_AEROBLOCK* _outBlock, size_t _idx );
	bool	ReadInterpolationData( TiXmlElement* _inputT, HBXDef::_AEROBLOCK* _outBlock, size_t _lgt );

	HBXDef::_AEROTABLE&	GetAeroTable( std::string _str );	//��ȡ�������ݱ�


	~CXMLManage(void);

private:
	boost::filesystem::path	m_path;
	std::string		m_SrcFileName;

	//HBXFEMDef::_EltPtyInMap	m_ElemtPtyInMap;
	//Ϊ�˳������չ�ԣ��ɴ�XML���ȡÿ�ֵ�Ԫ��Ӧ������
	typedef std::map< std::string, boost::shared_ptr<HBXDef::_AEROTABLE> > _AeroTableInMap;
	typedef _AeroTableInMap::iterator _AeroTableMapIter;

	_AeroTableInMap	m_AeroTableInMap;
};
