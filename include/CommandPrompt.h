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
#include "Command.h"

using namespace std;

class CommandPrompt {
public:
	CommandPrompt();
	virtual ~CommandPrompt();

	string getPwd();
	void setPwd(string directory);
	void showPrompt();
	Command getCommand(string command);
	vector<string>* tokenize(string commandString, const char* delimiter);

private:
	string *pwd; // present working directory
};

#endif /* INCLUDE_COMMANDPROMPT_H_ */
