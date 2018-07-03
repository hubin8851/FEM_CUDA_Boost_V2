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
	//�ڵ���������
	//���Բ��м���ľ�����ʽ�洢�����ݣ���Ҫ���ڵ�Ԫ�ڵ�ŵļ�¼
	//���ںϲ����ʣ��ʽ��������е���ʽ�洢���ڳ�ʼ���ڵ�ʱ����֪����ǰ��Ԫ�Ľڵ�������
	//������push_back����
	template<typename _T>
	class  NSortMat
	{
	private:
		typedef NSortMat<_T> _SameClass;

		unsigned int m_dim;	//��ǰ���ݵ�ά��
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

		//���غ������������(),���ض�ά������Ԫ��(_row, _col)��һά�����е�λ��,��Ϊ��ֵ��дʱʹ��
		//��һ������Ϊ��λ�ã�����Ԫ�ţ��ڶ�������Ϊ��Ԫ�ڽڵ���
		_T& operator()(size_t _iSort, size_t _jNum)
		{
			if (_iSort < this->_MyData[0].size() && _jNum < this->m_dim)
			{
				return this->_MyData[_jNum][_iSort];
			}
			else
			{
				std::cerr << "����Խ��" << std::endl;
				//����Ҫע�⣬û�з���ֵ���������Խ����ܻᱨ��
				_T ErrCode = INF;
				return ErrCode;
			}
		}


		//���غ������������(),���ض�ά������Ԫ��(_row, _col)��һά�����е�λ��,��Ϊ��ֵ����ʱʹ��
		const _T operator()(size_t _iSort, size_t _jNum) const
		{
			Assert(_iSort < this->_MyData[0].size());//�϶�_row����ʵ�ʾ�����ֵ,Assert�Ѿ����ض���
			Assert(_jNum < this->m_dim);//�϶�_col����ʵ�ʾ�����ֵ,Assert�Ѿ����ض���
			
			return this->_MyData[_jNum][_iSort];
		}

		void push_back(const std::vector<_T>& _DataIn)
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
// 		//ת�Ƹ�ֵ��C11������
//		//�ƶ���ֵ���������δ��ʽ������ֵ��ֵ��������μ������ӡ�
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