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
#include<utility>

using namespace std;
class location
{

};
//位置信息

class create_record{
public:
	string table_name;
	int attribute_num;
//	vector<char> attribute_name;//属性名
//	vector<char> attribute_type;//属性类型
	vector<int> attribute_length;//属性长度(每个属性的字节数)

};

class attr_info{
public:
		vector<string> attribute_name;//属性名
		vector<int> attribute_type;//属性类型


};

class keyinfo{
public:
	string keyname;
	vector<pair<string,recordposition>> keys;//存储键值和对应的位置
};

class recordposition{
	int    recordnum;
	int    blocknum;
};

class create_index_info
{
public:
	string index_name;
	int lengh;//type's length(especially for char)
	string attribute_name;

	int type;//1是char 2是int 3是float

	vector<string> data;
	vector<string> position;
};

class update_index_info
{
public:
	string index_name;//由api赋值
	string value;//数据的值
	location loc;//数据所在位置
};


class search_info
{
public:
	string db_name;
	vector<string> condition;// = or <= or >= or < or >
	vector<string> attribute_name;//attribute name
	vector<string> value;
};

class search_result
{
public:
	vector<string> value;
};//一个search_result即为一条查询结果

//Function for Index
///////////////////////////////////////////////////////////////////////
//API to index
void createindex_index(create_index_info info);
//throw exception "createindex_error"

void dropindex(string indexname);
//throw exception "dropindex_error"

location searchbyindex(search_info info);//返回位置信息
//throw exception "not_find"

void insert_index(update_index_info info);
//throw exception "insertindex_error"

location delete_index(search_info info);//find the position
void delete_index(update_index_info);
//throw exception "deleteindex_error"

//////////////////////////////////////////////////////////////////////
//API to record
create_index_info& createindex_record(string tablename, string atbname);//create index前调用此函数
//因为create index时需要向record获得某属性所有的数据和位置信息
//throw exception "getcolumn_error"

vector<search_result> search_with_index(location loc,search_info info);
vector<search_result> search_without_index(search_info info);
//throw exception "not_find"

update_index_info insert_record(string table_name,vector<string> attribute_index,
								vector<string> values);//返回插入成功后的数据所在位置
//throw exception "insertdata_error"

void delete_record_withindex(location loc);
vector<update_index_info> detele_record_withoutindex(search_info info);
//throw exception "deletedata_error"


void create_table(string DB_name,create_record data);
//throw exception "createtable_error"

void drop_table(string tablename);
//throw exception "droptable_error"



#endif /* FUNCTIONOFINDEX_RECORD_H_ */
