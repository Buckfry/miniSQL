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
	string table_name;
	string attribute_name;
	string type;//int, float or char
	string index_name;
	int lengh;//type's length(especially for char)
	vector<string> data;
	vector<string> position;
};

class search_info
{
public:
	string tablename;
	string atbname;//attribute name
	string index_name;
	vector<string> condition;// = or <= or >= or < or >
	vector<location> value;
};

//Function for Index
///////////////////////////////////////////////////////////////////////
//API to index
void createindex_index(attribute_info info);
//throw exception "createindex_error"

void dropindex(string indexname);
//throw exception "dropindex_error"

vector<location> searchbyindex(search_info info);//return the position
//throw exception "not_find"

void insert_index(attribute_info info);
//throw exception "insertindex_error"

vector<location> delete_index(search_info info);//find the position
//throw exception "deleteindex_error"




//////////////////////////////////////////////////////////////////////
//API to record
attribute_info& createindex_record(string tablename, string atbname);
//throw exception "getcolumn_error"

vector<string> search_with_index(vector<string> position,search_info info);
vector<string> search_without_index(search_info info);
//throw exception "not_find"

attribute_info& insert_record(vector<string> values);//返回插入成功后的数据所在位置
//throw exception "insertdata_error"

void delete_record_withindex(vector<string> values,location position);
void detele_record_withoutindex(vector<string> values);
//throw exception "deletedata_error"

void create_table(string tablename,vector<string> attribute_name,vector<string> attribute_type,
			vector<int> attribute_length,vector<string> attribute_property);//这里的attribute_property是指attribute
																	//是primary key还是unique或者什么都不是
//throw exception "createtable_error"

void drop_table(string tablename);
//throw exception "droptable_error"



#endif /* FUNCTIONOFINDEX_RECORD_H_ */
