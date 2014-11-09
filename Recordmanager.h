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

#define FILEINFO_LEN 16

class record_fileInfo {
public:
	string fileName;		// the name of the file
	vector<char> attribute_name;
	vector<char> attribute_type;
	int recordAmount;		// the number of record in the file‘s block
	int recordLength;		// the length of the record in the file
//	fileInfo *next;			// the pointer points to the next file
//	block *firstBlock;	// point to the first block within the file
	int currentblocknum;// the free block number which could be used for the file
	int recordcount;   //the number of record in the current block

};


class condition_info
{
public:
//	char left_type; // i for int, c for char, f for float
	int left_len;
	long left_offset;//对应第几个属性值，-1 对应常量
	string left_index_name;
	char right_type; // i for int, c for char, f for float, t for const
	int right_len;
	long right_offset; //对应第几个属性值，-1 对应常量
	string right_index_name;
	string const_data;
	int condition; //0 for = , 1 for <>, 2 for <, 3for >, 4 for<= ,5 for >=
};

//定义select 语句显示各属性信息

//class attr_info
//{
//public:
//	string attr_name;
//	string index;
//	int offset;
//	int length;
//	char type;
//	bool PrimaryKey;
//	bool unique;
//	int attr;//attr的编号
//
//	attr_info(){
//		PrimaryKey=false;
//		unique=false;
//		index="";
//	}
//};


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

	//打印选择属性
	void Print_Head(vector<char> attr);
	//打印出选择结果
	void Print_To_Screen(vector<char> attr);
	//判断是否符合where条件组
	bool Confirm_To_Where(string record,condition_info conds[10],int count,char cond);
	//判断是否符合某个指定条件
	bool Confirm(string record,condition_info condition);

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

	//向表中插入元组
	void insert_record(string filename,vector<char> attr);

	//选择语句（无where）
	void Select_No_Where(string DB_Name,string filename,vector<char> attr);

	//有where，但无可用索引的select语句
	void Select_Without_Useful_Cond(string DB_Name,string Table_Name,condition_info conds[10],int count,attribute_info print[32],int Count,char cond);
	//有where且有可用索引的select 语句
	void Select_With_Useful_Cond(string DB_Name,string Table_Name,condition_info conds[10],int count,attribute_info print[32],int Count,char cond,int index);
	//用等于索引的选择语句
	void Select_With_Equal_Cond(string DB_Name,string Table_Name,condition_info conds[32],int count,attribute_info print[32],int Count,char cond,index_info Index);
	//用大于或大于等于索引的选择语句
	void Select_With_Greater_Cond(string DB_Name,string Table_Name,condition_info conds[32],int count,attribute_info print[32],int Count,char cond,index_info Index,int type);
	//用小于或小于等于索引的选择语句
	void Select_With_Smaller_Cond(string DB_Name,string Table_Name,condition_info conds[32],int count,attribute_info print[32],int Count,char cond,index_info Index,int type);

	//选择语句（有where）
	void Select_With_Where(string DB_Name,string Table_Name,condition_info conds[10],int count,char cond,attribute_info print[32],int Count);
	//删除语句（有where）
	void Delete_With_Where(string DB_Name,string Table_Name,condition_info conds[10],int count,index_info nodes[32],int num,char cond);
	//有where,但无可用索引的delete语句
	void Delete_Without_Useful_Cond(string DB_Name,string Table_Name,condition_info conds[10],int count,index_info nodes[32],int num,char cond);
	//有where且有可用索引的delete 语句
	void Delete_With_Useful_Cond(string DB_Name,string Table_Name,condition_info conds[10],int count,index_info nodes[32],int num,char cond,int index);
	//用等于索引的删除语句
	void Delete_With_Equal_Cond(string DB_Name,string Table_Name,condition_info conds[10],int count,index_info nodes[32],int num,char cond,index_info Index);
	//用大于或大于等于索引的删除语句
	void Delete_With_Greater_Cond(string DB_Name,string Table_Name,condition_info conds[10],int count,index_info nodes[32],int num,char cond,index_info Index,int type);
	//用小于或小于等于索引的删除语句
	void Select_With_Smaller_Cond(string DB_Name,string Table_Name,condition_info conds[32],int count,index_info nodes[32],int num,char cond,index_info Index,int type);
	//查找有无可用索引
	bool Find_Useful_Cond(condition_info conds[10],int count,int & index);
	//插入所有索引节点
	void Insert_Index_All(string DB_Name,string Table_Name,string Index_Name,int length,int offset,int type);
	//验证可否插入
	bool Verify_Insertable(string DB_Name,string Table_Name,index_info nodes[32],int count,string Attr);
	//退出程序
	void Quit(string DB_Name);

};

#endif /* RECORDMANAGER_H_ */
