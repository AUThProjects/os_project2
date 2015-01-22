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
			vector<string>* arguments,
			Command* pipelineTo,
			typeOfRedirection redirectTo,
			string* fileToRedirectTo,
			bool redirectFrom,
			string* fileToRedirectFrom,
			bool inBackground)
{
	this->schedulerPID = schedulerPID;
	this->commandName = commandName;
	this->arguments = arguments;
	this->pipelineTo = pipelineTo;
	this->redirectTo = redirectTo;
	this->fileToRedirectTo = fileToRedirectTo;
	this->redirectFrom = redirectFrom;
	this->fileToRedirectFrom = fileToRedirectFrom;
	this->inBackground = inBackground;
}


Command::~Command() {
	delete commandName;
	this->arguments->clear();
	if(pipelineTo!=nullptr)
		delete pipelineTo;
	if(fileToRedirectTo!=nullptr)
		delete fileToRedirectTo;
	if(fileToRedirectFrom!=nullptr)
		delete fileToRedirectFrom;
}
void Command::setPipelineTo(Command* pipelineTo) {
		this->pipelineTo = pipelineTo;
}

int Command::invoke()
{
	errno = 0;
	if(*(this->commandName) == "cd")
	{
		// 0 -> when found
		// -1 -> not found
		cout << "inside invoke chdir" << endl;
		chdir(arguments->at(0).c_str());
		return errno;
	}
	else if(*(this->commandName) == "exit")
	{
		// Scheduler will handle SIGINT signal
		// and will kill all child processes
		// and exit.
		int exitCode = kill(schedulerPID, SIGINT);
		exit(exitCode);
	}
	else
	{
		int fd;
		switch(redirectTo) {
			case none:
				break;
			case replace:
			{
				fd = open(this->fileToRedirectTo->c_str(), O_WRONLY | O_CREAT | O_TRUNC , S_IRUSR | S_IWUSR |
						S_IRGRP | S_IWGRP | S_IROTH);
				if (fd < 0) {return -1;}
				break;
			}
			case append:
			{
				fd = open(this->fileToRedirectTo->c_str(), O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR |
						S_IRGRP | S_IWGRP | S_IROTH);
				if (fd < 0) {return -1;}
				break;
			}
		}
		if (redirectFrom) {
			fd = open(this->fileToRedirectFrom->c_str(), O_RDONLY );
			if (fd < 0) {return -1;}
		}
		if (pipelineTo) {

		}





		signal(SIGCHLD, SIG_IGN);
		pid_t id = fork();
		if(id==0)
		{
			//exec the command
			cout << "Execution here.." << endl;
			switch(redirectTo) {
				case none:
					break;
				case replace:
				{
					if (dup2(fd,1) < 0) {return -1;}
					break;
				}
				case append:
				{
					if (dup2(fd,1) < 0) {return -1;}
					break;
				}
			}
			if (redirectFrom) {
				if (dup2(fd,0) < 0) {return -1;}
			}
			if (pipelineTo) {

			}

			char** args = argsConversion();
			int returnCode = execvp(this->commandName->c_str(), args);
		}
		else
		{
			int* status;
			cerr << "Process " << id << " started." << endl;
			if (!inBackground)
				return waitpid(id, status, 0);
			else {
				// write in file
				// sendSIgstop
				waitpid(id,status,WNOHANG);
				int sigstopRes = kill(id, SIGSTOP);
				if (sigstopRes == -1) {
					cerr << "SIGSTOP failed" << endl;
					char* errorInfo;
					perror(errorInfo); // prints it out to cerr
				}
				else {
					fstream outputStream;
					outputStream.open(Utils::processesFile, ios_base::out|ios_base::app);
					if (!outputStream)
						cerr<<"Could not open file"<<endl;
					else
						outputStream << id << endl;
					outputStream.close();
				}
				 return id;
			}
		}
	}
	return 0;
}


void Command::printInfo() {
	cout << *commandName << " ";
	for (int i=0;i<arguments->size();++i) {
		cout << " " << arguments->at(i);
	}
	if(redirectTo==replace)
		cout << " replaces to: " << *fileToRedirectTo << endl;
	if(redirectTo==append)
			cout << " appends to: " << *fileToRedirectTo << endl;
	if(redirectFrom)
		cout << " redirects from: " << *fileToRedirectFrom << endl;
	if(pipelineTo!=nullptr)
	{
		cout << " and pipelines with: " << endl;
		pipelineTo->printInfo();
	}
	cout << endl;
}

char** Command::argsConversion() {
	int numberOfArgs = arguments->size();
	char** toBeReturned = new char*[numberOfArgs+1];
	for(int i=0;i<numberOfArgs;++i) {
		toBeReturned[i] = new char[arguments->at(i).length()];
		strcpy(toBeReturned[i], arguments->at(i).c_str());
	}
	toBeReturned[numberOfArgs] = (char *)NULL;
	return toBeReturned;
}
