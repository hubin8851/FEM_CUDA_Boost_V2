#pragma once

#include <HBXPreDef.h>
#include <MyTimer.h>
#include <libCUFEM\domain.h>
#include <libCUFEM\BaseSlnRecord.h>
#include <libCUFEM\BaseReader.h>
#include <libCUFEM\MetaStep.h>
#include <libCUFEM\solverEnum.h>
#include <libCUFEM\ContextOutputMode.h>
#include <libCUFEM\Dispatch.h>

#include <boost\bimap.hpp>
#include <boost\bimap\multiset_of.hpp>

namespace HBXFEMDef
{
//	using namespace HBXDef;

	class MetaStep;	//拓展时间步，用以流固耦合的交互，控制系统等
	class TimeStep;	//时间步
	class BaseReader;
	class BaseSlnRecord;	//解算类，包括了解算的所有信息，从文本中获的，传给Engng以便解算
	class AttriRecord;	//属性记录类
	class BaseNumMethod;//数值计算方法基类
	class EngngModelContext;//Engng的上下文用于两个Engng之间的数据交互
	class Domain;
	class MessageDispatcher;
	class HBXDef::MyTimer;	//暂定，自身创建的用于引擎的类，包括了总时间，计算时间，数据传输时间，内存拷贝时间等。如果有CUDA支持，可以使用cuda的高精度计时器。

	class BOOST_SYMBOL_EXPORT EngngModelContext
	{
	protected:
	public:
		EngngModelContext() {}
		~EngngModelContext() {}
	};

	//新增的容器，用以实现消息机制，源自WestWorldWithMessage，
	//http://blog.csdn.net/shanyongxu/article/details/48024271
	typedef std::map<int, BaseComponent*> EntityMap;

	using namespace boost::bimaps;
	struct SameComponent {};
	typedef bimap <
		int,
		multiset_of< tags::tagged<BaseComponent*, SameComponent> >
	> ComponentMap;	//该映射是域与组件之间的映射

	//解算步通过metastep类组织。metastep可以认为有着同样属性设置的解算步。
	//对每个元步，引擎模块通过元步引擎属性刷新属性，以便切换不同的时间增量或不同的控制方程。
	//如果元步未定义，引擎模块会定义一个统一的解算步。第一步通过initialFrom设定在计算中不被改变的属性
	//第二部是updateAttributes函数更新时变的属性。若元步未定义，选用引擎模型中导入的默认属性。
	class BOOST_SYMBOL_EXPORT Engng
	{
	public:
		enum EngngCommuType { _Default, _nLocal };
		
	protected:
		//receiver的ID
		int iMyID;
		//该类下域的数量
		size_t ndomains;
		//域的列表，域内只放相关数据
		std::vector< std::unique_ptr<Domain> > domainList;
		//解算所用的时间步
		size_t NumOfStep;
		//当前时间步下的等式数量
		size_t NumOfEq;
		//当前时间步或特定时间步的方程数量
		size_t NumOfPrescribedEq;
		//重置number标志位
		bool bReNum;
		//metastep的数量
		size_t nMetaStep;
		//metastep的列表
		std::vector< MetaStep > _metaStepList;
		//当前时间步
		std::shared_ptr<TimeStep> _CurtStep;
		//之前的时间步
		std::shared_ptr<TimeStep> _PreviousStep;

		//计时器
		HBXDef::MyTimer _timer;

		//模块内交互的消息管理器
		MessageDispatcher* MyDispatcher;

		//输出的流，目前是FILE，但是可以通过boost的log改变。20171026
		FILE* MyOutStream;

		//输出模型的域的上下文
		HBXFEMDef::ContextOutputMode_t eContextMode;

		//输出模块管理器


		//初始化模块管理器


		//域模式
		problemMode eMode;

		//域计算规模
		HBXFEMDef::problemScale_t eScale;
		
		//并行计算标志位
		bool bParralel;

		Engng* _MyMaster;

		//上下文，用以两个Engng之间的数据交互
		EngngModelContext*	MyContext;

		//非线性模型的计算模式（全程或刷新模式）
//		nlinearFormul_t nlinearFormulation;

		//创建的所有组件的实体存放map。
		EntityMap	m_EntityMap;
		//组件映射表。各种派生类由其基类类型索引
		ComponentMap	m_ComponentMap;
	public:
		//默认构造函数
//		Engng();

		//构造函数
		//@i:驱动i...
		//@master:master的Engng
		Engng( Engng * _master, int _id = classType::ENGNG);

		//拷贝构造函数
		//@_rhsEngng:拷贝原型
		Engng( const Engng& _rhsEngng ) = delete;

		//重载等于
		//@_rhsEngng:拷贝原型
		Engng &operator=( const Engng& _rhsEngng ) = delete;

		//@i:驱动i...
		//@_fileIn:输入的文件名，是否在Engng内内置xml解析器？！
		//@_master:master的Engng
		//Engng(int _i, Engng* _master = nullptr, const char* _fileIn = nullptr ) = delete;

		virtual ~Engng();
#pragma region 前处理相关
		//设置当前计算域模型
		void SetProblemMode(HBXFEMDef::problemMode_t _Mode) { this->eMode = _Mode; };

		//设置当前计算规模（tips：真的用得到么？？？）
		void SetProblemScale(HBXFEMDef::problemScale_t _scale) { this->eScale = _scale; };

		//设置当前并行模式
		void SetParrallelMode(bool _ParallelFlag) { this->bParralel = _ParallelFlag; };

		//设置重构建等式标志位为真
		virtual void SetRenumFlag() { this->bReNum = true; };
		//将重构建等式标志位复位为false
		virtual void ResetRenumFlag() { this->bReNum = false; };
		//询问当前标志位状态
		virtual bool RequiresRenum() { return this->bReNum; };

		//设置后处理
		void SetContextOutputMode(ContextOutputMode_t _context) { this->eContextMode = _context; };

		//返回给定的域
		Domain* GetDomain( unsigned int _nDomain );

		//返回当前所有域的数量
		size_t GetNumOfDomain() { return ndomains; };

		//获取消息管理器的指针
		MessageDispatcher* GetDispatcher() { return MyDispatcher; };

		FILE* GetOutStream();
		//重置当前上下文,主要两个作用，奇异回溯计算历史（对非线性），第二重置当前模型上下文以便超线程计算
		//重置当前所有的位移、速度、加速度信息，重置单元应力、应变和材料历史加载项
		virtual bool RestoreContext();
		//获的当前Engng的上下文
		virtual EngngModelContext* GetContext() { return this->MyContext; };

		//返回当前计算域模型
		HBXFEMDef::problemMode_t GetProblemMode() { return eMode; };

		//返回当前计算规模
		HBXFEMDef::problemScale_t GetProblemScale() { return eScale; };
#pragma endregion 前处理相关

		//根据问题描述初始化整个解算器,为初始化两步走的第一步。
		//完成输出文件流的打开，实例化其他模块的接收器等
		void InstanceInit( BaseReader* _dr, const char *_outputFileName);

		//实例化所有的元步属性
		virtual UserStatusError_t InstanceMetaStep( BaseReader* _dr );
		//如果nmsteps为0则实例化默认元步
		virtual UserStatusError_t InstanceDefaultMetaStep();

		//对所有的域过初始化，即初始化的第二步
		void postInit();

#pragma region 计算相关函数

		//计算,最主要的函数
		virtual void Solve();

		//计算当前步
		virtual void SolveAt( TimeStep* _ts );

		//刷新传入的元步内的所有属性
		virtual void UpdateMStepAttr(MetaStep* _MStep);

		//强制重构等式
		virtual int RenumEquation() { return 0; };

		//在完成计算步后完成内部数据的更新（例如所有的数据在之前的解算中需要更新）。单元内的积分点和材料属性同样需要更新。
		virtual void UpdataSelf(TimeStep* _ts);

		//结束时间步。默认情况下调用print函数输出至给定的流
		virtual void Terminate( TimeStep* _ts );

		//结束计算
		virtual void TerminateCal();

		void RegistEntity( int _process ,BaseComponent* pEntity );
		EntityMap* GetEntity();
		BaseComponent* GetEntity(const char* _name) const;
		BaseComponent* GetEntityFromID( int _id) const;
		void RemoveEntity(BaseComponent* pEntity);

#pragma endregion 计算相关函数

		//确定是否使用并行算法，布尔值
		bool isParallel() const { return ( false != bParralel ); }

		//保存特定步的上下文
		void SaveStepContext( TimeStep* _ts );

//		virtual void SetCurrentStep(TimeStep* _stp);
		//获得当前时间步
		TimeStep* GetCurrentStep();

		//返回之前的时间步
		TimeStep* GetPreviousStep();

		//返回第i步的metastep
		MetaStep* GetMetaStep( size_t _i );

		//返回所有的时间步
		size_t GetNumOfStep() { if (_MyMaster) { return _MyMaster->GetNumOfStep(); } else { return NumOfStep; } };

		//返回当前计算步的数值计算方法,在基类中和部分派生类中为nullptr，
		//因为在这个分析过程中计算方法不变。
		//但是在部分分析中，计算方法在某些步内需发生变化，故有此函数
		virtual BaseNumMethod* GetNumericalMethod(MetaStep* _MStep) { return nullptr; };

		//完成给定稀疏矩阵的组装
		//@_result:输出，给定形式的稀疏矩阵
		//@_Step: 给定的时间步
		//@_domain: 给定的域

		//获得当前类名
		virtual const char* GetClassName() const { return "Engng"; };

		//获取当前类ID
		virtual classType GetClassId() const { return classType::ENGNG; };

	};

	
	//template<typename _T> Engng* EngngCreator() { return new T(); }
	//用以在classfactory中注册
	template<typename T> Engng* EngngCreator(Engng * _master, int _id) { return ( new T( _master, _id) ); }
}