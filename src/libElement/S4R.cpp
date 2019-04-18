#include <libElement\S4R.h>

namespace HBXFEMDef
{


	S4RElemt::S4RElemt(Engng * _engng, Domain * _dm, int _id) 
		:BaseElem(_engng, _dm, _id)
	{
		m_pStateMachine = new StateMachine<S4RElemt>(this);

//		m_pStateMachine->SetCurrentState(S4RElemt::Instance());
	}

	S4RElemt::~S4RElemt()
	{
	}

	void S4RElemt::Update()
	{
		m_pStateMachine->Update();
	}

	bool S4RElemt::HandleMessage(const Package & msg)
	{
		return false;
	}

	void S4RElemt::StiffMatCal()
	{

	}

	void S4RElemt::MassMatCal()
	{

	}
}

