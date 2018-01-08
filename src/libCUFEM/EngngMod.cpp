#include "EngngMod.h"

namespace HBXFEMDef
{
// 	Engng::Engng()
// 	{
// 
//   }

	Engng::Engng(int _n, Engng * _master) :iMyID(_n)
	{
		NumOfEq = 0;
		NumOfStep = 0;
		NumOfPrescribedEq = 0;
		bReNum = false;
		nMetaStep = 0;
		ndomains = 0;
		if (_master)
		{
			this->MyContext = _master->GetContext();
		}
		else
		{
			this->MyContext = new EngngModelContext();
		}
	}

// 	Engng::Engng(int _i, const char * _fileIn, Engng * _master)
// 	{
// 
// 	}

	Engng::~Engng()
	{
		if ( nullptr != MyContext )
		{
			delete MyContext;
		}
		if ( nullptr != MyOutStream )
		{
			fclose( MyOutStream );
		}
		
	}

	Domain* Engng::GetDomain(unsigned int _nDomain)
	{
		if ((_nDomain > 0) && (_nDomain <= this->domainList.size())) {
			return this->domainList[_nDomain - 1].get();
		}
		else {
			std::cerr<<"Undefined domain"<<std::endl;
		}
		return nullptr;
	}

	FILE * Engng::GetOutStream()
	{
		if (!MyOutStream)
		{
			std::cerr << "�����δ����ȷ��ʼ��" << std::endl;
		}
		return MyOutStream;
	}

	bool Engng::RestoreContext()
	{
		return false;
	}

	void Engng::TerminateCal()
	{
	}

	void Engng::InstanceInit( BaseReader* _dr, const char *_outputFileName )
	{
		if (_postProcessor == this->GetProblemMode())
		{
			//���߳�������ļ������ز���
		}

		if ( nullptr == (MyOutStream = fopen(_outputFileName, "w")) )
		{
			std::cerr << "�򿪲�д���ļ�ʧ��" << std::endl;
		}

#pragma region	����domainlist��� 		
		domainList.clear();
		domainList.reserve(ndomains);//�����vector�ĳߴ�����Ϊndomains����С�������ڴ�ʹ����
		for (size_t i=0; 1<ndomains; i++)
		{
			domainList.emplace_back(new Domain(i, 0, this));
		}
#pragma region	����domainlist���

		if ( 0 == this->nMetaStep )
		{
			this->InstanceDefaultMetaStep();
		}
		else
		{
			this->InstanceMetaStep(_dr);
		}
	}

	UserStatusError_t Engng::InstanceMetaStep( BaseReader* _dr )
	{
		_metaStepList.clear();
		_metaStepList.reserve(nMetaStep);	//http://www.cplusplus.com/reference/vector/vector/reserve/
		//����Ԫ��
		for (size_t i=0; i<this->nMetaStep; i++)
		{
			_metaStepList.emplace_back(i, this);
		}
		//��ȡ����Ϣ
		BaseSlnRecord _tmpRecord = _dr->GetSlnInpair()->first;
		for (size_t i = 0; i<this->nMetaStep; i++)
		{			
			_metaStepList[i].InitFrom(&_tmpRecord.m_vAtrriRecord[i]);
		}
		return USER_STATUS_SUCCESS;
	}

	UserStatusError_t Engng::InstanceDefaultMetaStep()
	{
		if (0 == NumOfStep)
		{
			return UserStatusError_t::USER_STATUS_INVALID_VALUE;
		}
		this->nMetaStep = 1;
		_metaStepList.clear();
		_metaStepList.emplace_back(1, this, NumOfStep);
		std::cout << "���н��㲽����ĿΪ��" << NumOfStep << std::endl;
		return UserStatusError_t::USER_STATUS_SUCCESS;
	}

	void Engng::postInit()
	{
		// to be continue;

	}

	void Engng::Solve()
	{
		unsigned int iStMeta = 1, iStStep = 1;	//��ʼԪ�����Ӳ�����
		//��ʼ������
		if (this->_CurtStep)
		{

		}

		for (unsigned int iCurrMeta = iStMeta; iCurrMeta <= nMetaStep; iCurrMeta++, iStStep++ )	//Ԫ��ѭ��
		{
			MetaStep* currMStep = this->GetMetaStep(iCurrMeta);
			//ˢ��Ԫ������
			this->UpdateMStepAttr(currMStep);

			//��ȡԪ���ڵ��Ӳ��� 
			unsigned int iTermStep = currMStep->GetNumOfSteps();
			for (unsigned int currStp = iStStep; currStp <= iTermStep; currStp++)
			{
				//�жϵ�ǰ�Ӳ��Ƿ�����-��ģ��֮����ϼ���������������ˢ�µ�ǰ�Ӳ��ڵ�����
				if ( this->RequiresRenum() )
				{
					this->RenumEquation();
					std::cout << "��ʽ�ض����С" << std::endl;
				}

//				this->SetCurrentStep(currStp);
				this->SolveAt(this->GetCurrentStep());
				this->UpdataSelf(this->GetCurrentStep());
			}
		}

		return;
	}

	void Engng::SolveAt(TimeStep* _ts)
	{
	}

	void Engng::UpdateMStepAttr(MetaStep* _MStep)
	{
		MetaStep* _tmpMStep = this->GetMetaStep(_MStep->GetNum());
		AttriRecord* _tmpAttr = _MStep->GetAttrRecord();

		if (this->GetNumericalMethod(_MStep))
		{
			//to be continue,��������仯��δ���
		}
	}

	void Engng::Terminate(TimeStep *)
	{
	}

	void Engng::SaveStepContext(TimeStep *)
	{
	}

	void Engng::UpdataSelf(TimeStep* _ts)
	{
		//�Ե�ǰ������ÿ����ѭ������
		for ( auto &domain:domainList )
		{
			//ˢ��ÿ�����µ�����,to be continue
			for ( auto &dman: domain->GetBoundry())
			{

			}
		}
	}

	TimeStep * Engng::GetCurrentStep()
	{
		if (_MyMaster) { return _MyMaster->GetCurrentStep(); }
		return _CurtStep.get();
	}

	TimeStep * Engng::GetPreviousStep()
	{
		if (_MyMaster) { return _MyMaster->GetPreviousStep(); }
		return _PreviousStep.get();
	}

	MetaStep * Engng::GetMetaStep(size_t _i)
	{
		return &this->_metaStepList[_i];
	}




}


