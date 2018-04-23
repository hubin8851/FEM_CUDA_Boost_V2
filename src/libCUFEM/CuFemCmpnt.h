#pragma once
#include <ExportDef.h>
#include <string>




namespace HBXFEMDef
{
	class Domain;



	//所有组件的基类，提供最基础的接口，定义属性和方法的通用接口。
	//其一number用以从数据文件中读取数据
	//其二domain用以表征当前计算域以便同其他模块进行数据交互
	class CUFEM_API CuFemComponent
	{
	private:
	protected:
		int number;//组件编号
		Domain *MyDomain;	//用以表征当前计算域以便同其他模块进行数据交互
	public:
		//构造函数
		CuFemComponent(int n, Domain* _d) :number(n), MyDomain(_d) {};
		//析构函数
		~CuFemComponent() {};

		void ResetNumber(int _n) { this->number = _n; };
		void ResetDomain(Domain* _dm) { this->MyDomain = _dm; };

		virtual const char* GetClassName() const = 0;
		
		virtual Domain* GetDomain() const { return this->MyDomain; };

		std::string errorInfo(const char* func) const;
	};

}
