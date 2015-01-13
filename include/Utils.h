/*
 * Utils.h
 *
 *  Created on: Jan 3, 2015
 *      Author: anagnoad
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <string>
using namespace std;

class Utils {
public:
	Utils();
	virtual ~Utils();
	static constexpr char* processesFile = "/tmp/processesFile.txt";
};

#endif /* UTILS_H_ */
