#pragma once

#include <windows.h>
#include <chrono>

#define MAKE_TIMER(timer)			time_s timer
#define TIMER_START(timer)			timer.t = std::chrono::system_clock::now()
#define TIMER_STOP_DISPLAY(timer)	timer.ns = std::chrono::system_clock::now() - timer.t;\
									printf("%lld ns\n", timer.ns.count())

struct time_s
{
	std::chrono::system_clock::time_point t;
	std::chrono::nanoseconds ns;
};