/*
 * sql.h
 *
 *  Created on: 2014��11��3��
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
	SqlMethod       method;			//���ִ�еķ���

	private:
	int             argc;				//����в�������Ŀ
    string          targetName;		//����Ŀ�����/���ݿ���/������

	public:
	 vector < string > argv;			//����еĲ���
	 vector < string > value;			//����в�����Ӧ����ֵ
	 vector < string > result;			//select�����ѡ���ӡ������
	 vector < Operator > operators;		//where�еķ���
	 vector < LogicalOperator > logicalOperators;//where�е��߼���Ŀǰֻ֧��and
	 vector < Type > types;			//create tableʱ�����Ͳ���
	 vector < string >& aux;			//create tableʱ��char����ֵ
	 vector < int > isUnique;			//create table��Щ��unique

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
    INT_PRIMARY ,  //��ʾ��������primary key
    FLOAT_PRIMARY ,
    CHAR_PRIMARY ,
};
#endif // _SQL_H_




