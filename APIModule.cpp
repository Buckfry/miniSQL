/*
 * APIModule.cpp
 *
 *  Created on: 2014年11月5日
 *      Author: Even
 */


#include "APIModule.h"
#include "RecordManager.h"
#include "CatalogManager.h"
#include <map>
#include <set>
#include <iostream>
#include <fstream>
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
    	}

    	else if (sql.method == QUIT){
    		//
    	}

    	else if (sql.method == CREATETABLE){
    		CreateTable(sql , Current_DBname);
    	}

    	else if (sql.method == DROPTABLE){
    		DropTable(sql , Current_DBname);
    		UseDatabase(sql);
    	}

    	else if (sql.method == CREATEINDEX){
    		//
    	}

    	else if (sql.method == DROPINDEX){
    		dropindex(sql.targetName);
    	}


       //TAble
    	else if(tableMethods.count(sql.method)){ //存在这个操作
           if (Current_DBname) {  //在当前database
              if(findtable(sql.targetName , Current_DBname)){ //存在这个table catalog的函数


                //select
                //@1 select */attrName from table;  //输出结果如何要问record
                if(sql.method == SELECTALL){
                    //

                }

                //select */attrName from table where argv operation value;
                else if(sql.method == SELECTWHERE){


                }
                else if(sql.method == INSERT){

                }

                else if(sql.method == DELETEALL){

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

