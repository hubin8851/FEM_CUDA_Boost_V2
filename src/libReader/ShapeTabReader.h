#pragma once
#include <ExportDef.h>
#include <BoostPreDef.h>
#include <HBXPreDef.h>
#include <HbxDefMacro.h>
#include <libCUFEM\BaseReader.h>

typedef struct ShapeTable
{
	long flag;
	int nCol;//������ģ̬
	int nRow;//վ����
	int iStart, iEnd;
	double t; //data = icon * inum + 19;???Ϊʲô��һ��24�����֡�һ��ʱ���156��
	double* _ShapeData;

	//ShapeTable(int _nCol, int _nRow):nCol(_nCol),nRow(_nRow)
	//{
	//	malloc( sizeof(HBXDef::UserReadPrec)*( nCol*nRow + 20 ) );
	//}

	bool AllocShapeStructArray()
	{
		this->_ShapeData = (HBXDef::UserReadPrec*)malloc(sizeof(HBXDef::UserReadPrec)*(this->nCol*this->nRow + 20));
		if (nullptr == this->_ShapeData)
		{
			return false;
		}
		return true;
	}

}ShapeTable_s;//һ��shapeTable��ĳ��ʱ����������Ϣ



namespace HBXFEMDef
{
	class AttriRecord;
	class Domain;
	class InputRecord;
	class BaseElem;


	class CUFEM_API ShapeTabReader :
		public BaseReader
	{
	private:
		int nPoint;
		int nBlock;
	protected:
		/* �������͵�����ϵ��������Ƶ�ʡ�������(����)����������(ת������)  */
		//���򡢷�����ף�Ťת3��
		HBXDef::UserReadPrec  E1_Ksai[5], E1_Omiga[5], E1_Gen_Force[5], E1_Gen_Mass[5];
		HBXDef::UserReadPrec  E2_Ksai[5], E2_Omiga[5], E2_Gen_Force[5], E2_Gen_Mass[5];
		HBXDef::UserReadPrec  E3_Ksai[3], E3_Omiga[3], E3_Gen_Force[3], E3_Gen_Mass[3];

		/* �������� */
		int Node;
		HBXDef::UserReadPrec  E1_Mode1_Shape[400], E1_Mode2_Shape[400], E1_Mode3_Shape[400];
		HBXDef::UserReadPrec  E1_Mode4_Shape[400], E1_Mode5_Shape[400];
		HBXDef::UserReadPrec  E2_Mode1_Shape[400], E2_Mode2_Shape[400], E2_Mode3_Shape[400];
		HBXDef::UserReadPrec  E2_Mode4_Shape[400], E2_Mode5_Shape[400];
		HBXDef::UserReadPrec  E3_Mode1_Shape[400], E3_Mode2_Shape[400], E3_Mode3_Shape[400];

		ShapeTable _shapeTableNow;
		std::vector<std::shared_ptr<ShapeTable>> _vShape;

		//��ȡ��������
		//_nBlock:ʱ��˳���ģ̬���ݿ����
		//_fname:�ļ����� 
		double* ReadShapeData(int _nBlock, const char* _fname);

		//long flag:��־λ
		//t:ʱ��
		double MakeShapeData( double _t, long _flag, int med = true );

		//��ȡ�񶯲���
		//_t:ʱ��
		//_structType:�ṹ���ͣ�ʵ��Ϊflag��־λ��ʾ
		int GetVibData( double _t, long _structType );


	public:
		//Ĭ�Ϲ��캯��
		ShapeTabReader(const std::string _SourceFile = "",
			std::string _savepath = "F:\\data_from_HBX_phd\\vs2015");
		//�������캯��
		//		ShapeTabReader(const _SameClass&) {};
		~ShapeTabReader() {};

		//���inp\xml\PW�ļ��ĵ��룬���������������ܸվ����ά�ȵȲ���
		InputFileResult SetInputData();


		//ǿ�ƶϿ�������������
		void terminate() {};
	};



	CUFEM_API BaseReader* InstanceShapeTabReader(const std::string _SourceFile = "",
		std::string _savepath = "F:\\data_from_HBX_phd\\vs2015");
}
