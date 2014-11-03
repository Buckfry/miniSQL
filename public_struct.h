/*
 * Public_struct.h
 *
 *  Created on: 2014年11月3日
 *      Author: zhanghui2014
 */

struct condition_info
{
	char left_type; // i for int, c for char, f for float
	int left_len;
	long left_offset;//对应第几个属性值，-1 对应常量
	CString left_index_name;
	char right_type; // i for int, c for char, f for float, t for const
	int right_len;
	long right_offset; //对应第几个属性值，-1 对应常量
	CString right_index_name;
	CString const_data;
	int condition; //0 for = , 1 for <>, 2 for <, 3for >, 4 for<= ,5 for >=
};

//定义select 语句显示各属性信息
struct attr_info
{
	CString attr_name;
	CString index;
	int offset;
	int length;
	char type;
	bool PrimaryKey;
	bool unique;
	int attr;//attr的编号
	attr_info(){
		PrimaryKey=false;
		unique=false;
		index="";
	}
};
