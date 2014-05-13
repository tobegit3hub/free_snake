//CSnake.cpp

#include"CSnake.h"


CSnake::CSnake()
{
	m_length = 4;
	pPos = new CPOINT [m_length];
	m_direction = Right;									//up means 1; right 2; down 3;left 4


	for(int i=0;i<m_length;++i)                        //initial the original positions
	{
		pPos[i].x = 0;
		pPos[i].y = NUMOFHEIGHT/2;
	}
}

CSnake::~CSnake()
{
	delete []pPos;
	pPos = NULL;
}


void CSnake::Move()
{
	for(int i=m_length-1;i>0;--i)                      //move the body and tail
	{
		pPos[i].x = pPos[i-1].x;
		pPos[i].y = pPos[i-1].y;
	}

	switch(m_direction)                                 //move the head
	{
	case 1:
		{
			pPos[0].y -= 1;                   
		}break;

	case 2:
		{
			pPos[0].x += 1;
		}break;
		
	case 3:
		{
			pPos[0].y += 1;
		}break;
		
	case 4:
		{
			pPos[0].x -= 1;
		}break;
	}
}

void CSnake::ChangeDirection(EDirection rDirection)
{
	if(2!=m_direction-rDirection && -2!=m_direction-rDirection)            //not allow to change opposite directly
	{
		m_direction = rDirection;
	}
}

void CSnake::Add()
{
	++m_length;


	CPOINT *pNewPos = new CPOINT [m_length];                   //create a new pPos to store the new positions
	for(int i=0;i<m_length-1;++i)
	{
		pNewPos[i].x = pPos[i].x;                             //set the old position
		pNewPos[i].y = pPos[i].y;
	}
	pNewPos[m_length-1].x = pPos[m_length-2].x;                 //set the new one position
	pNewPos[m_length-1].y = pPos[m_length-2].y;

	delete []pPos;                                           //release the memory
	pPos = NULL;
	pPos = pNewPos;                                           //get the new pPos
	pNewPos = NULL;



	//CPOINT *pTemPos = new CPOINT[m_length-1];               //another method to set the new pPos, and it works
	//for(int i=0;i<m_length-1;++i)
	//{
	//	pTemPos[i].x = pPos[i].x;                            
	//	pTemPos[i].y = pPos[i].y;
	//}

	//delete []pPos;
	//pPos = NULL;
	//pPos = new CPOINT[m_length];
	//for(int i=0;i<m_length-1;++i)
	//{
	//	pPos[i].x = pTemPos[i].x;                            
	//	pPos[i].y = pTemPos[i].y;
	//}
	//pPos[m_length-1].x = pTemPos[m_length-2].x;
	//pPos[m_length-1].y = pTemPos[m_length-2].y;

	//delete []pTemPos;
	//pTemPos = NULL;
}

void CSnake::Reduce()
{
	if(m_length>1)
	{
		--m_length;

		CPOINT *pNewPos = new CPOINT [m_length];                   //create a new pPos to store the new positions
		for(int i=0;i<m_length;++i)
		{
			pNewPos[i].x = pPos[i].x;                             //set the old position
			pNewPos[i].y = pPos[i].y;
		}

		delete []pPos;                                           //release the memory
		pPos = NULL;
		pPos = pNewPos;                                           //get the new pPos
		pNewPos = NULL;
	}
}