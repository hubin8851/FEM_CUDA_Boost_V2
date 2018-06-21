#include "BaseElement.h"
#include <libCUFEM\EngngMod.h>
#include <FSM\StateMachine.h>
#include "FSM\BaseElementOwnedStates.h"

namespace HBXFEMDef
{

	BaseElem::BaseElem(Engng* _engng, Domain* _dm, int _id):BaseComponent(_id), 
		MyEngng(_engng), MyDomain(_dm),
		_iNode(nullptr), _iMat(0), m_bFresh(true)
	{
		m_pStateMachine = new StateMachine<BaseElem>(this);

		m_pStateMachine->SetCurrentState(ReadyToCal::Instance());

		MyEngng->RegistEntity(_id, this);
	}

// 	BaseElem::BaseElem(HBXDef::UserCalPrec * _begin, size_t _lgth, int _id):BaseComponent(_id)
// 	{
// 		_iNode = new size_t(_lgth);
// 		memcpy_s(_iNode, _lgth, _begin, _lgth);
// 	}

	BaseElem::~BaseElem()
	{
		delete m_pStateMachine;
		if (_iNode)
		{
			delete[] _iNode;
			_iNode = nullptr;
		}
		_iMat = 0;
		MyEngng->RemoveEntity(this);
	}

	//初始化输入数据，获取材料的相关属性
	InputFileResult_t BaseElem::InitFrom(InputRecord * _dr)
	{


		return InputFileResult_t::IRRT_OK;
	}

	void BaseElem::Update()
	{
	//	m_pStateMachine->Update();
	}

	bool BaseElem::HandleMessage(const Package & msg)
	{
		return m_pStateMachine->HandleMessage(msg);
	}


}


