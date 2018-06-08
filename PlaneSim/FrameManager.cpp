#include "FrameManager.h"
#include <iostream>
#include <Windows.h>
using namespace std;

FrameManager* FrameManager::instance;

FrameManager::FrameManager(){
	FPS = 0.0f;
	startTime = getTime()/1000;
	endTime = getTime()/1000;
	currentFPS = 0;
	elapsedTime = 0;
	previousTime = getTime();
}

//Had to be retrofitted to use clockspeed as a calculation due to lack of C++11 (chrono) support
double FrameManager::getTime(){
	static LARGE_INTEGER s_frequency;
	static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	if(s_use_qpc){
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000LL * now.QuadPart) / s_frequency.QuadPart;
		}
	else{
		return GetTickCount();
	}

}

double FrameManager::calculateFrameRate(){
	currentTime = getTime()/1000;
	deltaTime = getTime() - previousTime;
	elapsedTime += deltaTime;
	//std::cout << "The time is " << getTime() << std::endl;
	previousTime = getTime();
	endTime = currentTime;
	FPS++;
	if(elapsedTime > 1.0f){
		currentFPS = FPS;
		FPS = 0;
		elapsedTime = 0;
	}
	return currentFPS;
}


double FrameManager::getDeltaTime(){
	return deltaTime;
}