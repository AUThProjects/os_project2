/*
 * Scheduler.cpp
 *
 *  Created on: Jan 3, 2015
 *      Author: anagnoad
 */

#include "../include/Scheduler.h"

vector<pid_t>* Scheduler::backgroundPids = new vector<pid_t>();
int Scheduler::indexOfRunningProcess = 0;
int *Scheduler::pipefds = new int [4];

Scheduler::Scheduler() {
	// static class, private and not really used
}

Scheduler::~Scheduler() {
	// static class, private and not really used
}

/**
 * Reads from pipe with main process (command prompt process) and
 * submits the command for scheduling (invocation and SIGSTOP).
 */
void Scheduler::submitCommandsFromPipe() {
	Command* current = nullptr;
	char readbuffer_bg[BG_BUFFER_SIZE];
	char readbuffer_cmd[BUFFER_SIZE];
	int nbytes = read(pipefds[2], readbuffer_bg, BG_BUFFER_SIZE);
	int bg_buffer = atoi(readbuffer_bg);
	while (bg_buffer>0) {
		int nbytes = read(pipefds[0], readbuffer_cmd, BUFFER_SIZE);
		string* toRead = new string(readbuffer_cmd);
		current = Command::readFromString(*toRead);
		--bg_buffer;
		/*
		 * After reading from pipe, we run the command and immediately send
		 * a SIGSTOP signal before handing it to the scheduler handling service.
		 */
		pid_t pidOfNewCommandInBackground = current->invoke();

		backgroundPids->push_back(pidOfNewCommandInBackground);
		kill(pidOfNewCommandInBackground, SIGSTOP);
		delete current; // not used after invokation as commmand object
//		|--------DEBUG--------|
//		cout << "New command stopped: " << pidOfNewCommandInBackground << endl;
	}
	// Update buffer
	sprintf(readbuffer_bg, "%d", bg_buffer);
	write(pipefds[3], readbuffer_bg, BG_BUFFER_SIZE);
}

/**
 *  the main service of the scheduler. Invoked by the itimer.
 */
void Scheduler::timerHandler(int signal) {
	// Read from pipe
	submitCommandsFromPipe();

	if (backgroundPids->empty()) { return; } // no need to run the scheduling process
	int indexOfProcessToRun = (indexOfRunningProcess+1)%backgroundPids->size(); // cyclic list

	pid_t runningProcessPid = (*backgroundPids)[indexOfRunningProcess];
	int runningProcessStatus;
	waitpid(runningProcessPid, &runningProcessStatus, WNOHANG);
	if (kill(runningProcessPid, 0) != 0) { // process has exited,remove from the list
		backgroundPids->erase(backgroundPids->begin() + indexOfRunningProcess); // assuming PID it has not changed
		if (backgroundPids->size() != 0) {indexOfProcessToRun = indexOfRunningProcess%(backgroundPids->size());}
//		|--------DEBUG--------|
//		cerr << "Exited: process " << runningProcessPid << endl;
	}
	else {
		kill((*backgroundPids)[indexOfRunningProcess], SIGSTOP);
	}
	kill((*backgroundPids)[indexOfProcessToRun], SIGCONT);

	// the process has been removed from the file, that's why we don't increment.
	indexOfRunningProcess = indexOfProcessToRun;
}

/**
 *  initial call of the scheduler after forking from command prompt
 */
void Scheduler::invoke(int *pipefd1) {
	// register exit signal listener
	signal(SIGINT, &Scheduler::finalize);
	// initialization of pipes and itimer call
	pipefds = pipefd1;
	close(pipefds[1]);
	//Setup timer
	struct itimerval timer={0};
	timer.it_value.tv_sec = 1;
	timer.it_interval.tv_sec = 1;
	setitimer(ITIMER_REAL, &timer, NULL);
	//Setup signal handler
	signal(SIGALRM, &timerHandler);
	while(true) {} // spinlock
}

/**
 * Called to stop running processes and exit when user exits the program.
 * Responds to signal from main process.
 */
void Scheduler::finalize(int signum) {
	for(int i=0;i<Scheduler::backgroundPids->size();++i) {
		kill(backgroundPids->at(i), SIGKILL);
	}
	delete Scheduler::backgroundPids;
	delete Scheduler::pipefds;
	exit(signum);
}
