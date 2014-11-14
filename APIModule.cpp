/*
 * APIModule.cpp
 *
 *  Created on: 2014年11月5日
 *      Author: Even
 */


#include "APIModule.h"
#include "RecordManager.h"
#include "Catalog.h"
#include "publichead.h"
#include "Recordmanager.h"
#include <iostream>
#include <algorithm>

using namespace std;

set<SqlMethod> tableMethods;  //保存有关table的操作



void initAPI(){
    tableMethods.insert(SELECTWHERE);
    tableMethods.insert(SELECTALL);
    tableMethods.insert(INSERT);
    tableMethods.insert(DELETEALL);
    tableMethods.insert(DELETEWHERE);

}




void APIModule(const SQL &sql, string Current_DBname){
	Error catalogerror;
//	try{
    if(sql.method !=NOP){  //没有操作
    	//DATABASE
    	if( sql.method == CREATEDATABASE){
    		CreateDatabase(sql);
    	}

    	else if (sql.method == DROPDATABASE ){
    		DropDatabase(sql);
    	}

    	else if (sql.method == USEDATABASE){
    		//initBuffer();
    		Current_DBname=sql.targetName;
    		UseDatabase(sql);
    	}

    	else if (sql.method == QUIT){

    		Quit(Current_DBname); //record
    	}

    	else if (sql.method == CREATETABLE){
    		int temp;
    		   for(int i=0; i< sql.isUnique.size(); i++){
    		    	if(sql.isUnique[i]==1){
    		    	temp=i;
    		    	break;
    		    	}
    		   }
           string indexname=sql.argv[temp];


    		create_record  CR;  //record 的类
    		CR.table_name=sql.targetName;
    		CR.attribute_num=sql.argc;
    		CR.attribute_length=sql.length;  //vector?

    		Bnode bnode(indexname);

    		keyinfo key;

    		CreateTable(sql , Current_DBname);  //检查有没有抛错即可
    		bnode.createindex(sql.length[temp],key);
    		create_table(Current_DBname , CR);
    	}

    	else if (sql.method == DROPTABLE){

    		IndexInfo indexinfo;
    		indexinfo= DropTable(sql , Current_DBname);  //得到indexinfo 有vector的indexname  传给index



    	   for(int i=0; i<indexinfo.indexname.size(); i++){
    		   Bnode bnode(indexinfo.indexname[i]);
    		   bnode.drop_index( indexinfo.indexname[i]);
    	   }

           Drop_Table(Current_DBname , sql.targetName);
    	}

    	else if (sql.method == CREATEINDEX){
    		Index index;
            index = CreateIndex(sql,Current_DBname); //返回attribute的length和类型

    		 keyinfo key;
    		 key = getkeyinfo(Current_DBname, sql.aux[0],sql.argv[0]);

            Bnode bnode(sql.targetName);
    		bnode.createindex(index.length ,key);


    		//
    	}

    	else if (sql.method == DROPINDEX){
    		DropIndex(sql.targetName);
    		Bnode bnode(sql.targetName);
    		bnode.drop_index(sql.targetName);// index
    	}


       //TAble
    	else if(tableMethods.count(sql.method)){ //存在这个操作
           if (Current_DBname) {  //在当前database
              if(findtable(sql.targetName , Current_DBname)){ //存在这个table catalog的函数


                //select
                //@1 select */attrName from table;  //输出结果如何要问record
                if(sql.method == SELECTALL){
                   IndexInfo indexinfo;
                   indexinfo = Select(sql,Current_DBname);

                   attr_info attribute_info;
                   attribute_info.attribute_name=indexinfo.table.attrname;
                   attribute_info.attribute_length=indexinfo.table.length;
                   attribute_info.attribute_type=indexinfo.table.type;



                  Select_With_Useful_No_Where(Current_DBname,sql.targetName,sql.result,attribute_info);


                }

                //select */attrName from table where argv operation value;
                else if(sql.method == SELECTWHERE){
                   IndexInfo indexinfo;
                   indexinfo = Select(sql,Current_DBname);

            	   condition_info coninfo;
            	   coninfo.conditionattr=sql.argv;
            	   coninfo.condition=sql.operators;
            	   coninfo.comparedvalue=sql.value;
            	   attr_info attribute_info;
            	   attribute_info.attribute_name=indexinfo.table.attrname;
            	   attribute_info.attribute_length=indexinfo.table.length;
            	   attribute_info.attribute_type=indexinfo.table.type;

                   if(indexinfo.indexname.size()==0){

                	  Select_Without_Useful_Cond(Current_DBname , sql.targetName , sql.result,coninfo ,  attribute_info);
                   }

                   else{

                      for(int i=0; i<indexinfo.indexname.size(); i++){
                         int temp;
                         for(int j=0; j<sql.argv.size(); j++){
                	       if(indexinfo.attrname[i]==sql.argv[j]){
                		        temp=j;
                	       }
                         }
                         Bnode bnode(indexinfo.indexname[i]);
                         vector<recordposition> recordposi;   //需要修改
                         recordposi=bnode.searchbyindex(sql.operators[temp], sql.value[temp]);
                      }

                   Select_With_Useful_Cond(Current_DBname,sql.targetName,sql.result,recordposi ,coninfo ,attribute_info);
                  }

                }
                else if(sql.method == INSERT){
                	 IndexInfo  indexinfo;
                	 indexinfo = Insert(sql,Current_DBname); // 会抛错

                	 int temp;
                	 for(int i=0; i<indexinfo.table.attrtype.size(); i++ ){
                		 if(indexinfo.table.attrtype[i]==1)
                			temp=i;
                	 }

                	 string indexname=indexinfo.table.attrname[temp];
                	 Bnode bnode(indexname);

                 //需要修改
                	 for(int i=0; i< sql.argv.size(); i++){
                		 vector<recordposition> recordposi;
                		recordposi = bnode.searchbyindex("=",sql.argv[i]) ;
                	 }
                 //需抛出异常；

                	 recordposition rp;

                	 attr_info attribute_info;
                	 attribute_info.attribute_name=indexinfo.table.attrname;
                	 attribute_info.attribute_type=indexinfo.table.type;
                	 attribute_info.attribute_length=indexinfo.table.length;

                	 rp = insert_record(Current_DBname,sql.targetName,sql.argv,attribute_info);

                	 for(int i=0; i< sql.argv.size(); i++){
                		 bnode.InserttoIndex(sql.argv[i], rp);
                	 }

                }

                else if(sql.method == DELETEALL){
                IndexInfo indexinfo;
                indexinfo = Delete(sql,Current_DBname);

                attr_info attribute_info;
                attribute_info.attribute_name=indexinfo.table.attrname;
                attribute_info.attribute_length=indexinfo.table.length;
                attribute_info.attribute_type=indexinfo.table.type;

                Delete_No_Where(Current_DBname,sql.targetName,attribute_info);

                }
                else if(sql.method == DELETEWHERE){
                	IndexInfo indexinfo;
                    indexinfo = Select(sql,Current_DBname);

                	condition_info coninfo;
                	coninfo.conditionattr=sql.argv;
                	coninfo.condition=sql.operators;
                	coninfo.comparedvalue=sql.value;
                    attr_info attribute_info;
                	attribute_info.attribute_name=indexinfo.table.attrname;
                	attribute_info.attribute_length=indexinfo.table.length;
                	attribute_info.attribute_type=indexinfo.table.type;

                	vector<recordposition> recordposition;

                	if(indexinfo.indexname.size()==0){

                		recordposition = Delete_Without_Useful_Cond(Current_DBname,sql.targetName, coninfo, attribute_info);
                	 }

                	 else{

                	       for(int i=0; i<indexinfo.indexname.size(); i++){
                	              int temp;
                	              for(int j=0; j<sql.argv.size(); j++){
                	                  if(indexinfo.attrname[i]==sql.argv[j]){
                	                		  temp=j;
                	                   }
                	               }
                	               Bnode bnode(indexinfo.indexname[i]);
                	               vector<recordposition> recordposi;   //需要修改
                	               recordposi=bnode.searchbyindex(sql.operators[temp], sql.value[temp]);
                	         }

                	      recordposition = Delete_With_Useful_Cond(Current_DBname,sql.targetName ,recordposi ,coninfo , attribute_info);
                	                  }
                }
            }
                else{
                cout << "No such table " << sql.targetName << " exists" << endl;
                return ;
            }
            }
            else
                cout << "There is no current database, please use \"use databasename\" to use a database." << endl;
        }

    //FILE
        else if(sql.method == EXECFILE){
            execFile(sql);
        }
        else if(sql.method == HELP){
            helpInfo(sql);
        }
    }
	//}
}

