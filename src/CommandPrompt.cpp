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
//	vector<string>* arguments;
//	Command* pipelineTo;
//	typeOfRedirection redirectTo = none;
//	string* fileToRedirectTo;
//	bool redirectFrom = false;
//	string* fileToRedirectFrom;
//	bool inBackground;
	Command* theCommand = new Command();
	vector<string> *theTwoParts;
	theTwoParts = parseForOperator(pipel, command, theCommand);
	theTwoParts = parseForOperator(to_app, theTwoParts->at(0), theCommand);
	if(theTwoParts->size() == 1)
		theTwoParts = parseForOperator(to, theTwoParts->at(0), theCommand);
	theTwoParts = parseForOperator(from, theTwoParts->at(0) , theCommand);







//	cout << "After switch of redirection" << endl;
//	cout << "theTwoParts[0]" << (*theTwoParts)[0] << "$"<< endl;
	vector<string> *args = tokenize((*theTwoParts)[0], delimiter);

//	cout << "After tokenize" << endl;
	bool isBackground = parseForBackgroundProcess(args);
	cout << "After parseForBGProcess" << endl;
	string *commandName = new string(args->at(0));
	theCommand->setCommandName(commandName);
	theCommand->setArguments(args);
	theCommand->setInBackground(isBackground);
	// args->erase(args->begin());
//	Command *myCmd = new Command(0,
//			commandName,
//			args,
//			pipelineTo,
//			redirectTo,
//			fileToRedirectTo,
//			redirectFrom,
//			fileToRedirectFrom,
//			isBackground);
//	myCmd->printInfo();
//	for(int i=0;i<args->size();++i)
//		cout << i <<". "<< (*args)[i] << " ";
//	cout << endl;
//	return myCmd;
	theCommand->printInfo();
	return theCommand;
}

vector<string>* CommandPrompt::tokenize(string commandString, const char* delimiter) {
//	cout << "Inside tokenize with delimiter: " << delimiter << "and commandString" << commandString << endl;

	int stringLength = commandString.length();
	int delimiterLength = strlen(delimiter);
	int* delimiterPositions = new int[30]; // keeps track of the first (potentially) delimiter char
	delimiterPositions[0] = -delimiterLength;
	int numberOfDelimiters = 1; // size of delimiterPositions
	int currentDelimiter = commandString.find(delimiter, delimiterPositions[0]+ delimiterLength); // +delimiterLength
	while (currentDelimiter != string::npos) {
		delimiterPositions[numberOfDelimiters++] = currentDelimiter;
		currentDelimiter = commandString.find(delimiter, delimiterPositions[numberOfDelimiters-1] + delimiterLength);
	}
	delimiterPositions[numberOfDelimiters++] = stringLength;
//	cout << "Delimiter Positions: " << endl;
//	for(int i=0;i<numberOfDelimiters;++i) {
//			cout << delimiterPositions[i] << " ";
//		}

	vector<string> *toBeReturned = new vector<string>();
	for(int i=0;i<numberOfDelimiters-1;++i) {
		if (delimiterPositions[i]+delimiterLength == delimiterPositions[i+1]) {continue;};
		toBeReturned->push_back(commandString.substr(delimiterPositions[i]+delimiterLength, delimiterPositions[i+1]-(delimiterPositions[i]+delimiterLength)));
	}
	toBeReturned->shrink_to_fit();
//	cout << "tokenize() successful" << endl;
//	for (int i=0;i<toBeReturned->size();++i) {
//		cout << toBeReturned->at(i) << " ";
//	}
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

// returns the parsed elements of the whole stringToParse
vector<string>* CommandPrompt::parseForOperator(TypeOfOperator too, string stringToParse, Command* command) {
	vector<string>* theTwoStrings;
	// the cases where theTwoStrings consist of 1 element are not handled due to default values in the ctor
	switch(too) {
	case from:
		theTwoStrings = tokenize(stringToParse, "<");
		if (theTwoStrings->size() == 2) {
			vector<string>* tokens = tokenize(theTwoStrings->at(1), " ");
			(*theTwoStrings)[1] = tokens->at(0);
			command->setRedirectFrom(&theTwoStrings->at(1));
		}
		return theTwoStrings;
		break;
	case to:
		theTwoStrings = tokenize(stringToParse, ">");
		if (theTwoStrings->size() == 2) {
			vector<string>* tokens = tokenize(theTwoStrings->at(1), " ");
			(*theTwoStrings)[1] = tokens->at(0);
			command->setRedirectTo(&theTwoStrings->at(1), replace);
		}
		return theTwoStrings;
		break;
	case to_app:
		theTwoStrings = tokenize(stringToParse, ">>");
		if (theTwoStrings->size() == 2) {
			vector<string>* tokens = tokenize(theTwoStrings->at(1), " ");
			(*theTwoStrings)[1] = tokens->at(0);
			command->setRedirectTo(&theTwoStrings->at(1), append);
		}
		return theTwoStrings;
		break;
	case pipel:
		theTwoStrings = tokenize(stringToParse, "|");
		if (theTwoStrings->size() == 2) {
			command->setPipelineTo(getCommand(theTwoStrings->at(1)));
		}
		return theTwoStrings;
		break;
	}
}
