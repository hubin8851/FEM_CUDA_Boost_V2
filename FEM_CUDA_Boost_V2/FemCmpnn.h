#pragma once

//当前文档的智能指针皆为boost下的，为统一接口
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
		//虚析构函数
		virtual ~FemComponent() {};
		//运行时类型检查,
		virtual classType getClassID() const { return HBXDef::FemComponentClass };
		//运行时类型检查, 可以是用typeid——可用boost完成...20171019
		//virtual std::string getClassName() const { return ** };

		//设置当前域
		virtual void SetDomain(std::shared_ptr<_Domain<_T>> _din) { this->_myDomain = _din; };
		//返回当前所在域
		std::shared_ptr<_Domain<_T>> GetDomain() const { return _myDomain };

		//设置当前域
		void SetNum( size_t _num ) { this->_myNum = _num; };
		//返回当前所在域
		size_t GetNum() const { return _myNum };

		//**本地重新编号，为了便于并行计算的负载平衡。由多组件构成的有限元模型应允许重编号。
		virtual void updataLocalNum() {};

		//为了便于内部数据的检测。例如在计算前确认单元的材料属性
		virtual int checkConsistency() { return 1; };

		//在给定的时间步输出,输入参数应该包括输出的流以及输出的时间步
		virtual void printOutputAt() { };
		
		//使用boost的log。不直接使用这些方法，以期避免特定文件和行参数。
		//输出警告信息
		void warning(const char* file, int line, const char* format, ...) const;
		//输出错误信息并退出
		void error(const char* file, int line, const char* format, ...) const;

	};

	BOOST_TYPEOF_REGISTER_TYPE( HBXFEMDef::FemComponent<UserCalPrec> )	//向typeof库注册类
}
