/*
 * main.cpp
 *
 *  Created on: 28 Dec 2014
 *      Author: anthony
 */


#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "../include/Scheduler.h"
#include "../include/Utils.h"
#include "../include/CommandPrompt.h"
#include "../include/Command.h"


using namespace std;

/**
 * Entry point for our application.
 */
int main() {
	/*
	 * Double pipelining.
	 * 1st lane("Commands lane"): commands to be executed in background
	 * 2nd lane("Sync lane"): synchronization lane
	 */
	int pipefd[4]; // pipeline with scheduler process
	if (pipe(pipefd)<0) {return -1;}
	if (pipe(pipefd+2)<0) {return -1;}
	write(pipefd[3], "0", BG_BUFFER_SIZE); // first sync message to sync lane

	//Start scheduler process
	pid_t schedulerPid = fork();

	if(schedulerPid==0) { // Scheduler process
		close(pipefd[1]); // close read end from the commands lane
		Scheduler::invoke(pipefd); // launch the scheduler timer
	}
	else if (schedulerPid < 0) {
		cerr << "Could not start scheduler." << endl; // and do what?
		exit(EXIT_SCHEDULER_INIT_FAIL);
	}
	else {
		int schedulerProcessStatus; // for waitpid
		waitpid(schedulerPid, &schedulerProcessStatus, WNOHANG); // we need parallel execution of Command Prompt and Scheduler
		CommandPrompt *cmd = new CommandPrompt(schedulerPid);
		while (true) {
			cmd->showPrompt();
			string input;
			cin.clear();
			fflush(stdin);
			getline(cin, input); // read command from the user
			if (input.empty())
				continue;
			Command* myCommand = cmd->getCommand(input); // create the actual command object
			if (myCommand == nullptr)
				continue;
			// Submit job to scheduler if the command is to be run in background
			/*
			 * We need background processes to be children of the scheduler process.
			 */
			if (myCommand->isBackground()) {
				char readbuffer[BG_BUFFER_SIZE];
				read(pipefd[2], readbuffer, BG_BUFFER_SIZE);

				int bg_buffer = atoi(readbuffer);
				char* buffer = new char[BG_BUFFER_SIZE];
				sprintf(buffer, "%d", ++bg_buffer);
				write(pipefd[3], buffer, BG_BUFFER_SIZE);
				string* s = myCommand->toString();
				write(pipefd[1], s->c_str(), BUFFER_SIZE);
				delete buffer;
				delete s;
			}
			else
				myCommand->invoke(); // invoking command in foreground
			/*
			 * Foreground processes are children of Command Prompt process.
			 */

//			CommandPrompt::sysCallErrorHandling();

		}
		delete cmd;
	}
}
