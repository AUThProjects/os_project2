/*
 * main.cpp
 *
 *  Created on: 28 Dec 2014
 *      Author: anthony
 */


#include <iostream>
#include <cstdio>
#include "../include/CommandPrompt.h"

using namespace std;

int main()
{
	while (true) {
		CommandPrompt *cmd = new CommandPrompt();
		cmd->showPrompt();
		string input;
		cin.clear();
		fflush(stdin);
		getline(cin, input);
		Command myCommand = cmd->getCommand(input);
		myCommand.printInfo();
		myCommand.invoke();
	}
}

