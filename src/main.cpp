/*
 * main.cpp
 *
 *  Created on: 28 Dec 2014
 *      Author: anthony
 */


#include <iostream>
#include <cstdio>
#include "../include/Scheduler.h"
#include "../include/Utils.h"
#include "../include/CommandPrompt.h"

using namespace std;

int main()
{
	//Start scheduler process
	pid_t id = fork();
	if(id==0) {
		//Scheduler
		// Scheduler* sch = new Scheduler(Utils::processesFile);
		// sch->invoke();
		//Scheduler::invoke();
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
			myCommand->printInfo();
			myCommand->invoke();
//			CommandPrompt::sysCallErrorHandling();

		}
	}
}
