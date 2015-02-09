/*
 * Utils.cpp
 *
 *  Created on: Jan 3, 2015
 *      Author: anagnoad
 */

#include "../include/Utils.h"

Utils::Utils() {
}

Utils::~Utils() {
}

// tokenizer for Command Prompt (works with different delimeters)
vector<string>* Utils::tokenize(string commandString, const char* delimiter) {
	int stringLength = commandString.length();
	int delimiterLength = strlen(delimiter);
	int* delimiterPositions = new int[30]; // keeps track of the first (potentially) delimiter char
	delimiterPositions[0] = -delimiterLength;
	int numberOfDelimiters = 1; // size of delimiterPositions
	int currentDelimiter = commandString.find(delimiter, delimiterPositions[0]+ delimiterLength); // +delimiterLength
	while (currentDelimiter != string::npos) {
		delimiterPositions[numberOfDelimiters++] = currentDelimiter;
		currentDelimiter = commandString.find(delimiter, delimiterPositions[numberOfDelimiters-1] + delimiterLength);
	}
	delimiterPositions[numberOfDelimiters++] = stringLength;

//	|--------DEBUG--------|
//	cout << "Delimiter Positions: " << endl;
//	for(int i=0;i<numberOfDelimiters;++i) {
//			cout << delimiterPositions[i] << " ";
//		}

	vector<string> *toBeReturned = new vector<string>();
	for(int i=0;i<numberOfDelimiters-1;++i) {
		if (delimiterPositions[i]+delimiterLength == delimiterPositions[i+1]) {continue;}; // handling of multiple delimiters in succession
		toBeReturned->push_back(commandString.substr(delimiterPositions[i]+delimiterLength, delimiterPositions[i+1]-(delimiterPositions[i]+delimiterLength)));
	}
	toBeReturned->shrink_to_fit();

//	|--------DEBUG--------|
//	cout << "tokenize() successful" << endl;
//	for (int i=0;i<toBeReturned->size();++i) {
//		cout << toBeReturned->at(i) << " ";
//	}

	return toBeReturned;
}
