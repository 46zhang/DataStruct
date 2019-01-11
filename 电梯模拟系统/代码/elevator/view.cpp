#include <stdio.h>
#include <windows.h>
#include <iostream>
using namespace std;
#include <iomanip>
#include"elevator.h"

extern int MaxClientNum;
extern int  MaxRunTime;
void ShowTitle()
{
	printf("\n\n\n");
	printf("\n\t\t\t *********************************************************");
	printf("\n\t\t\t _________________________________________________________");
	printf("\n\t\t\t ��������������������ӭ�������ݹ���ϵͳ ������������������\n");
	printf("\t\t\t ---------------------------------------------------------");
	printf("\n\t\t\t _________________________________________________________");
	printf("\n\t\t\t ����������������author: ����İ� ����骡�����������������\n");
	printf("\t\t\t ---------------------------------------------------------");
	printf("\n");
	printf("\n\t\t\t *********************************************************\n");
	
}
//����ȷ����Ϣ�����λ��
void gotoxy(short x, short y)
{
	COORD coord = { x, y };
	/*COORD��Windows API�ж����һ�ֽṹ����ʾһ���ַ��ڿ���̨��Ļ�ϵ����ꡣ�䶨��Ϊ��

	typedef struct _COORD {

	SHORT X; // horizontal coordinate

	SHORT Y; // vertical coordinate
	} COORD;*/

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
//������ݵ���������
void PrintElevatorFrame(void) {
	int j, i, k;
	for (i = 0; i <= 1; i++)
	{
		for (j = MaxFloor+1; j >= 0; j--)
		{
			if (j == MaxFloor)
			{
				gotoxy(6+i*27,9);
				printf("--------------------------");
			}
			gotoxy(6+i*27,9+j* FloorHight);
			printf("--------------------------");
			
		}
		j= MaxFloor + 1;
		for (k = 9; k <= 9 + j * FloorHight; k ++)
		{
			gotoxy(5+i*27,k);
			printf("|");
			gotoxy(5+i*27+26,k);
			printf("|");
		}
	}

}
//������x��ĵ���
void DisappearEvelatorBody(Elevator e,int k,int x)
{
	int i, j;
	//����������Ϊ������ɫ(��ɫ)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
	for (i = LeftElevatorX + k * 27; i <= LeftElevatorX + k * 26 + 24; i++)
	{
		gotoxy(i, LeftElevatorY - 1 + (4 - x)*FloorHight);
		printf(" ");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
//��x����ʾ����
void ShowElevatorBody(Elevator e, int k,int x)
{
	int i, j;
	Client *p;
	gotoxy(LeftElevatorX+k*27,LeftElevatorY-1+(4-x)*FloorHight);
	printf("[");
	for (i = 0; i < e->ClientNum; i++)
	{
		if(e->ClientID[i]!=0)
			printf(" %d", e->ClientID[i]);
	}
	gotoxy(LeftElevatorX+k*26+24,LeftElevatorY - 1 + (4 - x)*FloorHight);
	printf("]");
}
//��ʾ�ȴ����еĳ˿�,flag Ϊ0 ��ʾ�ö��еĳ˿�Ҫ���£�flagΪ1��ʾ����
//num��ʾͬ������һ�����е���������ʾ��������֮�󣬲�Ȼ�Ḳ����һ�����е�����
void ShowWaitingClient(WaitQueue Q, int flag,int num)
{
	ClientQueueNode * p;
	p = Q.front->next;
	int j=num;
	while (p != NULL)
	{
		gotoxy(RightElevatorX + 26+j*6, RightElevatorY-1 + (4-p->data->InFloor)*FloorHight);
		printf(" %d", p->data->ClientID);
		if (flag)
			printf("��");
		else
			printf("��");
		printf("%d",p->data->OutFloor);
		p = p->next;
		j++;
	}
}
//�������ڵȴ�״̬�ĳ˿�
//�����³˿ͼ�������г˿�����ݣ��˿��뿪ʱ����Ҫ����
void DisappearWaitingClient(WaitQueue Q, int flag,int num)
{
	//�����������ɫ���ñ�����ɫ��дһ�������ȥ���� 0�Ǻ�ɫ 7�ǰ�ɫ
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
	ShowWaitingClient(Q, flag,num);
	//������������»ָ�������ɫ
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

//��ʾ���ݵĶ���
void PrintfElevatorMove(Elevator e,int k)
{
	//Opening, Opened, Closing, Closed, Moving, SlowDown, Waiting, Accelerate
	gotoxy(10 + 30 * k, 4);
	printf("                         ");
	gotoxy(10+30*k,4);
	switch (e->move)
	{
	case Opening:printf(" %d���� �����ڴ�", k); break;
	case Opened:printf(" %d���� ���Ѿ���", k); break;
	case Closing:printf(" %d���� ���ڹ���", k); break;
	case Closed:printf(" %d���� ���Ѿ�����", k); break;
	case Moving:printf(" %d���� �����ƶ�", k); break;
	case SlowDown:printf(" %d���� ���ڼ���", k); break;
	case Waiting:printf(" %d���� ���ڵȴ�", k); break;
	case Accelerate:printf(" %d���� ���ڼ���", k); break;
	}
}
//��ʾ˵����Ϣ
void PrintfHelp()
{
	gotoxy(60, 7);
	printf("˵����ǰ������ֱ�ʾ�˿�ID����ͷ��ʾ�ó˿����ϻ���");
	gotoxy(60, 9);
	printf("��������ִ���ó˿͵�Ŀ��¥��");
}