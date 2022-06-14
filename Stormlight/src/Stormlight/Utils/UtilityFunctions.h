#pragma once
#include <random>

namespace Stormlight {

	static float GetRandomFloat(float min = 0.0f, float max = 1.0f)
	{
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_real_distribution<> dist(min, max);

		return static_cast<float>(dist(rng));
	}

	static int GetRandomInt(int min, int max)
	{
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);

		return dist(rng);
	}

	class Timer 
	{
	public:
		Timer(float timerDuration, float elapsedTimerDuration) : m_TimerDuration(timerDuration), m_ElapsedTimerDuration(elapsedTimerDuration) {}

		bool OnUpdate(Timestep ts) 
		{
			if (m_StartTimer)
			{
				m_ElapsedTimerDuration += ts;
				return m_ElapsedTimerDuration > m_TimerDuration ? true : false;
			}
			else
				false;
			
			return false;
		}

		inline void SetTimer(float timerDuration, float elapsedTimerDuration)
		{ m_TimerDuration = timerDuration; m_ElapsedTimerDuration = elapsedTimerDuration; }
		inline void SetTimerDuration(float timerDuration) { m_TimerDuration = timerDuration; }
		inline void ResetTimer()   { m_ElapsedTimerDuration = 0.0f; m_StartTimer = false; }
		inline void RestartTimer() { m_ElapsedTimerDuration = 0.0f; m_StartTimer = true; }
		inline void StartTimer()   { m_StartTimer = true; }
		inline bool IsTimerTicking() { return m_StartTimer; }

		float GetElapsedTimerDuration() { return m_ElapsedTimerDuration; }
		float GetRemainingTimerDuration() { return m_TimerDuration - m_ElapsedTimerDuration; }
	private:
		float m_TimerDuration;
		float m_ElapsedTimerDuration;
		bool m_StartTimer = false;
	};
}