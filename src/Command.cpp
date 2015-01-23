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
//	delete commandName;
//	this->arguments->clear();
//	if(pipelineTo!=nullptr)
//		delete pipelineTo;
//	if(fileToRedirectTo!=nullptr)
//		delete fileToRedirectTo;
//	if(fileToRedirectFrom!=nullptr)
//		delete fileToRedirectFrom;
}

// returns the pid of the process if no pipeline, and the pid of the Receiver's part
// of the pipeline in case of pipelining.
int Command::invoke()
{
	errno = 0;
	if(*(this->commandName) == "cd")
	{
		// 0 -> when found
		// -1 -> not found
		cout << "inside invoke chdir" << endl;
		chdir(arguments->at(0).c_str());
//		return errno;
		return 0;
	}
	else if(*(this->commandName) == "exit")
	{
		// Scheduler will handle SIGINT signal
		// and will kill all child processes
		// and exit.
		int exitCode = kill(schedulerPID, SIGINT);
		exit(exitCode);
	}
	else // execvp commands
	{
		int fd, fd2;
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
			fd2 = open(this->fileToRedirectFrom->c_str(), O_RDONLY );
			if (fd2 < 0) {return -1;}
		}
		signal(SIGCHLD, SIG_IGN);
		pid_t id = fork();
		if (id <= -1)
			return -1;
		else if(id==0) //fork before pipeline
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
				if (dup2(fd2,0) < 0) {return -1;}
			}
			int pipefd[2];
			if (pipelineTo) {
				if (pipe(pipefd)<0)
					return -2;
				pid_t pipeSenderPid = fork();
				if (pipeSenderPid < 0)
					return -1;
				else if (pipeSenderPid == 0) { //child
					close(pipefd[0]); // closes stdin pipelining
					if (dup2(pipefd[1], 1) < 0) {return -1;}
					char** args = argsConversion();
					int returnCode = execvp(this->commandName->c_str(), args); // execute the first part of the pipeline
				}
				else { // parent
					close(pipefd[1]); //closes its write end
					dup2(pipefd[0], 0);
				}
				close(pipefd[0]);
				close(pipefd[1]);
				int status;
				wait(&status);
				this->pipelineTo->invoke(); // command after pipeline
//				char** args = this->pipelineTo->argsConversion();
//				int returnCode = execvp(this->pipelineTo->commandName->c_str(), args);
			}
			cout << "From command: " << *commandName << endl;
			cout << "is this executed in pipeline?" << endl;
			int returnCode;
			if (pipelineTo == nullptr) {
				char** args = argsConversion();
				returnCode = execvp(this->commandName->c_str(), args); // execute the second part of the pipeline
			}
		}
		else
		{
			int* status;
			cerr << "Process " << id << " started." << endl;
			if (!inBackground)
				return waitpid(id, status, 0);
			else {
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

string* Command::toString(){
	string delimeter = "@";
	string* toBeReturned = new string();
	(*toBeReturned) += *commandName;
	(*toBeReturned) += delimeter;
	for(int i=0; i< arguments->size();++i)
	{
		(*toBeReturned) += arguments->at(i);
		(*toBeReturned) += " ";
	}
	(*toBeReturned) += delimeter;
	(*toBeReturned) += std::to_string(redirectTo);
	(*toBeReturned) += delimeter;
	if(redirectTo!=none)
		(*toBeReturned) += *fileToRedirectTo;
	else
		(*toBeReturned) += "NULL";
	(*toBeReturned) += delimeter;
	(*toBeReturned) += std::to_string(redirectFrom);
	(*toBeReturned) += delimeter;
	if(redirectFrom)
		(*toBeReturned) += *fileToRedirectFrom;
	else
		(*toBeReturned) += "NULL";
	(*toBeReturned) += delimeter;
	(*toBeReturned) += std::to_string(inBackground ? 1 : 0);
	(*toBeReturned) += delimeter;
	(*toBeReturned) += std::to_string(pipelineTo!=nullptr ? 1 : 0);
	(*toBeReturned) += delimeter;
	if(pipelineTo!=nullptr)
		(*toBeReturned) += *(pipelineTo->toString());
	else
		(*toBeReturned) += "NULL";
	return toBeReturned;
}

 Command* Command::readFromString(string s)
{
	 cout << "The string is: " << s << endl;
	vector<string>* tokens = Utils::tokenize(s, "@");
	Command* command = new Command();
	command->setCommandName(&tokens->at(0));
	vector<string>* args = Utils::tokenize(tokens->at(1)," ");
	command->setArguments(args);
	typeOfRedirection tor = static_cast<typeOfRedirection>(atoi(tokens->at(2).c_str()));
	if(tor!=none){
		command->setRedirectTo(&tokens->at(3),tor);
	}
	else
		command->setRedirectTo(nullptr, tor);
	if(atoi(tokens->at(4).c_str())>0){
		command->setRedirectFrom(&tokens->at(5));
	}
	command->setInBackground((atoi(tokens->at(6).c_str())>0));
	if(atoi(tokens->at(7).c_str())>0) {
		string *theRestOfCommand = new string();
		for (int i=8;i<tokens->size();++i) {
			*theRestOfCommand += tokens->at(i);
			*theRestOfCommand += "@";
		}
		command->setPipelineTo(readFromString(*theRestOfCommand));
	}
	else
		command->setPipelineTo(nullptr);
	return command;
}

char** Command::argsConversion() {
	cout << "inside argsConversion" << endl;
	int numberOfArgs = arguments->size();
	char** toBeReturned = new char*[numberOfArgs+1];
	for(int i=0;i<numberOfArgs;++i) {
		toBeReturned[i] = new char[arguments->at(i).length()];
		strcpy(toBeReturned[i], arguments->at(i).c_str());
	}
	toBeReturned[numberOfArgs] = (char *)NULL;
	cout << "End of argsConversion" << endl;
	return toBeReturned;
}

bool Command::isBackground() {
	return inBackground || ((pipelineTo != nullptr) && pipelineTo->inBackground);
}


// Accessors
void Command::setPipelineTo(Command* pipelineTo) {
	this->pipelineTo = pipelineTo;
}
void Command::setCommandName(string* commandName) {
	this->commandName = commandName;
}
void Command::setArguments(vector<string>* arguments) {
	this->arguments = arguments;
}


void Command::setRedirectTo(string* fileToRedirectTo, typeOfRedirection tor) {
	this->fileToRedirectTo = fileToRedirectTo;
	this->redirectTo = tor;
}

void Command::setRedirectFrom(string* fileToRedirectFrom) {
	this->redirectFrom = true;
	this->fileToRedirectFrom = fileToRedirectFrom;
}

void Command::setInBackground(bool inBackground) {
	this->inBackground = inBackground;
}
