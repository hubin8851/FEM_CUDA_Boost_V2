#pragma once
#include <TPOW.h>
#include "HbxCudaDef\CuComponent.cuh"
#include <libInterpolate\interval_lib.h>


#ifndef ZeroMemory
	#define ZeroMemory(_des,_size) memset(_des,0,_size)
#endif // !ZeroMemory

typedef double spfloat;
//#include<cmath>

#ifndef MAX_PATH
#define MAX_PATH 260
#endif //MAX_PATH

#define IFNULL(_ptr_des,_s) if(!_ptr_des){printf("%s\n",_s);return 0;}
using namespace std;




#ifndef TPOW2(_VAL)
#define TPOW2(_VAL) TPOW<2,_VAL>::VAL
#endif // !#define TPOW2(_VAL)

template<unsigned _T>
class baselag :public HBXDef::_CuBase_uni_
{
public:
	enum 
	{
		VALUE = _T
	};
baselag(HBXDef::cuTable<_T>* _table,unsigned int _blockid)
	{
//		m_lag_cordinate	= nullptr;

		m_table			= nullptr;
		m_isinitiall = false;
		m_table = _table;
		if (!m_table)
		{
			printf("��ȡ���ݱ����");
			return;
		}

		unsigned int _demention = m_table->GetDim();
		if (_demention <= 0) return;

		memcpy(m_cordial_demtion, m_table->m_numperdim, sizeof(unsigned int)*_T);

		ZeroMemory(m_lag_cordinate, _T * sizeof(unsigned int));
		m_cordials = m_table->m_cordials;
		m_beg = m_table->m_data;

		m_isinitiall = true;
	}
	__host__ baselag(const baselag<_T>& _theold)
	{
		//m_lag_cordinate = new unsigned int[_T];
		//m_demention = _theold.m_demention;
		//m_temp_proper = new spfloat[_T];
		ZeroMemory(m_lag_cordinate, _T * sizeof(unsigned int));
	//	memcpy(m_lag_cordinate, _theold.m_lag_cordinate, _T * sizeof(unsigned int));
		memcpy(m_cordial_demtion, _theold.m_cordial_demtion, _T * sizeof(unsigned int));
		m_cordials = _theold.m_cordials;
		m_beg = _theold.m_beg;

		m_isinitiall = true;
	}


	~baselag()
	{
	
		if (m_table)
		{
			delete m_table;
			m_table = nullptr;
		}
	}
	__host__ unsigned int GetDemntion() { return _T; };
public:
	
	unsigned int	m_select_block;			//ѡ���block
	HBXDef::cuTable<_T>* m_table;				//���ݱ�����
public://������N?�õ����ݣ����ٵ؈}?ת���Զ��������D?��
	//unsigned int		_T;
	//unsigned int		m_demention;		//����?��ά��
	unsigned int			m_cordial_demtion[_T];	//ÿ��ά���ϵ��������
	spfloat*		m_cordials;			//ÿ��ά���ϵ�����̶�
	spfloat*		m_beg;				//���ݏ�?
public://���Ԧ����W?������
	unsigned int			m_lag_cordinate[_T];	//�弱?��λ��
	spfloat			m_lag[_T];				//����?��



public:
	spfloat get(spfloat* _lag/*, texture<spfloat> s_texcordials, texture<spfloat> s_texbeg*/)
	{
		//memcpy(m_lag, _t1, sizeof(spfloat)*_T);
		using namespace HBXDef;
		unsigned int		_temp_position[TPOW2(_T)];		//�ݹ�������ʱ����
		spfloat		_temp_reducdata[TPOW2(_T)];	//�ݹ����ݵ���ʱ����
		spfloat		_temp_proper[_T];		//�ݹ�ı���ϵ��
											//spfloat		m_lag[_T];
		ZeroMemory(_temp_position, TPOW2(_T) * sizeof(unsigned int));
//		ZeroMemory(_temp_reducdata, TPOW2(_T) * sizeof(spfloat));
		findcordi(_temp_position, _temp_proper, _lag);
		//�����ݹ����
		unsigned int tempindex = TPOW2(_T) / 2;
		for (unsigned int index = 0; index < TPOW2(_T); index++)
		{
			_temp_reducdata[index] = /*tex1Dfetch(s_texbeg, m_temp_position[index])*/m_beg[_temp_position[index]];
		}
		for (unsigned int index = _T - 1; index >0; index--)
		{
			for (unsigned int index2 = 0; index2 < tempindex; index2++)
			{
				_temp_reducdata[index2] = _temp_reducdata[index2 + tempindex] - _temp_proper[index] * (_temp_reducdata[index2 + tempindex] - _temp_reducdata[index2]);
			}
			tempindex = tempindex >> 1;
		}
		return _temp_reducdata[1] - _temp_proper[0] * (_temp_reducdata[1] - _temp_reducdata[0]);
	}
public:
	
	void findcordi(unsigned int m_temp_position[HBXDef::TPOW2(_T)], spfloat	m_temp_proper[_T], spfloat m_lag[_T])
	{
		using namespace HBXDef;
		unsigned int tempmul = 1;		//λ�õ������ځv
		unsigned int temppow = 1;		//λ�õ�������ƽ��
		unsigned int temp = 0;			//λ�õ������ڱ߽�
		unsigned int		_length = 0;
		spfloat*	_cordials = nullptr;
		unsigned int		_cordial = 0;

		for (unsigned int index1 = 0; index1 < _T; index1++)
		{

			_length = m_cordial_demtion[index1];		//Ѱ�ұ߽糤��

			_cordials = &m_cordials[temp];				//��ֵ����
			_cordial = m_lag_cordinate[index1];			//��¼��ֵ����λ��
														//�����ǰά���ڱ߽�ʱ


														/*�жϵ�ǰ��ֵ��������һ����ֵ���󷽻����ҷ�*/
			if (m_lag[index1] > _cordials[_cordial]) {//�ҷ�
				for (unsigned int index2 = _cordial; index2 < _length; ++index2) {
					if ((_length - 1) == index2)
					{
						//ֱ�ӽ���������Ϊ�ұ߽�
						m_temp_proper[index1] = 0;				//�ݹ�����
						m_lag_cordinate[index1] = _length - 1;
					}
					else {
						if (m_lag[index1] < _cordials[index2 + 1]) {
							m_lag_cordinate[index1] = index2 + 1;
							m_temp_proper[index1] = (_cordials[index2 + 1] - m_lag[index1]) / (_cordials[index2 + 1] - _cordials[index2]);
							break;
						}
					}
				}
			}
			else {//��
				for (unsigned int index2 = _cordial; index2 >= 0; --index2) {
					if (0 == index2)
					{
						m_temp_proper[index1] = 1;				//�ݹ�����
						m_lag_cordinate[index1] = 1;
						break;
					}
					else {
						if (m_lag[index1] > _cordials[index2 - 1]) {
							m_lag_cordinate[index1] = index2;
							m_temp_proper[index1] = (_cordials[index2] - m_lag[index1]) / (_cordials[index2] - _cordials[index2 - 1]);
							break;
						}
					}
				}
			}
			for (unsigned int index2 = 0; index2 < TPOW2(_T); index2++)
			{
				if (0 != (index2 / temppow) % 2)	//����
					m_temp_position[index2] += m_lag_cordinate[index1] * tempmul;
				else			//����
					m_temp_position[index2] += (m_lag_cordinate[index1] - 1) * tempmul;
			}
			tempmul *= _length;
			temp += _length;
			temppow = temppow << 1;
		}
	}
	
public://���Ԧ���
	

public:
//	unsigned int		m_temp_positionlength;	//�ݹ����ݳ���
	bool		m_isinitiall;
};

template __declspec(dllexport) class baselag<2>;
template __declspec(dllexport) class baselag<3>;


