#include "EngngMod.h"
#include <helper_cuda.h>
#include <libCUFEM\ExportModuleManager.h>
#include <libCUFEM\timestep.h>

namespace HBXFEMDef
{
// 	Engng::Engng()
// 	{
// 
//   }

	Engng::Engng(Engng * _master, int _id) :iMyID(_id), MyDispatcher(new MessageDispatcher(_master))
	{
		InitFlag = false;
		NumOfEq = 0;
		NumOfStep = 0;
		NumOfPrescribedEq = 0;
		bReNum = false;
		nMetaStep = 0;
		ndomains = 0;
		if (_master)
		{
			this->_MyContext = _master->GetContext();
		}
		else
		{
			this->_MyContext = new EngngModelContext();
		}

		_myExportModule = new ExportModuleManager(this);
	}

// 	Engng::Engng(int _i, const char * _fileIn, Engng * _master)
// 	{
// 
// 	}

	Engng::~Engng()
	{
		if ( nullptr != _MyContext )
		{
			delete _MyContext;
		}
		if ( nullptr != MyOutStream )
		{
			fclose( MyOutStream );
		}
		
	}

	void Engng::InitGPUs()
	{
		checkCudaErrors(cudaGetDeviceCount(&m_nGPUs));
		printf("Found %d CUDA capable GPUs\n", m_nGPUs);

		if (m_nGPUs > 32)
		{
			printf("simpleCallback only supports 32 GPU(s)\n");
		}

		for (int devid = 0; devid < m_nGPUs; devid++)
		{
			int SMversion;
			cudaDeviceProp deviceProp;
			cudaSetDevice(devid);
			cudaGetDeviceProperties(&deviceProp, devid);
			SMversion = deviceProp.major << 4 + deviceProp.minor;
			printf("GPU[%d] %s supports SM %d.%d", devid, deviceProp.name, deviceProp.major, deviceProp.minor);
			printf(", %s GPU Callback Functions\n", (SMversion >= 0x11) ? "capable" : "NOT capable");

			if (SMversion >= 0x11)
			{
				gpuInfo[m_MaxGPUs++] = devid;
			}
		}
		printf("%d GPUs available to run Callback Functions\n", m_MaxGPUs);
	}


	Domain* Engng::GetDomain(unsigned int _nDomain)
	{
		if ((_nDomain > 0) && (_nDomain <= this->domainList.size())) {
			return &this->domainList[_nDomain - 1];
		}
		else {
			std::cerr<<"Undefined domain"<<std::endl;
			return nullptr;
		}

	}

	int Engng::GetNumOfDomainEquations(int _id)
	{
		return this->domainList[_id].GetNumOfEquations();
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

	void Engng::RegistEntity(int _process, BaseComponent* pEntity)
	{
		m_EntityMap.insert(std::make_pair(_process, pEntity));
	}

	EntityMap* Engng::GetEntity()
	{
		return &this->m_EntityMap;
	}

	// 	BaseComponent * ClassFactory::GetEntity(unsigned int _id) const
	// 	{
	// 		auto tmpid = std::make_pair(m_EntityMap.size(), "");
	// 		EntityMap::const_iterator _iter = m_EntityMap.find(tmpid);
	// 		
	// 		return nullptr;
	// 	}

	BaseComponent * Engng::GetEntityFromID(int _id) const
	{
		EntityMap::const_iterator _iter = m_EntityMap.find(_id);

		Assert( (_iter != m_EntityMap.end()) && "<EntityManager::GetEntityFromID>: invalid ID");
//		_iter->second->ResetID(_iter->first + _iter->second->GetID());
		return _iter->second;

	}

	void Engng::RemoveEntity(BaseComponent * pEntity)
	{
		m_EntityMap.erase(m_EntityMap.find(pEntity->GetID()));
	}

	UserStatusError_t Engng::InstanceSelf( BaseReader* _dr, BaseSlnRecord* _sln, const char *_outputFileName )
	{
		bool _bFinish = true;

		if (_postProcessor == this->GetProblemMode())
		{
			//超线程问题的文件输出相关操作
		}

		if ( nullptr == (MyOutStream = fopen(_outputFileName, "w")) )
		{
			std::cerr << "打开并写入文件失败" << std::endl;
		}

		this->Instance_init();
		
		this->InstanceFrom(_sln);
		//初始化数据
		this->InstanceDomains(_dr);
		
		//初始化解算信息
		this->InstanceFrom(_dr->GetSlnRecord());

		//控制metastep
		if ( 0 == this->nMetaStep )
		{
			this->InstanceDefaultMetaStep();
		}
		else
		{
			this->InstanceMetaStep(_dr);
		}

		return UserStatusError_t::USER_STATUS_SUCCESS;
	}

	void Engng::Instance_init()
	{
#pragma region	清理domainlist相关 		
		domainList.clear();
		domainList.reserve(ndomains);//清理后将vector的尺寸重置为ndomains个大小，减少内存使用量
//		_DomainList.clear();
//		_DomainList.reserve(ndomains);
		for (size_t i = 0; 1 < ndomains; i++)
		{
			domainList[i] = std::move(Domain(i, 0, this));
		}
#pragma region	清理domainlist相关
	}

	void Engng::InstanceDomains( BaseReader* _dr )
	{
		for ( auto &domain: domainList )
		{
			domain.InstanceSelf(_dr->GetInputRecord());
		}

		this->postInit();

		return;
	}

	//处理输入文件,获取解算信息
	void Engng::InstanceFrom( BaseSlnRecord* _Slnr )
	{

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
		BaseSlnRecord* _tmpRecord = _dr->GetSlnRecord();
		for (size_t i = 0; i<this->nMetaStep; i++)
		{			
			_metaStepList[i].InitFrom(&_tmpRecord->m_vAtrriRecord[i]);
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
		// 设置metasetp的边界
		int istep = this->GetNumberOfFirstStep(true);
		for (auto &metaStep : _metaStepList) {
			istep = metaStep.SetStepBounds(istep);
		}
	}

	UserStatusError_t Engng::Assemble(SparseMat& answer, TimeStep* _tstep, Domain* _dm)
	{
		int _nElem = _dm->GetNumOfElem();
		this->_timer.resumeTimer(EngngTimer::EMT_ASSEMBLE);

		int _nElt = _dm->GetNumOfElem();
		for (int i=0; i<_nElem;i++)
		{
			BaseElem* _tElemt = _dm->GetElem(i);
		}

		this->_timer.pauseTimer(EngngTimer::EMT_ASSEMBLE);

		return UserStatusError_t::USER_STATUS_SUCCESS;
	}

	int Engng::GetNumberOfFirstStep(bool _hasReciver)
	{
		if ( _MyMaster && (!_hasReciver) )
		{
			return _MyMaster->GetNumberOfFirstStep(true);
		}
		else
		{
			return 1;
		}
	}

	void Engng::Solve()
	{
		//初始化操作
		unsigned int iStMeta = 1, iStStep = 1;	//起始元步和子步总数
		
		//若给定步数
		if (this->_CurtStep)
		{
			iStMeta = this->_CurtStep->GetMetaStepNum();
			iStStep = this->GetMetaStep(iStMeta)->GetRelativeStepNum( this->_CurtStep->GetMetaStepNum() );
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
				//开始计时
				this->_timer.startTimer(EngngTimer::EMT_SOLUTION);

				//判断当前子步是否有流-固模块之间耦合计算的需求，如果有需刷新当前子步内的数据
				if ( this->RequiresRenum() )
				{
					this->RenumEquation();
					std::cout << "等式重定义大小" << std::endl;
				}

				this->InitAt(this->GetCurrentStep());
				this->SolveAt(this->GetCurrentStep());
				this->UpdataSelf(this->GetCurrentStep());


				this->_timer.stopTimer(EngngTimer::EMT_SOLUTION);

#ifdef _DEBUG
				double _stepT = this->_timer.getTimer(EngngTimer::EMT_SOLUTION)->GetTime();
				std::cout << "当前步计算时间" << _stepT << std::endl;
#endif
			}
		}

		return;
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

	int Engng::RenumEquation()
	{
		this->NumOfEq = 0;
		this->NumOfPrescribedEq = 0;

		for (int i=0;i<this->ndomains;i++)
		{
			_DoaminNeqs[i] = 0;
			this->NumOfEq += this->RenumEquation(i);
		}

		for (int i = 0; i < ndomains; i++) {
			this->NumOfPrescribedEq += _DoaminPrescribedNeqs[i];
		}

		return this->NumOfEq;
	}

	int Engng::RenumEquation(int _id)
	{
		Domain *domain = this->GetDomain(_id);
		TimeStep *currStep = this->GetCurrentStep();

		this->_DoaminNeqs[_id] = 0;
		this->_DoaminPrescribedNeqs[_id] = 0;

		//未完待续，2018-7-9

		return _DoaminNeqs[_id];
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
			for ( auto &elt: domain.GetElemList())
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


