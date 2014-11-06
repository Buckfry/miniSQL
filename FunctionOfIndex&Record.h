/*
 * FunctionOfIndex&Record.h
 *
 *  Created on: 2014年11月6日
 *      Author: Administrator
 */

#ifndef FUNCTIONOFINDEX_RECORD_H_
#define FUNCTIONOFINDEX_RECORD_H_
#include<iostream>
#include<vector>

using namespace std;
class attribute_info
{
public:
	string type;//int, float or char
	int lengh;//type's length(especially for char)
	vector<string> data;
};

class search_info
{
public:
	string tablename;
	string atbname;//attribute name
	string condition;// = or <= or >= or < or >
	string value;
};

//Function for Index
///////////////////////////////////////////////////////////////////////
//API to index
void createindex_index(attribute_info atb_info,string indexname);

void dropindex(string indexname);

vector<string> searchbyindex(search_info info);//return the position

void insert_index(search_info info);//find the position
//throw exception "insert_error"

vector<string> delete_index(search_info info);//find the position
//throw exception "delete_error"




//////////////////////////////////////////////////////////////////////
//API to record
attribute_info& createindex_record(string tablename, string atbname);

vector<string> search_with_index(vector<string> position);
vector<string> search_without_index(search_info info);

void insert_record(vector<string> values);
//throw exception "insert_error"

void delete_record_withindex(vector<string> values,string position);
void detele_record_withoutindex(vector<string> values);
//throw exception "delete_error"

void create_table(string tablename,vector<string> attribute_name,vector<string> attribute_type,
		vector<int> attribute_length,string primarykey);
//throw exception "createtable_error"

void drop_table(string tablename);
//throw exception "droptable_error"



#endif /* FUNCTIONOFINDEX_RECORD_H_ */
