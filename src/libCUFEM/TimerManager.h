#pragma once
#include <MyTimer.h>
#include <ExportDef.h>

namespace HBXFEMDef
{
	class CUFEM_API EngngTimer
	{
	public:
		typedef enum EngngTimerType
		{
			EMT_ANALYSIS,
			EMT_DATATRANS,
			EMT_SOLUTION,
			EMT_ASSEMBLE,
			EMT_LOADBALACE,
			EMT_ALLTIME,
			EMT_LAST
		}EngngTimerType_t;

	private:
		HBXDef::MyTimer timers[EMT_LAST];

	public:
		EngngTimer() { }
		~EngngTimer() { }

		void initTimer(EngngTimerType_t t) { timers[t].InitTimer(); }
		//��ʼ��ʱ
		void startTimer(EngngTimerType_t t) { timers[t].start(); }
		//ֹͣ��ʱ
		void stopTimer(EngngTimerType_t t) { timers[t].stop(); }
		//��ͣ��ʱ
		void pauseTimer(EngngTimerType_t t) { timers[t].Pause(); }
		//�ָ���ʱ
		void resumeTimer(EngngTimerType_t t) { timers[t].Resume(); }
		
		//��ȡĳһ��ʱ����ָ��
		const HBXDef::MyTimer *getTimer(EngngTimerType_t t) { return timers + t; }


	};


}