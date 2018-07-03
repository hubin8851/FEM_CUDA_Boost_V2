#pragma once
#include <string.h>
#include <ExportDef.h>
#include <HbxDefMacro.h>
#include <libCUFEM\InputRecord.h>

namespace HBXFEMDef
{
	class Domain;
	class InputRecord;

	//������Ҫʵ�ֲ�����ص��㷨
	class CUFEM_API BaseMaterial
	{
	private:
	protected:
	public:
		std::string _myName;


		BaseMaterial(Domain* _dm, int _id) {};
		BaseMaterial(const BaseMaterial&) {};

		virtual ~BaseMaterial() {};

		//�����ں�ʱ��������Ϣ
		virtual InputFileResult_t InitFrom(InputRecord * _dr);
	};



	template<typename T> BaseMaterial* MaterialCreator() { return new T(); };
	template<typename T> BaseMaterial* MaterialCreator(Domain *_dm, int _id) { return new T(_dm, _id); };


}
