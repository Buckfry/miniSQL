/*
 * sql.h
 *
 *  Created on: 2014年11月3日
 *      Author:
 */

#ifndef SQL_H_
#define SQL_H_


#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <list>

using namespace std;

string Current_Dataname="";


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

enum Operator {
    EQU=0,
	NEQ=1,
    GRE=2,
	GEQ=3,
    LES=4,
    LEQ=5 ,
};

enum LogicalOperator {
    OR=0,
	AND=1,
};



class SQL {
public:
	SqlMethod       method;			//语句执行的方法

	int             argc;				//语句中参数的数目
    string          targetName;		//语句的目标表名/数据库名/索引名
	vector < string > argv;			//语句中的参数
	vector < string > value;			//语句中参数对应的数值
	vector < string > result;			//select语句中选择打印的属性
	vector < Operator > operators;		//where中的符号
	vector < LogicalOperator > logicalOperators;//where中的逻辑，目前只支持and
	vector < int > types;			//create table时的类型参数   int=2,float=3,char=1;
	vector < string > aux;			//create table时的各个类型的长度
	vector < int > isUnique;			//create table哪些是unique


};

class IndexInfo{
public:
	vector<string> indexname;
	vector<string> attrname;
};
class TableInfo{
public:
	vector<string> attrname;
	vector<int> type;//char=1,int=2,float=3
	vector<int> attrtype;//primary=1,unique=2,not null=3
	vector<int> length;
};





#endif // _SQL_H_




