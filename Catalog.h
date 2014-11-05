/*
 * Catalog.h
 *
 *  Created on: 2014年11月1日
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
	bool attrfind(string tablename,string attrname);//返回属性是否存在表中
	string attrindex(string tablename,string attrname);//返回属性是否有index,有时返回indexname，否则返回NULL
	string findprimary(string tablename);//寻找主索引
	int findpost(string tablename);//查找对应的table在数据字典文件的具体位置
	int findattrnum(int tablepost);//查找属性的数量
	int findattrtype(int tablepost,int n);//查找属性的类型
	int findattrlength(int tablepost,int n);//查找属性的长度
	static const string DATA_DIR;
	static const string CONFIG;
};





#endif /* CATALOG_H_ */
