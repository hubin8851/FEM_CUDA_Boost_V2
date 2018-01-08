#pragma once
#include <boost/dll.hpp>
namespace HBXFEMDef
{
	typedef enum BOOST_SYMBOL_EXPORT problemScale {
		macroScale,
		microScale
	}problemScale_t;


	typedef enum BOOST_SYMBOL_EXPORT problemMode {
		_processor,
		_postProcessor
	}problemMode_t;

}

