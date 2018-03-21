#pragma once
#include <HBXPreDef.h>
#include <VtkPreDef.h>
#include "..\libCUFEM\domain.h"

class BOOST_SYMBOL_EXPORT BaseVtk
{
private:
	std::vector< vtkSmartPointer<vtkPolyDataMapper> > m_vMappers;
	std::vector< vtkSmartPointer<vtkRenderer> > m_vRenderers;
	std::vector< vtkSmartPointer<vtkActor> > m_vActor;
	std::vector< vtkSmartPointer<vtkPolyDataAlgorithm> > m_vGeometricObjectSources;
	std::vector< vtkSmartPointer<vtkTextMapper> >	m_vTextMapper;
public:
	BaseVtk();
	~BaseVtk();

	virtual UserStatusError_t Initial();

	virtual UserStatusError_t SetData( HBXFEMDef::Domain* _dm );

};
