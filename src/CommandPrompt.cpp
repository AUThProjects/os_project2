/*
 * CommandPrompt.cpp
 *
 *  Created on: Jan 2, 2015
 *      Author: steve
 */

#include "../include/CommandPrompt.h"


CommandPrompt::CommandPrompt() {
	const int BUFFERSIZE = 150;
	char *pwd = new char[BUFFERSIZE];
	getcwd(pwd, BUFFERSIZE);
	this->pwd = new string(pwd);
	delete pwd;
}

CommandPrompt::~CommandPrompt() {
	// TODO Auto-generated destructor stub
}

string CommandPrompt::getPwd() {
	return *(this->pwd);
}

void CommandPrompt::setPwd(string directory) {
	this->pwd = new string(directory);
}

void CommandPrompt::showPrompt() {
	const int BUFFERSIZE = 30;
	char *hostname = new char[BUFFERSIZE];
	gethostname(hostname, BUFFERSIZE);
	char *username = new char[BUFFERSIZE];
	username = "username should be here";

	cout << username << '@' << hostname << ':' << *pwd << "$ ";
}

Command *CommandPrompt::getCommand(string command) {
	cout << "-------------" << endl;
	const char* delimiter = " ";
	pid_t schedulerPID;
	vector<string>* arguments;
	Command* pipelineTo;
	typeOfRedirection redirectTo;
	string* fileToRedirectTo;
	bool redirectFrom;
	string* fileToRedirectFrom;
	bool inBackground;
	vector<string> *theTwoParts = tokenize(command, "|");
	switch(theTwoParts->size()){
		case 1:
			pipelineTo = nullptr;
			theTwoParts = tokenize(command, ">>");
			switch(theTwoParts->size())
			{
				case 1:
					redirectTo = none;
					fileToRedirectTo=nullptr;
					theTwoParts = tokenize(command, ">");
					switch(theTwoParts->size())
					{
						case 1:
							theTwoParts = tokenize(command, "<");
							switch(theTwoParts->size())
							{
								case 1:
									redirectFrom = false;
									fileToRedirectFrom = nullptr;
									break;
								case 2:
									redirectFrom = true;
									fileToRedirectFrom = &(*theTwoParts)[1];
									break;
							}
							break;
						case 2:
							redirectTo = replace;
							fileToRedirectTo= &(*theTwoParts)[1]; //check
							break;
						default:
							break;
					}
					break;
				case 2:
					cout << "String 1: " << (*theTwoParts)[0] << endl;
					cout << "String 2: " << (*theTwoParts)[1] << endl;
					redirectTo = append;
					fileToRedirectTo= &(*theTwoParts)[1]; // we have to check for errors
					break;
				default:
					break;
			}
			break;
		case 2:
			Command* secondCommand = getCommand((*theTwoParts)[1]);
			Command* firstCommand = getCommand((*theTwoParts)[0]);
			firstCommand->setPipelineTo(secondCommand);
			return firstCommand;
			break;
	}
	vector<string> *args = tokenize((*theTwoParts)[0], delimiter);
	bool isBackground = parseForBackgroundProcess(args);
	string *commandName = new string(args->at(0));
	// args->erase(args->begin());
	Command *myCmd = new Command(0,
			commandName,
			args,
			pipelineTo,
			redirectTo,
			fileToRedirectTo,
			redirectFrom,
			fileToRedirectFrom,
			isBackground);

//	for(int i=0;i<args->size();++i)
//		cout << i <<". "<< (*args)[i] << " ";
//	cout << endl;
	return myCmd;
}

vector<string>* CommandPrompt::tokenize(string commandString, const char* delimiter) {
	int stringLength = commandString.length();
	int* delimiterPositions = new int[30];
	delimiterPositions[0] = -1;
	int numberOfDelimiters = 1;
	int currentDelimiter = commandString.find(delimiter, delimiterPositions[0]+1);
	while (currentDelimiter != string::npos) {
		delimiterPositions[numberOfDelimiters++] = currentDelimiter;
		currentDelimiter = commandString.find(delimiter, delimiterPositions[numberOfDelimiters-1]);
	}
	delimiterPositions[numberOfDelimiters++] = stringLength;
//	cout << "Delimiter Positions: " << endl;
//	for(int i=0;i<numberOfDelimiters;++i) {
//			cout << delimiterPositions[i] << " ";
//		}
	vector<string> *toBeReturned = new vector<string>();
	for(int i=0;i<numberOfDelimiters-1;++i) {
		if (delimiterPositions[i]+1 == delimiterPositions[i+1]) continue;
		toBeReturned->push_back(commandString.substr(delimiterPositions[i]+1, delimiterPositions[i+1]-delimiterPositions[i]-1));
	}
	toBeReturned->shrink_to_fit();
	return toBeReturned;
}

bool CommandPrompt::parseForBackgroundProcess(vector<string>* args) {
	string lastArgument = args->back();
	args->pop_back();
	if (lastArgument.back() == '&') {
		lastArgument.pop_back();
		if (lastArgument.size() != 0)
			args->push_back(lastArgument);
		return true;
	}
	args->push_back(lastArgument);
	return false;
}

void CommandPrompt::sysCallErrorHandling() {
	perror("Errno");
}
