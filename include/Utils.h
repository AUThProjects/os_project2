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

#define EXIT_OK 0
#define EXIT_SCHEDULER_INIT_FAIL -1

using namespace std;

class Utils {
public:
	static vector<string>* tokenize(string commandString, const char* delimiter);
private:
	Utils();
	virtual ~Utils();
};
#endif /* UTILS_H_ */
