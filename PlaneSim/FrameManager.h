#ifndef FrameManager_H
#define FrameManager_H
#include <stddef.h>
#include <iostream>
#include <ctime>

//Written by Jack L. Clements
class FrameManager{
public:
	static FrameManager* getInstance(){
		if(instance == NULL){
			std::cout << "initialising" << std::endl;
			instance = new FrameManager;
		}
		return instance;		
	}; //create an instance using Singleton design pattern - there can only be one!
	double getTime();
	double calculateFrameRate();
	double getDeltaTime();

private:
	static FrameManager* instance;
	double FPS;
	double startTime;
	double endTime;
	double deltaTime; //Time in seconds that has elapsed since the last frame
	double currentTime; //Current time in seconds
	double currentFPS;
	time_t previousTime;
	double elapsedTime;
	//private constructor for single instance
	FrameManager();
	FrameManager(const FrameManager& fm); //copy constructor
	FrameManager& operator=(const FrameManager& fm); //assignment operator for copy constructor - saves time, even if not needed

};

#endif