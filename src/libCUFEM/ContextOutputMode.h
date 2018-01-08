#pragma once

namespace HBXFEMDef
{
	//现在使用枚举值，届时是否可以考虑用xml导入规则？
	typedef enum ContextOutputMode
	{
		NOCONTEXT,
		ALWAYS,
		REQUIRED,
		USERDEFINED
	}ContextOutputMode_t;



}