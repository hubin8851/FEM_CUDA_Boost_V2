#pragma once

#include <HBXPreDef.h>
#include <vector>
#include <initializer_list>

namespace HBXFEMDef
{
	//用以并行计算的矩阵形式存储的数据，主要便于单元节点号的记录
	//便于合并访问，故将数据以列的形式存储，在初始化节点时仅需知道当前单元的节点数即可
	//重载了push_back函数
	template<typename _T>
	class  MatArray
	{
	private:
		typedef MatArray<_T> _SameClass;

		unsigned int m_dim;	//当前数据的维度
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
			Assert(m_dim == _DataIn.size());//判断两者维度是否相等
			for (auto i = 0; i < m_dim; i++)
			{
				_MyData[i].push_back(_DataIn[i]);
			}
		}

		void push_back(std::initializer_list<_T> _DataIn)
		{
			Assert(m_dim == _DataIn.size());//判断两者维度是否相等
			int i = 0;
			for (auto p = _DataIn.begin(); p != _DataIn.end(); p++, i++)   //使用迭代器访问参数
				_MyData[i].emplace_back(*p);
		};

		void emplace_back(std::vector<_T> _DataIn)
		{
			Assert(m_dim == _DataIn.size());//判断两者维度是否相等
			for (auto i = 0; i < m_dim; i++)
			{
				_MyData[i].emplace_back(_DataIn[i]);
			}
		};

		void emplace_back( std::initializer_list<_T> _DataIn )
		{
			Assert(m_dim == _DataIn.size());//判断两者维度是否相等
			int i = 0;
			for (auto p = _DataIn.begin(); p != _DataIn.end(); p++, i++)   //使用迭代器访问参数
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