/*
 * CommandPrompt.h
 *
 *  Created on: Jan 2, 2015
 *      Author: steve
 */

#ifndef INCLUDE_COMMANDPROMPT_H_
#define INCLUDE_COMMANDPROMPT_H_

#include <string>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <pwd.h>
#include "Command.h"
#include "Utils.h"

using namespace std;

enum TypeOfOperator {from, to, to_app, pipel};
class CommandPrompt {
public:
	CommandPrompt(pid_t schedulerPid);
	virtual ~CommandPrompt();

	string getPwd();
	void setPwd(string directory);
	void showPrompt();
	Command *getCommand(string command);
	static void sysCallErrorHandling();
private:
	pid_t schedulerPid;
	string *pwd; // present working directory
	bool parseForBackgroundProcess(vector<string>* args);
	vector<string>* parseForOperator(TypeOfOperator too, string stringToParse, Command* command);
};

#endif /* INCLUDE_COMMANDPROMPT_H_ */
