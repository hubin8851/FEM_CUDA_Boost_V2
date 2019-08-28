#pragma once
#include <ExportDef.h>
#include <BoostPreDef.h>
#include <HBXPreDef.h>
#include <HbxDefMacro.h>
#include <libCUFEM\BaseReader.h>

typedef struct ShapeTable
{
	long flag;
	int nCol;//各方向模态
	int nRow;//站点数
	int iStart, iEnd;
	double t; //data = icon * inum + 19;???为什么，一行24个数字。一个时间点156行
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

}ShapeTable_s;//一个shapeTable是某个时间点的振型信息



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
		/* 各次振型的阻尼系数、固有频率、广义力(力矩)、广义质量(转动惯量)  */
		//轴向、法向五阶，扭转3阶
		HBXDef::UserReadPrec  E1_Ksai[5], E1_Omiga[5], E1_Gen_Force[5], E1_Gen_Mass[5];
		HBXDef::UserReadPrec  E2_Ksai[5], E2_Omiga[5], E2_Gen_Force[5], E2_Gen_Mass[5];
		HBXDef::UserReadPrec  E3_Ksai[3], E3_Omiga[3], E3_Gen_Force[3], E3_Gen_Mass[3];

		/* 各次振型 */
		int Node;
		HBXDef::UserReadPrec  E1_Mode1_Shape[400], E1_Mode2_Shape[400], E1_Mode3_Shape[400];
		HBXDef::UserReadPrec  E1_Mode4_Shape[400], E1_Mode5_Shape[400];
		HBXDef::UserReadPrec  E2_Mode1_Shape[400], E2_Mode2_Shape[400], E2_Mode3_Shape[400];
		HBXDef::UserReadPrec  E2_Mode4_Shape[400], E2_Mode5_Shape[400];
		HBXDef::UserReadPrec  E3_Mode1_Shape[400], E3_Mode2_Shape[400], E3_Mode3_Shape[400];

		ShapeTable _shapeTableNow;
		std::vector<std::shared_ptr<ShapeTable>> _vShape;

		//读取振型数据
		//_nBlock:时间顺序的模态数据块个数
		//_fname:文件名称 
		double* ReadShapeData(int _nBlock, const char* _fname);

		//long flag:标志位
		//t:时间
		double MakeShapeData( double _t, long _flag, int med = true );

		//获取振动参数
		//_t:时间
		//_structType:结构类型，实则为flag标志位表示
		int GetVibData( double _t, long _structType );


	public:
		//默认构造函数
		ShapeTabReader(const std::string _SourceFile = "",
			std::string _savepath = "F:\\data_from_HBX_phd\\vs2015");
		//拷贝构造函数
		//		ShapeTabReader(const _SameClass&) {};
		~ShapeTabReader() {};

		//完成inp\xml\PW文件的导入，填充各容器并计算总刚矩阵的维度等参数
		InputFileResult SetInputData();


		//强制断开所有数据连接
		void terminate() {};
	};



	CUFEM_API BaseReader* InstanceShapeTabReader(const std::string _SourceFile = "",
		std::string _savepath = "F:\\data_from_HBX_phd\\vs2015");
}
