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

Command CommandPrompt::getCommand(string command) {
	const char* delimiter = " ";
	vector<string> *args = tokenize(command, delimiter);
	string *commandName = new string(args->at(0));
	args->erase(args->begin());
	Command *myCmd = new Command(0, commandName, args, nullptr, none, nullptr, false, nullptr);
	return *myCmd;

}

vector<string>* CommandPrompt::tokenize(string commandString, const char* delimiter) {
	int stringLength = commandString.length();
	int* delimiterPositions = new int[30];
	delimiterPositions[0] = -1;
	int numberOfDelimiters = 1;
	int currentDelimiter = commandString.find_first_of(delimiter, delimiterPositions[0]+1);
	while (currentDelimiter != string::npos) {
		delimiterPositions[numberOfDelimiters++] = currentDelimiter;
		currentDelimiter = commandString.find_first_of(delimiter, delimiterPositions[numberOfDelimiters-1]+1);
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

void CommandPrompt::sysCallErrorHandling() {
	perror("Errno");
}
