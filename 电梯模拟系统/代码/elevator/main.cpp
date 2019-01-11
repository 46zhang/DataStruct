#include<stdio.h>
#include"elevator.h"
#include <stdlib.h>
#include <conio.h>

void InputMaxTime();
void init(Elevator * E, int i);
void CheckGiveUp(Elevator E[]);

int MaxClientNum = 7;					//���˿�����
int MaxRunTime;							//�������ʱ��
int NextClientInterTime;				//��һ���˿ͽ���ϵͳ��ʱ��
int TotalClient=1;						//��ǰ�ܹ��ж��ٸ��˿�(�����������ظ������)
WaitQueue FloorWaitQueue[2][5];			//ÿ��¥�������ȴ����У�0����,1����
int CallUp[5], CallDown[5], CallCar[5];	//¥����������°�ť,�Լ�Ŀ��¥��,ȡֵΪ1��ʾ����,-1��ʾ�����Ѿ���Ӧ						
void main()
{
	//�ֱ��Ӧÿ��ĵ������ϰ�ť�����°�ť���Լ�Ŀ��¥��
	int  i, Time = 0, t[2];
	Elevator E[2];
	srand((unsigned)time(NULL));
	init(E,2);										//ȫ�ֳ�ʼ������
	t[1] = t[0] = 0;
	ShowTitle();
	InputMaxTime();		
	system("cls");
	PrintElevatorFrame();							//������ݿ��
	PrintfHelp();
	ShowElevatorBody(E[0],0,1);
	ShowElevatorBody(E[1], 1,1);						//�����������
	while (Time++ < MaxRunTime)
	{
		//�����µĳ˿�
		if (NextClientInterTime == 0)
			NewClient();
		else
			NextClientInterTime--;
		//����Ƿ�˿ͷ������
		CheckGiveUp(E);
		//���õ��ݵ��Ⱥ���
			Controler(E);
		for (i = 0; i < 2; i++)
		{
			//������ڿ���״̬��ÿ��һ����ʱ��������������˿�
			if (E[i]->move == Opened)
			{
				//ÿ��25s��һ����
				if (t[i] == InOutTime)
				{
					t[i] = 0;
					//�ȴ��ĳ˿ͽ�����ݣ�Ӧ��������������ʾ�ڵȴ��ĳ˿�
					DisappearWaitingClient(FloorWaitQueue[1][E[i]->floor], 0, 0);
					DisappearWaitingClient(FloorWaitQueue[0][E[i]->floor], 1, FloorWaitQueue[1][E[i]->floor].WaitClientNum);
					//�˿ͽ���,���ݵ���ʾ��������ı䣬��ʾ����
					if (TRUE == ClientInOrOut(E[i], i))
					{
						//����֮ǰ�ĵ��ݣ���ʾ�˿ͽ����ĵ���
						DisappearEvelatorBody(E[i],i,E[i]->floor);
						ShowElevatorBody(E[i],i,E[i]->floor);
					}//����������ʾ
					ShowWaitingClient(FloorWaitQueue[1][E[i]->floor], 0, 0);
					ShowWaitingClient(FloorWaitQueue[0][E[i]->floor], 1, FloorWaitQueue[1][E[i]->floor].WaitClientNum);
				}
				else
					t[i]++;
			}
			//�����ǰ״̬��ʱ�����ڵ��ˣ���Ҫ�ı�״̬
			if (TimeUp(E[i]) == TRUE)
			{
				ElevatorChangeMove(E[i], i);
				PrintfElevatorMove(E[i],i);
			}
			else 
				E[i]->StateCount--;			//��ʱ
		}
		if (Time % 10 == 0)
		{
			gotoxy(20,2);
			printf("ʱ��Ϊ�� %d", Time);
		}
		if(Time%100==0)
			PrintElevatorFrame();							//������ݿ��
		Sleep(80);
	}
	gotoxy(60,30);
	printf("��ʾ����������������˳�\n");
	getch();
}
void init(Elevator * E,int num)
{
	int i;
	//��ʼ���ȴ����в���
	for (i = 0; i < 5; i++)
	{
		InitQueue(FloorWaitQueue[0][i]);
		InitQueue(FloorWaitQueue[1][i]);
	}
	//��ʼ����̨����
	for (i = 0; i < num; i++)
	{
		E[i] = (Elevator)malloc(sizeof(ElevatorNode));
		E[i]->ClientNum = 0;
		//��ʼ״̬��1¥ͣ��
		E[i]->floor = 1;
		E[i]->State = Idle;
		E[i]->StateCount = 0;
		E[i]->move = Waiting;
		for (int j = 0; j < 5; j++)
		{
			//Ŀ��¥���ʼ��Ϊ0
			E[i]->CallCar[j] = 0;
			//��ʼ���õ��ݵĳ˿�ջ
			InitStack(E[i]->Stack[j]);
		}
		//��ʼ���˿�ID����
		for (int j = 0; j <= 8; j++)
			E[i]->ClientID[j] = 0;
	}
	//��ʼ���������ݰ�ť����
	for (i = 0; i < 5; i++)
	{
		CallUp[i] = 0;
		CallDown[i] = 0;
	}
}
void CheckGiveUp(Elevator E[])
{
	int i;
	//����Ƿ�˿ͷ������
		for (i = 0; i < 5; i++)
		{
			
			ClientGiveUp(FloorWaitQueue[0][i], E, i);
				
			ClientGiveUp(FloorWaitQueue[1][i], E, i);
				
		}
}
void InputMaxTime()
{
	printf("\n\t\t\t��������ݵ��������ʱ�䣬�ﵽ��ʱ�䣬����ϵͳ�����Զ�ֹͣ\n");
	printf("\t\t\t������:");
	while (scanf("%d", &MaxRunTime) != 1 || MaxRunTime <= 500 || MaxRunTime > 10000)
	{
		printf("\t\t\t���������������루500=<x<10000��\n");
		while (getchar() != '\n')
			continue;
		printf("\t\t\t������:");
	}
}