/*
 * block.cpp
 *
 *  Created on: 2014年11月3日
 *      Author: emmachen
 */
#include <iostream>
#include "block.h"
using namespace std;

block::block() {

	this->initial();
}

block::~block() {

}
void block::initial(){
	blockNum=-1;
	fileName="";
	iTime=0;
	dirty=false;
	pin=0;
	writen=false;
	for(int i=0;i<BLOCK_LEN;i++)
		data[i]='@';
    data[BLOCK_LEN]='\0';
}


