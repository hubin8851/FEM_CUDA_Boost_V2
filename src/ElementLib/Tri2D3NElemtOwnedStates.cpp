#include "Tri2D3NElemtOwnedStates.h"
#include <iostream>

namespace HBXFEMDef
{
	Tri2D3NReadyToCal* Tri2D3NReadyToCal::Instance()
	{
		static Tri2D3NReadyToCal instance;
		return &instance;
	}

	void Tri2D3NReadyToCal::Enter(Tri2D3NElemt * _ele)
	{
		std::cout<<"\n"<< GetClassFactory.
		if (0)
		{

		}
	}

	void Tri2D3NReadyToCal::Execute(Tri2D3NElemt * _ele)
	{
	}

	void Tri2D3NReadyToCal::Exit(Tri2D3NElemt * _ele)
	{
	}

	bool Tri2D3NReadyToCal::OnMessage(Tri2D3NElemt * _ele, const Telegram & msg)
	{
		return false;
	}

}


