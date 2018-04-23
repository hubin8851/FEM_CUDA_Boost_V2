#pragma once
#include <ExportDef.h>


namespace HBXFEMDef
{
	class Domain;

	class CUFEM_API BaseBoundary
	{
	public:
		enum CoordSys
		{
			Global,
			Local,
			UpdateGlobal
		};
	private:
	public:
		BaseBoundary(Domain* _dm, int _id) {};

	};


}
