/*
 * exception.h
 *
 *  Created on: 2014年11月7日
 *      Author: Administrator
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_
#include<iostream>

using namespace std;
/////////////////////////////////////////////////////
//exceptions for index & record


//index & record 共有的异常
class not_find{};


//index
class insertindex_error{};
class deleteindex_error{};
class createindex_error{};
class not_unique{};//建立索引时，对应的属性不是unique，从而建立索引失败
class dropindex_error{};

//record
class getcolumn_error{};//create index的时候用
class insertdata_error{};
class deletedata_error{};
class createtable_error{};
class droptable_error{};
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
//exceptions for buffer




///////////////////////////////////////////////////////


///////////////////////////////////////////////////////
//exceptions for catalog
class no_such_table{//
public:
	string table_name;
};
class no_such_attribute
{
public:
	string attribute_name;
};

class type_error
{
	string attribute_name;
	string error_type;
	string correct_type;
};





///////////////////////////////////////////////////////

#endif /* EXCEPTION_H_ */
