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


class SQL {
	public:
	SqlMethod       method;			//语句执行的方法

	private:
	int             argc;				//语句中参数的数目
    string          targetName;		//语句的目标表名/数据库名/索引名

	public:
	 vector < string > argv;			//语句中的参数
	 vector < string > value;			//语句中参数对应的数值
	 vector < string > result;			//select语句中选择打印的属性
	 vector < Operator > operators;		//where中的符号
	 vector < LogicalOperator > logicalOperators;//where中的逻辑，目前只支持and
	 vector < Type > types;			//create table时的类型参数
	 vector < string >& aux;			//create table时的char的数值
	 vector < int > isUnique;			//create table哪些是unique

	public:
	  int getargc();
	  string gettargetName();

};

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
    EQU,
	NEQ,
    GRE,
	GEQ,
    LES ,
    LEQ ,
};

enum LogicalOperator {
    OR,
	AND,
};

enum Type {
    NOTYPE,
    INT ,
    FLOAT,
    WITHARGUMENT,
    CHAR ,
    INT_PRIMARY ,  //表示该类型是primary key
    FLOAT_PRIMARY ,
    CHAR_PRIMARY ,
};
#endif // _SQL_H_




