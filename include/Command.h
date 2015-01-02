/*
 * Command.h
 *
 *  Created on: 2 Jan 2015
 *      Author: anthony
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <string>
#include <vector>
using namespace std;

enum typeOfRedirection {none, write, append};

class Command {
public:
	Command();
	Command(string* commandName,
			vector<string> arguments,
			Command* pipelineTo,
			typeOfRedirection redirectTo,
			string* fileToRedirectTo,
			bool redirectFrom,
			string* fileToRedirectFrom);
	virtual ~Command();
private:
	string* commandName;
	vector<string> arguments;

	Command* pipelineTo;

	typeOfRedirection redirectTo;
	string* fileToRedirectTo;

	bool redirectFrom;
	string* fileToRedirectFrom;
};

#endif /* COMMAND_H_ */
