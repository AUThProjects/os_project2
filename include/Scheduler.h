/*
 * Scheduler.h
 *
 *  Created on: Jan 3, 2015
 *      Author: anagnoad
 */


#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <vector>
#include <string>
#include <cstring>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>

using namespace std;

class Scheduler {

public:
	Scheduler(string processesFile);
	virtual ~Scheduler();
	void readPids();
	void switchExecution();
	void deletePid(pid_t pid);
	void invoke();


private:
	vector<int>* backgroundPids;
	string processesFile;
	static void timerHandler(int signal);
	static Scheduler instance;
};


#endif /* SCHEDULER_H_ */
