/*
 * Utils.cpp
 *
 *  Created on: Jan 3, 2015
 *      Author: anagnoad
 */

#include "../include/Utils.h"

Utils::Utils() {
	// TODO Auto-generated constructor stub

}

Utils::~Utils() {
	// TODO Auto-generated destructor stub
}

vector<string>* Utils::tokenize(string commandString, const char* delimiter) {
//	cout << "Inside tokenize with delimiter: " << delimiter << "and commandString" << commandString << endl;

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
//	cout << "Delimiter Positions: " << endl;
//	for(int i=0;i<numberOfDelimiters;++i) {
//			cout << delimiterPositions[i] << " ";
//		}

	vector<string> *toBeReturned = new vector<string>();
	for(int i=0;i<numberOfDelimiters-1;++i) {
		if (delimiterPositions[i]+delimiterLength == delimiterPositions[i+1]) {continue;};
		toBeReturned->push_back(commandString.substr(delimiterPositions[i]+delimiterLength, delimiterPositions[i+1]-(delimiterPositions[i]+delimiterLength)));
	}
	toBeReturned->shrink_to_fit();
//	cout << "tokenize() successful" << endl;
//	for (int i=0;i<toBeReturned->size();++i) {
//		cout << toBeReturned->at(i) << " ";
//	}
	return toBeReturned;
}
