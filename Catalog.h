/*
 * Catalog.h
 *
 *  Created on: 2014��11��1��
 *      Author: yunshen
 */

#ifndef CATALOG_H_
#define CATALOG_H_
#include"SQL.h"
using namespace std;
class Catalog{
public:
	Catalog();
	~Catalog();
	void CreateDatabase(SQL *b);
	void DropDatabase(SQL *b);
	void CreateTable(SQL *b);
	void DropTable(SQL *b);
	IndexInfo CreateIndex(SQL *b);
	void DropIndex(SQL *b);
	void UseDatabase(SQL *b);
	void QuitDatabase();
	IndexInfo Select(SQL *b);
    IndexInfo Delete(SQL *b);
    IndexInfo Insert(SQL *b);
	TableInfo ShowTable(SQL *b);
	void execFile(SQL *b);
	void helpInfo();
private:
	int findtable(SQL *b,string tablename);
	bool attrfind(string tablename,string attrname);//���������Ƿ���ڱ���
	string attrindex(string tablename,string attrname);//���������Ƿ���index,��ʱ����indexname�����򷵻�NULL
	string findprimary(string tablename);//Ѱ��������
	int findpost(string tablename);//���Ҷ�Ӧ��table�������ֵ��ļ��ľ���λ��
	int findattrnum(int tablepost);//�������Ե�����
	int findattrtype(int tablepost,int n);//�������Ե�����
	int findattrlength(int tablepost,int n);//�������Եĳ���
	static const string DATA_DIR;
	static const string CONFIG;
};





#endif /* CATALOG_H_ */
