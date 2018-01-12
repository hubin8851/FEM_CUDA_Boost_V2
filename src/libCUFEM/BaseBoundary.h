#pragma once
#include <boost/dll.hpp>
#include <ExportDef.h>


namespace HBXFEMDef
{
	class Domain;

	class BOOST_SYMBOL_EXPORT BaseBoundary
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
