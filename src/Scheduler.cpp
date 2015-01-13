/*
 * Scheduler.cpp
 *
 *  Created on: Jan 3, 2015
 *      Author: anagnoad
 */

#include "../include/Scheduler.h"


Scheduler::Scheduler() {
}

Scheduler::~Scheduler() {
}
void Scheduler::readPidsFromFile(){
	ifstream inputStream;
	inputStream.open(Utils::processesFile, ios::in);
	if (!inputStream)
		cerr<<"Could not open file"<<endl;
	else {
		// needs flushing the vector of pids
		Scheduler::backgroundPids->clear();
		std::string line = "";
		while (std::getline(inputStream, line))
		{
		    std::istringstream iss(line);
		    pid_t pid;
		    if (!(iss >> pid)) { break; } // error
		    backgroundPids->push_back(pid);
		}
	}
	inputStream.close();

}

void Scheduler::writePidsToFile(){
	fstream outputStream;
	outputStream.open(Utils::processesFile, ios_base::out|ios_base::app);
	if (!outputStream)
		cerr<<"Could not open file"<<endl;
	else {
		for(int i=0;i<backgroundPids->size();++i)
			outputStream << (*backgroundPids)[i] << endl;
	}
	outputStream.close();
}

// the main function of the scheduler
void Scheduler::timerHandler(int signal){
	// read list
	// wait pid with wnohang sto current process
	// if finished, delete from list
	// write list to file
	pid_t runningProcessPid = (*backgroundPids)[indexOfRunningProcess];
	readPidsFromFile();
	int runningProcessStatus;
	waitpid(runningProcessPid, &runningProcessStatus, WNOHANG);
	if WIFEXITED(runningProcessStatus) {
		backgroundPids->erase(backgroundPids->begin()+indexOfRunningProcess); // assuming it has not changed
		cerr << "Process " << runningProcessPid << "finished" << endl;
		writePidsToFile();
	}
	else {
		kill((*backgroundPids)[indexOfRunningProcess], SIGSTOP);
		indexOfRunningProcess = ++indexOfRunningProcess%backgroundPids->size();
	}
	kill((*backgroundPids)[indexOfRunningProcess], SIGCONT);
	cerr << "Changing to pid " << (*backgroundPids)[indexOfRunningProcess] << endl;
	// cout << "hello" << endl;
}

// initial call of the scheduler
void Scheduler::invoke(){
	 //Setup timer
	backgroundPids = new vector<pid_t>();
	indexOfRunningProcess = -1;
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
