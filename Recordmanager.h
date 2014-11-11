/*
 * RecordManager.h
 *
 *  Created on: 2014年11月5日
 *      Author: zhanghui2014
 */

#ifndef RECORDMANAGER_H_
#define RECORDMANAGER_H_

#include<iostream>
#include<string>
#include<FunctionOfIndex&Record.h>
#include"block1.h"
#include"BufferManager.h"
#include"sql.h"
using namespace std;

#define MAX_ATTR_NUM 18


class record_fileInfo {
public:
	string fileName;		// the name of the file, no longer than 2byte
	vector<char> attribute_name;  //no more than 9 attributes
	vector<char> attribute_type;  // int, float or char
	int recordAmount;		// the number of record in the file‘s block
	int recordLength;		// the length of the record in the file
//	fileInfo *next;			// the pointer points to the next file
//	block *firstBlock;	// point to the first block within the file
	int currentblocknum;// the number of blocks which are to be used in the file
	int recordcount;   //the number of record in the current block

};


class condition_info
{
public:
	vector<char> conditionattr;
	vector<char> comparedvalue;
	vector<int>  condition;//0 for = , 1 for <>, 2 for <, 3for >, 4 for<= ,5 for >=
};
class RecordManager {
private:
	BufferManager databuffer;
	fileHead fh;
	record_fileInfo fi;

	//确认所操作的表
	void focus_current_db(string DB_name);
	//将class fileInfo转换为数组来存储
	char* translate_fileinfo(record_fileInfo fi);
	//读入filehead中的fileinfo
	void getfileinfo(char* fileinfo);
	//打印属性
	void Print(vector<char> attr);
	//找出所查找的属性在attr中的位置
	vector<int>& findposition(vector<char> attr,vector<char> searchedattr);
	//判断是否符合指定条件
	bool Confirm(vector<char> attr,vector<char> attrvalue,condition_info condition);

public:
	//构造
	RecordManager();
      //析构
	virtual ~RecordManager();
	//释放数据库的内存缓冲区
	void Close_Database(string DB_name);
	//释放表或索引的内存缓冲区
	void Close_File(string DB_name,string filename);
	//建表时初始化表头文件
	void create_table(string DB_name,create_record data);
	//向表中插入元组,同时返回所插入元组的位置
	recordposition& insert_record(string DB_name,string filename,vector<char> attr);
	//选择语句（无where）且无索引
	void Select_Without_Useful_No_Where(string DB_name,string filename,vector<char> attr);
	//选择语句（无where）有索引
	void Select_With_Useful_No_Where(string DB_name,string filename,vector<char> attr,vector<recordposition> rp);
	//有where，但无可用索引的select语句
	void Select_Without_Useful_Cond(string DB_name,string filename,vector<char> attr,condition_info cond);
	//有where，有索引可用的select语句
	void Select_With_Useful_Cond(string DB_name,string filename,vector<char> attr,vector<recordposition> rp,condition_info cond);
	//删除语句（无where)
	void Delete_No_Where(string DB_name,string filename);
	//有where,但无可用索引的delete语句,同时返回删除位置
	vector<recordposition>& Delete_Without_Useful_Cond(string DB_name,string filename,vector<char> attr,condition_info cond);
	//有where,有可用索引的delete语句,同时返回删除位置
	vector<recordposition>& Delete_With_Useful_Cond(string DB_name,string filename,vector<char> attr,vector<recordposition> rp,condition_info cond);
	//给index提供键的值和位置
	keyinfo& getkeyinfo(string DB_name, string filename,char keyname);

//
//
//	//验证可否插入(prime? unique?)
//	bool Verify_Insertable(string DB_Name,string Table_Name,index_info nodes[32],int count,string Attr);
//	//退出程序
//	void Quit(string DB_Name);

};

#endif /* RECORDMANAGER_H_ */
