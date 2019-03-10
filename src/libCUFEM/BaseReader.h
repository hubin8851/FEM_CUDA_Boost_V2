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

	//���ж���������Ļ��࣬��������ܰ���XML�������Լ�INP�ļ���ȡ����PW�ļ���ȡ����
	//�����¼���б���Ҫ���ݼ�¼���ͺ������������������boost��
	//Ӧ��������һ�������¼���Ҷ�Ӧ��Ӧ�Ľ�����Ϣ��
	class CUFEM_API BaseReader
	{
	private:
	protected:
		std::string	m_SrcFileName;		//Դ�ļ�����
		std::string	m_path;	//Դ�ļ�·��
		std::string	m_descriptor;

		// ������
//		std::list< InputRecord* > ::iterator MyIter;
		/// All record types will be appended to this list, no split in terms of InputRecordType is implemented yet.
		std::list< InputRecord* > RecordList;

//		std::map<std::string, InputRecord* > RecordMap;
		InputRecord* m_Record;

		//ʹ��pair����Ϊÿ��ʵ��������Ķ�ȡ��ֻ����洢һ�������������һ������ʵ��
		//std::map< BaseSlnRecord, std::list< std::string, Domain* >, RecordLess > m_SlnInPair;
		BaseSlnRecord* m_SlnRecord;
	public:
		typedef enum InputRecorType
		{
			DomainRec,	//���¼
			EModeRec,	//ģ�ͼ�¼
			MStepRec,	//Ԫ����¼
			BoundaryRec	//�߽��¼
		}InputRecorType_t;

		BaseReader(); 

//		BaseReader(HBXFEMDef::_EltPtyInMap & _EltPty) = delete;
//		BaseReader &operator = (const BaseReader& src) = delete;
		virtual ~BaseReader() {};

		//��������·��������ݵ�ǰ����·���������ļ����������������ĵ�һ����ǰ�ļ�����
		virtual bool SetSourceFilePath(const std::string _SourceFile, std::string _savepath);

		//���inp\xml\PW�ļ��ĵ��룬���������������ܸվ����ά�ȵȲ���
		virtual InputFileResult SetInputData() = 0;

		//�����ļ����������¼��
		InputRecord* GetInputRecord() { return m_Record; }

		//ǿ�ƶϿ�������������
		virtual void terminate() = 0;
		

		void InsertInputRecord(InputRecord* _record) { this->RecordList.emplace_back(_record); };

		//��ȡ�����ļ�·��
		const char* GetSourcePath() const { return this->m_path.c_str(); };
		//��ȡԴ�ļ�����
		const char* GetSourceName() const { return this->m_SrcFileName.c_str(); };
//		virtual const char* GetSourceName() const = 0;

		//����ļ�����
		std::string GetDescriptor() const { return this->m_descriptor; };
		
		//��ȡ����map����
//		_SlnPropInpair*	GetSlnInpair() { return &this->m_SlnInPair; };

		//��ȡ��������
		BaseSlnRecord*	GetSlnRecord();

		//�������
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