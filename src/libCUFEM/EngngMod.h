#pragma once

#include <HBXPreDef.h>
#include <MyTimer.h>
#include <libCUFEM\domain.h>
#include <libCUFEM\BaseSlnRecord.h>
#include <libCUFEM\BaseReader.h>
#include <libCUFEM\MetaStep.h>
#include <libCUFEM\solverEnum.h>
#include <libCUFEM\ContextOutputMode.h>
#include <libCUFEM\Dispatch.h>

#include <boost\bimap.hpp>
#include <boost\bimap\multiset_of.hpp>

namespace HBXFEMDef
{
//	using namespace HBXDef;

	class MetaStep;	//��չʱ�䲽������������ϵĽ���������ϵͳ��
	class TimeStep;	//ʱ�䲽
	class BaseReader;
	class BaseSlnRecord;	//�����࣬�����˽����������Ϣ�����ı��л�ģ�����Engng�Ա����
	class AttriRecord;	//���Լ�¼��
	class BaseNumMethod;//��ֵ���㷽������
	class EngngModelContext;//Engng����������������Engng֮������ݽ���
	class Domain;
	class MessageDispatcher;
	class HBXDef::MyTimer;	//�ݶ���������������������࣬��������ʱ�䣬����ʱ�䣬���ݴ���ʱ�䣬�ڴ濽��ʱ��ȡ������CUDA֧�֣�����ʹ��cuda�ĸ߾��ȼ�ʱ����

	class BOOST_SYMBOL_EXPORT EngngModelContext
	{
	protected:
	public:
		EngngModelContext() {}
		~EngngModelContext() {}
	};

	//����������������ʵ����Ϣ���ƣ�Դ��WestWorldWithMessage��
	//http://blog.csdn.net/shanyongxu/article/details/48024271
	typedef std::map<int, BaseComponent*> EntityMap;

	using namespace boost::bimaps;
	struct SameComponent {};
	typedef bimap <
		int,
		multiset_of< tags::tagged<BaseComponent*, SameComponent> >
	> ComponentMap;	//��ӳ�����������֮���ӳ��

	//���㲽ͨ��metastep����֯��metastep������Ϊ����ͬ���������õĽ��㲽��
	//��ÿ��Ԫ��������ģ��ͨ��Ԫ����������ˢ�����ԣ��Ա��л���ͬ��ʱ��������ͬ�Ŀ��Ʒ��̡�
	//���Ԫ��δ���壬����ģ��ᶨ��һ��ͳһ�Ľ��㲽����һ��ͨ��initialFrom�趨�ڼ����в����ı������
	//�ڶ�����updateAttributes��������ʱ������ԡ���Ԫ��δ���壬ѡ������ģ���е����Ĭ�����ԡ�
	class BOOST_SYMBOL_EXPORT Engng
	{
	public:
		enum EngngCommuType { _Default, _nLocal };
		
	protected:
		//receiver��ID
		int iMyID;
		//�������������
		size_t ndomains;
		//����б�����ֻ���������
		std::vector< std::unique_ptr<Domain> > domainList;
		//�������õ�ʱ�䲽
		size_t NumOfStep;
		//��ǰʱ�䲽�µĵ�ʽ����
		size_t NumOfEq;
		//��ǰʱ�䲽���ض�ʱ�䲽�ķ�������
		size_t NumOfPrescribedEq;
		//����number��־λ
		bool bReNum;
		//metastep������
		size_t nMetaStep;
		//metastep���б�
		std::vector< MetaStep > _metaStepList;
		//��ǰʱ�䲽
		std::shared_ptr<TimeStep> _CurtStep;
		//֮ǰ��ʱ�䲽
		std::shared_ptr<TimeStep> _PreviousStep;

		//��ʱ��
		HBXDef::MyTimer _timer;

		//ģ���ڽ�������Ϣ������
		MessageDispatcher* MyDispatcher;

		//���������Ŀǰ��FILE�����ǿ���ͨ��boost��log�ı䡣20171026
		FILE* MyOutStream;

		//���ģ�͵����������
		HBXFEMDef::ContextOutputMode_t eContextMode;

		//���ģ�������


		//��ʼ��ģ�������


		//��ģʽ
		problemMode eMode;

		//������ģ
		HBXFEMDef::problemScale_t eScale;
		
		//���м����־λ
		bool bParralel;

		Engng* _MyMaster;

		//�����ģ���������Engng֮������ݽ���
		EngngModelContext*	MyContext;

		//������ģ�͵ļ���ģʽ��ȫ�̻�ˢ��ģʽ��
//		nlinearFormul_t nlinearFormulation;

		//���������������ʵ����map��
		EntityMap	m_EntityMap;
		//���ӳ����������������������������
		ComponentMap	m_ComponentMap;
	public:
		//Ĭ�Ϲ��캯��
//		Engng();

		//���캯��
		//@i:����i...
		//@master:master��Engng
		Engng( Engng * _master, int _id = classType::ENGNG);

		//�������캯��
		//@_rhsEngng:����ԭ��
		Engng( const Engng& _rhsEngng ) = delete;

		//���ص���
		//@_rhsEngng:����ԭ��
		Engng &operator=( const Engng& _rhsEngng ) = delete;

		//@i:����i...
		//@_fileIn:������ļ������Ƿ���Engng������xml����������
		//@_master:master��Engng
		//Engng(int _i, Engng* _master = nullptr, const char* _fileIn = nullptr ) = delete;

		virtual ~Engng();
#pragma region ǰ�������
		//���õ�ǰ������ģ��
		void SetProblemMode(HBXFEMDef::problemMode_t _Mode) { this->eMode = _Mode; };

		//���õ�ǰ�����ģ��tips������õõ�ô��������
		void SetProblemScale(HBXFEMDef::problemScale_t _scale) { this->eScale = _scale; };

		//���õ�ǰ����ģʽ
		void SetParrallelMode(bool _ParallelFlag) { this->bParralel = _ParallelFlag; };

		//�����ع�����ʽ��־λΪ��
		virtual void SetRenumFlag() { this->bReNum = true; };
		//���ع�����ʽ��־λ��λΪfalse
		virtual void ResetRenumFlag() { this->bReNum = false; };
		//ѯ�ʵ�ǰ��־λ״̬
		virtual bool RequiresRenum() { return this->bReNum; };

		//���ú���
		void SetContextOutputMode(ContextOutputMode_t _context) { this->eContextMode = _context; };

		//���ظ�������
		Domain* GetDomain( unsigned int _nDomain );

		//���ص�ǰ�����������
		size_t GetNumOfDomain() { return ndomains; };

		//��ȡ��Ϣ��������ָ��
		MessageDispatcher* GetDispatcher() { return MyDispatcher; };

		FILE* GetOutStream();
		//���õ�ǰ������,��Ҫ�������ã�������ݼ�����ʷ���Է����ԣ����ڶ����õ�ǰģ���������Ա㳬�̼߳���
		//���õ�ǰ���е�λ�ơ��ٶȡ����ٶ���Ϣ�����õ�ԪӦ����Ӧ��Ͳ�����ʷ������
		virtual bool RestoreContext();
		//��ĵ�ǰEngng��������
		virtual EngngModelContext* GetContext() { return this->MyContext; };

		//���ص�ǰ������ģ��
		HBXFEMDef::problemMode_t GetProblemMode() { return eMode; };

		//���ص�ǰ�����ģ
		HBXFEMDef::problemScale_t GetProblemScale() { return eScale; };
#pragma endregion ǰ�������

		//��������������ʼ������������,Ϊ��ʼ�������ߵĵ�һ����
		//�������ļ����Ĵ򿪣�ʵ��������ģ��Ľ�������
		void InstanceInit( BaseReader* _dr, const char *_outputFileName);

		//ʵ�������е�Ԫ������
		virtual UserStatusError_t InstanceMetaStep( BaseReader* _dr );
		//���nmstepsΪ0��ʵ����Ĭ��Ԫ��
		virtual UserStatusError_t InstanceDefaultMetaStep();

		//�����е������ʼ��������ʼ���ĵڶ���
		void postInit();

#pragma region ������غ���

		//����,����Ҫ�ĺ���
		virtual void Solve();

		//���㵱ǰ��
		virtual void SolveAt( TimeStep* _ts );

		//ˢ�´����Ԫ���ڵ���������
		virtual void UpdateMStepAttr(MetaStep* _MStep);

		//ǿ���ع���ʽ
		virtual int RenumEquation() { return 0; };

		//����ɼ��㲽������ڲ����ݵĸ��£��������е�������֮ǰ�Ľ�������Ҫ���£�����Ԫ�ڵĻ��ֵ�Ͳ�������ͬ����Ҫ���¡�
		virtual void UpdataSelf(TimeStep* _ts);

		//����ʱ�䲽��Ĭ������µ���print�����������������
		virtual void Terminate( TimeStep* _ts );

		//��������
		virtual void TerminateCal();

		void RegistEntity( int _process ,BaseComponent* pEntity );
		EntityMap* GetEntity();
		BaseComponent* GetEntity(const char* _name) const;
		BaseComponent* GetEntityFromID( int _id) const;
		void RemoveEntity(BaseComponent* pEntity);

#pragma endregion ������غ���

		//ȷ���Ƿ�ʹ�ò����㷨������ֵ
		bool isParallel() const { return ( false != bParralel ); }

		//�����ض�����������
		void SaveStepContext( TimeStep* _ts );

//		virtual void SetCurrentStep(TimeStep* _stp);
		//��õ�ǰʱ�䲽
		TimeStep* GetCurrentStep();

		//����֮ǰ��ʱ�䲽
		TimeStep* GetPreviousStep();

		//���ص�i����metastep
		MetaStep* GetMetaStep( size_t _i );

		//�������е�ʱ�䲽
		size_t GetNumOfStep() { if (_MyMaster) { return _MyMaster->GetNumOfStep(); } else { return NumOfStep; } };

		//���ص�ǰ���㲽����ֵ���㷽��,�ڻ����кͲ�����������Ϊnullptr��
		//��Ϊ��������������м��㷽�����䡣
		//�����ڲ��ַ����У����㷽����ĳЩ�����跢���仯�����д˺���
		virtual BaseNumMethod* GetNumericalMethod(MetaStep* _MStep) { return nullptr; };

		//��ɸ���ϡ��������װ
		//@_result:�����������ʽ��ϡ�����
		//@_Step: ������ʱ�䲽
		//@_domain: ��������

		//��õ�ǰ����
		virtual const char* GetClassName() const { return "Engng"; };

		//��ȡ��ǰ��ID
		virtual classType GetClassId() const { return classType::ENGNG; };

	};

	
	//template<typename _T> Engng* EngngCreator() { return new T(); }
	//������classfactory��ע��
	template<typename T> Engng* EngngCreator(Engng * _master, int _id) { return ( new T( _master, _id) ); }
}