//CTable.h

#ifndef _H_H_CTable
#define _H_H_CTable 


#include"CSnake.h"


class CTable
{
private:

public:
	CTable();
	~CTable();

	CSnake snake;
	CSnake snake2;                      //make another snake
	int **table;
	void SetWall();

};


#endif