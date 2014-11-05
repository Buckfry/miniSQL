/*
 * Error.h
 *
 *  Created on: 2014��11��1��
 *      Author: yunshen
 */

#ifndef ERROR_H_
#define ERROR_H_
#include<string>
#include<stdlib.h>
#include<iostream>
#include<exception>
using namespace std;


class Error :public exception {

private:
	string data;
	string message;
public:
	Error(int code,string text) ;
	void printError() const throw();
	string getMessage();
	~Error() throw (){}
};




#endif /* ERROR_H_ */
