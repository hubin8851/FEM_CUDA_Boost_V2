#include <stdlib.h>
#include <regex>
#include <libCUFEM\BaseReader.h>
#include "ShapeTabReader.h"
#include "HbxDefMacro.h"


int LocateShaptTab( int _bottomSort, int _upSort)
{

	if (0 == _upSort % 2)
	{
		_upSort = _midSort;
	}
	else _upSort = _upSort / 2 + 1;
}


namespace HBXFEMDef
{


	double* ShapeTabReader::ReadShapeData(int _nBlock, const char* _fname)
	{
		using namespace std;
		using namespace HBXDef;

		std::ifstream inFile;
		std::string stringLine;		//读取的当前行
		std::cmatch result;	//得到的结果容器,same as std::match_results<const char*> cm;
		std::string regex_str("\d+\.\d+E-\d+");//按照传统正则表达式，科学计数法的 ^([-])?(?!0\d)\d+(\.\d{1,})?(E([-])?\d+)?$
		std::regex pattern1(regex_str, regex::icase);//https://www.cnblogs.com/sgdd123/p/7778599.html

		inFile.open(_fname);
		if (inFile.bad() || !inFile.is_open())
		{
			std::cerr << "Error1： Can not open input data file" << std::endl;
			//return InputFileResult_t::IRRT_BAD_FORMAT;
		}

		getline(inFile, stringLine);//先读取第一行
		// 定义分割方式为英文逗号，中文逗号和空格，构造一个分词器，  
		boost::char_separator<char> sep(" ,，*");
		typedef boost::tokenizer<boost::char_separator<char> >  CustonTokenizer;
		boost::trim(stringLine);//去掉两端的空格
		CustonTokenizer tok(stringLine, sep);//将获得的string填入分割器
		this->nBlock = boost::lexical_cast<HBXDef::UserReadPrec>(*tok.end()); //获得point
		//完成对容器的内存分配

		int cnt;//
		int m;//m表示的是每行数字的个数

		//正则查找
		for (int i=0; i< this->nBlock; i++)
		{
			std::shared_ptr<ShapeTable> _tmpShapeTable = std::make_shared<ShapeTable>();
			getline(inFile, stringLine);//读取block块的第一行
			boost::trim(stringLine);//去掉两端的空格
			_tmpShapeTable->t = std::atof(stringLine.c_str());//对t赋值
			getline(inFile, stringLine);//读取block块的第二行
			boost::trim(stringLine);//去掉两端的空格
			_tmpShapeTable->flag = std::atof(stringLine.c_str());//对标志位赋值
			getline(inFile, stringLine);//读取block块的第三行
			boost::trim(stringLine);//去掉两端的空格
			_tmpShapeTable->nRow = std::atof(stringLine.c_str());//对行数赋值
			_tmpShapeTable->nCol = 24;
			_tmpShapeTable->AllocShapeStructArray();

			for (int j = 0; j < _vShape[i]->nRow + 3; j++)
			{
				cnt = 0;//重置cnt
				getline(inFile, stringLine);//读取block块的第三行
				boost::trim(stringLine);//去掉两端的空格
				std::regex_match( stringLine.c_str(), result, pattern1 );

				//迭代器声明
				//std::string::const_iterator iterBegin = stringLine.begin();
				//std::string::const_iterator iterEnd = stringLine.end();
				//std::regex_search(iterBegin, iterEnd, result, pattern1);
				m = result.size();
				for (int k = 0; k < m; k++)
				{
					_tmpShapeTable->_ShapeData[cnt + k] = std::atof(result[k].second);
				}
				cnt += m;
			}
			
			
		}

		inFile.close();
		

		return nullptr;
	}

	double ShapeTabReader::MakeShapeData(double _t, long _flag, int med)
	{
		//采用二分法获取邻近节点
		int _bottomSort = 0;
		int _upSort = nBlock - 1;
		int _midSort = _upSort/2;
		
		while (true)
		{
			//判断上下限的索引差值是否为1，若为1说明在该区间范围内,跳出循环
			if ( 1 == (_upSort - _bottomSort) )
			{
				break;
			}

			if (_vShape[_bottomSort]->t < _t <_vShape[_midSort]->t)//如果在区间下半部分，收缩上限为mid
			{
				if (0 == _upSort%2)
				{
					_upSort = _midSort;
				}
				else _upSort = _upSort / 2 + 1;
			}
			else if ( _vShape[_midSort]->t < _t < _vShape[_upSort]->t )//如果在区间上半部分，收缩上限为mid
			{
				_bottomSort = _midSort;
			}
			int _midSort = _upSort / 2;
		}
		

		
		return 0.0;
	}

	int ShapeTabReader::GetVibData(double _t, long _structType)
	{
		this->MakeShapeData( _t, _structType );//依照当前时间，插值得到当前振型数据

		for (int i = 0; i < 3; i++) E3_Omiga[i] = _shapeTableNow._ShapeData[i];//扭转频率
		for (int i = 0; i < 5; i++) E1_Omiga[i] = _shapeTableNow._ShapeData[i + 3];//轴向频率
		for (int i = 0; i < 5; i++) E2_Omiga[i] = _shapeTableNow._ShapeData[i + 8];//法向频率
		int cnt = 13;

		for (int i = 0; i < _shapeTableNow.nRow; i++)
		{
			//扭转模态3+法向模态5+轴向模态5  * 站点数
			E3_Mode1_Shape[i] = _shapeTableNow._ShapeData[i*_shapeTableNow.nCol + 1 + cnt];
			E3_Mode2_Shape[i] = _shapeTableNow._ShapeData[i*_shapeTableNow.nCol + 2 + cnt];
			E3_Mode3_Shape[i] = _shapeTableNow._ShapeData[i*_shapeTableNow.nCol + 3 + cnt];
			E1_Mode1_Shape[i] = _shapeTableNow._ShapeData[i*_shapeTableNow.nCol + 4 + cnt];
			E1_Mode2_Shape[i] = _shapeTableNow._ShapeData[i*_shapeTableNow.nCol + 5 + cnt];
			E1_Mode3_Shape[i] = _shapeTableNow._ShapeData[i*_shapeTableNow.nCol + 6 + cnt];
			E1_Mode4_Shape[i] = _shapeTableNow._ShapeData[i*_shapeTableNow.nCol + 7 + cnt];
			E1_Mode5_Shape[i] = _shapeTableNow._ShapeData[i*_shapeTableNow.nCol + 8 + cnt];
			E2_Mode1_Shape[i] = _shapeTableNow._ShapeData[i*_shapeTableNow.nCol + 9 + cnt];
			E2_Mode2_Shape[i] = _shapeTableNow._ShapeData[i*_shapeTableNow.nCol + 10 + cnt];
			E2_Mode3_Shape[i] = _shapeTableNow._ShapeData[i*_shapeTableNow.nCol + 11 + cnt];
			E2_Mode4_Shape[i] = _shapeTableNow._ShapeData[i*_shapeTableNow.nCol + 12 + cnt];
			E2_Mode5_Shape[i] = _shapeTableNow._ShapeData[i*_shapeTableNow.nCol + 13 + cnt];
		}

		return 0;
	}


	ShapeTabReader::ShapeTabReader(const std::string _SourceFile, std::string _savepath)
	{
		BaseReader::m_SrcFileName = _SourceFile;
		BaseReader::m_path = _savepath;

		std::cout << "create ShapeTabReader!" << std::endl;
	}




	CUFEM_API BaseReader*  InstanceShapeTabReader(const std::string _SourceFile, std::string _savepath)
	{
		return nullptr;
	}




}





