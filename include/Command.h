/*
 * Command.h
 *
 *  Created on: 2 Jan 2015
 *      Author: anthony
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include "Utils.h"

using namespace std;

enum typeOfRedirection {none, replace, append};

class Command {
public:
	Command();
	Command(string* commandName,
			vector<string>* arguments,
			Command* pipelineTo,
			typeOfRedirection redirectTo,
			string* fileToRedirectTo,
			bool redirectFrom,
			string* fileToRedirectFrom,
			bool inBackground);
			void printInfo();
	virtual ~Command();

	int invoke();

	void setCommandName(string* commandName);
	void setArguments(vector<string>* arguments);
	void setPipelineTo(Command* pipelineTo);
	void setRedirectTo(string* fileToRedirectTo, typeOfRedirection tor);
	void setRedirectFrom(string* fileToRedirectFrom);
	void setInBackground(bool inBackground);
	static Command* readFromString(string s);
	bool isBackground();
	string* toString();
private:
	string* commandName;
	vector<string>* arguments;

	Command* pipelineTo;

	typeOfRedirection redirectTo;
	string* fileToRedirectTo;

	bool redirectFrom;
	string* fileToRedirectFrom;

	bool inBackground;
	int fd; // file descriptor for redirection
	char** argsConversion();
};

#endif /* COMMAND_H_ */
