/*
 * block.h
 *
 *  Created on: 2014Äê11ÔÂ3ÈÕ
 *      Author: Administrator
 */

#ifndef BLOCK_H_
#define BLOCK_H_
#include<iostream>
using namespace std;

class block {
public:
	int length;//16kb or 4096kb
	string id;//block's id
	char* data;//string or char*
	block();
	virtual ~block();
private:
	bool pinned;
	bool dirty;
};
#endif /* BLOCK_H_ */
