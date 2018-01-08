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
			std::cerr << "输出流未能正确初始化" << std::endl;
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
			//超线程问题的文件输出相关操作
		}

		if ( nullptr == (MyOutStream = fopen(_outputFileName, "w")) )
		{
			std::cerr << "打开并写入文件失败" << std::endl;
		}

#pragma region	清理domainlist相关 		
		domainList.clear();
		domainList.reserve(ndomains);//清理后将vector的尺寸重置为ndomains个大小，减少内存使用量
		for (size_t i=0; 1<ndomains; i++)
		{
			domainList.emplace_back(new Domain(i, 0, this));
		}
#pragma region	清理domainlist相关

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
		//创建元步
		for (size_t i=0; i<this->nMetaStep; i++)
		{
			_metaStepList.emplace_back(i, this);
		}
		//读取域信息
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
		std::cout << "所有解算步的数目为：" << NumOfStep << std::endl;
		return UserStatusError_t::USER_STATUS_SUCCESS;
	}

	void Engng::postInit()
	{
		// to be continue;

	}

	void Engng::Solve()
	{
		unsigned int iStMeta = 1, iStStep = 1;	//起始元步和子步总数
		//初始化操作
		if (this->_CurtStep)
		{

		}

		for (unsigned int iCurrMeta = iStMeta; iCurrMeta <= nMetaStep; iCurrMeta++, iStStep++ )	//元步循环
		{
			MetaStep* currMStep = this->GetMetaStep(iCurrMeta);
			//刷新元步属性
			this->UpdateMStepAttr(currMStep);

			//获取元步内的子步数 
			unsigned int iTermStep = currMStep->GetNumOfSteps();
			for (unsigned int currStp = iStStep; currStp <= iTermStep; currStp++)
			{
				//判断当前子步是否有流-固模块之间耦合计算的需求，如果有需刷新当前子步内的数据
				if ( this->RequiresRenum() )
				{
					this->RenumEquation();
					std::cout << "等式重定义大小" << std::endl;
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
			//to be continue,如果方法变化如何传参
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
		//对当前引擎下每个域循环遍历
		for ( auto &domain:domainList )
		{
			//刷新每个域下的属性,to be continue
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


