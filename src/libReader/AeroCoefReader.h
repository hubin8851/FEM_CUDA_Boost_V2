#pragma once

#include <HBXPreDef.h>
#include <tinyxml\tinyxml.h>
#include <tinyxml\tinyxml2.h>
#include <libCUFEM\BaseReader.h>
#include <libCUFEM\ElemtProp.h>
#include <HbxXmlDef\AeroTable.h>

namespace HBXFEMDef
{
	class BOOST_SYMBOL_EXPORT AeroCoefReader :
		public BaseReader
	{
	private:
		//Ϊ�˳������չ�ԣ��ɴ�XML���ȡÿ�ֵ�Ԫ��Ӧ������
		typedef std::map< std::string, boost::shared_ptr<HBXDef::_AEROTABLE> > _AeroTableInMap;
		typedef _AeroTableInMap::iterator _AeroTableMapIter;
		//�����ĳ�Ա
		_AeroTableInMap	m_AeroTableInMap;
	protected:
		//��ȡ��������������غ���
		InputFileResult	ReadAeroCoef();
		//��ȡ������
		bool	ReadTableAttri(TiXmlElement* _inputT, HBXDef::_AEROTABLE* _outTable);
		//��ȡ������
		bool	ReadBlock(TiXmlElement* _inputT, HBXDef::_AEROBLOCK* _outBlock);
		//��ȡ������
		bool	ReadBlockAttri(TiXmlElement* _inputT, HBXDef::_AEROBLOCK* _outBlock);

		//@_inputT�������xml���νṹ��ĳ���ڵ�
		//@_outBlock�������_AeroBlockָ��
		//@_idx��_AeroBlock�µ�ά�ȱ��������
		//����ֵ����ǰά���µĲ�ֵ�����
		size_t	ReadDemtion(TiXmlElement* _inputT, HBXDef::_AEROBLOCK* _outBlock, size_t _idx);
		//��ȡ��ֵ������
		bool	ReadInterpolationData(TiXmlElement* _inputT, HBXDef::_AEROBLOCK* _outBlock, size_t _lgt);

	public:
		AeroCoefReader(const std::string& _SourceFile = "EltProperty.xml",
			boost::filesystem::path _savepath = "F:\\data from HBX_phd\\VS2015");	//���캯��

		HBXDef::_AEROTABLE&	GetAeroTable(std::string _str);	//��ȡ�������ݱ�

		//ͨ�ýӿ�
		bool SetInputData();

		//ǿ�ƶϿ�������������
		void terminate();
	};


	CUFEM_EXPORT BaseReader* InstanceAeroCoefReader();
}



