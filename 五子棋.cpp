/*-------------------------------------------------
   CHECKER1.C -- Mouse Hit-Test Demo Program No. 1
                 (c) Charles Petzold, 1998
  -------------------------------------------------*/
#pragma once
#include <windows.h>
#include <mmsystem.h>
#include <time.h>
#include "resource.h"

#pragma comment(lib, "Winmm.lib")

#define FRAME		40

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

#include "define.h"
#include "main.hpp"
#include "resource.h"//**************************************

GoBang_Game gbgame;
clock_t start,end;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR  szCmdLine, int iCmdShow)
{	
	//******声明win api的类
     static TCHAR szAppName[] = TEXT ("五子棋") ;
     HWND         hwnd ;	//*****window handle 
     MSG          msg ;     //*****message		 Gets or sets the message identifier.
     WNDCLASS     wndclass ;      //******Win类  
     
     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
//     wndclass.hIcon         = LoadIcon (NULL, MAKEINTRESOURCE(IDI_APPLICATION)) ;
	 wndclass.hIcon       = LoadIcon(hInstance, MAKEINTRESOURCE("IDI_ICON1"));//************************
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (LTGRAY_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;
     
     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("Program requires Windows NT!"), 
                      szAppName, MB_OK) ;
		  MessageBeep (0) ;
          return 0 ;
     }


	 //******建立页面#define CreateWindowA(lpClassName, lpWindowName, dwStyle, x, y,\
       //nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)\

     hwnd = CreateWindow (szAppName, TEXT ("快乐五子棋"),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          FRAME*(SIZE_BOARD+1)+10, FRAME*(SIZE_BOARD+1)+30,
                          NULL, NULL, hInstance, NULL) ;//*******hInstance程序实例句柄
     
     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;
     
	 //PlaySound(TEXT("start.wav"),NULL,SND_ASYNC);    //******背景音乐播放


	 //********GetMessage（LPMSG lpMsg，HWND hWnd，UINT wMsgFilterMin，UINT wMsgFilterMax)
     while (GetMessage (&msg, NULL, 0, 0))
     {
          TranslateMessage (&msg) ;
		  end = clock();//只要有消息进来就看看时间到了没有，到了就催！
		  if ((end-start)>10000)
		  {
			  //PlaySound(TEXT("kuai.wav"),NULL,SND_ASYNC);
			  start = end = clock();//催了之后要重新开始计时
		  }
          DispatchMessage (&msg) ;
     }
     return msg.wParam ;
}

int new_x,new_y;

void NewGame(HWND hwnd,int who_win)
{
	gbgame.Record(0,0,who_win);
	gbgame.Clear();
	new_x = 0;
	new_y = 0;
	start = end = clock();
	InvalidateRect (hwnd, NULL, TRUE);
	UpdateWindow(hwnd);
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     HDC         hdc ;
     int         x, y ;
     PAINTSTRUCT ps ;
     RECT        rect ;
     int i;
	 char szAppName[] = "五子棋" ;

     switch (message)
     {          
     case WM_LBUTTONDOWN :         
		 
          x = (LOWORD (lParam)-FRAME/2) / FRAME + 1;
          y = (HIWORD (lParam)-FRAME/2) / FRAME + 1;
          
          if ( x <= SIZE_BOARD && y <= SIZE_BOARD &&
			  (LOWORD (lParam)-FRAME/2)>=0  && (HIWORD (lParam)-FRAME/2)
			  && !gbgame.broad.Table[y][x] )
          {
              hdc = BeginPaint (hwnd, &ps) ;
			  
			  gbgame.broad.Table [y][x] = PLAYER;
			  gbgame.evaluation.Change_State(y,x,gbgame.broad);
			  gbgame.Record(y,x);

//			  SelectObject (hdc, GetStockObject (WHITE_BRUSH)) ;		
//			  Ellipse( hdc,x*FRAME-FRAME/2+1,y*FRAME-FRAME/2+1,x*FRAME+FRAME/2-1,y*FRAME+FRAME/2-1);
/*			  rect.left   = x*FRAME-FRAME/2+1 ;
			  rect.top    = y*FRAME-FRAME/2+1 ;
			  rect.right  = x*FRAME+FRAME/2-1 ;
              rect.bottom = y*FRAME+FRAME/2-1 ;	 
*/			  InvalidateRect(hwnd,NULL,FALSE);
			  UpdateWindow(hwnd);
			  
			  //PlaySound(TEXT("qizi.wav"),NULL,SND_ASYNC);
			  start = end = clock();//人类下子之后重新开始计时

			  //判断人类玩家下子后是否结束棋局
			  switch (gbgame.broad.Is_Over(y,x))
			  {
			  case NOT_OVER:
				  break;
			  case COMPUTER_WIN:
				  //PlaySound(TEXT("lose.wav"),NULL,SND_ASYNC);///////////////////
				  MessageBox (NULL, TEXT ("对不起，您失败了!"),
					  szAppName, MB_OK) ;
				  NewGame(hwnd,COMPUTER_WIN);				  
goto end;
			  case PLARER_WIN:
				  //PlaySound(TEXT("win.wav"),NULL,SND_ASYNC);///////////////////
				  MessageBox (NULL, TEXT ("恭喜你，您胜利了!"), 
					  szAppName, MB_OK) ;
				  NewGame(hwnd,PLARER_WIN);
goto end;
			  case OVER_N_DRAW:
				  //PlaySound(TEXT("qizi.wav"),NULL,SND_ASYNC);///////////////////
				  MessageBox (NULL, TEXT ("不分胜负，和棋!"), 
					  szAppName, MB_OK) ;
				  NewGame(hwnd,OVER_N_DRAW);
goto end;
			  }	
 ///////////////////////////////////////////////////////////////////////////////////
			  gbgame.Turn_To_Computer(y,x);	  
			  gbgame.Record(y,x);
			  new_x = x;
			  new_y = y;

/*			  SelectObject (hdc, GetStockObject (BLACK_BRUSH)) ;		
		      Ellipse( hdc,x*FRAME-FRAME/2+1,y*FRAME-FRAME/2+1,x*FRAME+FRAME/2-1,y*FRAME+FRAME/2-1);
			
			  SelectObject (hdc, GetStockObject (WHITE_BRUSH)) ;		
			  Ellipse( hdc,new_x*FRAME-FRAME/2+15,new_y*FRAME-FRAME/2+15,new_x*FRAME+FRAME/2-15,new_y*FRAME+FRAME/2-15);
*/
/*			  rect.left   = x*FRAME-FRAME/2+1 ;
			  rect.top    = y*FRAME-FRAME/2+1 ;
			  rect.right  = x*FRAME+FRAME/2-1 ;
              rect.bottom = y*FRAME+FRAME/2-1 ;	 
*/			  InvalidateRect(hwnd,NULL,FALSE);
			  UpdateWindow(hwnd);

			  //PlaySound(TEXT("qizi.wav"),NULL,SND_ASYNC);
			  
			  start = end = clock();//电脑下子之后开始计时

			  //判断电脑玩家下子后是否结束棋局
			  switch (gbgame.broad.Is_Over(y,x))
			  {
			  case NOT_OVER:
				  break;
			  case COMPUTER_WIN:
				  //PlaySound(TEXT("lose.wav"),NULL,SND_ASYNC);///////////////////
				  MessageBox (NULL, TEXT ("对不起，您失败了!"),
					  szAppName, MB_OK) ;
				  NewGame(hwnd,COMPUTER_WIN);				  
goto end;
			  case PLARER_WIN:
				 // PlaySound(TEXT("win.wav"),NULL,SND_ASYNC);///////////////////
				  MessageBox (NULL, TEXT ("恭喜你，您胜利了!"), 
					  szAppName, MB_OK) ;
				  NewGame(hwnd,PLARER_WIN);
goto end;
			  case OVER_N_DRAW:
				  //PlaySound(TEXT("lose.wav"),NULL,SND_ASYNC);///////////////////
				  MessageBox (NULL, TEXT ("不分胜负，和棋!"), 
					  szAppName, MB_OK) ;
				  NewGame(hwnd,OVER_N_DRAW);
goto end;
			  }			  
			  EndPaint (hwnd, &ps) ; 
		  }
          else
		  {
			  MessageBeep (0) ;
		  }

end:
          return 0 ;
          
     case WM_PAINT :
          hdc = BeginPaint (hwnd, &ps) ;
		  
		  //画标志数字
		  char flagnum[2];
		  flagnum[0] = '1';
		  flagnum[1] = '1';
		  char flagchar[1];
		  flagchar[0] = 'A';
		  SetBkMode(hdc,TRANSPARENT);
		  for ( i=1; i <= SIZE_BOARD; i++ )
		  {
			  if (i<=9)
			  {
				  TextOut(hdc, 5, FRAME*i-10, &flagnum[1], 1);
				  flagnum[1] ++;			
			  }
			  else
			  {
				  flagnum[1] = i+48-10;
				  TextOut(hdc, 5, FRAME*i-10, flagnum, 2);
			  }
			  TextOut(hdc, FRAME*i-4, 5, flagchar, 1);
			  flagchar[0]++;
		  }
		 
		for ( i=1; i<=SIZE_BOARD; i++)//画直线
		{
			MoveToEx( hdc, FRAME, i*FRAME, NULL);
			LineTo(	hdc, SIZE_BOARD*FRAME, i*FRAME);
			MoveToEx( hdc, i*FRAME, FRAME, NULL);
			LineTo( hdc, i*FRAME, SIZE_BOARD*FRAME);
		}

		for (x = 1 ; x <= SIZE_BOARD ; x++)
		{
			for (y = 1 ; y <= SIZE_BOARD ; y++)//画棋子
			{
				if ( gbgame.broad.Table [y][x]==PLAYER )
				{
					SelectObject (hdc, GetStockObject (WHITE_BRUSH)) ;		
					Ellipse( hdc,x*FRAME-FRAME/2+1,y*FRAME-FRAME/2+1,x*FRAME+FRAME/2-1,y*FRAME+FRAME/2-1);         
				}//if
				if ( gbgame.broad.Table [y][x]==COMPUTER )
				{
					SelectObject (hdc, GetStockObject (BLACK_BRUSH)) ;		
					Ellipse( hdc,x*FRAME-FRAME/2+1,y*FRAME-FRAME/2+1,x*FRAME+FRAME/2-1,y*FRAME+FRAME/2-1);
				}//if
			}//for
		}//for

		//画小圈，标志最新棋子
		if (new_x)
		{
			SelectObject (hdc, GetStockObject (WHITE_BRUSH)) ;		
			Ellipse( hdc,new_x*FRAME-FRAME/2+15,new_y*FRAME-FRAME/2+15,new_x*FRAME+FRAME/2-15,new_y*FRAME+FRAME/2-15);
		}

		EndPaint (hwnd, &ps) ;
        return 0 ;
               
     case WM_DESTROY :
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}
