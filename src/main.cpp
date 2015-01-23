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

int main()
{
	int pipefd[4];
	if (pipe(pipefd)<0) {return -1;}
	if (pipe(pipefd+2)<0) {return -1;}
	write(pipefd[3], "0", BG_BUFFER_SIZE);
	//Start scheduler process
	pid_t id = fork();

	if(id==0) {
		close(pipefd[1]);
		Scheduler::invoke(pipefd);
	}
	else
	{
		while (true) {
			CommandPrompt *cmd = new CommandPrompt();
			cmd->showPrompt();
			string input;
			cin.clear();
			fflush(stdin);
			getline(cin, input);
			Command* myCommand = cmd->getCommand(input);
			string* s = myCommand->toString();
			cout << *s << endl;
//			Command *newCommand = Command::readFromString(*s);
//			cout << "printing info in main:" << endl;
//			newCommand->printInfo();
			if (myCommand->isBackground()) {
				char readbuffer[BG_BUFFER_SIZE];
				read(pipefd[2], readbuffer, BG_BUFFER_SIZE);
				cout << "main: just read from 2nd pipe: " << readbuffer << endl;
				int bg_buffer = atoi(readbuffer);
				char* buffer = new char[BG_BUFFER_SIZE];
				sprintf(buffer, "%d", ++bg_buffer);
				write(pipefd[3], buffer, BG_BUFFER_SIZE);
				write(pipefd[1], s->c_str(), BUFFER_SIZE);
			}
			else
				myCommand->invoke();
//			CommandPrompt::sysCallErrorHandling();

		}
	}
}
