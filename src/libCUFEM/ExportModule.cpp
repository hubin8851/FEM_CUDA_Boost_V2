#include "ExportModule.h"

namespace HBXFEMDef
{
	ExportModule::ExportModule(Engng * _model, int _id)
	{
		this->_id = _id;
		this->_myModel = _model;
	}

	ExportModule::~ExportModule()
	{
		this->_myModel = nullptr;
	}



}

