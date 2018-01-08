#pragma once
#include <iostream>
#include <vector>
#include <boost/dll.hpp>
#include <HbxDefMacro.h>

namespace HBXFEMDef
{

	class BOOST_SYMBOL_EXPORT Set
	{
	public:
		std::vector<unsigned int>	_vSort;
		unsigned int m_begin;	//起始单元号
		unsigned int m_end;		//结束单元号
		//目前inp文件内对应的generate尚未赋值
		bool _ElOrN;	//节点还是单元集合标志位，false表示节点，true表示单元

	public:
		Set(unsigned int* _begin, size_t _lgth, bool _whichset = 1):_ElOrN(_whichset)
		{
			if (nullptr != _begin)
			{
				_vSort.clear();
				for (size_t i = 0; i < _lgth; i++)
				{
					_vSort.emplace_back(*_begin);
				}
				m_begin = _vSort[0];
				//以下判断是作为
				if (1 == _vSort.size())
				{
					m_end = m_begin;
				}
				else m_end = _vSort[1];
			}
			else
			{
				std::cerr << "传入空指针初始化失败" << std::endl;
			}

		};
		~Set() 
		{
			_vSort.clear();
		};

	};


}
