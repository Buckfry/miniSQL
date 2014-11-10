/*
 * RecordManager.cpp
 *
 *  Created on: 2014年11月5日
 *      Author: zhanghui2014
 */

#include "Recordmanager.h"
#include"sql.h"
#include"block1.h"
#include"string.h"
#include"BufferManager.h"
#include<iterator>
#include<algorithm>
#include<bitset>
#include<utility>
#include<sstream>


RecordManager::RecordManager() {
	// TODO Auto-generated constructor stub

}

RecordManager::~RecordManager() {
	// TODO Auto-generated destructor stub
}

void RecordManager::focus_current_db(string DB_name)
{
	if(DB_name!=databuffer.DBname)
					databuffer.setDatabase(DB_name);
}
//释放数据库的内存缓冲区
void RecordManager::Close_Database(string DB_name)
{
	this->focus_current_db(DB_name);
        databuffer.closeDatabase();
}
//释放表或索引的内存缓冲区
void RecordManager::Close_File(string DB_name, string filename)
{
	if(DB_name!=databuffer.DBname)
					databuffer.setDatabase(DB_name);
	databuffer.closeFile(filename);
}
//把fileinfo转为char*存储
char* RecordManager::translate_fileinfo(record_fileInfo fi)
{
	char translate[MAX_ATTR_NUM];
	int i =0;
	  for(vector<char>::iterator it=fi.attribute_name.begin();it!=fi.attribute_name.end();it++,i++)
	  {
		  translate[i]=*it;
	  }
	 for(vector<char>::iterator it=fi.attribute_type.begin();it!=fi.attribute_type.end();it++,i++)
	 	  {
		 translate[i]=*it;
	 	  }
	 string temp(&translate);
     stringstream tp;
     tp<<fi.recordAmount<<fi.recordLength<<fi.recordcount<<fi.currentblocknum<<fi.fileName;
     temp+=tp.str();
       return  &(temp.c_str());
}
//把数组里的数据写到fi里面去
void RecordManager::getfileinfo(char* fileinfo)
{
	int i=0;
	for(vector<char>::iterator it=fi.attribute_name.begin();it!=fi.attribute_name.end();it++,i++)
		  {
			 *it=fileinfo[i];
		  }
	for(vector<char>::iterator it=fi.attribute_type.begin();it!=fi.attribute_type.end();it++,i++)
		 	  {
			 *it=fileinfo[i];
		 	  }
    string temp(&(fileinfo[i]));
    stringstream tp;
    temp>>tp;
    tp>>fi.recordAmount>>fi.recordLength>>fi.recordcount>>fi.currentblocknum>>fi.fileName;
}
//在建表的时候初始化它的头文件信息
void RecordManager::create_table(string DB_name,create_record data)
{
	this->focus_current_db(DB_name);

	fi.fileName=data.table_name;
	fi.recordLength=0;
	fi.attribute_name=data.attribute_name;
	fi.attribute_type=data.attribute_type;

	for(vector<string>::iterator it=data.attribute_name.begin();it!= data.attribute_name.end();it++,fi.recordLength++);

    fi.recordAmount=BLOCK_LEN/fi.recordLength;
    fi.currentblocknum=1;
    fi.recordcount=0;

    databuffer.createTable(data.table_name,translate_fileinfo(this-> fi));

}
//向表中插入元组,同时返回插入位置
recordposition& RecordManager::insert_record(string DB_name,string filename,vector<char> attr)
{
	this->focus_current_db(DB_name);
    getfileinfo(databuffer.getFileInfo(filename));
    block datablock;
    recordposition rp;

     if(fi.recordcount>fi.recordAmount)
     {
    	 if(fi.currentblocknum>MAX_BLOCK)
    		 throw("This table is filled, you cann't add this tulpe into it!");
    	 else
    	 {
    		 fi.currentblocknum++;
    		 fi.recordcount=0;
    	 }

     }


    int i=fi.recordcount*fi.recordLength;
    datablock(*(databuffer.readBlock(filename,(fi.currentblocknum-1))));

    for(vector<char>::iterator it=attr.begin(); it!=attr.end();it++,i++)
    {
    	datablock.data[i]=*it;

    }

    fi.recordcount++;
    char* p=&(datablock.data);
    databuffer.insertBlock(filename,(fi.currentblocknum-1),p);
    databuffer.updateFileInfo(filename,this->translate_fileinfo(fi));
    rp.blocknum=(fi.currentblocknum-1);
    rp.recordnum=fi.recordcount;
    return rp;
}
//打印属性
void RecordManager::Print(vector<char> attr)
{
	for(vector<char>::iterator it=attr.begin();it!=attr.end();it++)
	{
		cout<<"\t"<<*it;
	}
	cout<<endl;
}
//找出所查找的属性在所有属性中的位置
vector<int>& RecordManager::findposition(vector<char>attr,vector<char>searchedattr)
{
	vector<int> selected_attr;
    for(vector<char>::iterator it=searchedattr.begin();it!=searchedattr.end();it++)
    {
    	   vector<char>::iterator findit = find(attr.begin(),attr.end(),*it);
    	   int num=0;
           for(vector<char>::size_type n=findit-attr.begin();n--!=0;num++)
           {
        	   selected_attr.push_back(num);
           }
    }
    return selected_attr;
}
//选择语句（无where）且无索引
void RecordManager::Select_Without_Useful_No_Where(string DB_name,string filename,vector<char> attr)
{
	this->focus_current_db(DB_name);
    getfileinfo(databuffer.getFileInfo(filename));
	block datablock;
	vector<int> attrnum(findposition(fi.attribute_name,attr));
	for(int searchblock=0;searchblock!=fi.currentblocknum;searchblock++)
	    {
	    	datablock(*(databuffer.readBlock(filename,searchblock)));
	    	for(int searchrecord=0;searchrecord!=fi.recordAmount;searchrecord++)
	    	{
	    		vector<char> attrvalue;
	    		for(vector<int>::iterator it=attrnum.begin();it!=attrnum.end();it++)
	    		{
	    			attrvalue.push_back(datablock.data[searchrecord*fi.recordLength+*it]);
	    		}
	    		this->Print(attrvalue);
	    	}
	    }
}
//选择语句（无where）有索引
void RecordManager::Select_With_Useful_No_Where(string DB_name,string filename,vector<char> attr,vector<recordposition> rp)
{
	this->focus_current_db(DB_name);
    getfileinfo(databuffer.getFileInfo(filename));
    block datablock;
    vector<int> attrnum(findposition(fi.attribute_name,attr));
    for(vector<recordposition>::iterator po=rp.begin();po!=rp.end();po++)
    {
    	datablock(*(databuffer.readBlock(filename,(*po).blocknum)));

   		vector<char> attrvalue;
   		for(vector<int>::iterator it=attrnum.begin();it!=attrnum.end();it++)
   		{
   			attrvalue.push_back(datablock.data[(*po).recordnum*fi.recordLength+*it]);
   		}
    	this->Print(attrvalue);
    }
}
//给index提供keyinfo
keyinfo& RecordManager::getkeyinfo(string DB_name,string filename,char keyname)
{
	this->focus_current_db(DB_name);
	getfileinfo(databuffer.getFileInfo(filename));
	vector<char> temp;
	temp.push_back(keyname);
	keyinfo keyinformation;
	vector<pair<char,recordposition>> keys;
	pair<char,recordposition> key;

	block datablock;
	vector<int> attrnum(findposition(fi.attribute_name,temp));
	for(int searchblock=0;searchblock!=fi.currentblocknum;searchblock++)
	    {
	    	datablock(*(databuffer.readBlock(filename,searchblock)));
	    	for(int searchrecord=0;searchrecord!=fi.recordAmount;searchrecord++)
	    	{
	    		vector<char> attrvalue;
	    		for(vector<int>::iterator it=attrnum.begin();it!=attrnum.end();it++)
	    		{
	    			key.first=datablock.data[searchrecord*fi.recordLength+*it];
	    			key.second.blocknum=searchblock;
	    			key.second.recordnum=searchrecord;
	    			keys.push_back((key));
	    		}
	    	}
	    }
	keyinformation.keyname=keyname;
	keyinformation.keys=keys;
	return keyinformation;
}
//删除语句（无where)
void RecordManager::Delete_No_Where(string DB_name,string filename)
{
	this->focus_current_db(DB_name);
    getfileinfo(databuffer.getFileInfo(filename));
	block datablock;
	for(int searchblock=0;searchblock!=fi.currentblocknum;searchblock++)
	    {
	    	datablock(*(databuffer.readBlock(filename,searchblock)));
	    	for(int searchrecord=0;searchrecord!=fi.recordAmount;searchrecord++)
	    	{
	    		for(int i =0;i!=fi.recordLength;i++)
	    		{
	    			datablock.data[searchrecord*fi.recordLength+i]="NULL";
	    		}
	    	}
	        char* p=&(datablock.data);
	        databuffer.updateBlock(filename,searchblock,p);
	    }
	fi.currentblocknum=1;
	fi.recordcount=0;
    databuffer.updateFileInfo(filename,this->translate_fileinfo(fi));
}
//判断是否符合条件condition
bool RecordManager::Confirm(vector<char> attr,vector<char> attrvalue, condition_info condition)
{
	 vector<int> conditionattrnum(findposition(attr,condition.conditionattr));
	 vector<int> conditionattrtypenum(findposition(fi.attribute_name,condition.conditionattr));
	 vector<char>::iterator it;
	 vector<char>::iterator itype;
	 int i=0;
	for(it=conditionattrnum.begin(),itype=conditionattrtypenum.begin();it!=conditionattrnum.end();it++,i++,itype++)
	{
		//得到所比较属性的类型fi.attribute_type[*it]
		//得到左比较值attr[*it]，进行比较
		switch(condition.condition[i]) //比较条件
		{
		case 0: // ‘=’
			{
				if(fi.attribute_type[*itype]=='int')
				{
					int i1 = (int)attr[*it];
					int i2 = (int)condition.comparedvalue;
					if(i1==i2) continue;
					else return 0;
				}
				else if(fi.attribute_type[*itype] == 'char')
				{
					char *p1 = (LPSTR)(LPCTSTR)attr[*it];
					char *p2 = (LPSTR)(LPCTSTR)condition.comparedvalue;
					if(!strcmp(p1,p2)) continue;
					else  return 0;
				}
				else if(fi.attribute_type[*itype] == 'float')
				{
					float f1 = (float)atof((char *)(LPTSTR)(LPCTSTR)attr[*it]);
					float f2 = (float)atof((char *)(LPTSTR)(LPCTSTR)condition.comparedvalue);
					if(f1==f2) continue;
					else  return 0;
				}

			}
			break;
		case 1://‘<>’
		{
			if(fi.attribute_type[*itype]=='int')
			{
				int i1 = (int)attr[*it];
				int i2 = (int)condition.comparedvalue;
				if(i1!=i2) continue;
				else return 0;
			}
			else if(fi.attribute_type[*itype] == 'char')
			{
				char *p1 = (LPSTR)(LPCTSTR)attr[*it];
				char *p2 = (LPSTR)(LPCTSTR)condition.comparedvalue;
				if(strcmp(p1,p2)) continue;
				else  return 0;
			}
			else if(fi.attribute_type[*itype] == 'float')
			{
				float f1 = (float)atof((char *)(LPTSTR)(LPCTSTR)attr[*it]);
				float f2 = (float)atof((char *)(LPTSTR)(LPCTSTR)condition.comparedvalue);
				if(f1!=f2) continue;
				else  return 0;
			}

		}
		    break;

		case 2: // '<'
			{
				if(fi.attribute_type[*itype]=='int')
				{
					int i1 = (int)attr[*it];
					int i2 = (int)condition.comparedvalue;
					if(i1<i2) continue;
					else return 0;
				}
				else if(fi.attribute_type[*itype] == 'char')
				{
					char *p1 = (LPSTR)(LPCTSTR)attr[*it];
					char *p2 = (LPSTR)(LPCTSTR)condition.comparedvalue;
					if(strcmp(p1,p2)<0) continue;
					else  return 0;
				}
				else if(fi.attribute_type[*itype] == 'float')
				{
					float f1 = (float)atof((char *)(LPTSTR)(LPCTSTR)attr[*it]);
					float f2 = (float)atof((char *)(LPTSTR)(LPCTSTR)condition.comparedvalue);
					if(f1<f2) continue;
					else  return 0;
				}

			}
			break;
		case 3: // '>'
		{
			if(fi.attribute_type[*itype]=='int')
			{
				int i1 = (int)attr[*it];
				int i2 = (int)condition.comparedvalue;
				if(i1>i2) continue;
				else return 0;
			}
			else if(fi.attribute_type[*itype] == 'char')
			{
				char *p1 = (LPSTR)(LPCTSTR)attr[*it];
				char *p2 = (LPSTR)(LPCTSTR)condition.comparedvalue;
				if(strcmp(p1,p2)>0) continue;
				else  return 0;
			}
			else if(fi.attribute_type[*itype] == 'float')
			{
				float f1 = (float)atof((char *)(LPTSTR)(LPCTSTR)attr[*it]);
				float f2 = (float)atof((char *)(LPTSTR)(LPCTSTR)condition.comparedvalue);
				if(f1>f2) continue;
				else  return 0;
			}

		}
		    break;

		case 4: // '<='
		{
			if(fi.attribute_type[*itype]=='int')
			{
				int i1 = (int)attr[*it];
				int i2 = (int)condition.comparedvalue;
				if(i1<=i2) continue;
				else return 0;
			}
			else if(fi.attribute_type[*itype] == 'char')
			{
				char *p1 = (LPSTR)(LPCTSTR)attr[*it];
				char *p2 = (LPSTR)(LPCTSTR)condition.comparedvalue;
				if(strcmp(p1,p2)<=0) continue;
				else  return 0;
			}
			else if(fi.attribute_type[*itype] == 'float')
			{
				float f1 = (float)atof((char *)(LPTSTR)(LPCTSTR)attr[*it]);
				float f2 = (float)atof((char *)(LPTSTR)(LPCTSTR)condition.comparedvalue);
				if(f1<=f2) continue;
				else  return 0;
			}

		}
			break;
		case 5: // '>='
		{
			if(fi.attribute_type[*itype]=='int')
			{
				int i1 = (int)attr[*it];
				int i2 = (int)condition.comparedvalue;
				if(i1>=i2) continue;
				else return 0;
			}
			else if(fi.attribute_type[*itype] == 'char')
			{
				char *p1 = (LPSTR)(LPCTSTR)attr[*it];
				char *p2 = (LPSTR)(LPCTSTR)condition.comparedvalue;
				if(strcmp(p1,p2)>=0) continue;
				else  return 0;
			}
			else if(fi.attribute_type[*itype] == 'float')
			{
				float f1 = (float)atof((char *)(LPTSTR)(LPCTSTR)attr[*it]);
				float f2 = (float)atof((char *)(LPTSTR)(LPCTSTR)condition.comparedvalue);
				if(f1>=f2) continue;
				else  return 0;
			}

		}
			break;
		default:
			{cout<<"An error in record_200+ condition"<<endl;
			exit(1);}
		}
	}
		return 1;
}
//有where，但无可用索引的select语句
void RecordManager::Select_Without_Useful_Cond(string DB_name,string filename,vector<char> attr,condition_info cond)
{
	this->focus_current_db(DB_name);
    getfileinfo(databuffer.getFileInfo(filename));
    block datablock;
    vector<int> attrnum(findposition(fi.attribute_name,attr));


    for(int searchblock=0;searchblock!=fi.currentblocknum;searchblock++)
    {
    	datablock(*(databuffer.readBlock(filename,searchblock)));
    	for(int searchrecord=0;searchrecord!=fi.recordAmount;searchrecord++)
    	{
    		vector<char> attrvalue;
    		for(vector<int>::iterator it=attrnum.begin();it!=attrnum.end();it++)
    		{
    			attrvalue.push_back(datablock.data[searchrecord*fi.recordLength+*it]);
    		}
    		if(this->Confirm(attr,attrvalue,cond))
    		this->Print(attrvalue);
    	}
    }

}
//有where,但无可用索引的delete语句
vector<recordposition>& RecordManager::Delete_Without_Useful_Cond(string DB_name,string filename,vector<char> attr, condition_info cond)
{
	this->focus_current_db(DB_name);
	 getfileinfo(databuffer.getFileInfo(filename));
	 recordposition rp;
	 vector<recordposition> rps;
	    block datablock;
	    vector<int> attrnum(findposition(fi.attribute_name,attr));


	    for(int searchblock=0;searchblock!=fi.currentblocknum;searchblock++)
	    {
	    	datablock(*(databuffer.readBlock(filename,searchblock)));
	    	for(int searchrecord=0;searchrecord!=fi.recordAmount;searchrecord++)
	    	{
	    		vector<char> attrvalue;
	    		for(vector<int>::iterator it=attrnum.begin();it!=attrnum.end();it++)
	    		{
	    			attrvalue.push_back(datablock.data[searchrecord*fi.recordLength+*it]);
	    		}
	    		if(this->Confirm(attr,attrvalue,cond))
	    		{
	    			for(int i =0; i!=fi.recordLength;i++)
	    			{
	    				datablock.data[searchrecord*fi.recordLength+i]="NULL";//已删除的record仍旧占用空间，不更新fileinfo
	    			}
	    			rp.blocknum=searchblock;
	    			rp.recordnum=searchrecord;
	    			rps.push_back(rp);
	    		}
	    	}
	        char* p=&(datablock.data);
	        databuffer.updateBlock(filename,searchblock,p);
	    }
	    return rps;
}
//有where，有索引可用的select语句
void RecordManager::Select_With_Useful_Cond(string DB_name,string filename,vector<char> attr,vector<recordposition> rp,condition_info cond)
{
	this->focus_current_db(DB_name);
    getfileinfo(databuffer.getFileInfo(filename));
    block datablock;
    vector<int> attrnum(findposition(fi.attribute_name,attr));
    for(vector<recordposition>::iterator po=rp.begin();po!=rp.end();po++)
    {
    	datablock(*(databuffer.readBlock(filename,(*po).blocknum)));

   		vector<char> attrvalue;
   		for(vector<int>::iterator it=attrnum.begin();it!=attrnum.end();it++)
   		{
   			attrvalue.push_back(datablock.data[(*po).recordnum*fi.recordLength+*it]);
   		}
   		if(this->Confirm(attr,attrvalue,cond))
    	this->Print(attrvalue);
    }
}
//有where,有可用索引的delete语句,同时返回删除位置
vector<recordposition>& RecordManager::Delete_With_Useful_Cond(string DB_name,string filename,vector<char> attr,vector<recordposition> rp,condition_info cond)
{
	this->focus_current_db(DB_name);
	 getfileinfo(databuffer.getFileInfo(filename));
	 recordposition resultrp;
	 vector<recordposition> resultrps;
	    block datablock;
	    vector<int> attrnum(findposition(fi.attribute_name,attr));


	    for(vector<recordposition>::iterator po=rp.begin();po!=rp.end();po++)
	    {
	    	datablock(*(databuffer.readBlock(filename,(*po).blocknum)));
	   		vector<char> attrvalue;
	    	for(vector<int>::iterator it=attrnum.begin();it!=attrnum.end();it++)
	    	{
	    		attrvalue.push_back(datablock.data[(*po).recordnum*fi.recordLength+*it]);
	    	}
	    	if(this->Confirm(attr,attrvalue,cond))
	    	{
	    		for(int i =0; i!=fi.recordLength;i++)
	    		{
	    			datablock.data[((*po).recordnum)*fi.recordLength+i]="NULL";//已删除的record仍旧占用空间，不更新fileinfo
	    		}
	    		resultrp.blocknum=(*po).blocknum;
	    		resultrp.recordnum=(*po).recordnum;
	    		resultrps.push_back(resultrp);
	    	}
	       char* p=&(datablock.data);
	       databuffer.updateBlock(filename,(*po).blocknum,p);
	   }
	    return resultrps;
}











