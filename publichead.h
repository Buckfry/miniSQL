/*
 * publichead.h
 *
 *  Created on: 2014年11月14日
 *      Author: Administrator
 */

#ifndef PUBLICHEAD_H_
#define PUBLICHEAD_H_
#include "BufferManager.h"
//////////////////////////////////////////////////////////////////
//catalog
//class SQL{
//public:
//	string targetName="";
//	int argc=0;
//	vector<string> argv;//属性名
//	vector<int> type;//属性的类型，int,char,float
//	vector<int> isUnique;//是否primary=1,unique=2,not null=3
//	vector<int> length;
//	vector<string> value;
//	vector<string> result;
//	vector<string> aux;
//	SQL(){}
//	~SQL(){}
//};

////////////////////////////////////////
//公共类


BufferManager datamanager;

class TableInfo{
public:
	vector<string> attrname;
	vector<int> type;//char=1,int=2,float=3
	vector<int> attrtype;//primary=1,unique=2,not null=3
	vector<int> length;
};
class IndexInfo{
public:
	vector<string> indexname;
	vector<string> attrname;
	TableInfo table;
};
class Index{
public:
	int type;
	int length;
};

//属性既不是primary或者unique时，att=0;
//create index的时候attribute 类型的长度和类型



//////////////////////////////////////////////////////////////////
//调用index函数要用到的类
class keyinfo{
public:
	string keyname;
	vector<pair<string,recordposition>> keys;	//存储键值和对应的位置
};


class recordposition{
	int    recordnum;
	int    blocknum;
};
//////////////////////////////////////////////////////////////////
//调用record函数要用到的类
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
		vector<int> attribute_length;//属性长度(每个属性的字节数)

};
////////////////////////////////////////////////////////////////
enum SqlMethod {
	NOP,
	SELECTALL,
	SELECTWHERE,
	INSERT,
	DELETEALL,
	DELETEWHERE,
	CREATEDATABASE,
	CREATETABLE,
	CREATEINDEX,
	DROPDATABASE,
	DROPTABLE,
	DROPINDEX,
	USEDATABASE,
	HELP,
	QUIT,
	EXECFILE,
};



class SQL {
public:
	SqlMethod       method;			//语句执行的方法

	int             argc;				//语句中参数的数目
    string          targetName;		//语句的目标表名/数据库名/索引名
    vector < int >   length;
	vector < string > argv;			//语句中的参数
	vector < string > value;			//语句中参数对应的数值
	vector < string > result;			//select语句中选择打印的属性
	vector < string > operators;		//where中的符号
	vector < string > logicalOperators;//where中的逻辑，目前只支持and
	vector < int > types;			//create table时的类型参数   int=2,float=3,char=1;
	vector < string > aux;			//create index时tablename 存放地
	vector < int > isUnique;			//create table哪些是unique


};





#endif /* PUBLICHEAD_H_ */
