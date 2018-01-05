#pragma once

//��ǰ�ĵ�������ָ���Ϊboost�µģ�Ϊͳһ�ӿ�
#include <hbxdef\BoostPreDef.h>
#include <hbxdef\HBXPreDef.h>
#include <hbxdef\HbxDefMacro.h>
#include <hbxdef\HBXDefStruct.h>

#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

namespace HBXFEMDef
{
	using namespace HBXDef;
	using namespace boost;

	extern UserCalPrec;
	


	template< typename _T >
	class FemComponent
	{
	private:
	protected:
		size_t _myNum;
		std::shared_ptr<_Domain<_T>> _myDomain;

	public:
		FemComponent() { };
		FemComponent( size_t _n, std::shared_ptr<_Domain<_T>> _domain ) :_myNum(_n), _myDomain(_domain)
		{

		}
		//����������
		virtual ~FemComponent() {};
		//����ʱ���ͼ��,
		virtual classType getClassID() const { return HBXDef::FemComponentClass };
		//����ʱ���ͼ��, ��������typeid��������boost���...20171019
		//virtual std::string getClassName() const { return ** };

		//���õ�ǰ��
		virtual void SetDomain(std::shared_ptr<_Domain<_T>> _din) { this->_myDomain = _din; };
		//���ص�ǰ������
		std::shared_ptr<_Domain<_T>> GetDomain() const { return _myDomain };

		//���õ�ǰ��
		void SetNum( size_t _num ) { this->_myNum = _num; };
		//���ص�ǰ������
		size_t GetNum() const { return _myNum };

		//**�������±�ţ�Ϊ�˱��ڲ��м���ĸ���ƽ�⡣�ɶ�������ɵ�����Ԫģ��Ӧ�����ر�š�
		virtual void updataLocalNum() {};

		//Ϊ�˱����ڲ����ݵļ�⡣�����ڼ���ǰȷ�ϵ�Ԫ�Ĳ�������
		virtual int checkConsistency() { return 1; };

		//�ڸ�����ʱ�䲽���,�������Ӧ�ð�����������Լ������ʱ�䲽
		virtual void printOutputAt() { };
		
		//ʹ��boost��log����ֱ��ʹ����Щ���������ڱ����ض��ļ����в�����
		//���������Ϣ
		void warning(const char* file, int line, const char* format, ...) const;
		//���������Ϣ���˳�
		void error(const char* file, int line, const char* format, ...) const;

	};

	BOOST_TYPEOF_REGISTER_TYPE( HBXFEMDef::FemComponent<UserCalPrec> )	//��typeof��ע����
}
