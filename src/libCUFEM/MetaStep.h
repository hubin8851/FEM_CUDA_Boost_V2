#pragma once

#include <ExportDef.h>
#include <HbxDefMacro.h>
#include <boost\dll.hpp>
#include <libCUFEM\AttriRecord.h>

namespace HBXFEMDef
{
	class Engng;
	class BaseReader;
	class AttriRecord;

	//所谓metastep，就是解算步的一个序列。metastep是为了在工程中区分不同时步段的常规属性
	//例如，metastep代表一个解算步序列，基于模型的常规属性。为存储这些相关属性，metastep记录用以被读取
	class CUFEM_API MetaStep
	{
	private:
	protected:
		HBXDef::MyCounter _iNum;	//当前元步ID
		Engng*	_EngBelong;

		AttriRecord		_AttrRecord;	//当前元步属性表

		HBXDef::MyCounter NumOfSteps;	//当前元步内子步数

		//需要接收的其他解算模块的起始步数索引
		int StartIdx;
	public:
		MetaStep( HBXDef::MyCounter _n, Engng* _belong );
		MetaStep(HBXDef::MyCounter _n, Engng* _belong, HBXDef::MyCounter _nStp );

		UserStatusError_t InitFrom(AttriRecord * _attrRec);

		~MetaStep();

		//获取当前元步ID
		HBXDef::MyCounter GetNum() const { return _iNum; };

		//设置给定步数至其他解算器
		//实质是第若干步输出的偏置量
		int SetStepBounds( int _StartStepNum );

		//设置当前元步下的子步数
		void SetNumOfSteps( HBXDef::MyCounter _newNum);
		//获的当前元步下的子步数
		HBXDef::MyCounter GetNumOfSteps() const { return this->NumOfSteps; };

		//获取相对步数
		int GetRelativeStepNum(int _nStep) { return (_nStep - StartIdx + 1); };

		//返回当前元步的属性表
		AttriRecord* GetAttrRecord() { return &this->_AttrRecord; };

	};

}
