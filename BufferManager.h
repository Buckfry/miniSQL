/*
 * BufferManager.h
 *
 *  Created on: 2014年11月1日
 *      Author: emmachen
 */

#ifndef BUFFERMANAGER_H_
#define BUFFERMANAGER_H_

#include <string>
#include <set>
#include <stack>
#include <vector>
#include "block1.h”
using namespace std;

#define MAX_FILE_ACTIVE 	5		// limit the active files in the buffer
#define MAX_BLOCK			40		// the max number of the blocks
#define OFFSET				16

class fileHead{
public:
	string filename;
	int age;
	char fileInfo[OFFSET];
	fileHead();
	~fileHead();
	void initFileHead();
//private:
	bool dirty;
	bool writen;
};

class BufferManager{
public:
	string DBname;
	 block Buffer[MAX_BLOCK];
	fileHead fileHlist[MAX_FILE_ACTIVE];

	BufferManager(string DB_Name);//initial BufferManager with a DB_name
	~BufferManager();
	//interface
	void setDatabase(string DB_Name);
	string getDatabase();

	void closeFile(string fileName);
	void closeDatabase();

	char* getFileInfo(string fileName);
	void updateFileInfo(string fileName,char* fileInfo);

	void createTable(string tableName,char* fileInfo);
	void deleteTable(string tableName);



	block* readBlock(string fileName, int blockNum);
	void updateBlock(string fileName,int blockNum,char*newdata);
	void insertBlock(string fileName,int blockNum,char*data);

private:
	void initBuffer();

	block* findFreeBlock();
	fileHead* findFreeFileHead();

	void writeBlock(block * w_block);//to disk
	void Get_Block_Disk(string File_Name,int blockNum,char data[]);//from disk
	void Get_File_Info_Disk(string File_Name,char getInfo[]);//from disk
	void Write_File_Info_Disk(fileHead *fileH);//to disk


};


#endif /* BUFFERMANAGER_H_ */
