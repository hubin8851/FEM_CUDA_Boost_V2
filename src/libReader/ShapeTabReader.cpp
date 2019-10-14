#include <stdlib.h>
#include <regex>
#include <libCUFEM\BaseReader.h>
#include "ShapeTabReader.h"
#include "HbxDefMacro.h"


int LocateShaptTab( int _bottomSort, int _upSort)
{
	int _midSort = 0;
	if (0 == _upSort % 2)
	{
		_upSort = _midSort;
	}
	else _upSort = _upSort / 2 + 1;
	//to be continue...
	return 0;
}


namespace HBXFEMDef
{


	double* ShapeTabReader::ReadShapeData(int _nBlock, const char* _fname)
	{
		using namespace std;
		using namespace HBXDef;

		std::ifstream inFile;
		std::string stringLine;		//��ȡ�ĵ�ǰ��
		std::cmatch result;	//�õ��Ľ������,same as std::match_results<const char*> cm;
		std::string regex_str("\d+\.\d+E-\d+");//���մ�ͳ������ʽ����ѧ�������� ^([-])?(?!0\d)\d+(\.\d{1,})?(E([-])?\d+)?$
		std::regex pattern1(regex_str, regex::icase);//https://www.cnblogs.com/sgdd123/p/7778599.html

		inFile.open(_fname);
		if (inFile.bad() || !inFile.is_open())
		{
			std::cerr << "Error1�� Can not open input data file" << std::endl;
			//return InputFileResult_t::IRRT_BAD_FORMAT;
		}

		getline(inFile, stringLine);//�ȶ�ȡ��һ��
		// ����ָʽΪӢ�Ķ��ţ����Ķ��źͿո񣬹���һ���ִ�����  
		boost::char_separator<char> sep(" ,��*");
		typedef boost::tokenizer<boost::char_separator<char> >  CustonTokenizer;
		boost::trim(stringLine);//ȥ�����˵Ŀո�
		CustonTokenizer tok(stringLine, sep);//����õ�string����ָ���
		this->nBlock = boost::lexical_cast<HBXDef::UserReadPrec>(*tok.end()); //���point
		//��ɶ��������ڴ����

		int cnt;//
		int m;//m��ʾ����ÿ�����ֵĸ���

		//�������
		for (int i=0; i< this->nBlock; i++)
		{
			std::shared_ptr<ShapeTable> _tmpShapeTable = std::make_shared<ShapeTable>();
			getline(inFile, stringLine);//��ȡblock��ĵ�һ��
			boost::trim(stringLine);//ȥ�����˵Ŀո�
			_tmpShapeTable->t = std::atof(stringLine.c_str());//��t��ֵ
			getline(inFile, stringLine);//��ȡblock��ĵڶ���
			boost::trim(stringLine);//ȥ�����˵Ŀո�
			_tmpShapeTable->flag = std::atof(stringLine.c_str());//�Ա�־λ��ֵ
			getline(inFile, stringLine);//��ȡblock��ĵ�����
			boost::trim(stringLine);//ȥ�����˵Ŀո�
			_tmpShapeTable->nRow = std::atof(stringLine.c_str());//��������ֵ
			_tmpShapeTable->nCol = 24;
			_tmpShapeTable->AllocShapeStructArray();

			for (int j = 0; j < _vShape[i]->nRow + 3; j++)
			{
				cnt = 0;//����cnt
				getline(inFile, stringLine);//��ȡblock��ĵ�����
				boost::trim(stringLine);//ȥ�����˵Ŀո�
				std::regex_match( stringLine.c_str(), result, pattern1 );

				//����������
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
		ShapeTable _tmpShapeTable;
		_tmpShapeTable.t = _t;

		//���ö��ַ���ȡ�ڽ��ڵ�
		int _bottomSort = 0;
		int _upSort = nBlock - 1;
		int _midSort = _upSort/2;
		
		while (true)
		{
			//�ж������޵�������ֵ�Ƿ�Ϊ1����Ϊ1˵���ڸ����䷶Χ��,����ѭ��
			if ( 1 == (_upSort - _bottomSort) || 
				fabs(_t - _bottomSort)< ERRORTOLERATE ) //�ڵײ��߽�
				
			{
				_upSort = _bottomSort +1;
				break;
			}
			else if ( fabs(_t - _upSort) < ERRORTOLERATE )		//���ϲ��߽�)
			{
				_bottomSort = _upSort - 1;
			}

			if (_vShape[_bottomSort]->t < _t <_vShape[_midSort]->t)//����������°벿�֣���������Ϊmid
			{
				if (0 == _upSort%2)
				{
					_upSort = _midSort;
				}
				else _upSort = _upSort / 2 + 1;
			}
			else if ( _vShape[_midSort]->t < _t < _vShape[_upSort]->t )//����������ϰ벿�֣���������Ϊmid
			{
				_bottomSort = _midSort;
			}
			int _midSort = _upSort / 2;
		}
		
		int n = _tmpShapeTable.nRow*_tmpShapeTable.nCol + 19;
		for (int i = 0; i < n; i++)
			_tmpShapeTable._ShapeData[i] = (_vShape[_upSort]->_ShapeData[i] - _vShape[_bottomSort]->_ShapeData[i])
											*(_t - _vShape[_bottomSort]->t) / (_vShape[_upSort]->t - _vShape[_bottomSort]->t) 
											+ _vShape[_bottomSort]->_ShapeData[i];
		
		return _t - _vShape[_bottomSort]->t;
	}

	int ShapeTabReader::GetVibData(double _t, long _structType)
	{
		this->MakeShapeData( _t, _structType );//���յ�ǰʱ�䣬��ֵ�õ���ǰ��������

		for (int i = 0; i < 3; i++) E3_Omiga[i] = _shapeTableNow._ShapeData[i];//ŤתƵ��
		for (int i = 0; i < 5; i++) E1_Omiga[i] = _shapeTableNow._ShapeData[i + 3];//����Ƶ��
		for (int i = 0; i < 5; i++) E2_Omiga[i] = _shapeTableNow._ShapeData[i + 8];//����Ƶ��
		int cnt = 13;

		for (int i = 0; i < _shapeTableNow.nRow; i++)
		{
			//Ťתģ̬3+����ģ̬5+����ģ̬5  * վ����
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

	InputFileResult ShapeTabReader::SetInputData()
	{
		return InputFileResult::IRRT_OK;
	}




	CUFEM_API BaseReader*  InstanceShapeTabReader(const std::string _SourceFile, std::string _savepath)
	{
		return nullptr;
	}




}





