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
	delete this->pwd;
	this->pwd = new string(directory);
}

void CommandPrompt::showPrompt() {
	const int BUFFERSIZE = 30;
	char *hostname = new char[BUFFERSIZE];
	gethostname(hostname, BUFFERSIZE);
	char *username = new char[BUFFERSIZE];
	passwd *passwd;
	passwd = getpwuid(getuid());
	username = passwd->pw_name;
	if (username == nullptr)
		username = "unknown";

	cout << username << '@' << hostname << ':' << *pwd << "$ ";
}

Command *CommandPrompt::getCommand(string command) {
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
	vector<string> *args = Utils::tokenize((*theTwoParts)[0], delimiter);
	cout << "printing args tokenized:" << endl;
//	cout << "After Utils::tokenize" << endl;
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
//	theCommand->printInfo();
	return theCommand;
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
		theTwoStrings = Utils::tokenize(stringToParse, "<");
		if (theTwoStrings->size() == 2) {
			vector<string>* tokens = Utils::tokenize(theTwoStrings->at(1), " ");
			(*theTwoStrings)[1] = tokens->at(0);
			command->setRedirectFrom(&theTwoStrings->at(1));
		}
		return theTwoStrings;
		break;
	case to:
		theTwoStrings = Utils::tokenize(stringToParse, ">");
		if (theTwoStrings->size() == 2) {
			vector<string>* tokens = Utils::tokenize(theTwoStrings->at(1), " ");
			(*theTwoStrings)[1] = tokens->at(0);
			command->setRedirectTo(&theTwoStrings->at(1), replace);
		}
		return theTwoStrings;
		break;
	case to_app:
		theTwoStrings = Utils::tokenize(stringToParse, ">>");
		if (theTwoStrings->size() == 2) {
			vector<string>* tokens = Utils::tokenize(theTwoStrings->at(1), " ");
			(*theTwoStrings)[1] = tokens->at(0);
			command->setRedirectTo(&theTwoStrings->at(1), append);
		}
		return theTwoStrings;
		break;
	case pipel:
		theTwoStrings = Utils::tokenize(stringToParse, "|");
		if (theTwoStrings->size() == 2) {
			command->setPipelineTo(getCommand(theTwoStrings->at(1)));
		}
		return theTwoStrings;
		break;
	}
}
