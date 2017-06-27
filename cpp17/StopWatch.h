// Author: Fabian Wahlster
// Copyright 2017
// fwahlster@tum.de

#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>
#include <iostream>
#include <string>

template <class ClockType = std::chrono::system_clock>
class StopWatch
{
public:
	StopWatch(const std::string& _sInfo) : m_Start(ClockType::now()), m_sInfo(_sInfo) {}
	~StopWatch() { const float fElapsed = Elapsed(); std::cout << m_sInfo << ": " << fElapsed << "s" << std::endl; }

	template <class Fp = float>
	inline Fp Elapsed() const { return std::chrono::duration<Fp>(ClockType::now() - m_Start).count(); }

private:
	std::chrono::time_point<ClockType> m_Start;
	const std::string m_sInfo;
};
#endif // !STOPWATCH_H
