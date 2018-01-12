#pragma once
#include <HBXPreDef.h>
#include "libCUFEM\ElementProp.h"
#include "libCUFEM\domain.h"
#include <libCUFEM\BaseSlnRecord.h>


namespace HBXFEMDef
{
	class Domain;
	class BaseSlnRecord;

	//所有读入数据类的基类，派生类可能包括XML解析器以及INP文件读取器，PW文件读取器。
	//输入记录的判别主要依据记录类型和组件数量。该类依赖boost。
	class BOOST_SYMBOL_EXPORT BaseReader
	{
	private:
	protected:
		std::string	m_SrcFileName;		//源文件名称
		boost::filesystem::path	m_path;	//源文件路径
		std::string	m_descriptor;

		//使用pair是因为每个实例化对象的读取器只至多存储一个解决方案，即一个仿真实例
//		std::map< BaseSlnRecord, std::pair< std::string, Domain* >, RecordLess > m_SlnInPair;
		BaseSlnRecord* m_SlnRecord;
	public:
		typedef enum InputRecorType
		{
			DomainRec,	//域记录
			EModeRec,	//模型记录
			MStepRec,	//元步记录
			BoundaryRec	//边界记录
		}InputRecorType_t;

		BaseReader(); 

//		BaseReader(HBXFEMDef::_EltPtyInMap & _EltPty) = delete;
//		BaseReader &operator = (const BaseReader& src) = delete;
		virtual ~BaseReader() {
		};

		//设置索引路径，会根据当前索引路径下所有文件夹索引，搜索到的第一个当前文件读入
		virtual bool SetSourceFilePath(const std::string _SourceFile, boost::filesystem::path _savepath);

		//完成inp文件的导入，填充各容器并计算总刚矩阵的维度等参数
		virtual bool SetInputData() = 0;

		//强制断开所有数据连接
		virtual void terminate() = 0;
		

		//获取输入文件路径
		std::string GetSourcePath() const { return this->m_path.string(); };
		//获取源文件名称
		std::string GetSourceName() const { return this->m_SrcFileName; };
//		virtual const char* GetSourceName() const = 0;

		//获的文件描述
		std::string GetDescriptor() const { return this->m_descriptor; };
		
		//获取整个map容器
//		_SlnPropInpair*	GetSlnInpair() { return &this->m_SlnInPair; };

		//获取解算属性
		BaseSlnRecord*	GetSlnRecord();

		//输出错误
		//@_errstr:
		//@proc:
		//@kwd:
		//@_file:
		//@_line:
//		void export_error(const char* _errstr, const char* proc, const char* kwd, const char* _file, int _line);

	};

	//typedef BaseReader* (*Func)();

	template<typename T> BaseReader* ReaderCreator() { return (new T()); };
	template<typename T> BaseReader* ReaderCreator(Domain *_dm, int _id) { return (new T(_dm, _id)); };


	//extern "C" BOOST_SYMBOL_EXPORT BaseReader CBaseReader;
// 	BOOST_DLL_ALIAS(
// 		HBXFEMDef::BaseReader,
// 		MyBaseReader
// 	)


}