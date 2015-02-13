/*
 * CommandPrompt.cpp
 *
 *  Created on: Jan 2, 2015
 *      Author: steve
 */

#include "../include/CommandPrompt.h"

/**
 * Ctor for CommandPrompt. Scheduler's pid is passed.
 */
CommandPrompt::CommandPrompt(pid_t schedulerPid) {
	const int BUFFERSIZE = 150;
	char *pwd = new char[BUFFERSIZE];
	getcwd(pwd, BUFFERSIZE);
	this->pwd = new string(pwd);
	this->schedulerPid = schedulerPid;
	delete pwd;
}

/**
 * Dtor for CommandPrompt. Releases resources.
 */
CommandPrompt::~CommandPrompt() {
	delete this->pwd;
}


/**
 * Method showing command prompt to the user's terminal.
 */
void CommandPrompt::showPrompt() {
	const int BUFFERSIZE = 30; // max length for username and hostname
	char *hostname = new char[BUFFERSIZE];
	gethostname(hostname, BUFFERSIZE);
	char *username = new char[BUFFERSIZE];
	passwd *passwd;
	passwd = getpwuid(getuid());
	username = passwd->pw_name;
	if (username == nullptr)
		username = "unknown"; // username could not be retrieved

	cout << username << '@' << hostname << ':' << *pwd << "$ ";
}

/**
 * Parse command from string. Returns Command object.
 */
Command *CommandPrompt::getCommand(string command) {
	const char* delimiter = " ";
	pid_t schedulerPID;

	Command* theCommand = new Command();
	/*
	 * Two substrings returned from tokenized command string when unique delimiter.
	 */
	vector<string> *theTwoParts;
	theTwoParts = parseForOperator(pipel, command, theCommand);
	theTwoParts = parseForOperator(to_app, theTwoParts->at(0), theCommand);

	if(theTwoParts->size() == 1)
		theTwoParts = parseForOperator(to, theTwoParts->at(0), theCommand);

	theTwoParts = parseForOperator(from, theTwoParts->at(0) , theCommand);

	vector<string> *args = Utils::tokenize((*theTwoParts)[0], delimiter);

	bool isBackground = parseForBackgroundProcess(args);
	string *commandName = new string(args->at(0));
	theCommand->setCommandName(commandName);
	theCommand->setArguments(args);
	theCommand->setInBackground(isBackground);
	theCommand->setSchedulerPid(this->schedulerPid);

	return theCommand;
}

/**
 * Checks if the command is to be launched in background. True for yes, false otherwise.
 */
bool CommandPrompt::parseForBackgroundProcess(vector<string>* args) {
	string lastArgument = args->back();
	args->pop_back();
	if (lastArgument.back() == '&') {
		lastArgument.pop_back(); // c++1y compatibility
		if (lastArgument.size() != 0)
			args->push_back(lastArgument);
		return true;
	}
	args->push_back(lastArgument);
	return false;
}

//
//void CommandPrompt::sysCallErrorHandling() {
//	perror("Errno");
//}


/**
 * returns the parsed elements of the whole stringToParse.
 * Returns array of delimited parsed elements.
 * Command is passed by reference here.
 */
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

// Accessors - Mutators
string CommandPrompt::getPwd() {
	return *(this->pwd);
}

void CommandPrompt::setPwd(string directory) {
	delete this->pwd;
	this->pwd = new string(directory);
}
