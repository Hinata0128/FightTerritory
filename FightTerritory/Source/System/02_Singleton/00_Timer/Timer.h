#pragma once
#include <chrono>

/********************************************************************
*	Timerクラス.
**/

class Timer final
{
public:
	~Timer();

	static Timer& GetInstance();

	void Update();

	//差分の時間計測.
	float DeltaTime() const;
	//ゲーム開始からの総経過時間.
	float ElapsedTime() const;

private:
	Timer();
	Timer(const Timer&) = delete;
	Timer& operator = (const Timer&) = delete;
private:
	float m_DeltaTime;
	std::chrono::high_resolution_clock::time_point m_lastTimer;

	float m_elapsedTime;
};