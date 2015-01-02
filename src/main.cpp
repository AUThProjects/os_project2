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
	CommandPrompt *cmd = new CommandPrompt();
	for(int i=0;i<2;++i)
	{
		cmd->showPrompt();
		string input;
		cin.clear();
		fflush(stdin);
		getline(cin, input);
		vector<string> myV = cmd->tokenize(input, " ");
		for (int i=0;i<myV.size();++i) {
			cout << myV.at(i) << endl;
		}
//		Command myCommand = cmd->getCommand(input);
//		myCommand.printInfo();
//		myCommand.invoke();
	}
}

