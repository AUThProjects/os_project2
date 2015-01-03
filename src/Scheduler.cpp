/*
 * Scheduler.cpp
 *
 *  Created on: Jan 3, 2015
 *      Author: anagnoad
 */

#include "../include/Scheduler.h"


Scheduler::Scheduler(string processesFile) {
	this->processesFile = processesFile;
	this->backgroundPids = new vector<int>();
}

Scheduler::~Scheduler() {
	// TODO Auto-generated destructor stub
}
void Scheduler::readPids(){

}

void Scheduler::switchExecution(){

}
void Scheduler::deletePid(pid_t pid){


}
void Scheduler::timerHandler(int signal){
	//
	cout << "hello" << endl;
}
void Scheduler::invoke(){
	 //Setup timer
	struct itimerval timer={0};
	timer.it_value.tv_sec = 1;
	timer.it_interval.tv_sec = 1;
	setitimer(ITIMER_REAL, &timer, NULL);
	//Setup signal handler
	signal(SIGALRM, &timerHandler);
	int sleepLeft=sleep(10);
	while(sleepLeft) sleepLeft=sleep(sleepLeft);
	//return 0;
}
