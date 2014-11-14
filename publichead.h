/*
 * publichead.h
 *
 *  Created on: 2014年11月14日
 *      Author: Administrator
 */

#ifndef PUBLICHEAD_H_
#define PUBLICHEAD_H_

class keyinfo{
public:
	string keyname;
	vector<pair<string,recordposition>> keys;	//存储键值和对应的位置
};


class recordposition{
	int    recordnum;
	int    blocknum;
};




#endif /* PUBLICHEAD_H_ */
