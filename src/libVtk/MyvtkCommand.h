#pragma once
#include <VtkPreDef.h>

class vtkMyCallBack :
	public vtkCommand
{
public:
	vtkMyCallBack() {};
	~vtkMyCallBack() {};
	static vtkMyCallBack *New()
	{
		return new vtkMyCallBack;
	}

	void Execute(vtkObject* _caller, unsigned long _eventId, void* _callData) VTK_OVERRIDE
	{
		
	}
private:

};