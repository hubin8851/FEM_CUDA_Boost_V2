#pragma once

#include <vector>
#include <initializer_list>
#include <ExportDef.h>
#include <HbxDefMacro.h>
#include <HBXDefStruct.h>
#include <Package.h>
#include <libCUFEM\AttriRecord.h>

namespace HBXFEMDef
{
	//节点索引矩阵
	//用以并行计算的矩阵形式存储的数据，主要便于单元节点号的记录
	//便于合并访问，故将数据以列的形式存储，在初始化节点时仅需知道当前单元的节点数即可
	//重载了push_back函数
	template<typename _T>
	class  NSortMat
	{
	private:
		typedef NSortMat<_T> _SameClass;

		unsigned int m_dim;	//当前数据的维度
		std::vector< std::vector<_T> > _MyData;
	protected:

	public:

		NSortMat(unsigned int _dim) :m_dim(_dim)
		{ 
			_MyData.resize(m_dim); 
		};

		size_t GetRowNum()
		{
			return this->_MyData[0].size();
		}

		size_t GetColNum()
		{
			return this->m_dim;
		}

		unsigned int GetDim()const { return this->m_dim; };

		//重载函数调用运算符(),返回二维矩阵中元素(_row, _col)在一维数组中的位置,作为左值被写时使用
		//第一个参数为列位置，即单元号，第二个参数为单元内节点编号
		_T& operator()(size_t _iSort, size_t _jNum)
		{
			if (_iSort < this->_MyData[0].size() && _jNum < this->m_dim)
			{
				return this->_MyData[_jNum][_iSort];
			}
			else
			{
				std::cerr << "索引越界" << std::endl;
				//这里要注意，没有返回值，如果索引越界可能会报错。
				_T ErrCode = INF;
				return ErrCode;
			}
		}


		//重载函数调用运算符(),返回二维矩阵中元素(_row, _col)在一维数组中的位置,作为右值被读时使用
		const _T operator()(size_t _iSort, size_t _jNum) const
		{
			Assert(_iSort < this->_MyData[0].size());//断定_row不超实际矩阵行值,Assert已经被重定义
			Assert(_jNum < this->m_dim);//断定_col不超实际矩阵行值,Assert已经被重定义
			
			return this->_MyData[_jNum][_iSort];
		}

		void push_back(const std::vector<_T>& _DataIn)
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

		_SameClass& operator = (const _SameClass& _rhs)
		{
			Assert(this->m_dim == _rhs.GetDim());
			for (int i =0; i<_MyData.size(); i++)
			{
				this->_MyData[i] = _rhs._MyData[i];
			}
			return *this;
		};
// 
// 		//转移赋值，C11新特性
//		//移动赋值运算符，但未显式声明赋值赋值运算符。参见下链接。
//		//https://docs.microsoft.com/zh-cn/cpp/error-messages/compiler-errors-1/compiler-error-c2280?f1url=https%3A%2F%2Fmsdn.microsoft.com%2Fquery%2Fdev15.query%3FappId%3DDev15IDEF1%26l%3DZH-CN%26k%3Dk(C2280)%26rd%3Dtrue#example-movable-deletes-implicit-copy
// 		_SameClass& operator = (const _SameClass&& _rhs)
// 		{
// 			Assert(m_dim == _rhs.GetDim());
// 			for (int i = 0; i < _MyData.size(); i++)
// 			{
// 				this->_MyData[i] = _rhs._MyData[i];
// 			}
// 			return *this;
// 		};
	};

	template class CUFEM_API NSortMat<double>;
	template class CUFEM_API NSortMat<float>;

}