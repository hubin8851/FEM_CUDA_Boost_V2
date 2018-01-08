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
		unsigned int m_begin;	//��ʼ��Ԫ��
		unsigned int m_end;		//������Ԫ��
		//Ŀǰinp�ļ��ڶ�Ӧ��generate��δ��ֵ
		bool _ElOrN;	//�ڵ㻹�ǵ�Ԫ���ϱ�־λ��false��ʾ�ڵ㣬true��ʾ��Ԫ

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
				//�����ж�����Ϊ
				if (1 == _vSort.size())
				{
					m_end = m_begin;
				}
				else m_end = _vSort[1];
			}
			else
			{
				std::cerr << "�����ָ���ʼ��ʧ��" << std::endl;
			}

		};
		~Set() 
		{
			_vSort.clear();
		};

	};


}
