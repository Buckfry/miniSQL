/*
 * Bnode.h
 *
 *  Created on: 2014��11��2��
 *      Author: Administrator
 */

#ifndef BNODE_H_
#define BNODE_H_
#include<iostream>
#include"FunctionOfIndex&Record.h";
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

	void insert(attribute_info attribute);


public:
	string search(attribute_info atb_info);


};

class Binfo
{
public:
	int length=0;
	string indexname="";
	string firstleaf="";//第一个叶节点位置，4字节长
	string empty="";//第一个空块位置，4字节长
	Binfo(string data)
	{
		int i;
		for(i=0;i<4;i++)
		{
			length+=(data[i]-'0')*10;
		}
		indexname = data.substr(4,8);
		firstleaf = data.substr(8,12);
		empty = data.substr(12,16);
	}
};

#endif /* BNODE_H_ */
