/*
 * interpreter.h
 *
 *  Created on: 2014年11月5日
 *      Author: Even
 */

#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "sql.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cassert>



void initSQL(SQL &sql);
void initInterpreter();

bool interpret(istringstream& in, SQL& sql);

string getquery(istream& in = cin);
string getLastError();  //输出错误信息



#endif /* INTERPRETER_H_ */
