#pragma once

#include "FSM\State.h"
#include <Package.h>

namespace HBXFEMDef
{
	class Tri2D3NElemt;
	class Package;


	class Tri2D3NReadyToCal : public State<Tri2D3NElemt>
	{
	private:
		Tri2D3NReadyToCal() {}
		Tri2D3NReadyToCal(const Tri2D3NReadyToCal&);
		Tri2D3NReadyToCal& operator=(const Tri2D3NReadyToCal&);
	protected:
	public:
		static Tri2D3NReadyToCal* Instance();

		virtual void Enter(Tri2D3NElemt* _ele);

		virtual void Execute(Tri2D3NElemt* _ele);

		virtual void Exit(Tri2D3NElemt* _ele);

		virtual bool OnMessage(Tri2D3NElemt* _ele, const Package& msg);
	};


	class Tri2D3NFreshCal : public State<Tri2D3NElemt>
	{
	private:
		Tri2D3NFreshCal() {}
		Tri2D3NFreshCal(const Tri2D3NFreshCal&);
		Tri2D3NFreshCal& operator=(const Tri2D3NFreshCal&);
	protected:
	public:
		static Tri2D3NFreshCal* Instance();

		virtual void Enter(Tri2D3NElemt* _ele);

		virtual void Execute(Tri2D3NElemt* _ele);

		virtual void Exit(Tri2D3NElemt* _ele);

		virtual bool OnMessage(Tri2D3NElemt* _ele, const Package& msg);
	};


}