//main.cpp�ṩ���·���
#pragma once
#include <limits.h>
#include <math.h>
#include <iostream>

#include "define.h"

//�����
class Broad
{
public:
	Broad();	
	//���캯����������˽�������Ա����ֵȫ����ʼ��Ϊ0
	
	int Is_Over(int x,int y);
	//�ж������������������Σ�������ĳһ��ʤ�����˳�����

	void MakeMove(int x,int y,int who);
	//���Ӻ���
	
	void UnMakeMove(int x,int y);
	//�������Ӻ���

	//���������̴洢�ṹ
	//Ϊ�˱��ڱ��룬���洢�ṹ����һȦ���������һȦ�ϵ�ֵ����-1
	
	int Table[SIZE_BOARD+2][SIZE_BOARD+2];
	//���̵Ĵ洢�ṹ��0����û�����ӣ�1������ҵ����ӣ�2������Ե�����
	
	int Link_Table[SIZE_BOARD+2][SIZE_BOARD+2][4];//����������״̬�Ĵ洢�ṹ����Table[x][y]==0ʱLink_Table[x][y]û�����壬
	//��Table[x][y]!=0ʱ��Link_Table[x][y][0 ~ 3]�ֱ����(x,y)���Ϻᡢ�������Խ��ߡ����Խ����ĸ����������ӵ������Ŀ
	
	int Score[SIZE_BOARD+2][SIZE_BOARD+2];
	//�����ϸõ�ķ���
};

Broad::Broad()
//���캯��
{
	int i,j,k;
	for ( i=1; i<=SIZE_BOARD; i++)		//��˽�������Ա��ʵ�ʴ洢����ʼ��Ϊ0
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
	
	for ( i=0,j=0; i<=SIZE_BOARD+1; i++)//��˽�������Ա���������ʼ��Ϊ-1
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

int Broad::Is_Over(int x,int y)//�жϵ�ǰ����Ƿ��Ѿ��ֳ�ʤ��
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
int Broad::Is_Over()//�жϵ�ǰ����Ƿ��Ѿ��ֳ�ʤ��
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

//��ֵ������

//������ֶ���������ֹ�ֵ

//ע�⣺���Basic_Evaluate��Advanced_Evaluateͬʱʹ�ã�Ӧ����Basic_Evaluate��ǰ

class Evaluation
{
public:
	int Get_Value(int type,Broad &broad,int who);
	//����������п�λ�ϵķ����͵�ǰ���巽�����ظ���ֵķ���
	
	void Change_State(int x,int y,Broad &broad);
	//�޸���(x,y)�����Ӻ����̵�״̬
	
private:
	void Basic_Evaluate(Broad &broad);
	//������ϵ����п�λ���м򵥵Ĵ��
	
	void Advanced_Evaluate(Broad &broad);
	//������ϵ����п�λ�������޾�ɱ���жϣ��еĻ������øõ���������Ǻܸߵķ��������Ա��ֵ��ȷ

	int Direct(int mode,int x,int y,int dir,int len,Broad &broad);
	//����(��modeָ��)������(x,y)��dir�����Ͼ���Ϊlen��λ�õ���ֵ

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

void Evaluation::Change_State(int x,int y,Broad &broad)//�޸���(x,y)�����Ӻ����̵�״̬
{	
	int i,j,start,end;
	int temp_x,temp_y;
	temp_x = x;
	temp_y = y;
	
	//�᷽��
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
	
	//������
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
	
	//���Խ��߷���
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
	
	//���Խ��߷���
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
			if (broad.Table[x][y]!=0)//�����ǰλ���Ѿ��������ˣ����λ�÷���Ϊ�㣬��continue
			{
				broad.Score[x][y] = 0;
				continue;
			}

			//(x,y)Ϊ�գ����(x,y)�������֡�
			//���������ı�׼�ǣ�////////////////////////////////////////////////////////////////////////////////////////////
			//�ĸ�����ķ�����ӣ��õ��õ����

			//�Ƚ��õ������Ϊ1���Ա������ǰ��Ӱ��
			broad.Score[x][y] = 1;

			for ( i=0; i<4; i++)
			{
				//һ��һ��			
				if ((Direct(1,x,y,i,1,broad)==0 && Direct(1,x,y,i+4,1,broad)>0) ||
					(Direct(1,x,y,i,1,broad)>0 && Direct(1,x,y,i+4,1,broad)==0))
				{
				
					if ((Direct(2,x,y,i,1,broad)+Direct(2,x,y,i+4,1,broad)) <= 3)//�ռ���С�ڵ���3
					{
						if (Direct(1,x,y,i,1,broad)==0)
						{
							if (Direct(1,x,y,i+4,Direct(2,x,y,i+4,1,broad)+1,broad)==0)//�ӵ���һ��ҲΪ��
							{
								temp = Direct(2,x,y,i+4,1,broad) + Direct(2,x,y,i,1,broad) + Direct(2,x,y,i+4,Direct(2,x,y,i+4,1,broad)+1,broad);
								//temp��ʱ������+��+�ӵ���һ�˵Ŀ�
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
							else//�ӵ���һ�˱���
							{
								temp = 0;
								goto loop;
							}//else
						}//if
						else
						{
							if (Direct(1,x,y,i,Direct(2,x,y,i,1,broad)+1,broad)==0)//�ӵ���һ��ҲΪ��
							{
								temp = Direct(2,x,y,i,1,broad) + Direct(2,x,y,i+4,1,broad) + Direct(2,x,y,i,Direct(2,x,y,i,1,broad)+1,broad);
								//temp��ʱ������+��+�ӵ���һ�˵Ŀ�
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
							else//�ӵ���һ�˱���
							{
								temp = 0;
								goto loop;
							}//else
						}//else
					}//if
					else//��ʱ��+�Ӵ��ڵ���4
					{
						temp = Direct(1,x,y,i,1,broad)==0 ? Direct(2,x,y,i+4,1,broad) : Direct(2,x,y,i,1,broad);
						temp *= 10;
						temp += 10;
						goto loop;
					}
				}//if

				//�����в�ͬ��
				if ( (Direct(1,x,y,i,1,broad)!=Direct(1,x,y,i+4,1,broad)) &&
					Direct(1,x,y,i,1,broad)*Direct(1,x,y,i+4,1,broad)>0 ) 
				{
					if (Direct(2,x,y,i,1,broad)<=4 && Direct(2,x,y,i+4,1,broad)<=4)//���ε�������������4����Ϊ����
					{
						temp = 0;
						goto loop;
					}
					else//������Ǿ�ɱ
					{
						temp = 1000;
						goto loop;
					}
				}//if

				//���Ҷ�Ϊ�գ�ֱ����Ϊ0
				if (Direct(1,x,y,i,1,broad)==0 && Direct(1,x,y,i+4,1,broad)==0)
				{
					temp = 0;
					goto loop;
				}//if	

				//�����߽�����
				if (Direct(1,x,y,i,1,broad)==-1)
				{
					if (Direct(1,x,y,i+4,1,broad)<=0)//��һ���Ǳ߽��Ϊ�գ���Ϊ���壬��Ϊ0
					{
						temp = 0;
						goto loop;
					}
					else
					{
						if (Direct(1,x,y,i+4,Direct(2,x,y,i+4,1,broad)+1,broad)==0 && //�ӵ���һ��Ϊ�գ��ҿ��Գ�����
							Direct(2,x,y,i+4,1,broad)+Direct(2,x,y,i+4,Direct(2,x,y,i+4,1,broad)+1,broad)>=4)
						{
							temp = Direct(2,x,y,i+4,1,broad);
							temp *= 10;
							goto loop;
						}
						else
						{
							temp = 1;//�ӵ���һ�˱��£��������ᣬ����μӾ�ɱ�ж�
							goto loop;
						}
					}
				}//if
				if (Direct(1,x,y,i+4,1,broad)==-1)
				{
					if (Direct(1,x,y,i,1,broad)<=0)//��һ���Ǳ߽��Ϊ�գ���Ϊ���壬��Ϊ0
					{
						temp = 0;
						goto loop;
					}
					else
					{
						if (Direct(1,x,y,i,Direct(2,x,y,i,1,broad)+1,broad)==0 && //�ӵ���һ��Ϊ�գ��ҿ��Գ�����
							Direct(2,x,y,i,1,broad)+Direct(2,x,y,i,Direct(2,x,y,i,1,broad)+1,broad)>=4)
						{
							temp = Direct(2,x,y,i,1,broad);
							temp *= 10;
							goto loop;
						}
						else
						{
							temp = 1;//�ӵ���һ�˱��£��������ᣬ����μӾ�ɱ�ж�
							goto loop;
						}
					}
				}//if
				
				//����ͬ�ӣ�
				if (Direct(1,x,y,i,1,broad)==Direct(1,x,y,i+4,1,broad) && Direct(1,x,y,i,1,broad)>0)
				{
					if (Direct(1,x,y,i,Direct(2,x,y,i,1,broad)+1,broad)!=0 && 
						Direct(1,x,y,i+4,Direct(2,x,y,i+4,1,broad)+1,broad)!=0 &&
						Direct(2,x,y,i,1,broad)+Direct(2,x,y,i+4,1,broad)<=3)
					//������ӱ��£�����Ҳ���£��Ҳ��ɳ���������Ϊ0
					{
						temp = 0;
					}
					else//����ͬ�ӣ�����û����ȫ����
					{
						temp = Direct(2,x,y,i,1,broad)+Direct(2,x,y,i+4,1,broad);//temp����������֮�͵�ʮ��
						temp *= 10;

						if (Direct(1,x,y,i,Direct(2,x,y,i,1,broad)+1,broad)==0)//�˶�δ���£�temp��10
						{
							temp+=10;
						}
						if (Direct(1,x,y,i+4,Direct(2,x,y,i+4,1,broad)+1,broad)==0)//�˶�δ���£�temp��10
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
			if (broad.Table[x][y]!=0)//�����ǰλ���Ѿ��������ˣ����λ�÷���Ϊ�㣬��continue
			{
				broad.Score[x][y] = 0;
				continue;
			}

			if (broad.Score[x][y]==1)//����˴�����Ϊ1����˵���������Ǿ�ɱλ���ʶ�ֱ��pass����
			{
				continue;
			}
			//(x,y)Ϊ�գ����(x,y)�������֡�

			//�ȿ����޻��ġ����ġ��Գ塢������������������������������

			//������������ζ���֮Ϊ����ɱ�㡱�����Ƿ�Ϊһ��������������ɱ��
			//һ����ɱ��ȼ����

			//���ж�ĳ��Ϊ��ɱ���Ӧ���������к������ھ�ɱ���ж�������ֻ���ڷ������ڵ�ǰ����ʱ�ſɸ���
			//ԭ�����Զ��׼��ģ���һ������ĳ���һϵ���ж������У��õ���Ҫ��ֻ�������ܽ���


			//�ж����޳��ĺͻ���
			for ( i=0; i<8; i++)
			{
				if (Direct(1,x,y,i,1,broad)>0 && Direct(2,x,y,i,1,broad)==4 )
				{									
					if (Direct(1,x,y,i,1,broad)==COMPUTER)//�˵�Ϊ����һ����ɱ�����ؼ�����ֱ�ӷ���
					{
						broad.Score[x][y] = INT_MAX;
						return ;
					}
					else							//���ǶԷ���һ����ɱ��Ҳ�ü���̽��������
					{
						broad.Score[x][y] = INT_MAX-1;
					}					
				}
			}

			//�ж��ܷ��γɳ���
			for ( i=0; i<4 && broad.Score[x][y]<INT_MAX; i++)
			{
				if (Direct(1,x,y,i,1,broad)>0 &&
					Direct(1,x,y,i,1,broad)==Direct(1,x,y,i+4,1,broad) &&//������ԣ���ɫ��ͬ
					Direct(2,x,y,i,1,broad)+Direct(2,x,y,i+4,1,broad)>=4 )//�γɶԳ�
				{									
					if (Direct(1,x,y,i,1,broad)==COMPUTER)//�˵�Ϊ����һ����ɱ�����ؼ�����ֱ�ӷ���
					{
						broad.Score[x][y] = INT_MAX;
						return ;
					}
					else							//���ǶԷ���һ����ɱ��Ҳ�ü���̽��������
					{
						broad.Score[x][y] = INT_MAX-1;
					}
				}//if
			}//for
							
			//�ж����޻���
			for ( i=0; i<4 && broad.Score[x][y]<INT_MAX-2; i++)
			{
				if (Direct(1,x,y,i,1,broad)>0 &&
					Direct(1,x,y,i,1,broad)==Direct(1,x,y,i+4,1,broad) &&//������ԣ���ɫ��ͬ
					Direct(2,x,y,i,1,broad)+Direct(2,x,y,i+4,1,broad)>=3 && 
					Direct(1,x,y,i,Direct(2,x,y,i,1,broad)+1,broad)==0 && 
					Direct(1,x,y,i+4,Direct(2,x,y,i+4,1,broad)+1,broad)==0 )
				{
					if (Direct(1,x,y,i,1,broad)==COMPUTER)	//����Ǽ����Ķ�����ɱ�㣬��ֵΪ����һ
					{
						broad.Score[x][y] = INT_MAX-2;
					}
					else								//����ǶԷ��Ķ�����ɱ�㣬��ֵΪ������
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
					if (Direct(1,x,y,i,1,broad)==COMPUTER)	//����Ǽ����Ķ�����ɱ�㣬��ֵΪ����һ
					{
						broad.Score[x][y] = INT_MAX-2;
					}
					else								//����ǶԷ��Ķ�����ɱ�㣬��ֵΪ������
					{
						broad.Score[x][y] = INT_MAX-3;
					}	
				}//if
			}//for

			//�ж����޳�����
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
						if (Direct(1,x,y,i,1,broad)==COMPUTER)	//����Ǽ����Ķ�����ɱ�㣬��ֵΪ����һ
						{
							broad.Score[x][y] = INT_MAX-2;
						}
						else								//����ǶԷ��Ķ�����ɱ�㣬��ֵΪ������
						{
							broad.Score[x][y] = INT_MAX-3;
						}	
					}//if
				}//for
			}//for

			//�ж����޳������
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
						if (Direct(1,x,y,i,1,broad)==COMPUTER)		//����Ǽ����Ķ�����ɱ�㣬��ֵΪ����һ
						{
							broad.Score[x][y] = INT_MAX-2;
						}
						else									//����ǶԷ��Ķ�����ɱ�㣬��ֵΪ������
						{
							broad.Score[x][y] = INT_MAX-3;
						}	
					}
				}
			}

			//�ж����޻����
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
						if (Direct(1,x,y,i,1,broad)==COMPUTER)	//����Ǽ�����������ɱ�㣬��ֵΪ������
						{
							broad.Score[x][y] = INT_MAX-4;
						}
						else								//����ǶԷ���������ɱ�㣬��ֵΪ������
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
	if (mode==1)//����Table��ֵ
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
		
	if (mode==2)//����lnktable��ֵ
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


//����������

//������ֶ���͹�ֵ�������

//���ã�������������Ѱ����������ӵ�

//���漼����AlphaBeta��֦ + ����С����

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
//who��ʾ�ֵ�ĳһ�����ӣ�����x,y�Ա��������λ�ã�������ַ���
{
	num_searched_point++;

	if (deepth==0)//ת������
	{
		temp_broad = broad;
	}

	//�Ե�ǰ������й�ֵ
	const int who = deepth%2==0 ? COMPUTER : PLAYER;
	int worth;
	evaluation.Get_Value(BASIC,temp_broad,who);
	worth = evaluation.Get_Value(ADVANCED,temp_broad,who);

	//Ҷ�ӽڵ㷵�ع�ֵ
	if (deepth==SEEK_DEEPTH-1)
	{
		num_searched_point++;

		return worth;
	}//if

	int i,j,k;
	//�õ������ϸߵ����ɸ�λ�õ�����
	int good_place_x[SEEK_WIDTH],good_place_y[SEEK_WIDTH];
	for ( i=SEEK_WIDTH-1; i>=0; i--)
	{
		for ( j=1,scores[i]=0; j<=SIZE_BOARD; j++)//������������
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
		temp_broad.Score[good_place_x[i]][good_place_y[i]] = 0;//�������ߵ���Ϊ0
	}//for

	//�ڷ�����ߵ�λ���������ӣ����и���һ�������
	worth = INT_MIN;
	int temp_x=good_place_x[SEEK_WIDTH-1],temp_y=good_place_y[SEEK_WIDTH-1];
	for ( i=SEEK_WIDTH-1; i>=0 && scores[i]>1; i--)
	{
		temp_broad.MakeMove(good_place_x[i],good_place_y[i],who);//����
		evaluation.Change_State(good_place_x[i],good_place_y[i],temp_broad);//�ı����Ӻ�����״̬
		//�ж�����Ƿ����
		switch (temp_broad.Is_Over(good_place_x[i],good_place_y[i]))
		{
		case COMPUTER_WIN:
			return INT_MAX;
			
		case PLARER_WIN:
			return INT_MIN;
			
		case OVER_N_DRAW:
			return 0;
		}

		scores[i] = -1*DFSearch(temp_broad,deepth+1,beta*(-1),alpha*(-1));//�ݹ�����
		
		temp_broad.UnMakeMove(good_place_x[i],good_place_y[i]);//ȡ������
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


//��������Ϸ��

//������ֶ��������������͹�ֵ�������

//���ã��ڹ�ֵ���������������֧������������ҽ��в���

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

	fprintf(fp,"\n\n********************����*******************\n");
	fprintf(fp,"��%d����:	(%d,%d)\n",num_chessman,x,y);
	
	fclose(fp);
}//GoBang_Game

GoBang_Game::~GoBang_Game()
{

}//GoBang_Game

void GoBang_Game::Turn_To_Computer(int &x,int &y)
{			
	
	Get_Good_Place(x,y);		//����ͨ����ʶ���ơ����ֲ����ó�һ��Ŀǰ��������ʵ�λ��

	//��������
	broad.Table[x][y] = COMPUTER;	

	//�������״̬
	evaluation.Change_State(x,y,broad);

}//Turn_To_Computer

void GoBang_Game::Get_Good_Place(int &x,int &y)
//����ʵ�ε�ֵ���޸�����״̬��Ȼ������̽������֣����õ�һ����߷ֵ�λ�ã������ô���
{	
	//������ַ���
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

	if (max<INT_MAX-10 && num_chessman>=3)//���û�о�ɱ���ͶԸ����̽���������Ѱ����Ϊ���ʵ����ӵ�
	{		
		Search_Engine search_engine;
		search_engine.DFSearch(broad,0,INT_MIN,INT_MAX);	
		x = search_engine.x;
		y = search_engine.y;
		FILE * fp;
		fp = fopen("record.txt","a");
		fprintf(fp,"�漰�ڵ������%d\n",search_engine.num_searched_point);
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
		fprintf(fp,"��%d����:	(%d,%d)\n",num_chessman,x,y);
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
			fprintf(fp,"*�������Ի�ʤ����*\n");
		}
		else
		{
			fprintf(fp,"*���������ʤ����*\n");
		}
		fclose(fp);
	}
}//Record

void GoBang_Game::Clear()
{
	int i,j,k;
	for ( i=1; i<=SIZE_BOARD; i++)		//��˽�������Ա��ʵ�ʴ洢����ʼ��Ϊ0
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
	
	for ( i=0,j=0; i<=SIZE_BOARD+1; i++)//��˽�������Ա���������ʼ��Ϊ-1
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
	
	fprintf(fp,"\n\n********************����*******************\n");
	fprintf(fp,"��%d����:	(%d,%d)\n",num_chessman,x,y);
	
	fclose(fp);
}//Clear