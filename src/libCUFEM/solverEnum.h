#pragma once
#include <ExportDef.h>
#include <boost/dll.hpp>
namespace HBXFEMDef
{
	typedef enum CUFEM_API problemScale {
		macroScale,
		microScale
	}problemScale_t;


	typedef enum CUFEM_API problemMode {
		_processor,
		_postProcessor
	}problemMode_t;


	typedef enum CUFEM_API prallelMode {
		_serial,
		_openMP,
		_cuda,
	}prallelMode_t;
}

