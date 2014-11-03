/*
 * block.cpp
 *
 *  Created on: 2014��11��3��
 *      Author: Administrator
 */

#include "block.h"

block::block() {
	// TODO Auto-generated constructor stub
	this->initial();
}

block::~block() {
	// TODO Auto-generated destructor stub
}
void initial(){
	blockNum=-1;
	data=NULL;
	fileName="";
	iTime=0;
	dirty=false;
	pin=0;
	writen=false;

}

