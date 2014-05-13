//CSnake.h

#ifndef _H_H_CSnake
#define _H_H_CSnake


#define NUMOFWIDTH 32	//set the number of checkers in the screen
#define NUMOFHEIGHT 18

#include<Windows.h>


enum EDirection
{ Up=1, Right=2, Down=3, Left=4 };

struct CPOINT
{
	int x;
	int y;
};


class CSnake
{
private:

public:
	CSnake();
	~CSnake();

	int m_length;					//the number of box
	CPOINT *pPos;                  //each box's nx,ny which is the number of box
	EDirection m_direction;				 //direction of head
	int cxBox, cyBox;				//the width of each box           

	void Move();
	void Add();
	void Reduce();
	void ChangeDirection(EDirection rDirection);
};


#endif