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

}

Scheduler::~Scheduler() {
}
//void Scheduler::readPidsFromFile(){
//	ifstream inputStream;
//	inputStream.open(Utils::processesFile, ios::in);
//	if (!inputStream)
//		cerr<<"Could not open file"<<endl;
//	else {
//		// needs flushing the vector of pids
//		Scheduler::backgroundPids->clear();
//		std::string line = "";
//		while (std::getline(inputStream, line))
//		{
//		    std::istringstream iss(line);
//		    pid_t pid;
//		    if (!(iss >> pid)) { break; } // error
//		    backgroundPids->push_back(pid);
//		}
//	}
//	inputStream.close();
//	cout << "I read these pids and the size is: " << backgroundPids->size() << endl;
//	for(int i=0;i<backgroundPids->size();++i) {
//		cout << (*backgroundPids)[i] << " ";
//	}
//	cout << endl;
//}
//
//void Scheduler::writePidsToFile(){
//	fstream outputStream;
//	outputStream.open(Utils::processesFile, ios_base::out);
//	if (!outputStream)
//		cerr<<"Could not open file"<<endl;
//	else {
//		for(int i=0;i<backgroundPids->size();++i)
//			outputStream << (*backgroundPids)[i] << endl;
//	}
//	outputStream.close();
//}

void Scheduler::submitCommandsFromPipe() {
	Command* current = nullptr;
	char readbuffer_bg[BG_BUFFER_SIZE];
	char readbuffer_cmd[BUFFER_SIZE];
	int nbytes = read(pipefds[2], readbuffer_bg, BG_BUFFER_SIZE);
	cout << "Just read from 2nd pipe: " << readbuffer_bg << endl;
	int bg_buffer = atoi(readbuffer_bg);
	while (bg_buffer>0) {
		int nbytes = read(pipefds[0], readbuffer_cmd, BUFFER_SIZE);
		cout << "Just read from 1st pipe: " << readbuffer_cmd << endl;
		string* toRead = new string(readbuffer_cmd);
		current = Command::readFromString(*toRead);
		--bg_buffer;
		pid_t pid = current->invoke();
		kill(pid, SIGSTOP);
		backgroundPids->push_back(pid);
	}
	sprintf(readbuffer_bg, "%d", bg_buffer);
	write(pipefds[3], readbuffer_bg, BG_BUFFER_SIZE);
}



// the main function of the scheduler
void Scheduler::timerHandler(int signal){
	// read list
	// wait pid with wnohang sto current process
	// if finished, delete from list
	// write list to file
	submitCommandsFromPipe();
//	cout << "Scheduler timer handler caught a signal!" << endl;
//	readPidsFromFile();
//
	cout << "indexOfRunningProcess: " <<indexOfRunningProcess << endl;
	if (backgroundPids->size() == 0) { return; }
	int indexOfProcessToRun = (indexOfRunningProcess+1)%backgroundPids->size();
	cout << "indexOfProcessToRun: " <<indexOfProcessToRun << endl;
	pid_t runningProcessPid = (*backgroundPids)[indexOfRunningProcess];
	int runningProcessStatus;
	waitpid(runningProcessPid, &runningProcessStatus, WNOHANG);
	cout << "the pstatus is: " << runningProcessStatus << endl;
	cout << "the wife is: " << WIFEXITED(runningProcessStatus) << endl;
	if (WIFEXITED(runningProcessStatus)) { //process has exited
		backgroundPids->erase(backgroundPids->begin() + indexOfRunningProcess); // assuming it has not changed
		cerr << "Process " << runningProcessPid << " finished" << endl;
		indexOfProcessToRun = indexOfRunningProcess;
	}
	else {
		kill((*backgroundPids)[indexOfRunningProcess], SIGSTOP);
	}
	kill((*backgroundPids)[indexOfProcessToRun], SIGCONT);
	cerr << "Changing to pid " << (*backgroundPids)[indexOfProcessToRun] << endl;
	indexOfRunningProcess = indexOfProcessToRun;
	// the process has been removed from the file, that's why we don't increment.
	// cout << "hello" << endl;
}

// initial call of the scheduler
void Scheduler::invoke(int *pipefd1){
	pipefds = pipefd1;
	close(pipefds[1]);
	//Setup timer
	struct itimerval timer={0};
	timer.it_value.tv_sec = 1;
	timer.it_interval.tv_sec = 25;
	setitimer(ITIMER_REAL, &timer, NULL);
	//Setup signal handler
	signal(SIGALRM, &timerHandler);
	while(true) {}
	//return 0;
}
