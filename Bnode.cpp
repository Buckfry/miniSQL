/*
 * Bnode.cpp
 *
 *  Created on: 2014��11��2��
 *      Author: Administrator
 */

#include "Bnode.h"

Bnode::Bnode() {
	// TODO Auto-generated constructor stub

}

Bnode::~Bnode() {
	// TODO Auto-generated destructor stub
}

//class attribute_info
//{
//public:
//	string table_name;
//	string attribute_name;
//	string type;//int, float or char
//	string index_name;
//	int lengh;//type's length(especially for char)
//	vector<string> data;
//	vector<string> position;
//};
//
//class search_info
//{
//public:
//	string tablename;
//	string atbname;//attribute name
//	string index_name;
//	vector<string> condition;// = or <= or >= or < or >
//	vector<location> value;
//};
void Bnode::createindex_index(attribute_info info)
{
	char data[32];
	int i=0;
	for(;i<4;i++)
	{
		data[i]='0';
	}
	for(;i<8;i++)
	{

	}
}
