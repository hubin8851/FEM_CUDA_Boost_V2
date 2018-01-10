#include "BaseElement.h"
#include "FSM\BaseElementOwnedStates.h"

namespace HBXFEMDef
{

	BaseElem::BaseElem(int _id, Domain * _dm):BaseComponent(_id),
		_iNode(nullptr), _iMat(0)
	{
		m_pStateMachine = new StateMachine<BaseElem>(this);

		m_pStateMachine->SetCurrentState(ReadyToCal::Instance());
	}

	BaseElem::BaseElem(HBXDef::UserCalPrec * _begin, size_t _lgth) :BaseComponent(0)
	{
		_iNode = new size_t(_lgth);
		memcpy_s(_iNode, _lgth, _begin, _lgth);
	}

	BaseElem::~BaseElem()
	{
		delete m_pStateMachine;
		if (_iNode)
		{
			delete[] _iNode;
			_iNode = nullptr;
		}
		_iMat = 0;
	}
	void BaseElem::Update()
	{
		m_pStateMachine->Update();
	}
	bool BaseElem::HandleMessage(const Telegram & msg)
	{
		return m_pStateMachine->HandleMessage(msg);
	}
}


