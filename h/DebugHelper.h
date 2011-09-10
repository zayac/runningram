/*
 * DebugHelper.h
 *
 *  Created on: Sep 10, 2011
 *      Author: necto
 */

#ifndef DEBUGHELPER_H_
#define DEBUGHELPER_H_

#include "Logger.h"

class Canvas;

class DebugHelper
{
public:
	static Canvas* c;
	static bool showDBG;

	//TODO add easy error reporting

public:
	DebugHelper();
	virtual ~DebugHelper();
};

#endif /* DEBUGHELPER_H_ */
