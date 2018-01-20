#pragma once
#include <HBXPreDef.h>

namespace HBXFEMDef
{
	typedef const char *InputFieldType;


	class BOOST_SYMBOL_EXPORT InputRecord
	{
	public:
		//默认构造函数
		InputRecord() {};
		//拷贝构造函数
		InputRecord(const InputRecord&) {};
		//
		virtual ~InputRecord() {};

		InputRecord& operator = (const InputRecord&) { return *this; };

		virtual InputRecord* GiveCopy() = 0;

		virtual void SetField(double &_input, InputFieldType _id) = 0;

		virtual InputFileResult_t GiveField(double &answer, InputFieldType id) = 0;

		 
	};



}
