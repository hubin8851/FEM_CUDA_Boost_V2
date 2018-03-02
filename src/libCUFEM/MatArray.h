#pragma once

#include <HBXPreDef.h>
#include <vector>


namespace HBXFEMDef
{
	//用以并行计算的矩阵形式存储的数据，主要便于单元节点号的记录
	//便于合并访问，故将数据以列的形式存储，在初始化节点时仅需知道当前单元的节点数即可
	//重载了push_back函数
	template<typename _T>
	class  MatArray
	{
	private:
		unsigned int m_dim;	//当前数据的维度
		std::vector< std::vector<_T> > _MyData;
	protected:

	public:
		MatArray(unsigned int _dim) :m_dim(_dim) 
		{ 
			_MyData.resize(m_dim); 
		};
		~MatArray() {};

		void push_back(std::vector<_T> _DataIn)
		{
			Assert(m_dim == _DataIn.size());//判断两者维度是否相等
			for (auto i = 0; i < m_dim; i++)
			{
				_MyData[i].push_back(_DataIn[i]);
			}
		}

		void emplace_back(std::vector<_T> _DataIn)
		{
			Assert(m_dim == _DataIn.size());//判断两者维度是否相等
			for (auto i = 0; i < m_dim; i++)
			{
				_MyData[i].emplace_back(_DataIn[i]);
			}
		};
	};

	template class BOOST_SYMBOL_EXPORT MatArray<double>;
	template class BOOST_SYMBOL_EXPORT MatArray<float>;

}