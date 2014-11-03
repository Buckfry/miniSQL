/*
 * block.h
 *
 *  Created on: 2014��11��3��
 *      Author: Administrator
 */

#ifndef BLOCK_H_
#define BLOCK_H_
#include<iostream>
using namespace std;

#define BLOCK_LEN			4096	// the size of one block

//决定不要filetype了，效率变低了，能实现就好，所以张卉和李屹淳协调下，文件名不要设一样的
class block {
public:
	int blockNum;//block在文件中的顺序
	string fileName;
	int len;
	char data[BLOCK_LEN];//string or char*
	int iTime;//for LRU
	block();
	virtual ~block();
	initial();
private:
	bool pin;
	bool dirty;
	bool writen;
};
#endif /* BLOCK_H_ */
