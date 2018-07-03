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
		//开始计时
		void startTimer(EngngTimerType_t t) { timers[t].start(); }
		//停止计时
		void stopTimer(EngngTimerType_t t) { timers[t].stop(); }
		//暂停计时
		void pauseTimer(EngngTimerType_t t) { timers[t].Pause(); }
		//恢复计时
		void resumeTimer(EngngTimerType_t t) { timers[t].Resume(); }
		
		//获取某一计时器的指针
		const HBXDef::MyTimer *getTimer(EngngTimerType_t t) { return timers + t; }


	};


}