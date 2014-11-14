/*
 * APIModule.cpp
 *
 *  Created on: 2014年11月5日
 *      Author: Even
 */


#include "APIModule.h"
#include "RecordManager.h"
#include "Catalog.h"
#include "FunctionOfIndex&Record.h"
//#include <map>
//#include <set>
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
    		//
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

    		/*	class create_index_info
    		    		{
    		    		public:
    		    			string index_name;   //为primarykey的名字
    		    			int lengh;//type's length(especially for char) //属性类型的长度
    		    			string attribute_name;  //primarykey的名字
    		    			string type;//int, float or char
    		    			vector<string> data; //不需要
    		    			vector<string> position; //不需要
    		    		};  */

    		create_index_info CIT;   // index的类   primarykey自动    //只需要前4个星星，indexname为primarykey的名字

    		CIT.index_name=indexname;
    		CIT.length=sql.length[temp];
    		CIT.attribute_name=indexname;
    		CIT.type=sql.types[temp];


 /*class create_record{
 public:
	string table_name;
	//vector<char> attribute_name;//属性名
	//vector<char> attribute_type;//属性类型
	int attribute_num; //属性数目
	//vector<int> attribute_length;//属性长度(每个属性的字节数)

};*/
    		create_record  CR;  //record 的类
    		CR.table_name=sql.targetName;
    		CR.attribute_num=sql.argc;

    		CreateTable(sql , Current_DBname);  //检查有没有抛错即可
    		createindex_index(CIT);
    		create_table(Current_ DBname , CR);
    	}

    	else if (sql.method == DROPTABLE){

    		IndexInfo indexinfo;
    		indexinfo= DropTable(sql , Current_DBname);  //得到indexinfo 有vector的indexname  传给index
    	   for(int i=0; i<indexinfo.indexname.size(); i++){
    		   dropindex( indexinfo.indexname[i]);
    	   }
    		//dropindex 循环删除indexname；dropindex(string indexname);

    	}

    	else if (sql.method == CREATEINDEX){
    		Index index;
            index = CreateIndex(sql,Current_DBname); //返回attribute的length和类型

    		 keyinfo KInfo;
    		 KInfo = getkeyinfo(Current_DBname, sql.aux[0],sql.argv[0]);

    		 create_index_info CII;
    		 CII.index_name=sql.targetName;
    		 CII.length=index.length;
    		 CII.attribute_name=sql.argv[0];
    		 CII.type=index.type;
    	/*	class create_index_info
    		{
    		public:
    			string index_name;  //
    			int lengh;//type's length(especially for char)  //catalog会返回
    			string attribute_name;
    			int type;//int, float or char  //catalog会返回
    			vector<string> data; //张会返回填好
    			vector<string> position;// 张会返回填好
    		};  */
    		 createindex_index(CIT);
    /*class keyinfo{
public:
	char keyname;
	vector<pair<char,recordposition>> keys;//存储键值和对应的位置
};*/

    		//
    	}

    	else if (sql.method == DROPINDEX){
    		DropIndex(sql.targetName);
    		dropindex(sql.targetName); // index
    	}


       //TAble
    	else if(tableMethods.count(sql.method)){ //存在这个操作
           if (Current_DBname) {  //在当前database
              if(findtable(sql.targetName , Current_DBname)){ //存在这个table catalog的函数


                //select
                //@1 select */attrName from table;  //输出结果如何要问record
                if(sql.method == SELECTALL){
                    //
                	/*class IndexInfo{
                     public:
	                 vector<string> indexname;
	                 vector<string> attrname;
	                 TableInfo table; */
};
                	IndexInfo& Select(SQL *b,string database);

                /*	class condition_info
                	{
                	public:
                		vector<string> conditionattr;
                		vector<char> comparedvalue;  //string要改成char
                		vector<string>  condition;//0 for = , 1 for <>, 2 for <, 3for >, 4 for<= ,5 for >=

                	};*/





                }

                //select */attrName from table where argv operation value;
                else if(sql.method == SELECTWHERE){
                /*class IndexInfo{
                	public:
                		vector<string> indexname;
                		vector<string> attrname;
                		TableInfo table;
                	};*/



                }
                else if(sql.method == INSERT){
                	//  IndexInfo& Insert(SQL *b,string database);  会抛错


                /*	class recordposition{
                		int    recordnum;
                		int    blocknum;
                	}; */

              //  recordposition& insert_record(string DB_name,string filename,vector<char> attr);

                	/*class update_index_info
                	{
                	public:
                		string index_name;//由api赋值
                		string value;//index 的value    我要去table里找
                		location loc;//数据所在位置
                	};*/

                	//
                }

                else if(sql.method == DELETEALL){

                IndexInfo& Delete(SQL *b,string database);


                }
                else if(sql.method == DELETEWHERE){

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

