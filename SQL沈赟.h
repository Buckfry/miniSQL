/*
 * SQL.h
 *
 *  Created on: 2014年11月2日
 *      Author: yunshen
 */

#ifndef SQL_H_
#define SQL_H_
#include<string>
#include<vector>
using namespace std;
string Current_Dataname="";
class SQL{
	string targetName;
	int argc;
public:
	vector<string> argv;//属性名
	vector<int> type;//属性的类型，int,char,float
	vector<int> isUnique;//是否primary=1,unique=2,not null=3
	vector<int> length;
	vector<string> value;
	vector<string> result;
	vector<string> aux;
	string gettargetName(){
		return targetName;
	}
	int getargc(){
		return argc;
	}
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





#endif /* SQL_H_ */
