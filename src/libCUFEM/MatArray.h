#pragma once

#include <HBXPreDef.h>
#include <vector>


namespace HBXFEMDef
{
	//���Բ��м���ľ�����ʽ�洢�����ݣ���Ҫ���ڵ�Ԫ�ڵ�ŵļ�¼
	//���ںϲ����ʣ��ʽ��������е���ʽ�洢���ڳ�ʼ���ڵ�ʱ����֪����ǰ��Ԫ�Ľڵ�������
	//������push_back����
	template<typename _T>
	class  MatArray
	{
	private:
		unsigned int m_dim;	//��ǰ���ݵ�ά��
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
			Assert(m_dim == _DataIn.size());//�ж�����ά���Ƿ����
			for (auto i = 0; i < m_dim; i++)
			{
				_MyData[i].push_back(_DataIn[i]);
			}
		}

		void emplace_back(std::vector<_T> _DataIn)
		{
			Assert(m_dim == _DataIn.size());//�ж�����ά���Ƿ����
			for (auto i = 0; i < m_dim; i++)
			{
				_MyData[i].emplace_back(_DataIn[i]);
			}
		};
	};

	template class BOOST_SYMBOL_EXPORT MatArray<double>;
	template class BOOST_SYMBOL_EXPORT MatArray<float>;

}