//WinMain.cpp




#include<Windows.h>
#include<stdlib.h>		//to use the srand()
#include<ctime>
#include"CSnake.h" 
#include"CTable.h"

LRESULT CALLBACK WindowProc(  __in  HWND hWnd,
							  __in  UINT uMsg,
							  __in  WPARAM wParam,
							  __in  LPARAM lParam);

VOID CALLBACK TimerProc(HWND hwnd,      //represent the first snake
						UINT uMsg,
						UINT_PTR idEvent,
						DWORD dwTime);
VOID CALLBACK TimerProc2(HWND hwnd,		 //represent the second snake
						UINT uMsg,
						UINT_PTR idEvent,
						DWORD dwTime);
VOID CALLBACK TimerProc3(HWND hwnd,      //to make the wall in some time
						UINT uMsg,
						UINT_PTR idEvent,
						DWORD dwTime);

bool IsFood();		//for snake1, to judge whether it's food or not
bool IsFood2();		//for snake2, to judge whether it's food or not
bool IsWall();		//for snake1, to judge whether it's wall or not
bool IsWall2();		//for snake2, to judge whether it's wall or not
bool IsSelf();		//for snake1, to judge whether it hit itself or not
bool IsSelf2();		//for snake2, to judge whether it hit itself or not
void ClearFood();	//for snake1, to clear the food that snake ate	
void ClearFood2();	//for snake2, to clear the food that snake ate	
void AddFood();		//for both snakes, to add one food 




HWND hWnd;			//the global hWnd
HINSTANCE hInst;	//the global hInstance
CTable table;		//the main object in the game
int cx, cy;			//the length and width of screen
int cxChecker, cyChecker;	//the length and width of each checker
bool bState;		//the state of game
int speed = 200;		//the initial speed															//crack the speed
int nScore = 100;		//the initial score
int nScore2 = 100;		//make one more to count score of snake2

HBRUSH hBodyBrush;		//to draw the body of snake1
HBRUSH hBodyBrush2;		//to draw the body of snake2



int APIENTRY WinMain(HINSTANCE hInstance,
				     HINSTANCE hPrevInstance,
				     LPSTR lpCmdLine,
					 int nCmdShow)
{
	LPCWSTR lpAppName = TEXT("freeDoubleSnake");

	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_ASTERISK);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = lpAppName;
	wc.lpszMenuName = NULL;
	wc.style = CS_VREDRAW| CS_HREDRAW;

	RegisterClass(&wc);

	
	hWnd = CreateWindow( lpAppName, lpAppName,
						 WS_OVERLAPPEDWINDOW,
						 CW_USEDEFAULT, CW_USEDEFAULT,
						 CW_USEDEFAULT, CW_USEDEFAULT,
						 NULL, 
						 NULL,
						 hInstance,
						 NULL);

	ShowWindow(hWnd, SW_SHOWMAXIMIZED);		//to show in full screen
	
	UpdateWindow(hWnd);


	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;			 
}



LRESULT CALLBACK WindowProc(  __in  HWND hWnd,
							  __in  UINT uMsg,
							  __in  WPARAM wParam,
							  __in  LPARAM lParam)
{

	PAINTSTRUCT ps;
	HDC hDC;
	static HBRUSH hWhiteBrush;		//to paint the food

	switch(uMsg)
	{
	case WM_CREATE:
		{
			MessageBox(hWnd, TEXT("Both snakes are undying, just try to get more score!"), TEXT("Tips"), MB_OK|MB_ICONINFORMATION);     //make some tips			
			
			srand( (unsigned)time(0) );		//initial the rand()
			hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);		//to get the hInstance
			
			bState = true;		//set the initail state of game

			for(int i=0;i<NUMOFWIDTH*NUMOFHEIGHT/100;++i)		//(crack)set the initial food                        //crack the beginning number of food
			{
				AddFood();
			}

			hBodyBrush = CreateSolidBrush( RGB(255,255,255) );		//for snake1, a white to draw the initial color of snake body
			hBodyBrush2 = CreateSolidBrush( RGB(0,0,0) );		//for snake2, a black to draw the initial color of snake body


			SetTimer(hWnd, 1, speed, TimerProc);		//set the timer for snake1      
			SetTimer(hWnd, 2, speed, TimerProc2);		//set the timer for snake2                              
			SetTimer(hWnd, 3, 10000, TimerProc3);		//set the timer for adding wall

			for(int i=0;i<table.snake2.m_length;++i)		//change the initial position of snake2
			{
				table.snake2.pPos[i].x = 0;
				table.snake2.pPos[i].y = NUMOFHEIGHT/2+1;
			}
			table.snake2.m_direction = Right;
		}break;

	case WM_SIZE:
		{
			cx = LOWORD(lParam);		//get the cx and cy according to the window
			cy = HIWORD(lParam);
			cxChecker = cx/NUMOFWIDTH;		//get the cxChecker and cyChecker
			cyChecker = cy/NUMOFHEIGHT;	
		}
	case WM_PAINT:
		{
			hDC = BeginPaint(hWnd, &ps);

			HBRUSH  hWallBrush = CreateSolidBrush( RGB(255,0,0) );	//a red brush to paint the wall	
			hWhiteBrush = (HBRUSH)SelectObject(hDC, hWallBrush);	//paint the wall firstly							
			for(int i=0;i<NUMOFWIDTH;++i)											
			{
				for(int j=0;j<NUMOFHEIGHT;++j)
				{
					if(-1==table.table[i][j])
					{
						//Ellipse(hDC, i*cxChecker, j*cxChecker, (i+1)*cxChecker, (j+1)*cyChecker);		//another shape of wall
						Rectangle(hDC, (i+0.1) *cxChecker, (j+0.1) *cyChecker, (i+1-0.1) *cxChecker, (j+1-0.1) *cyChecker);  
					}
				}
			}


			HBRUSH hHeadBrush = CreateSolidBrush( RGB(rand()/255,rand()/255,rand()/255) );     //a colorful brush to paint head of snake
			SelectObject(hDC, hHeadBrush);													  													
			Rectangle(hDC, table.snake.pPos[0].x *cxChecker, table.snake.pPos[0].y *cyChecker, (table.snake.pPos[0].x+1) *cxChecker, (table.snake.pPos[0].y+1) *cyChecker);  //paint the head of snake1

			//Rectangle(hDC, table.snake2.pPos[0].x *cxChecker, table.snake2.pPos[0].y *cyChecker, (table.snake2.pPos[0].x+1) *cxChecker, (table.snake2.pPos[0].y+1) *cyChecker);  //paint the head of snake2			
			POINT a, b, c;																																						   //chose to paint a triangle head of snake2
			a.x = (table.snake2.pPos[0].x+1) *cxChecker;
			a.y = (table.snake2.pPos[0].y+0.5) *cyChecker;
			b.x = table.snake2.pPos[0].x *cxChecker;
			b.y = table.snake2.pPos[0].y *cyChecker;
			c.x = table.snake2.pPos[0].x *cxChecker;
			c.y = (table.snake2.pPos[0].y+1) *cyChecker;
			POINT pPoint[3] = {a,b,c};
			Polygon(hDC, pPoint, 3);


			SelectObject(hDC, hBodyBrush);														//paint the body of sanek1
			for(int i=1;i<table.snake.m_length;++i)
			{
				Ellipse(hDC, table.snake.pPos[i].x *cxChecker, table.snake.pPos[i].y *cyChecker, (table.snake.pPos[i].x+1) *cxChecker, (table.snake.pPos[i].y+1) *cyChecker);  
			}		

			SelectObject(hDC, hBodyBrush2);														//paint the body of snake2
			for(int i=1;i<table.snake2.m_length;++i)
			{
				Rectangle(hDC, table.snake2.pPos[i].x *cxChecker, table.snake2.pPos[i].y *cyChecker, (table.snake2.pPos[i].x+1) *cxChecker, (table.snake2.pPos[i].y+1) *cyChecker);  
			}


			DeleteObject(hWallBrush);
			DeleteObject(hHeadBrush);		

			
			hHeadBrush = (HBRUSH)SelectObject(hDC, hWhiteBrush);
			for(int i=0;i<NUMOFWIDTH;++i)											//paint the food
			{
				for(int j=0;j<NUMOFHEIGHT;++j)
				{
					if(1==table.table[i][j])
					{
						//Ellipse(hDC, i*cxChecker, j*cxChecker, (i+1)*cxChecker, (j+1)*cyChecker);		//another type of food
						Rectangle(hDC, (i+0.2) *cxChecker, (j+0.2) *cyChecker, (i+1-0.2) *cxChecker, (j+1-0.2) *cyChecker);		//make the food a little smaller
					}
				}
			}


			TCHAR buffer[256];		//paint the score
			//nScore = (table.snake.m_length+table.snake2.m_length) * 100 - 400;
			wsprintf(buffer,TEXT("%i"),nScore);
			//itoa(nScore, buffer, 10);
			if(nScore<1000)		//just set the format of text
			{
				TextOut(hDC, cx/4, 7, (LPCWSTR)buffer, 3);
			}
			else if(nScore<10000)
			{
				TextOut(hDC, cx/4, 7, (LPCWSTR)buffer, 4);
			}
			else 
			{
				TextOut(hDC, cx/4, 7, (LPCWSTR)buffer, 5);
			}

			wsprintf(buffer,TEXT("%i"),nScore2);			//to paint one more for snake2
			if(nScore2<1000)		//just set the format of text
			{
				TextOut(hDC, 3*cx/4, 7, (LPCWSTR)buffer, 3);
			}
			else if(nScore2<10000)
			{
				TextOut(hDC, 3*cx/4, 7, (LPCWSTR)buffer, 4);
			}
			else 
			{
				TextOut(hDC, 3*cx/4, 7, (LPCWSTR)buffer, 5);
			}


			EndPaint(hWnd, &ps);
		}break;


	case WM_KEYDOWN:
		{
			switch(wParam)		
			{
			case VK_UP:		//for snake1, turn up, right, down or left
				{
					table.snake.ChangeDirection(Up);		//by change snake's direction
				}break;

			case VK_RIGHT:
				{
					table.snake.ChangeDirection(Right);
				}break;

			case VK_DOWN:
				{
					table.snake.ChangeDirection(Down);
				}break;

			case VK_LEFT:
				{
					table.snake.ChangeDirection(Left);
				}break;


			case 'W':		//for snake1, turn up, right, down or left													//make another key for the second snake
				{
					table.snake2.ChangeDirection(Up);
				}break;

			case 'D':
				{
					table.snake2.ChangeDirection(Right);
				}break;

			case 'S':
				{
					table.snake2.ChangeDirection(Down);
				}break;

			case 'A':
				{
					table.snake2.ChangeDirection(Left);
				}break;


			case VK_SPACE:
				{
					if(true==bState)
					{
						KillTimer(hWnd, 1);
						KillTimer(hWnd, 2);
						bState = false;
					}
					else
					{
						SetTimer(hWnd, 1, speed, TimerProc);
						SetTimer(hWnd, 2, speed, TimerProc2);
						bState = true;
					}
				}break;
				{//Unbelievable				}
			case VK_ESCAPE:	//to reset the position of snake1 and snake2				{
					for(int i=0;i<table.snake.m_length;++i)
					{
						table.snake.pPos[i].x = 0;
						table.snake.pPos[i].y = NUMOFHEIGHT/2;
					}
					table.snake.m_direction = Right;

					for(int i=0;i<table.snake2.m_length;++i)
					{
						table.snake2.pPos[i].x = 0;
						table.snake2.pPos[i].y = NUMOFHEIGHT/2+1;
					}
					table.snake2.m_direction = Right;
				}break;			case 'E':						{					KillTimer(hWnd, 1);					KillTimer(hWnd, 2);					if(speed>10)					{						speed -=10;					}					SetTimer(hWnd, 1, speed, TimerProc);							SetTimer(hWnd, 2, speed, TimerProc2);				}break;			case 'Q':						{					KillTimer(hWnd, 1);					KillTimer(hWnd, 2);					if(speed<1000)					{						speed +=10;					}					SetTimer(hWnd, 1, speed, TimerProc);					SetTimer(hWnd, 2, speed, TimerProc2);				}break;			case 'P':				{					PostQuitMessage(0);				}break;			case VK_SHIFT:				{					for(int i=0;i<NUMOFWIDTH;++i)
					{
						for(int j=1;j<NUMOFHEIGHT-1;++j)                               //initial the table
						{
							if(-1==table.table[i][j])
							{
								table.table[i][j] = 0;                                          //make table avaliable beside the wall
							}
						}

						table.table[i][0] = -1;                                             //set the wall
						table.table[i][NUMOFHEIGHT-1] = -1;
					}					if(table.snake.m_length>2)					{						table.snake.m_length -= 1;					}					if(table.snake2.m_length>2)					{						table.snake2.m_length -= 1;					}					InvalidateRect(hWnd, NULL, true);				}break;			}
		}break;





	case WM_CLOSE:
		{
			PostQuitMessage(0);
		}break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}




VOID CALLBACK TimerProc(HWND hwnd,
						UINT uMsg,
						UINT_PTR idEvent,
						DWORD dwTime)
{
	table.snake.Move();
	InvalidateRect(hWnd, NULL, true);

	if( IsFood() )
	{
		table.snake.Add();		//add the length of snake1
		//table.snake.Reduce();
		nScore += 100;		//add the score


		for(int i=0;i<1;++i)                                            //set the wall
		{
			table.SetWall();
		}


		ClearFood();
		AddFood();

		DeleteObject(hBodyBrush);
		hBodyBrush = CreateSolidBrush( RGB(rand()/255,rand()/255,rand()/255) );
	}
	
	if( IsWall() )																//make nothing to kill the snake
	{

		if(Up==table.snake.m_direction) table.snake.m_direction = Down;			//make the snake head back
		else if(Down==table.snake.m_direction) table.snake.m_direction = Up;
		else if(Left==table.snake.m_direction) table.snake.m_direction = Right;
		else if(Right==table.snake.m_direction) table.snake.m_direction = Left;

		table.snake.Move();
		InvalidateRect(hWnd, NULL, true);

		//SendMessage(hWnd, WM_KEYDOWN, VK_SPACE, NULL);
		//int res = MessageBox(NULL, TEXT("Player1 :Do you want to continue?"), TEXT("Hope you were"), MB_YESNO|MB_ICONQUESTION);
		//if(IDYES==res)
		//{
		//	SendMessage(hWnd, WM_KEYDOWN, VK_SPACE, NULL);        //to continue
		//	SendMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, NULL);       //to reset the position
		//}
		//else
		//{
		//	PostQuitMessage( 0 );
		//}
	}
}


VOID CALLBACK TimerProc2(HWND hwnd,                                    //the copy of TimerProc(...) 
						UINT uMsg,
						UINT_PTR idEvent,
						DWORD dwTime)
{
	table.snake2.Move();		
	InvalidateRect(hWnd, NULL, true);

	if( IsFood2() )
	{
		table.snake2.Add();		//add the length of snake2
		//table.snake.Reduce();
		nScore2 += 100;		//add the score


		for(int i=0;i<1;++i)                                            //set the wall
		{
			table.SetWall();
		}

		ClearFood2();
		AddFood();

		DeleteObject(hBodyBrush2);
		hBodyBrush2 = CreateSolidBrush( RGB(rand()/255,rand()/255,rand()/255) );
	}
	
	if( IsWall2() )
	{
		if(Up==table.snake2.m_direction) table.snake2.m_direction = Down;
		else if(Down==table.snake2.m_direction) table.snake2.m_direction = Up;
		else if(Left==table.snake2.m_direction) table.snake2.m_direction = Right;
		else if(Right==table.snake2.m_direction) table.snake2.m_direction = Left;

		table.snake2.Move();		
		InvalidateRect(hWnd, NULL, true);

		//SendMessage(hWnd, WM_KEYDOWN, VK_SPACE, NULL);
		//int res = MessageBox(NULL, TEXT("Player2 : Do you want to continue?"), TEXT("Hope you were"), MB_YESNO|MB_ICONQUESTION);
		//if(IDYES==res)
		//{
		//	SendMessage(hWnd, WM_KEYDOWN, VK_SPACE, NULL);        //to continue
		//	SendMessage(hWnd, WM_KEYDOWN, VK_ESCAPE, NULL);       //to reset the position
		//}
		//else
		//{
		//	PostQuitMessage( 0 );
		//}
	}
}


VOID CALLBACK TimerProc3(HWND hwnd,
						UINT uMsg,
						UINT_PTR idEvent,
						DWORD dwTime)
{

	table.SetWall();
}




bool IsFood()
{
	if( 1==table.table[ table.snake.pPos[0].x ][ table.snake.pPos[0].y ] )
	{
		return true;
	}
	else
	{
		return false;
	}

}
bool IsFood2()																	//almost the same as IsFood()
{
	if( 1==table.table[ table.snake2.pPos[0].x ][ table.snake2.pPos[0].y ] )
	{
		return true;
	}
	else
	{
		return false;
	}

}

void ClearFood()
{
	table.table[ table.snake.pPos[0].x ][ table.snake.pPos[0].y ] = 0;
}
void ClearFood2()
{
	table.table[ table.snake2.pPos[0].x ][ table.snake2.pPos[0].y ] = 0;
}

void AddFood()
{
	int tempx, tempy;
	tempx = rand()%NUMOFWIDTH;
	tempy = rand()%(NUMOFHEIGHT-2)+1;
	
	while(true)															//to avoid making food in the snake
	{
		for(int i=0;i<( table.snake.m_length > table.snake2.m_length ? table.snake2.m_length : table.snake.m_length);++i)
		{
			if((table.snake.pPos[i].x!=tempx || table.snake.pPos[i].x!=tempy) && (table.snake.pPos[i].x!=tempx || table.snake.pPos[i].x!=tempy))
			{
				table.table[tempx][tempy] = 1;                             //set the food in the table
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

bool IsWall()
{
	if( -1==table.table[ table.snake.pPos[0].x ][ table.snake.pPos[0].y ] )
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool IsWall2()
{
	if( -1==table.table[ table.snake2.pPos[0].x ][ table.snake2.pPos[0].y ] )
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool IsSelf()
{
	for(int i=1;i<table.snake.m_length;++i)
	{
		if(table.snake.pPos[i].x == table.snake.pPos[0].x  &&  table.snake.pPos[i].y == table.snake.pPos[0].y )
		{
			return true;
		}
	}

	return false;
}
bool IsSelf2()
{
	for(int i=1;i<table.snake2.m_length;++i)
	{
		if(table.snake2.pPos[i].x == table.snake2.pPos[0].x  &&  table.snake2.pPos[i].y == table.snake2.pPos[0].y )
		{
			return true;
		}
	}

	return false;
}