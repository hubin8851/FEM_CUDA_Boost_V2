#pragma once
#include <HBXPreDef.h>

#include "libCUFEM\ElementProp.h"
#include "libCUFEM\domain.h"
#include "libCUFEM\BaseSlnRecord.h"

namespace HBXFEMDef
{
	class Domain;
	class BaseSlnRecord;

	//���ж���������Ļ��࣬��������ܰ���XML�������Լ�INP�ļ���ȡ����PW�ļ���ȡ����
	//�����¼���б���Ҫ���ݼ�¼���ͺ������������������boost��
	class BOOST_SYMBOL_EXPORT BaseReader
	{
	private:
	protected:
		std::string	m_SrcFileName;		//Դ�ļ�����
		boost::filesystem::path	m_path;	//Դ�ļ�·��
		std::string	m_descriptor;

		_SlnPropInpair m_SlnInPair;	//ʹ��pair����Ϊÿ��ʵ��������Ķ�ȡ��ֻ����洢һ�������������һ������ʵ��
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
		virtual bool SetSourceFilePath(const std::string _SourceFile, boost::filesystem::path _savepath);

		//���inp�ļ��ĵ��룬���������������ܸվ����ά�ȵȲ���
		virtual bool SetInputData() = 0;

		//ǿ�ƶϿ�������������
		virtual void terminate() = 0;
		

		//��ȡ�����ļ�·��
		std::string GetSourcePath() const { return this->m_path.string(); };
		//��ȡԴ�ļ�����
		std::string GetSourceName() const { return this->m_SrcFileName; };
//		virtual const char* GetSourceName() const = 0;

		//����ļ�����
		std::string GetDescriptor() const { return this->m_descriptor; };
		
		//��ȡ����map����
		_SlnPropInpair*	GetSlnInpair() { return &m_SlnInPair; };

		//��ȡ��������
		BaseSlnRecord*	GetSlnRecord() { return &m_SlnInPair.first; };

		//�������
		//@_errstr:
		//@proc:
		//@kwd:
		//@_file:
		//@_line:
		void export_error(const char* _errstr, const char* proc, const char* kwd, const char* _file, int _line);

	};

	//typedef BaseReader* (*Func)();

	template<typename _T> BaseReader* ReaderCreator() { return new T() };
	template<typename _T> BaseReader* ReaderCreator(int _n, Domain *_dm) {return new T(_n,_dm)};


	//extern "C" BOOST_SYMBOL_EXPORT BaseReader CBaseReader;
// 	BOOST_DLL_ALIAS(
// 		HBXFEMDef::BaseReader,
// 		MyBaseReader
// 	)


}