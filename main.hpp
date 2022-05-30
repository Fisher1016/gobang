//main.cpp提供如下服务：
#pragma once
#include <limits.h>
#include <math.h>
#include <iostream>

#include "define.h"

//棋局类
class Broad
{
public:
	Broad();	
	//构造函数，将两个私有数组成员的数值全部初始化为0
	
	int Is_Over(int x,int y);
	//判断有无五子相连的情形，若有则某一方胜利，退出程序

	void MakeMove(int x,int y,int who);
	//落子函数
	
	void UnMakeMove(int x,int y);
	//撤销落子函数

	//下面是棋盘存储结构
	//为了便于编码，将存储结构扩大一圈，扩大的那一圈上的值都是-1
	
	int Table[SIZE_BOARD+2][SIZE_BOARD+2];
	//棋盘的存储结构，0代表没有棋子，1代表玩家的棋子，2代表电脑的棋子
	
	int Link_Table[SIZE_BOARD+2][SIZE_BOARD+2][4];//棋盘上连子状态的存储结构，当Table[x][y]==0时Link_Table[x][y]没有意义，
	//当Table[x][y]!=0时，Link_Table[x][y][0 ~ 3]分别代表(x,y)点上横、竖、正对角线、反对角线四个方向上连子的最大数目
	
	int Score[SIZE_BOARD+2][SIZE_BOARD+2];
	//棋盘上该点的分数
};

Broad::Broad()
//构造函数
{
	int i,j,k;
	for ( i=1; i<=SIZE_BOARD; i++)		//将私有数组成员中实际存储区初始化为0
	{
		for ( j=1; j<=SIZE_BOARD; j++)
		{
			Table[i][j] = 0;
			Score[i][j] = 0;
		}//for j
	}//for i
	
	for ( i=1; i<=SIZE_BOARD; i++)
	{
		for ( j=1; j<=SIZE_BOARD; j++)
		{
			Link_Table[i][j][0] = SIZE_BOARD;
			Link_Table[i][j][1] = SIZE_BOARD;
			Link_Table[i][j][2] = SIZE_BOARD-abs(i-j);
			Link_Table[i][j][3] = SIZE_BOARD-abs(SIZE_BOARD-(i+j)+1);
		}
	}//for
	
	for ( i=0,j=0; i<=SIZE_BOARD+1; i++)//将私有数组成员中溢出区初始化为-1
	{
		Table[i][j] = BOUNDARY;
		Score[i][j] = BOUNDARY;
		Table[j][i] = BOUNDARY;
		Score[j][i] = BOUNDARY;
		Table[SIZE_BOARD+1][i] = BOUNDARY;
		Score[SIZE_BOARD+1][i] = BOUNDARY;
		Table[i][SIZE_BOARD+1] = BOUNDARY;
		Score[i][SIZE_BOARD+1] = BOUNDARY;
		for ( k=0; k<4; k++)
		{
			Link_Table[i][j][k] = BOUNDARY;
			Link_Table[j][i][k] = BOUNDARY;
			Link_Table[SIZE_BOARD+1][i][k] = BOUNDARY;
			Link_Table[i][SIZE_BOARD+1][k] = BOUNDARY;
		}
	}	
}//Broad

int Broad::Is_Over(int x,int y)//判断当前棋局是否已经分出胜负
{////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (Table[x][y]!=0)
	{
		if (Link_Table[x][y][0]>=5 || Link_Table[x][y][1]>=5 || Link_Table[x][y][2]>=5 || Link_Table[x][y][3]>=5)
		{
			return (Table[x][y]==COMPUTER?COMPUTER_WIN:PLARER_WIN);
		}//if
	}//if		
	
	return NOT_OVER;
}//Is_Over

/*
int Broad::Is_Over()//判断当前棋局是否已经分出胜负
{
	int x,y;
	for ( x=1; x<=SIZE_BOARD; x++)
	{
		for ( y=1; y<=SIZE_BOARD; y++)
		{
			if (Table[x][y]!=0)
			{
				if (Link_Table[x][y][0]>=5 || Link_Table[x][y][1]>=5 || Link_Table[x][y][2]>=5 || Link_Table[x][y][3]>=5)
				{
					return (Table[x][y]==COMPUTER?COMPUTER_WIN:PLARER_WIN);
				}//if
			}//if		
		}//for
	}//for
	for ( x=1; x<=SIZE_BOARD; x++)
	{
		for ( y=1; y<=SIZE_BOARD && Table[x][y]; y++)
		{
			;
		}
	}
	if (x==SIZE_BOARD+1 && y==SIZE_BOARD+1)
	{
		return OVER_N_DRAW;
	}
	
	return NOT_OVER;
}//Is_Over
*/
void Broad::MakeMove(int x,int y,int who)
{
	Table[x][y] = who;
}//MakeMove

void Broad::UnMakeMove(int x,int y)
{
	Table[x][y] = 0;
}//UnMakeMove

//估值引擎类

//调用棋局对象，用于棋局估值

//注意：如果Basic_Evaluate和Advanced_Evaluate同时使用，应当是Basic_Evaluate在前

class Evaluation
{
public:
	int Get_Value(int type,Broad &broad,int who);
	//根据棋局所有空位上的分数和当前走棋方，返回该棋局的分数
	
	void Change_State(int x,int y,Broad &broad);
	//修改在(x,y)上落子后棋盘的状态
	
private:
	void Basic_Evaluate(Broad &broad);
	//对棋局上的所有空位进行简单的打分
	
	void Advanced_Evaluate(Broad &broad);
	//对棋局上的所有空位进行有无绝杀的判断，有的话则重置该点分数（都是很高的分数），以便估值精确

	int Direct(int mode,int x,int y,int dir,int len,Broad &broad);
	//返回(由mode指定)数组中(x,y)的dir方向上距离为len的位置的数值

};

int Evaluation::Get_Value(int type,Broad &broad,int who)
{
	int i,j;
	switch (type)
	{
	case BASIC:
		Basic_Evaluate(broad);	
		break;

	case ADVANCED:
		Advanced_Evaluate(broad);
		break;
	}
		
	int max=INT_MIN;
	for ( i=1; i<=SIZE_BOARD; i++)
	{
		for ( j=1; j<=SIZE_BOARD; j++)
		{
			max = max>broad.Score[i][j]?max:broad.Score[i][j];   
		}//for
	}//for

	return who==COMPUTER?max:max*(-1);
}//Get_Value

void Evaluation::Change_State(int x,int y,Broad &broad)//修改在(x,y)上落子后棋盘的状态
{	
	int i,j,start,end;
	int temp_x,temp_y;
	temp_x = x;
	temp_y = y;
	
	//横方向
	for ( start=1,end=1; end<=SIZE_BOARD; )
	{
		for ( start=end; broad.Table[x][start]==broad.Table[x][end]; end++)
		{	
			;
		}//for
		for ( i=start,j=end-start; i<end; i++)
		{
			broad.Link_Table[x][i][0] = j;
		}//for
	}//for
	
	//竖方向
	for ( start=1,end=1; end<=SIZE_BOARD; )
	{
		for ( start=end; broad.Table[start][y]==broad.Table[end][y]; end++)
		{	
			;
		}//for
		for ( i=start,j=end-start; i<end; i++)
		{
			broad.Link_Table[i][y][1] = j;
		}//for
	}//for
	
	//正对角线方向
	x = temp_x - (temp_x<temp_y?temp_x:temp_y) + 1;
	y = temp_y - (temp_x<temp_y?temp_x:temp_y) + 1;
	for ( start=0,end=0; x+end<=SIZE_BOARD && y+end<=SIZE_BOARD; )
	{
		for ( start=end; broad.Table[x+start][y+start]==broad.Table[x+end][y+end]; end++)
		{
			;
		}//for
		for ( i=start,j=end-start; i<end; i++)
		{
			broad.Link_Table[x+i][y+i][2] = j;
		}//for
	}//for
	
	//反对角线方向
	if (temp_x+temp_y<=SIZE_BOARD+1)
	{
		x = temp_x + temp_y - 1;
		y = 1;
	}
	else
	{
		x = SIZE_BOARD;
		y = temp_x + temp_y - SIZE_BOARD;
	}
	for ( start=0,end=0; x-end>=1 && y+end<=SIZE_BOARD; )
	{
		for ( start=end; broad.Table[x-start][y+start]==broad.Table[x-end][y+end]; end++)
		{
			;
		}//for
		for ( i=start,j=end-start; i<end; i++)
		{
			broad.Link_Table[x-i][y+i][3] = j;
		}//for
	}//for
	
}//Change_Situation

void Evaluation::Basic_Evaluate(Broad &broad)
{
	int x,y;
	int i,j;
	int temp;

	for ( x=1; x<=SIZE_BOARD; x++)
	{
		for ( y=1; y<=SIZE_BOARD; y++)
		{
			if (broad.Table[x][y]!=0)//如果当前位置已经有棋子了，则此位置分数为零，并continue
			{
				broad.Score[x][y] = 0;
				continue;
			}

			//(x,y)为空，则对(x,y)进行评分。
			//分数评定的标准是：////////////////////////////////////////////////////////////////////////////////////////////
			//四个方向的分数相加，得到该点分数

			//先将该点分数置为1，以便摆脱以前的影响
			broad.Score[x][y] = 1;

			for ( i=0; i<4; i++)
			{
				//一空一子			
				if ((Direct(1,x,y,i,1,broad)==0 && Direct(1,x,y,i+4,1,broad)>0) ||
					(Direct(1,x,y,i,1,broad)>0 && Direct(1,x,y,i+4,1,broad)==0))
				{
				
					if ((Direct(2,x,y,i,1,broad)+Direct(2,x,y,i+4,1,broad)) <= 3)//空加子小于等于3
					{
						if (Direct(1,x,y,i,1,broad)==0)
						{
							if (Direct(1,x,y,i+4,Direct(2,x,y,i+4,1,broad)+1,broad)==0)//子的另一端也为空
							{
								temp = Direct(2,x,y,i+4,1,broad) + Direct(2,x,y,i,1,broad) + Direct(2,x,y,i+4,Direct(2,x,y,i+4,1,broad)+1,broad);
								//temp此时等于子+空+子的另一端的空
								if (temp>=4)
								{
									temp = Direct(2,x,y,i+4,1,broad);
									temp *= 10;
									goto loop;
								}
								else
								{
									temp = 0;
									goto loop;
								}//else
							}//if
							else//子的另一端被堵
							{
								temp = 0;
								goto loop;
							}//else
						}//if
						else
						{
							if (Direct(1,x,y,i,Direct(2,x,y,i,1,broad)+1,broad)==0)//子的另一端也为空
							{
								temp = Direct(2,x,y,i,1,broad) + Direct(2,x,y,i+4,1,broad) + Direct(2,x,y,i,Direct(2,x,y,i,1,broad)+1,broad);
								//temp此时等于子+空+子的另一端的空
								if (temp>=4)
								{
									temp = Direct(2,x,y,i,1,broad);
									temp *= 10;
									goto loop;
								}
								else
								{
									temp = 0;
									goto loop;
								}//else
							}//if
							else//子的另一端被堵
							{
								temp = 0;
								goto loop;
							}//else
						}//else
					}//if
					else//此时空+子大于等于4
					{
						temp = Direct(1,x,y,i,1,broad)==0 ? Direct(2,x,y,i+4,1,broad) : Direct(2,x,y,i,1,broad);
						temp *= 10;
						temp += 10;
						goto loop;
					}
				}//if

				//左右有不同子
				if ( (Direct(1,x,y,i,1,broad)!=Direct(1,x,y,i+4,1,broad)) &&
					Direct(1,x,y,i,1,broad)*Direct(1,x,y,i+4,1,broad)>0 ) 
				{
					if (Direct(2,x,y,i,1,broad)<=4 && Direct(2,x,y,i+4,1,broad)<=4)//两段的子数都不超过4，则为死棋
					{
						temp = 0;
						goto loop;
					}
					else//否则就是绝杀
					{
						temp = 1000;
						goto loop;
					}
				}//if

				//左右都为空，直接置为0
				if (Direct(1,x,y,i,1,broad)==0 && Direct(1,x,y,i+4,1,broad)==0)
				{
					temp = 0;
					goto loop;
				}//if	

				//遇到边界的情况
				if (Direct(1,x,y,i,1,broad)==-1)
				{
					if (Direct(1,x,y,i+4,1,broad)<=0)//另一端是边界或为空，则为死棋，置为0
					{
						temp = 0;
						goto loop;
					}
					else
					{
						if (Direct(1,x,y,i+4,Direct(2,x,y,i+4,1,broad)+1,broad)==0 && //子的另一端为空，且可以成五子
							Direct(2,x,y,i+4,1,broad)+Direct(2,x,y,i+4,Direct(2,x,y,i+4,1,broad)+1,broad)>=4)
						{
							temp = Direct(2,x,y,i+4,1,broad);
							temp *= 10;
							goto loop;
						}
						else
						{
							temp = 1;//子的另一端被堵，给个机会，让其参加绝杀判断
							goto loop;
						}
					}
				}//if
				if (Direct(1,x,y,i+4,1,broad)==-1)
				{
					if (Direct(1,x,y,i,1,broad)<=0)//另一端是边界或为空，则为死棋，置为0
					{
						temp = 0;
						goto loop;
					}
					else
					{
						if (Direct(1,x,y,i,Direct(2,x,y,i,1,broad)+1,broad)==0 && //子的另一端为空，且可以成五子
							Direct(2,x,y,i,1,broad)+Direct(2,x,y,i,Direct(2,x,y,i,1,broad)+1,broad)>=4)
						{
							temp = Direct(2,x,y,i,1,broad);
							temp *= 10;
							goto loop;
						}
						else
						{
							temp = 1;//子的另一端被堵，给个机会，让其参加绝杀判断
							goto loop;
						}
					}
				}//if
				
				//左右同子：
				if (Direct(1,x,y,i,1,broad)==Direct(1,x,y,i+4,1,broad) && Direct(1,x,y,i,1,broad)>0)
				{
					if (Direct(1,x,y,i,Direct(2,x,y,i,1,broad)+1,broad)!=0 && 
						Direct(1,x,y,i+4,Direct(2,x,y,i+4,1,broad)+1,broad)!=0 &&
						Direct(2,x,y,i,1,broad)+Direct(2,x,y,i+4,1,broad)<=3)
					//如果左子被堵，右子也被堵，且不成长连，则置为0
					{
						temp = 0;
					}
					else//左右同子，而且没有完全被堵
					{
						temp = Direct(2,x,y,i,1,broad)+Direct(2,x,y,i+4,1,broad);//temp等于左右子之和的十倍
						temp *= 10;

						if (Direct(1,x,y,i,Direct(2,x,y,i,1,broad)+1,broad)==0)//此端未被堵，temp加10
						{
							temp+=10;
						}
						if (Direct(1,x,y,i+4,Direct(2,x,y,i+4,1,broad)+1,broad)==0)//此端未被堵，temp加10
						{
							temp+=10;
						}
					}
				}

loop:	
				broad.Score[x][y] += temp;
			}//for i

		}//for y
	}//for x
}//Basic_Evaluate

void Evaluation::Advanced_Evaluate(Broad &broad)
{	
	int x,y;
	int i,j;

	for ( x=1; x<=SIZE_BOARD; x++)
	{
		for ( y=1; y<=SIZE_BOARD; y++)
		{
			if (broad.Table[x][y]!=0)//如果当前位置已经有棋子了，则此位置分数为零，并continue
			{
				broad.Score[x][y] = 0;
				continue;
			}

			if (broad.Score[x][y]==1)//如果此处分数为1，则说明不可能是绝杀位，故而直接pass即可
			{
				continue;
			}
			//(x,y)为空，则对(x,y)进行评分。

			//先看有无活四、冲四、对冲、活三、冲三三、冲三活二、活二二。

			//在这里，上述情形都称之为“绝杀点”，但是分为一级二级和三级绝杀点
			//一级绝杀点等级最高

			//在判定某点为绝杀点后，应当继续进行后续关于绝杀的判定，而且只有在分数高于当前分数时才可更新
			//原因是显而易见的，在一个关于某点的一系列判定过程中，该点重要性只能升不能降！


			//判断有无冲四和活四
			for ( i=0; i<8; i++)
			{
				if (Direct(1,x,y,i,1,broad)>0 && Direct(2,x,y,i,1,broad)==4 )
				{									
					if (Direct(1,x,y,i,1,broad)==COMPUTER)//此点为己方一级绝杀，不必继续，直接返回
					{
						broad.Score[x][y] = INT_MAX;
						return ;
					}
					else							//若是对方的一级绝杀，也得继续探测其他点
					{
						broad.Score[x][y] = INT_MAX-1;
					}					
				}
			}

			//判断能否形成长连
			for ( i=0; i<4 && broad.Score[x][y]<INT_MAX; i++)
			{
				if (Direct(1,x,y,i,1,broad)>0 &&
					Direct(1,x,y,i,1,broad)==Direct(1,x,y,i+4,1,broad) &&//方向相对，花色相同
					Direct(2,x,y,i,1,broad)+Direct(2,x,y,i+4,1,broad)>=4 )//形成对冲
				{									
					if (Direct(1,x,y,i,1,broad)==COMPUTER)//此点为己方一级绝杀，不必继续，直接返回
					{
						broad.Score[x][y] = INT_MAX;
						return ;
					}
					else							//若是对方的一级绝杀，也得继续探测其他点
					{
						broad.Score[x][y] = INT_MAX-1;
					}
				}//if
			}//for
							
			//判断有无活三
			for ( i=0; i<4 && broad.Score[x][y]<INT_MAX-2; i++)
			{
				if (Direct(1,x,y,i,1,broad)>0 &&
					Direct(1,x,y,i,1,broad)==Direct(1,x,y,i+4,1,broad) &&//方向相对，花色相同
					Direct(2,x,y,i,1,broad)+Direct(2,x,y,i+4,1,broad)>=3 && 
					Direct(1,x,y,i,Direct(2,x,y,i,1,broad)+1,broad)==0 && 
					Direct(1,x,y,i+4,Direct(2,x,y,i+4,1,broad)+1,broad)==0 )
				{
					if (Direct(1,x,y,i,1,broad)==COMPUTER)	//如果是己方的二级绝杀点，则赋值为最大减一
					{
						broad.Score[x][y] = INT_MAX-2;
					}
					else								//如果是对方的二级绝杀点，则赋值为最大减二
					{
						broad.Score[x][y] = INT_MAX-3;
					}
				}
			}
			for ( i=0; i<8 && broad.Score[x][y]<INT_MAX-2; i++)
			{
				if (Direct(1,x,y,i,1,broad)>0 &&
					Direct(2,x,y,i,1,broad)==3 && 
					Direct(1,x,y,i,4,broad)==0 &&
					Direct(1,x,y,i+4,1,broad)==0 )
				{
					if (Direct(1,x,y,i,1,broad)==COMPUTER)	//如果是己方的二级绝杀点，则赋值为最大减一
					{
						broad.Score[x][y] = INT_MAX-2;
					}
					else								//如果是对方的二级绝杀点，则赋值为最大减二
					{
						broad.Score[x][y] = INT_MAX-3;
					}	
				}//if
			}//for

			//判断有无冲三三
			for ( i=0; i<8 && broad.Score[x][y]<INT_MAX-2; i++)
			{
				for ( j=i+1; j<8; j++)
				{
					if (j%4==i)
					{
						continue;
					}
					if (Direct(1,x,y,i,1,broad)>0 && 
						Direct(1,x,y,i,1,broad)==Direct(1,x,y,j,1,broad) &&
						Direct(2,x,y,i,1,broad)==3 && Direct(2,x,y,j,1,broad)==3 && 
						(Direct(1,x,y,i,4,broad)==0 || Direct(1,x,y,i+4,1,broad)==0) &&	
						(Direct(1,x,y,j,4,broad)==0 || Direct(1,x,y,j+4,1,broad)==0))
					{
						if (Direct(1,x,y,i,1,broad)==COMPUTER)	//如果是己方的二级绝杀点，则赋值为最大减一
						{
							broad.Score[x][y] = INT_MAX-2;
						}
						else								//如果是对方的二级绝杀点，则赋值为最大减二
						{
							broad.Score[x][y] = INT_MAX-3;
						}	
					}//if
				}//for
			}//for

			//判断有无冲三活二
			for ( i=0; i<8 && broad.Score[x][y]<INT_MAX-2; i++)
			{
				for ( j=0; j<8 && broad.Score[x][y]<INT_MAX-2; j++)
				{
					if (j%4==i%4)
					{
						continue;
					}
					if (Direct(1,x,y,i,1,broad)>0 &&
						Direct(1,x,y,i,1,broad)==Direct(1,x,y,j,1,broad) &&
						Direct(2,x,y,i,1,broad)==3 && Direct(1,x,y,i+4,1,broad)==0 &&
						Direct(2,x,y,j,1,broad)==2 && Direct(1,x,y,j,3,broad)==0 &&
						Direct(1,x,y,j+4,1,broad)==0 && (Direct(1,x,y,j,4,broad)==0 || Direct(1,x,y,j+4,2,broad)==0) )
					{
						if (Direct(1,x,y,i,1,broad)==COMPUTER)		//如果是己方的二级绝杀点，则赋值为最大减一
						{
							broad.Score[x][y] = INT_MAX-2;
						}
						else									//如果是对方的二级绝杀点，则赋值为最大减二
						{
							broad.Score[x][y] = INT_MAX-3;
						}	
					}
				}
			}

			//判断有无活二二
			for ( i=0; i<8 && broad.Score[x][y]<INT_MAX-4; i++)
			{
				for ( j=0; j<8 && broad.Score[x][y]<INT_MAX-4; j++)
				{
					if (j%4==i%4)
					{
						continue;
					}
					if (Direct(1,x,y,i,1,broad)>0 &&
						Direct(1,x,y,i,1,broad)==Direct(1,x,y,j,1,broad) &&
						Direct(2,x,y,i,1,broad)==2 && Direct(1,x,y,i+4,1,broad)==0 && Direct(1,x,y,i,3,broad)==0 &&
						Direct(2,x,y,j,1,broad)==2 && Direct(1,x,y,j+4,1,broad)==0 && Direct(1,x,y,j,3,broad)==0 &&
						(Direct(1,x,y,i,4,broad)==0 || Direct(1,x,y,j,4,broad)==0) && 
						(Direct(1,x,y,i+4,2,broad)==0 || Direct(1,x,y,j+4,2,broad)==0) )
					{
						if (Direct(1,x,y,i,1,broad)==COMPUTER)	//如果是己方的三级绝杀点，则赋值为最大减二
						{
							broad.Score[x][y] = INT_MAX-4;
						}
						else								//如果是对方的三级绝杀点，则赋值为最大减三
						{
							broad.Score[x][y] = INT_MAX-5;
						}	
					}//if
				}//for
			}//for
		}//for y
	}//for x

}//Advanced_Evaluate

int Evaluation::Direct(int mode,int x,int y,int dir,int len,Broad &broad)
{
	if (mode==1)//返回Table的值
	{
		switch (dir%8)
		{
		case 0:
			return broad.Table[x][y+len];		
		case 1:
			return broad.Table[x+len][y];		
		case 2:
			return broad.Table[x+len][y+len];		
		case 3:
			return broad.Table[x+len][y-len];		
		case 4:
			return broad.Table[x][y-len];		
		case 5:
			return broad.Table[x-len][y];		
		case 6:
			return broad.Table[x-len][y-len];		
		case 7:
			return broad.Table[x-len][y+len];		
		}//switch
	}//if
		
	if (mode==2)//返回lnktable的值
	{
		switch (dir%8)
		{
		case 0:
			return broad.Link_Table[x][y+len][0];		
		case 1:
			return broad.Link_Table[x+len][y][1];		
		case 2:
			return broad.Link_Table[x+len][y+len][2];		
		case 3:
			return broad.Link_Table[x+len][y-len][3];		
		case 4:
			return broad.Link_Table[x][y-len][0];		
		case 5:
			return broad.Link_Table[x-len][y][1];		
		case 6:
			return broad.Link_Table[x-len][y-len][2];		
		case 7:
			return broad.Link_Table[x-len][y+len][3];		
		}//switch
	}//if

}//Direct


//搜索引擎类

//调用棋局对象和估值引擎对象

//作用：搜索博弈树，寻找优秀的落子点

//引擎技术：AlphaBeta剪枝 + 极大极小搜索

class Search_Engine
{
public:
	int DFSearch(Broad broad,int deepth,int alpha,int beta);

	Search_Engine();

	int x,y;

	int num_searched_point;

private:
	Evaluation evaluation;
	Broad temp_broad;
	int scores[SEEK_WIDTH];
};

Search_Engine::Search_Engine()
{
	num_searched_point=0;
}

int Search_Engine::DFSearch(Broad broad,int deepth,int alpha,int beta)
//who表示轮到某一方落子，引用x,y以便带回最优位置，返回棋局分数
{
	num_searched_point++;

	if (deepth==0)//转存数据
	{
		temp_broad = broad;
	}

	//对当前局面进行估值
	const int who = deepth%2==0 ? COMPUTER : PLAYER;
	int worth;
	evaluation.Get_Value(BASIC,temp_broad,who);
	worth = evaluation.Get_Value(ADVANCED,temp_broad,who);

	//叶子节点返回估值
	if (deepth==SEEK_DEEPTH-1)
	{
		num_searched_point++;

		return worth;
	}//if

	int i,j,k;
	//得到分数较高的若干个位置的坐标
	int good_place_x[SEEK_WIDTH],good_place_y[SEEK_WIDTH];
	for ( i=SEEK_WIDTH-1; i>=0; i--)
	{
		for ( j=1,scores[i]=0; j<=SIZE_BOARD; j++)//遍历整个棋盘
		{
			for ( k=1; k<=SIZE_BOARD; k++)
			{
				if (temp_broad.Score[j][k]>scores[i])
				{
					scores[i] = temp_broad.Score[j][k];
					good_place_x[i] = j;
					good_place_y[i] = k;
				}											
			}//for k						
		}//for j
		temp_broad.Score[good_place_x[i]][good_place_y[i]] = 0;//将这个最高点置为0
	}//for

	//在分数最高的位置依次落子，进行更深一层的搜索
	worth = INT_MIN;
	int temp_x=good_place_x[SEEK_WIDTH-1],temp_y=good_place_y[SEEK_WIDTH-1];
	for ( i=SEEK_WIDTH-1; i>=0 && scores[i]>1; i--)
	{
		temp_broad.MakeMove(good_place_x[i],good_place_y[i],who);//落子
		evaluation.Change_State(good_place_x[i],good_place_y[i],temp_broad);//改变落子后的棋局状态
		//判断棋局是否结束
		switch (temp_broad.Is_Over(good_place_x[i],good_place_y[i]))
		{
		case COMPUTER_WIN:
			return INT_MAX;
			
		case PLARER_WIN:
			return INT_MIN;
			
		case OVER_N_DRAW:
			return 0;
		}

		scores[i] = -1*DFSearch(temp_broad,deepth+1,beta*(-1),alpha*(-1));//递归搜索
		
		temp_broad.UnMakeMove(good_place_x[i],good_place_y[i]);//取消落子
		evaluation.Change_State(good_place_x[i],good_place_y[i],temp_broad);

		if (scores[i]>worth)
		{
			temp_x = good_place_x[i];
			temp_y = good_place_y[i];
			worth = scores[i];
			if (scores[i]>alpha)
			{
				alpha = scores[i];
			}
			if (scores[i]>beta)
			{
				break;
			}
		}//if
	}//for i

	x = temp_x;
	y = temp_y;
	return worth;
}//DFSearch


//五子棋游戏类

//调用棋局对象、搜索引擎对象和估值引擎对象

//作用：在估值函数和搜索引擎的支持下与人类玩家进行博弈

class GoBang_Game
{
public:
	GoBang_Game();
	~GoBang_Game();

	void Turn_To_Computer(int &x,int &y);

	void Get_Good_Place(int &x,int &y);
		
	void Record(int x,int y,int winer);

	void Clear();

	Broad broad;
	Evaluation evaluation;

private:
	int num_chessman;
	FILE * fp;
};

GoBang_Game::GoBang_Game()
{
	int x = SIZE_BOARD/2+1,y = SIZE_BOARD/2+1;
	broad.Table[x][y] = COMPUTER;	
	evaluation.Change_State(x,y,broad);

	num_chessman = 1;

	if ((fp=fopen("record.txt","a"))==NULL)
	{
		fp = fopen("record.txt","w+");	
	}

	fprintf(fp,"\n\n********************开局*******************\n");
	fprintf(fp,"第%d步棋:	(%d,%d)\n",num_chessman,x,y);
	
	fclose(fp);
}//GoBang_Game

GoBang_Game::~GoBang_Game()
{

}//GoBang_Game

void GoBang_Game::Turn_To_Computer(int &x,int &y)
{			
	
	Get_Good_Place(x,y);		//电脑通过认识局势、评分操作得出一个目前看来最合适的位置

	//电脑落子
	broad.Table[x][y] = COMPUTER;	

	//更改棋局状态
	evaluation.Change_State(x,y,broad);

}//Turn_To_Computer

void GoBang_Game::Get_Good_Place(int &x,int &y)
//根据实参的值，修改棋盘状态，然后对棋盘进行评分，最后得到一个最高分的位置，用引用带回
{	
	//更改棋局分数
	evaluation.Get_Value(BASIC,broad,COMPUTER);
	evaluation.Get_Value(ADVANCED,broad,COMPUTER);
	
	int i,j;
	int max=INT_MIN;
	int temp_x,temp_y;

	for ( i=1; i<=SIZE_BOARD; i++)
	{
		for ( j=1; j<=SIZE_BOARD; j++)
		{
			if (broad.Score[i][j]>max)
			{
				max = broad.Score[i][j];
				x = i;
				y = j;
			}
		}//for
	}//for

	if (max<INT_MAX-10 && num_chessman>=3)//如果没有绝杀，就对该棋盘进行搜索，寻找最为合适的落子点
	{		
		Search_Engine search_engine;
		search_engine.DFSearch(broad,0,INT_MIN,INT_MAX);	
		x = search_engine.x;
		y = search_engine.y;
		FILE * fp;
		fp = fopen("record.txt","a");
		fprintf(fp,"涉及节点个数：%d\n",search_engine.num_searched_point);
		fclose(fp);
	}//if	
}//Get_Good_Place

void GoBang_Game::Record(int x,int y,int winer=0)
{
	int i,j;
	if (winer==0)
	{
		num_chessman++;
		fp = fopen("record.txt","a");
		fprintf(fp,"第%d步棋:	(%d,%d)\n",num_chessman,x,y);
/*
		for ( i=1; i<SIZE_BOARD; i++)
		{
			for ( j=1; j<SIZE_BOARD; j++)
			{
				if (broad.Score[i][j]<INT_MAX-10)
				{
					fprintf(fp,"%3d ",broad.Score[i][j]);
				}
				else
				{
					fprintf(fp," ** ");
				}
			}
			fprintf(fp,"\n");
		}
*/		fclose(fp);
	}
	else
	{
		fp = fopen("record.txt","a");
		if (winer==COMPUTER_WIN)
		{
			fprintf(fp,"*——电脑获胜——*\n");
		}
		else
		{
			fprintf(fp,"*——人类获胜——*\n");
		}
		fclose(fp);
	}
}//Record

void GoBang_Game::Clear()
{
	int i,j,k;
	for ( i=1; i<=SIZE_BOARD; i++)		//将私有数组成员中实际存储区初始化为0
	{
		for ( j=1; j<=SIZE_BOARD; j++)
		{
			broad.Table[i][j] = 0;
			broad.Score[i][j] = 0;
		}//for j
	}//for i
	
	for ( i=1; i<=SIZE_BOARD; i++)
	{
		for ( j=1; j<=SIZE_BOARD; j++)
		{
			broad.Link_Table[i][j][0] = SIZE_BOARD;
			broad.Link_Table[i][j][1] = SIZE_BOARD;
			broad.Link_Table[i][j][2] = SIZE_BOARD-abs(i-j);
			broad.Link_Table[i][j][3] = SIZE_BOARD-abs(SIZE_BOARD-(i+j)+1);
		}
	}//for
	
	for ( i=0,j=0; i<=SIZE_BOARD+1; i++)//将私有数组成员中溢出区初始化为-1
	{
		broad.Table[i][j] = BOUNDARY;
		broad.Score[i][j] = BOUNDARY;
		broad.Table[j][i] = BOUNDARY;
		broad.Score[j][i] = BOUNDARY;
		broad.Table[SIZE_BOARD+1][i] = BOUNDARY;
		broad.Score[SIZE_BOARD+1][i] = BOUNDARY;
		broad.Table[i][SIZE_BOARD+1] = BOUNDARY;
		broad.Score[i][SIZE_BOARD+1] = BOUNDARY;
		for ( k=0; k<4; k++)
		{
			broad.Link_Table[i][j][k] = BOUNDARY;
			broad.Link_Table[j][i][k] = BOUNDARY;
			broad.Link_Table[SIZE_BOARD+1][i][k] = BOUNDARY;
			broad.Link_Table[i][SIZE_BOARD+1][k] = BOUNDARY;
		}
	}//for	

	int x = SIZE_BOARD/2+1,y = SIZE_BOARD/2+1;
	broad.Table[x][y] = COMPUTER;	
	evaluation.Change_State(x,y,broad);
	
	num_chessman = 1;
	
	if ((fp=fopen("record.txt","a"))==NULL)
	{
		fp = fopen("record.txt","w+");	
	}
	
	fprintf(fp,"\n\n********************开局*******************\n");
	fprintf(fp,"第%d步棋:	(%d,%d)\n",num_chessman,x,y);
	
	fclose(fp);
}//Clear