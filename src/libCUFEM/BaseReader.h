#pragma once
#include <HbxDefMacro.h>
#include <HBXDefStruct.h>
#include <ExportDef.h>
#include "libCUFEM\ElemtProp.h"
//#include "libCUFEM\domain.h"
//#include <libCUFEM\BaseSlnRecord.h>
//#include <libCUFEM\InputRecord.h>


namespace HBXFEMDef
{
	class Domain;
	class BaseSlnRecord;
	class InputRecord;

	//所有读入数据类的基类，派生类可能包括XML解析器以及INP文件读取器，PW文件读取器。
	//输入记录的判别主要依据记录类型和组件数量。该类依赖boost。
	//应包括至少一个输入记录，且对应相应的解算信息。
	class CUFEM_API BaseReader
	{
	private:
	protected:
		std::string	m_SrcFileName;		//源文件名称
		std::string	m_path;	//源文件路径
		std::string	m_descriptor;

		// 迭代器
//		std::list< InputRecord* > ::iterator MyIter;
		/// All record types will be appended to this list, no split in terms of InputRecordType is implemented yet.
		std::list< InputRecord* > RecordList;

//		std::map<std::string, InputRecord* > RecordMap;
		InputRecord* m_Record;

		//使用pair是因为每个实例化对象的读取器只至多存储一个解决方案，即一个仿真实例
		//std::map< BaseSlnRecord, std::list< std::string, Domain* >, RecordLess > m_SlnInPair;
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
		virtual ~BaseReader() {};

		//设置索引路径，会根据当前索引路径下所有文件夹索引，搜索到的第一个当前文件读入
		virtual bool SetSourceFilePath(const std::string _SourceFile, std::string _savepath);

		//完成inp\xml\PW文件的导入，填充各容器并计算总刚矩阵的维度等参数
		virtual InputFileResult SetInputData() = 0;

		//依据文件名称输出记录类
		InputRecord* GetInputRecord() { return m_Record; }

		//强制断开所有数据连接
		virtual void terminate() = 0;
		

		void InsertInputRecord(InputRecord* _record) { this->RecordList.emplace_back(_record); };

		//获取输入文件路径
		const char* GetSourcePath() const { return this->m_path.c_str(); };
		//获取源文件名称
		const char* GetSourceName() const { return this->m_SrcFileName.c_str(); };
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