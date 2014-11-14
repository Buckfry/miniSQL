/*
 * Catalog.h
 *
 *  Created on: 2014年11月11日
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
	void CreateTable(SQL *b,string database);
	IndexInfo& DropTable(SQL *b,string database);
	Index& CreateIndex(SQL *b,string database);
	void DropIndex(SQL *b,string database);
	void UseDatabase(SQL *b);
	//void QuitDatabase();
	IndexInfo& Select(SQL *b,string database);
    IndexInfo& Delete(SQL *b,string database);
    IndexInfo& Insert(SQL *b,string database);
	TableInfo& ShowTable(SQL *b,string databse);
	void execFile(SQL *b,string database);
	void helpInfo();
	int findtable(string tablename,string databse);//set public
private:
	//int findtable(SQL *b,string tablename);//set public
	bool attrfind(string database,string tablename,string attrname);//返回属性是否存在表中
	string attrindex(string database,string tablename,string attrname);//返回属性是否有index,有时返回indexname，否则返回NULL
	string findprimary(string database,string tablename);//寻找主索引
	int findpost(string database,string tablename);//查找对应的table在数据字典文件的具体位置
	int findattrnum(string database,int tablepost);//查找属性的数量
	int findattrtype(string database,int tablepost,int n);//查找属性的类型
	int findattrlength(string database,int tablepost,int n);//查找属性的长度
	static const string DATA_DIR;
	static const string CONFIG;
};





#endif /* CATALOG_H_ */
