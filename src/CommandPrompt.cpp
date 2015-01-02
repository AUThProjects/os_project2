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
	getlogin_r(username, BUFFERSIZE);

	cout << username << '@' << hostname << ':' << *pwd << "$ ";
}

Command CommandPrompt::getCommand(string command) {
	cout << "inside getCommand" << endl;
	char *commandInCharStar = new char[150];
	strcpy(commandInCharStar, command.c_str());
	cout << "after strcpy" << endl;
	const char* delimiter = " ";
	string* commandName = new string(strtok(commandInCharStar, delimiter));
	cout << *commandName << endl;
	vector<string> *args = new vector<string>();
	char *arg = new char[20];
	strcpy(arg, strtok(NULL, delimiter));
	cout << "after strtok" << "arg=" << arg << endl;
	int counter = 0;
	while(arg != NULL) {
		cout << counter << endl;
		args->push_back(*new string(arg));
		cout << "after pushback" << endl;
		arg = strtok(NULL, delimiter);
	}
	cout << *commandName << endl;
	for (int i=0;i<args->size();++i) {
		cout << args->at(i) << " " << endl;
	}
	Command *myCmd = new Command(0, commandName, args, nullptr, none, nullptr, false, nullptr);
	return *myCmd;

}

vector<string> CommandPrompt::tokenize(string commandString, char* delimiter) {
	int stringLength = commandString.length();
	int delimiterPos = 0;
	int* delimitedPos = new int[100];
	int currentPosCount = 0;
	delimitedPos[currentPosCount++] = -1;
	while (delimiterPos != string::npos) {
		delimiterPos = commandString.find_first_of(delimiter, delimiterPos+1);
		cout << "delimiterPos" << delimiterPos << endl;
		if (delimiterPos > 0) {
			delimitedPos[currentPosCount++] = delimiterPos;
		}
	}
	cout << currentPosCount << endl;
	delimitedPos[currentPosCount] = stringLength;
	vector<string> toBeReturned(currentPosCount);
	for(int i=0;i<currentPosCount-1;++i) {
		toBeReturned.push_back(commandString.substr(delimitedPos[i]+1, delimitedPos[i+1]-1));
	}
	return toBeReturned;
}

