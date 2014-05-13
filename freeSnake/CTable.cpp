//CTable.cpp

#include"CTable.h"

CTable::CTable()
{
	table = new int * [NUMOFWIDTH];
	for(int i=0;i<NUMOFWIDTH;++i)
	{
		table[i] = new int [NUMOFHEIGHT];

		for(int j=1;j<NUMOFHEIGHT-1;++j)                               //initial the table
		{
			table[i][j] = 0;                                          //make table avaliable beside the wall
		}

		table[i][0] = -1;                                             //set the wall
		table[i][NUMOFHEIGHT-1] = -1;

	}
}

CTable::~CTable()
{
	for(int i=0;i<NUMOFWIDTH;++i)
	{
		delete []table[i];
		table[i] = NULL;
	}

	delete []table;
	table = NULL;
}


void CTable::SetWall()
{
	int tempx, tempy;
	tempx = rand()%NUMOFWIDTH;
	tempy = rand()%(NUMOFHEIGHT-2)+1;
	
	while(true)															//to avoid making food in the snake
	{
		for(int i=0;i<(snake.m_length>snake2.m_length ? snake2.m_length : snake.m_length);++i)
		{
			if((snake.pPos[i].x!=tempx || snake.pPos[i].y!=tempy) && (snake2.pPos[i].x!=tempx || snake2.pPos[i].y!=tempy))     //make double condition
			{ 
				table[tempx][tempy] = -1;                             //set the food in the table
				return;
			}
			else
			{
				tempx = rand()%NUMOFWIDTH;
				tempy = rand()%(NUMOFHEIGHT-2)+1;
			}
		}
	}
}