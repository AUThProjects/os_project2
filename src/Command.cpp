/*
 * Command.cpp
 *
 *  Created on: 2 Jan 2015
 *      Author: anthony
 */

#include "../include/Command.h"

Command::Command() {
	this->schedulerPID = 0;
	this->commandName = new string();
	this->arguments = new vector<string>();
	this->pipelineTo = nullptr;
	this->redirectTo = none;
	this->fileToRedirectTo = nullptr;
	this->redirectFrom = false;
	this->fileToRedirectFrom = nullptr;
}

Command::Command(
			pid_t schedulerPID,
			string* commandName,
			vector<string> arguments,
			Command* pipelineTo,
			typeOfRedirection redirectTo,
			string* fileToRedirectTo,
			bool redirectFrom,
			string* fileToRedirectFrom)
{
	this->schedulerPID = schedulerPID;
	this->commandName = commandName;
	this->arguments = arguments;
	this->pipelineTo = pipelineTo;
	this->redirectTo = redirectTo;
	this->fileToRedirectTo = fileToRedirectTo;
	this->redirectFrom = redirectFrom;
	this->fileToRedirectFrom = fileToRedirectFrom;
}


Command::~Command() {
	delete commandName;
	this->arguments.clear();
	if(pipelineTo!=nullptr)
		delete pipelineTo;
	if(fileToRedirectTo!=nullptr)
		delete fileToRedirectTo;
	if(fileToRedirectFrom!=nullptr)
		delete fileToRedirectFrom;
}

int Command::invoke()
{
	if(this->commandName == "cd")
	{
		// 0 -> when found
		// -1 -> not found
		return chdir(arguments.at(0).c_str());
	}
	else if(this->commandName == "exit")
	{
		// Scheduler will handle SIGINT signal
		// and will kill all child processes
		// and exit.
		int exitCode = kill(schedulerPID, SIGINT);
		exit(exitCode);
	}
	else
	{
		pid_t id = fork();
		if(id==0)
		{
			//exec the command
			cout << "Execution here.." << endl;
		}
		else
		{
			int* status;
			cerr << "Process " << id << " started." << endl;
			return waitpid(id, status, 0);
		}
	}
	return 0;
}

