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
#include<fstream>
#include <vector>
#include "Utils.h"
using namespace std;

enum typeOfRedirection {none, replace, append};

class Command {
public:
	Command();
	Command(pid_t schedulerPID,
			string* commandName,
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

	void setPipelineTo(Command* pipelineTo);

private:
	pid_t schedulerPID;

	string* commandName;
	vector<string>* arguments;

	Command* pipelineTo;

	typeOfRedirection redirectTo;
	string* fileToRedirectTo;

	bool redirectFrom;
	string* fileToRedirectFrom;

	bool inBackground;
	char** argsConversion();
};

#endif /* COMMAND_H_ */
