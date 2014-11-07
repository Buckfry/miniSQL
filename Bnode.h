/*
 * Bnode.h
 *
 *  Created on: 2014��11��2��
 *      Author: Administrator
 */

#ifndef BNODE_H_
#define BNODE_H_
#include<iostream>
#include<FunctionOfIndex&Record.h>
using namespace std;

class Bnode {
public:
	Bnode();
	virtual ~Bnode();
	int point_num;
	bool isleaf;
	vector<string> values;
	vector<string> pointer;
	string father;
	string next;

public:
	string search1(attribute_info atb_info);
	vector<string> search2(attribute_info atb_info);
	vector<string> search3(attribute_info atb_info);


};

#endif /* BNODE_H_ */
