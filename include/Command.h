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
#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

enum typeOfRedirection {none, write, append};

class Command {
public:
	Command();
	Command(pid_t schedulerPID,
			string* commandName,
			vector<string> arguments,
			Command* pipelineTo,
			typeOfRedirection redirectTo,
			string* fileToRedirectTo,
			bool redirectFrom,
			string* fileToRedirectFrom);
	virtual ~Command();

	int invoke();

private:
	pid_t schedulerPID;

	string* commandName;
	vector<string> arguments;

	Command* pipelineTo;

	typeOfRedirection redirectTo;
	string* fileToRedirectTo;

	bool redirectFrom;
	string* fileToRedirectFrom;
};

#endif /* COMMAND_H_ */
