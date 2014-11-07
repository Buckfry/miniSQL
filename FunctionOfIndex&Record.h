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
class location
{

};
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
	vector<string> condition;// = or <= or >= or < or >
	vector<string> value;
	vector<string> logic;//"and" or "or"
};

//Function for Index
///////////////////////////////////////////////////////////////////////
//API to index
void createindex_index(attribute_info atb_info,string indexname);
//throw exception "createindex_error"

void dropindex(string indexname);
//throw exception "dropindex_error"

vector<location> searchbyindex(search_info info);//return the position
//throw exception "not_find"

void insert_index(search_info info);//find the position
//throw exception "insertindex_error"

vector<location> delete_index(search_info info);//find the position
//throw exception "deleteindex_error"




//////////////////////////////////////////////////////////////////////
//API to record
attribute_info& createindex_record(string tablename, string atbname);
//throw exception "getcolumn_error"

vector<string> search_with_index(vector<string> position);
vector<string> search_without_index(search_info info);
//throw exception "not_find"

void insert_record(vector<string> values);
//throw exception "insertdata_error"

void delete_record_withindex(vector<string> values,location position);
void detele_record_withoutindex(vector<string> values);
//throw exception "deletedata_error"

void create_table(string tablename,vector<string> attribute_name,vector<string> attribute_type,
		vector<int> attribute_length,string primarykey);
//throw exception "createtable_error"

void drop_table(string tablename);
//throw exception "droptable_error"



#endif /* FUNCTIONOFINDEX_RECORD_H_ */
