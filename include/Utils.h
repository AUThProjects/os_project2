/*
 * Utils.h
 *
 *  Created on: Jan 3, 2015
 *      Author: anagnoad
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <vector>
#include <cstring>
using namespace std;

class Utils {
public:
	Utils();
	virtual ~Utils();
	static constexpr char* processesFile = "/tmp/processesFile.txt";
	static vector<string>* tokenize(string commandString, const char* delimiter);
};

#endif /* UTILS_H_ */
