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



int notfullblock=0;

RecordManager::RecordManager(string DB_name) {
	// TODO Auto-generated constructor stub

}

RecordManager::~RecordManager() {
	// TODO Auto-generated destructor stub
}

//1.释放数据库的内存缓冲区
void RecordManager::Close_Database()
{
        databuffer.closeDatabase();
}


//2.释放表或索引的内存缓冲区
void RecordManager::Close_File(string filename)
{
	databuffer.closeFile(filename);
}

//3.向表中插入元组
void RecordManager::Insert_Item(string Table_Name,string Attr)
{

	while(1)
	{
		block* tempb = databuffer.;
//				readBlock(DB_Name, Table_Name, notfullblock , m_fileType); //blockNum初定为0，
		if((tempb->data)>=4096) //若有块满了，取下一块
		{
			notfullblock++;
			continue;
		}
		else
		{
			recordoffset = strlen(tempb->cBlock);
			recordoffset += notfullblock*10000;
			strcat(tempb->cBlock,Attr);
			tempb->charNum+=1;
			tempb->dirtyBit=1;
			AddRecord(DB_Name,Table_Name);
			return;
		}
	}
	return;
}

//4.打印选择属性
void RecordManager::Print_Head(attr_info print[32],int count)
{
	for(int i=0;i<count;i++)
	{
		printf("%s\t",print[i].attr_name);
	}
	printf("\n");
}
//5.打印出选择结果
void RecordManager::Print_To_Screen(string record,attr_info print[32],int count)
{
	int start=0;
	int end=-1;
	string temps;
	int flag=0;
	record.Replace(';',',');
	for(;;)
	{
		for(int i=0;i<count;i++)
		{
			end = record.Find(',',start);
			temps = record.Mid(start,end-start);//打印去掉 ，的属性值
			printf("%s\t",temps);
			start=end+1;
		}
		printf("\n");
		record = record.Mid(start);

		flag = record.Find(',',0);
		if(flag==-1)
			break;
		start=0;

	}
	//temps = record.Mid(start,record.GetLength()-start);

}

//6.有where，但无可用索引的select语句
void RecordManager::Select_Without_Useful_Cond(string DB_Name,string Table_Name,condition_info conds[10],int count,attr_info print[32],int Count,char cond)
{
	int m_fileType =0; //文件
	int num=0; //blockNum
	Print_Head(print,Count);//打印头
	while(1)
	{
		blockInfo* tempb = readBlock(DB_Name, Table_Name, num , m_fileType);
		if(tempb->charNum>0) //若该块有数据
		{
			int start=0;
			int end;
			string tempst = tempb->cBlock;
			while((end = tempst.Find(';',start))!=-1)
			{
				string temps = tempst.Mid(start,end-start+1); //取出一条record
				start=end+1;
				if(temps.GetAt(0)==' ')
					continue;
				int res = Confirm_To_Where(temps,conds,count,cond); //条件判断
				if(res==1)
					Print_To_Screen(temps,print,Count); //输出该record
				else
					continue;
			}
			num++; //到下一块
			continue;
		}
		else //若已经到了空块
		{
			return;
		}
	}
	return;
}
//7.有where且有可用索引的select 语句
void RecordManager::Select_With_Useful_Cond(string DB_Name,string Table_Name,condition_info conds[10],int count,attr_info print[32],int Count,char cond, int num, index_info  node)
{
	node.value = conds[num].const_data; //取该条件的值到value里
	if(conds[num].condition==0) //num为索引在第几个条件的属性值上
	{
		Select_With_Equal_Cond(DB_Name,Table_Name,conds,count,print,Count,cond,node);

	}
	else if(conds[num].condition>1 && conds[num].condition<6)
	{
		if(conds[num].condition==5)
		{
			Select_With_Greater_Cond(DB_Name,Table_Name,conds,count,print,Count,cond,node,2);
		}
		else if( conds[num].condition==3)
		{
			Select_With_Greater_Cond(DB_Name,Table_Name,conds,count,print,Count,cond,node,1);
		}
		else if(conds[num].condition==4)
		{
			Select_With_Smaller_Cond(DB_Name,Table_Name,conds,count,print,Count,cond,node,4);
		}
		else if(conds[num].condition==2)
		{
			Select_With_Smaller_Cond(DB_Name,Table_Name,conds,count,print,Count,cond,node,3);
		}
	}
}

//8.用等于索引的选择语句
void RecordManager::Select_With_Equal_Cond(string DB_Name,string Table_Name,condition_info conds[32],int count,attr_info print[32],int Count,char cond,index_info Index)
{
	ResetInform(Index);
	search_one(DB_Name,Table_Name, Index,0); //来自index_M
	if(Index.offset ==-1)
	{
		cout<<"No such record;"<<endl;
		return;
	}
	int m_fileType =0; //文件
	int num=Index.offset/10000;

	blockInfo* tempb = readBlock(DB_Name, Table_Name, num , m_fileType);
	if(tempb == NULL)
	{
		cout<<"error: can't read the block."<<endl;
	}
	else
	{
		string tempst = tempb->cBlock;
		int blockoffset = Index.offset%10000;
		int end = tempst.Find(';',blockoffset);
		string temps = tempst.Mid(blockoffset,end-blockoffset+1);
		int res = Confirm_To_Where(temps,conds,count,cond);
		if(res==1)
		{
			Print_Head(print,Count);
			Print_To_Screen(temps,print,Count);
		}
		return;
	}
}

//9.用大于或大于等于索引的选择语句
void Select_With_Greater_Cond(string DB_Name,string Table_Name,condition_info conds[32],int count,attr_info print[32],int Count,char cond,index_info Index,int type)
{
	int start=0;
	int end=-1;
	ResetInform(Index);

	int m_fileType =0; //文件
	string record;

	search_many(DB_Name, Table_Name,start,end,type, Index) ; //index里的函数
	long offset;
	int blocknum;
	int blockoffset;
	Index.offset = 0;
	while(1)
	{
		nextoffset(DB_Name,Index,start,offset,end,type);
		if(Index.offset<0) break;
		blocknum = offset /10000;
		blockoffset = offset % 10000;
		if(blockoffset<0) break;

		blockInfo* tempb = readBlock(DB_Name, Table_Name, blocknum, m_fileType);

		string tempst = tempb->cBlock;

		int e = tempst.Find(';',blockoffset);
		if(e<=0) break;
		string temps = tempst.Mid(blockoffset,e-blockoffset+1);
		int res = Confirm_To_Where(temps,conds,count,cond);
		if(res==1)
		{
			record+= temps; //若符合，加到record结尾
		}
	}

	Print_Head(print,Count);
	Print_To_Screen(record,print,Count);
	return;
}
//10.用小于或小于等于索引的选择语句
void Select_With_Smaller_Cond(string DB_Name,string Table_Name,condition_info conds[32],int count,attr_info print[32],int Count,char cond,index_info Index,int type)
{
	int start=0;
	int end=-1;
	ResetInform(Index);

	int m_fileType =0; //文件
	string record;

	search_many(DB_Name, Table_Name,start,end,type, Index) ; //index里的函数
	long offset;
	int blocknum;
	int blockoffset;
	Index.offset = 0;
	while(1)
	{
		nextoffset(DB_Name,Index,start,offset,end,type);
		if(Index.offset<0) break;
		blocknum = offset /10000;
		blockoffset = offset % 10000;
		if(blockoffset<0) break;

		blockInfo* tempb = readBlock(DB_Name, Table_Name, blocknum, m_fileType);

		string tempst = tempb->cBlock;

		int e = tempst.Find(';',blockoffset);
		if(e<=0) break;
		string temps = tempst.Mid(blockoffset,e-blockoffset+1);
		int res = Confirm_To_Where(temps,conds,count,cond);
		if(res==1)
		{
			record+= temps; //若符合，加到record结尾
		}
	}

	Print_Head(print,Count);
	Print_To_Screen(record,print,Count);
	return;
}


//11.判断是否符合where条件组
bool Confirm_To_Where(string record,condition_info conds[10],int count,char cond)
{
	for(int i=0;i<count;i++)
	{
		int res = Confirm(record,conds[i]);
		if(res == 0) return 0;
	}
	return 1;
}

//12.判断是否符合某个指定条件
bool Confirm(string record,condition_info condition)
{
	int start=0;
	int end=-1;
	string temps,temps1;

	int index = record.GetLength();
	record.SetAt(index-1,','); //把末位: 替换成 ;
	//取得左比较值
	for(int i=0;i<condition.left_offset+1;i++)
	{
		start=end+1;
		end = record.Find(',',start);
	}
	temps = record.Mid(start,end-start); //获得第left_offset个属性值

	//取得右比较值
	if(condition.right_type=='t') //若属性值和 常量比较
	{
		temps1 = condition.const_data;
	}
	else
	{
		start=0;
		end=-1;
		for(int i=0;i<condition.right_offset;i++)
		{
			start=end+1;
			end = record.Find(',',start);
		}
		temps1 = record.Mid(start,end-start); //获得第right_offset个属性值
	}
	//进行比较
	switch(condition.condition) //比较条件
	{
	case 0: // ‘=’
		{
			if(condition.left_type=='i') //比较类型
			{
				int i1 = atoi(temps);
				int i2 = atoi(temps1);
				if(i1==i2) return 1;
				else return 0;
			}
			else if(condition.left_type == 'c')
			{
				char *p1 = (LPSTR)(LPCTSTR)temps;
				char *p2 = (LPSTR)(LPCTSTR)temps1;
				if(!strcmp(p1,p2)) return 1;
				else  return 0;
			}
			else if(condition.left_type == 'f')
			{
				float f1 = (float)atof((char *)(LPTSTR)(LPCTSTR)temps);
				float f2 = (float)atof((char *)(LPTSTR)(LPCTSTR)temps1);
				if(f1==f2) return 1;
				else  return 0;
			}
		}
		break;
	case 1: // ‘<>’
		{
			if(condition.left_type=='i')
			{
				int i1 = atoi(temps);
				int i2 = atoi(temps1);
				if(i1!=i2) return 1;
				else return 0;
			}
			else if(condition.left_type == 'c')
			{
				char *p1 = (LPSTR)(LPCTSTR)temps;
				char *p2 = (LPSTR)(LPCTSTR)temps1;
				if(strcmp(p1,p2)) return 1;
				else  return 0;
			}
			else if(condition.left_type == 'f')
			{
				float f1 = (float)atof((char *)(LPTSTR)(LPCTSTR)temps);
				float f2 = (float)atof((char *)(LPTSTR)(LPCTSTR)temps1);
				if(f1!=f2) return 1;
				else  return 0;
			}
		}
		break;
	case 2: // '<'
		{
			if(condition.left_type=='i')
			{
				int i1 = atoi(temps);
				int i2 = atoi(temps1);
				if(i1<i2) return 1;
				else return 0;
			}
			else if(condition.left_type == 'c')
			{
				char *p1 = (LPSTR)(LPCTSTR)temps;
				char *p2 = (LPSTR)(LPCTSTR)temps1;
				if(strcmp(p1,p2)<0) return 1;
				else  return 0;
			}
			else if(condition.left_type == 'f')
			{
				float f1 = (float)atof((char *)(LPTSTR)(LPCTSTR)temps);
				float f2 = (float)atof((char *)(LPTSTR)(LPCTSTR)temps1);
				if(f1<f2) return 1;
				else  return 0;
			}

		}
		break;
	case 3: // '>'
		{
			if(condition.left_type=='i')
			{
				int i1 = atoi(temps);
				int i2 = atoi(temps1);
				if(i1>i2) return 1;
				else return 0;
			}
			else if(condition.left_type == 'c')
			{
				char *p1 = (LPSTR)(LPCTSTR)temps;
				char *p2 = (LPSTR)(LPCTSTR)temps1;
				if(strcmp(p1,p2)>0) return 1;
				else  return 0;
			}
			else if(condition.left_type == 'f')
			{
				float f1 = (float)atof((char *)(LPTSTR)(LPCTSTR)temps);
				float f2 = (float)atof((char *)(LPTSTR)(LPCTSTR)temps1);
				if(f1>f2) return 1;
				else  return 0;
			}
		}
		break;
	case 4: // '<='
		{
			if(condition.left_type=='i')
			{
				int i1 = atoi(temps);
				int i2 = atoi(temps1);
				if(i1<=i2) return 1;
				else return 0;
			}
			else if(condition.left_type == 'c')
			{
				char *p1 = (LPSTR)(LPCTSTR)temps;
				char *p2 = (LPSTR)(LPCTSTR)temps1;
				if(strcmp(p1,p2)<=0) return 1;
				else  return 0;
			}
			else if(condition.left_type == 'f')
			{
				float f1 = (float)atof((char *)(LPTSTR)(LPCTSTR)temps);
				float f2 = (float)atof((char *)(LPTSTR)(LPCTSTR)temps1);
				if(f1<=f2) return 1;
				else  return 0;
			}
		}
		break;
	case 5: // '>='
		{
			if(condition.left_type=='i')
			{
				int i1 = atoi(temps);
				int i2 = atoi(temps1);
				if(i1>=i2) return 1;
				else return 0;
			}
			else if(condition.left_type == 'c')
			{
				char *p1 = (LPSTR)(LPCTSTR)temps;
				char *p2 = (LPSTR)(LPCTSTR)temps1;
				if(strcmp(p1,p2)>=0) return 1;
				else  return 0;
			}
			else if(condition.left_type == 'f')
			{
				float f1 = (float)atof((char *)(LPTSTR)(LPCTSTR)temps);
				float f2 = (float)atof((char *)(LPTSTR)(LPCTSTR)temps1);
				if(f1>=f2) return 1;
				else  return 0;
			}
		}
		break;
	default:
		{printf("An error in record_200+ condition %s",errno);
		exit(1);}
	}
	return 0;
}

//13.选择语句（无where）
void Select_No_Where(string DB_Name,string Table_Name,attr_info print[32],int count)
{
	int m_fileType =0; //文件
	int num=0; //blockNum
	Print_Head(print,count);//打印头
	while(1)
	{
		blockInfo* tempb = readBlock(DB_Name, Table_Name, num , m_fileType);
		if(tempb->charNum>0) //若该块有数据
		{
			int start=0;
			int end;
			string tempst = tempb->cBlock;
			while((end = tempst.Find(';',start))!=-1)
			{
				string temps = tempst.Mid(start,end-start+1); //取出一条record
				start=end+1;
				if(temps.GetAt(0)==' ')
					continue;
				Print_To_Screen(temps,print,count); //输出该record
			}
			num++; //到下一块
			continue;
		}
		else //若已经到了空块
		{
			return;
		}
	}
	return;
}

//14.选择语句（有where）
void Select_With_Where(string DB_Name,string Table_Name,condition_info conds[10],int count,char cond,attr_info print[32],int Count)
{
	attr_info conattr[32];
	string attr;
	int attrcount = 0;
	index_info node;
	int num=0;
	// 首先向catalog查询，select条件组里的属性值是否有对应的索引，并返回一个没有offset的索引节点，但有索引名，然后用索引名在index里查找offset,即对应一系列record的第一个节点
	Get_Attr_Info_All(DB_Name,Table_Name,conattr,attrcount);//获得该表的属性值组
	for(int i=0;i<count;i++)
	{
		if(conds[i].condition == 1) // <>不用来判断有无索引，没意义
			continue;
		attr = conattr[conds[i].left_offset].attr_name; //对所有select条件遍历，取左属性名
		int res = Get_Index_Name(DB_Name, Table_Name,attr,node); //返回一个有索引名的索引节点；num返回第几个condition有索引
		if(res==1)
		{
			Select_With_Useful_Cond(DB_Name,Table_Name,conds,count,print,Count,cond,num,node);
			return;
		}
	}
	Select_Without_Useful_Cond(DB_Name,Table_Name,conds,count,print,Count,cond);//如果一直没有合适的
}

//15.删除语句（有where）
void Delete_With_Where(string DB_Name,string Table_Name,condition_info conds[10],int count,index_info nodes[32],char cond)
{
	attr_info conattr[32];
	string attr;
	int attrcount = 0;
	index_info node;
	int num=0;
	// 首先向catalog查询，select条件组里的属性值是否有对应的索引，并返回一个没有offset的索引节点，但有索引名，然后用索引名在index里查找offset,即对应一系列record的第一个节点
	Get_Attr_Info_All(DB_Name,Table_Name,conattr,attrcount);//获得该表的属性值组
	for(int i=0;i<count;i++)
	{
		attr = conattr[conds[i].left_offset].attr_name; //对所有delete条件遍历，取左属性名
		int res = Get_Index_Name(DB_Name, Table_Name,attr,node); //返回一个有索引名的索引节点；num返回第几个condition有索引
		if(res==1)
		{
			condition_info tempcond = conds[0]; //把该索引的条件放到conds数组第一个
			conds[0] = conds[i];
			conds[i] = tempcond;

			node.value = conds->const_data;
			Delete_With_Useful_Cond(DB_Name,Table_Name,conds,count,nodes,cond,node);
			return;
		}
	}
	Delete_Without_Useful_Cond(DB_Name,Table_Name,conds,count,nodes,cond);//如果一直没有合适的
}

//15.5 没有where的delete语句
void Delete_No_Where(string DB_Name,string Table_Name)
{
	int m_fileType =0; //文件
	int num=0; //blockNum
	while(1)
	{
		blockInfo* tempb = readBlock(DB_Name, Table_Name, num , m_fileType);
		if(tempb->charNum>0) //若该块有数据
		{
			tempb->charNum=0;
			memset(tempb->cBlock,'\0',BLOCK_LEN);
			tempb->dirtyBit=1;
		}
		else //若已经到了空块
		{
			return;
		}
		num++;
	}
	return;
}

//16.有where,但无可用索引的delete语句
void Delete_Without_Useful_Cond(string DB_Name,string Table_Name,condition_info conds[10],int count,index_info nodes[32],char cond)
{
	int m_fileType =0; //文件
	int num=0; //blockNum
	while(1)
	{
		blockInfo* tempb = readBlock(DB_Name, Table_Name, num , m_fileType);
		if(tempb->charNum>0) //若该块有数据
		{
			int start=0;
			int end;
			string tempst = tempb->cBlock;
			while((end = tempst.Find(';',start))!=-1)
			{
				string temps = tempst.Mid(start,end-start+1); //取出一条record

				int res = Confirm_To_Where(temps,conds,count,cond); //条件判断
				if(res==1)
				{
					string temps1 = tempst.Left(start); //取该组的左边
					string temps2 = tempst.Mid(end+1); //取该组的右边
					string nulltemp = "";
					for(int i=0;i<(int )strlen(temps)-1;i++)
					{
						nulltemp+=' ';
					}
					nulltemp += ';';
					temps.Format(_T("%s%s%s"),temps1, nulltemp , temps2); //左+右，写回，即删除了中间
					memset(tempb->cBlock,'\0',BLOCK_LEN);
					char * p = (LPSTR)(LPCTSTR)temps;//写回
					strcpy(tempb->cBlock,p);
					tempb->charNum--; //记录数-1
					tempb->dirtyBit=1; //置脏
					start=end+1; //到下一条record
					DeleteRecord(DB_Name,Table_Name);
				}
				else
				{
					start=end+1;
					continue;
				}
			}
			num++; //到下一块
			continue;
		}
		else //若已经到了空块
		{
			return;
		}
	}
	return;
}

//17.有where且有可用索引的delete 语句   这里不理解传入的nodes要干什么
void Delete_With_Useful_Cond(string DB_Name,string Table_Name,condition_info conds[10],int count,index_info nodes[32],char cond,index_info node)   // int index 改成了  node ,不知道index能干嘛？  .h中保存index,方便改回来;若保持node,注意修改.h
{
	if(conds[0].condition==0) //num为索引在第几个条件的属性值上
	{
		Delete_With_Equal_Cond(DB_Name,Table_Name,conds,count,nodes,cond,node);

	}
	else if(conds[0].condition>1 && conds[0].condition<6)
	{
		if(conds[0].condition==5)
		{
			Delete_With_Greater_Cond(DB_Name,Table_Name,conds,count,nodes,cond,node,2);
		}
		else if( conds[0].condition==3)
		{
			Delete_With_Greater_Cond(DB_Name,Table_Name,conds,count,nodes,cond,node,1);
		}
		else if(conds[0].condition==4)
		{
			Delete_With_Smaller_Cond(DB_Name,Table_Name,conds,count,nodes,cond,node,4);
		}
		else if(conds[0].condition==2)
		{
			Delete_With_Smaller_Cond(DB_Name,Table_Name,conds,count,nodes,cond,node,3);
		}
	}
}

//18.用等于索引的删除语句
void Delete_With_Equal_Cond(string DB_Name,string Table_Name,condition_info conds[10],int count,index_info nodes[32],char cond,index_info Index)
{
	ResetInform(Index);
	search_one(DB_Name,Table_Name, Index,0); //来自index_M
	int m_fileType =0; //文件
	int blocknum = Index.offset/10000;

	blockInfo* tempb = readBlock(DB_Name, Table_Name, blocknum , m_fileType);

	int blockoffset = Index.offset%10000;
	string tempst = tempb->cBlock;
	int end = tempst.Find(';',blockoffset);
	string temps = tempst.Mid(blockoffset,end-blockoffset+1);
	int res = Confirm_To_Where(temps,conds,count,cond);
	if(res==1)
	{
		string temps1 = tempst.Mid(0,blockoffset); //取该组的左边
		string temps2 = tempst.Mid(end+1); //取该组的右边
		string nulltemp = "";
		for(int i=0;i<(int )strlen(temps)-1;i++)
		{
			nulltemp+=' ';
		}
		nulltemp += ';';
		temps.Format(_T("%s%s%s"),temps1, nulltemp , temps2); //左+右，写回，即删除了中间
		memset(tempb->cBlock,'\0',BLOCK_LEN);
		char * p = (LPSTR)(LPCTSTR)temps;//写回
		strcpy(tempb->cBlock,p);
		tempb->charNum--; //记录数-1
		tempb->dirtyBit=1; //置脏
		DeleteRecord(DB_Name,Table_Name);
	}
	return;

}

//19.用大于或大于等于索引的删除语句
void Delete_With_Greater_Cond(string DB_Name,string Table_Name,condition_info conds[10],int count,index_info nodes[32],char cond,index_info Index,int type)
{
	int start=0;
	int end=-1;
	int m_fileType =0; //文件
	string record;
	ResetInform(Index);

	search_many(DB_Name, Table_Name,start,end,type, Index) ; //index里的函数
	long offset;
	int blocknum;
	int blockoffset;
	Index.offset = 0;
	while(1)
	{
		nextoffset(DB_Name,Index,start,offset,end,type);
		if(Index.offset<0) break;
		blocknum = offset /10000;
		blockoffset = offset % 10000;
		if(blockoffset<0) break;

		blockInfo* tempb = readBlock(DB_Name, Table_Name, blocknum, m_fileType);

		string tempst = tempb->cBlock;

		int e = tempst.Find(';',blockoffset);
		if(e<=0) break;
		string temps = tempst.Mid(blockoffset,e-blockoffset+1);
		int res = Confirm_To_Where(temps,conds,count,cond);
		if(res==1)
		{
			string temps1 = tempst.Mid(0,blockoffset); //取该组的左边
			string temps2 = tempst.Mid(blockoffset+temps.GetLength()); //取该组的右边
			string nulltemp = "";
			for(int i=0;i<(int )strlen(temps)-1;i++)
			{
				nulltemp+=' ';
			}
			nulltemp += ';';
			temps.Format(_T("%s%s%s"),temps1, nulltemp , temps2); //左+右，写回，即删除了中间

			memset(tempb->cBlock,'\0',BLOCK_LEN);
			char * p = (LPSTR)(LPCTSTR)temps;//写回
			strcpy(tempb->cBlock,p);
			tempb->charNum--; //记录数-1
			tempb->dirtyBit=1; //置脏
			DeleteRecord(DB_Name,Table_Name);
		}
	}

	return;

}


//20.用小于或小于等于索引的删除语句
void Delete_With_Smaller_Cond(string DB_Name,string Table_Name,condition_info conds[32],int count,index_info nodes[32],char cond,index_info Index,int type)
{
	int start=0;
	int end=-1;
	int m_fileType =0; //文件
	string record;
	ResetInform(Index);

	search_many(DB_Name, Table_Name,start,end,type, Index) ; //index里的函数
	long offset;
	int blocknum;
	int blockoffset;
	Index.offset = 0;
	while(1)
	{
		nextoffset(DB_Name,Index,start,offset,end,type);
		if(Index.offset<0) break;
		blocknum = offset /10000;
		blockoffset = offset % 10000;
		if(blockoffset<0) break;

		blockInfo* tempb = readBlock(DB_Name, Table_Name, blocknum, m_fileType);

		string tempst = tempb->cBlock;

		int e = tempst.Find(';',blockoffset);
		if(e<=0) break;
		string temps = tempst.Mid(blockoffset,e-blockoffset+1);
		int res = Confirm_To_Where(temps,conds,count,cond);
		if(res==1)
		{
			string temps1 = tempst.Mid(0,blockoffset); //取该组的左边
			string temps2 = tempst.Mid(blockoffset+temps.GetLength()); //取该组的右边
			string nulltemp = "";
			for(int i=0;i<(int )strlen(temps)-1;i++)
			{
				nulltemp+=' ';
			}
			nulltemp += ';';
			temps.Format(_T("%s%s%s"),temps1, nulltemp , temps2); //左+右，写回，即删除了中间
			memset(tempb->cBlock,'\0',BLOCK_LEN);
			char * p = (LPSTR)(LPCTSTR)temps;//写回
			strcpy(tempb->cBlock,p);
			tempb->charNum--; //记录数-1
			tempb->dirtyBit=1; //置脏
			DeleteRecord(DB_Name,Table_Name);
		}
	}

	return;
}


//21.查找有无可用索引,这个应该不用了,调用catelogy里的新加模块
bool Find_Useful_Cond(condition_info conds[10],int count,int & index)
{
	return 0;
}
//22.插入所有索引节点
void Insert_Index_All(string DB_Name,string Table_Name,string Index_Name, string Attr, int length,int offset,int type)
{
	index_info node;
	attr_info print[32];
	int count=0;
	node.index_name = Index_Name;
	int blocknum=0;

	Get_Attr_Info_All(DB_Name,Table_Name,print,count);
	while(1)
	{
		blockInfo* tempb = readBlock(DB_Name, Table_Name, blocknum ,0);
		if(tempb->charNum <= 0) //若该块没有数据
		{
			return;
		}
		else
		{
			int start=0;
			int end;
			string temps = tempb->cBlock;
			while((end = temps.Find(';',start))!=-1)
			{
				string tempsi = temps.Mid(start,end-start+1); //取出一条record
				start=end+1;
				Insert_Index(DB_Name,Table_Name,Attr,print,count,node,blocknum*10000+start,tempsi);
			}
		}
		blocknum++;//到下一块
	}
	return;
}
//23.验证可否插入,没写，不知道主键怎么判断和定义
bool Verify_Insertable(string DB_Name,string Table_Name,string Attr, attr_info print[],int count)
{
	int indexlength=0;
	string temps = Attr;
	index_info node;
	string index_name;
	int index = temps.GetLength();
	temps.SetAt(index-1,','); //把末位: 替换成 ;
	//计算length
	int PKlength;

	int PK ;
	//查询主键
	PK = ReadPK( DB_Name,Table_Name, "", index_name, PKlength);
	node.length = PKlength;
	if(PKlength>0) //若有主键
	{

		int indexs=0;
		int indexe=-1;
		for(int ii=0;ii<PK+1;ii++)
		{
			indexs = indexe+1;
			indexe = temps.Find(',',indexs);
		}
		string PKvalue = temps.Mid(indexs,indexe-indexs);
		node.value = "";
		int ii = node.length-PKvalue.GetLength();
		for(int i=0;i<ii;i++)
			node.value +='0';

		node.value +=PKvalue;
		node.index_name = index_name;

		search_one(DB_Name, Table_Name, node, 0 );//这里node能否传回？
		if(node.offset>=0) //已有这个记录
		{
			return 0;
		}
	}

	for(int i=0;i<count;i++)
	{
		PK = ReadPK( DB_Name,Table_Name, print[i].attr_name, index_name, PKlength);
		node.length = PKlength;
		if(PKlength<0)
			continue;

		Get_Index_Name(DB_Name,Table_Name, print[i].attr_name,node);
		int indexs=0;
		int indexe=-1;
		for(int ii=0;ii<PK+1;ii++)
		{
			indexs = indexe+1;
			indexe = temps.Find(',',indexs);
		}
		string PKvalue = temps.Mid(indexs,indexe-indexs);
		node.value = "";
		int ii = node.length-PKvalue.GetLength();
		for(int i=0;i<ii;i++)
			node.value +='0';

		node.value +=PKvalue;
		search_one(DB_Name, Table_Name, node, 0 );//这里node能否传回？
		if(node.offset>=0) //已有这个记录
		{
			return 0;
		}
	}

	return 1;
}


//24.退出程序
void Quit(string DB_Name)
{
	quitProg(DB_Name);
}

//25.插入索引节点
void Insert_Index(string DB_Name,string Table_Name, string Attr, attr_info print[],int count, index_info node,long offset, string temps)
{
	string index_name=node.index_name;
	int PKlength;
	int PK ;
	PK = ReadPK( DB_Name,Table_Name, Attr, index_name, PKlength);
	if(PK==-1)
		return;
	//node.index_name = index_name;
	node.length = PKlength;

	int index = temps.GetLength();
	temps.SetAt(index-1,','); //把末位: 替换成 ;
	int indexs=0;
	int indexe=-1;
	for(int ii=0;ii<PK+1;ii++)
	{
		indexs = indexe+1;
		indexe = temps.Find(',',indexs);
	}
	string PKvalue = temps.Mid(indexs,indexe-indexs);
	node.value = "";
	int ii = node.length-PKvalue.GetLength();
	for(int i=0;i<ii;i++)
		node.value +='0';
	node.value +=PKvalue;

	node.offset = offset; //offset赋值，在对应文件中找位置
	insert_one(DB_Name, Table_Name, node);
}


//26.调整index.value
void ResetInform(struct index_info& inform)
{
	if(inform.value.GetLength()<inform.length)
		inform.value = string('0',inform.length-inform.value.GetLength())+inform.value;
	else if(inform.value.GetLength()>inform.length)
	{
		cout<<"Error!"<<endl;
		exit(1);
	}
	return;
}
