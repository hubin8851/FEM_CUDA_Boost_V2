#include <libElement\Tri2D3NElemt.h>
#include <libElement\Tri2D3NElemtOwnedStates.h>

namespace HBXFEMDef
{


	Tri2D3NElemt::Tri2D3NElemt(Engng* _engng, Domain* _dm,int _id ) :BaseElem(_engng, _dm, _id )
	{
		m_pStateMachine = new StateMachine<Tri2D3NElemt>(this);

		m_pStateMachine->SetCurrentState(Tri2D3NReadyToCal::Instance());
	}


	Tri2D3NElemt::~Tri2D3NElemt()
	{

	}

	void Tri2D3NElemt::Update()
	{
		m_pStateMachine->Update();
 	}

	bool Tri2D3NElemt::HandleMessage(const Package & msg)
	{
		return false;
	}

	void Tri2D3NElemt::StiffMatCal()
	{
	}

	void Tri2D3NElemt::MassMatCal()
	{

	}


}

