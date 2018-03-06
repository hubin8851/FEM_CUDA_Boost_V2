#pragma once

#include <HBXPreDef.h>
#include <vector>
#include <initializer_list>

namespace HBXFEMDef
{
	//���Բ��м���ľ�����ʽ�洢�����ݣ���Ҫ���ڵ�Ԫ�ڵ�ŵļ�¼
	//���ںϲ����ʣ��ʽ��������е���ʽ�洢���ڳ�ʼ���ڵ�ʱ����֪����ǰ��Ԫ�Ľڵ�������
	//������push_back����
	template<typename _T>
	class  MatArray
	{
	private:
		typedef MatArray<_T> _SameClass;

		unsigned int m_dim;	//��ǰ���ݵ�ά��
		std::vector< std::vector<_T> > _MyData;
	protected:

	public:
		MatArray(unsigned int _dim) :m_dim(_dim) 
		{ 
			_MyData.resize(m_dim); 
		};
		~MatArray() {};

		void push_back(std::vector<_T>& _DataIn)
		{
			Assert(m_dim == _DataIn.size());//�ж�����ά���Ƿ����
			for (auto i = 0; i < m_dim; i++)
			{
				_MyData[i].push_back(_DataIn[i]);
			}
		}

		void push_back(std::initializer_list<_T> _DataIn)
		{
			Assert(m_dim == _DataIn.size());//�ж�����ά���Ƿ����
			int i = 0;
			for (auto p = _DataIn.begin(); p != _DataIn.end(); p++, i++)   //ʹ�õ��������ʲ���
				_MyData[i].emplace_back(*p);
		};

		void emplace_back(std::vector<_T> _DataIn)
		{
			Assert(m_dim == _DataIn.size());//�ж�����ά���Ƿ����
			for (auto i = 0; i < m_dim; i++)
			{
				_MyData[i].emplace_back(_DataIn[i]);
			}
		};

		void emplace_back( std::initializer_list<_T> _DataIn )
		{
			Assert(m_dim == _DataIn.size());//�ж�����ά���Ƿ����
			int i = 0;
			for (auto p = _DataIn.begin(); p != _DataIn.end(); p++, i++)   //ʹ�õ��������ʲ���
				_MyData[i].emplace_back(*p);
		};

		unsigned int GetDim()const { return this->m_dim; };

		_SameClass& operator = (const _SameClass& _rhs)
		{
			Assert(m_dim == _rhs.GetDim());
			for (int i =0; i<_MyData.size(); i++)
			{
				this->_MyData[i] = _rhs._MyData[i];
			}
			return *this;
		};

		_SameClass& operator = (const _SameClass&& _rhs)
		{
			Assert(m_dim == _rhs.GetDim());
			for (int i = 0; i < _MyData.size(); i++)
			{
				this->_MyData[i] = _rhs._MyData[i];
			}
			return *this;
		};
	};

	template class BOOST_SYMBOL_EXPORT MatArray<double>;
	template class BOOST_SYMBOL_EXPORT MatArray<float>;

}