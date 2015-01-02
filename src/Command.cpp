/*
 * Command.cpp
 *
 *  Created on: 2 Jan 2015
 *      Author: anthony
 */

#include "../include/Command.h"

Command::Command() {
	// TODO Auto-generated constructor stub
	this->commandName = new string();
	this->arguments = new vector<string>();
	this->pipelineTo = nullptr;
	this->redirectTo = none;
	this->fileToRedirectTo = nullptr;
	this->redirectFrom = false;
	this->fileToRedirectFrom = nullptr;
}

Command::Command(string* commandName,
			vector<string> arguments,
			Command* pipelineTo,
			typeOfRedirection redirectTo,
			string* fileToRedirectTo,
			bool redirectFrom,
			string* fileToRedirectFrom)
{
	this->commandName = commandName;
	this->arguments = arguments;
	this->pipelineTo = pipelineTo;
	this->redirectTo = redirectTo;
	this->fileToRedirectTo = fileToRedirectTo;
	this->redirectFrom = redirectFrom;
	this->fileToRedirectFrom = fileToRedirectFrom;
}


Command::~Command() {
	// TODO Auto-generated destructor stub
	delete commandName;
	this->arguments.clear();
	if(pipelineTo!=nullptr)
		delete pipelineTo;
	if(fileToRedirectTo!=nullptr)
		delete fileToRedirectTo;
	if(fileToRedirectFrom!=nullptr)
		delete fileToRedirectFrom;
}

