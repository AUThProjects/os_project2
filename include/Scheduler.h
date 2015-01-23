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
#include <fstream>
#include <sstream>
#include "Utils.h"
#include "Command.h"

#define BG_BUFFER_SIZE 2
#define BUFFER_SIZE 512

using namespace std;

class Scheduler {

public:
	static void readPidsFromFile();
	static void writePidsToFile();
	static void submitCommandsFromPipe();
	static void invoke(int *pipefd);
	static int *pipefds;


private:
	Scheduler();
	~Scheduler();

	static vector<pid_t>* backgroundPids;
	static int indexOfRunningProcess;
	static void timerHandler(int signal);
};
//
//vector<pid_t>* Scheduler::backgroundPids;
//int Scheduler::indexOfRunningProcess;

#endif /* SCHEDULER_H_ */
