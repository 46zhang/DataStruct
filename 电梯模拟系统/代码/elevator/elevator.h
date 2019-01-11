#ifndef ELEVATOR_H_INCLUDED
#define ELEVATOR_H_INCLUDED

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
using namespace std;
#include <iomanip>
#include <time.h>

//���еĺ궨��
#define TRUE		1
#define	FALSE		0
#define OK			1
#define ERROR		0
#define INFEASIBLE	-1
#define OVERFLOW	-2
#define	STACK_INIT_SIZE	8	//�洢�ռ��ʼ������
#define	STACKINCREMENT	5	//�洢�ռ��������
#define Run 1;
//#define Debug 1
#ifdef Run
#define	CloseTestTime 40	//���ݹ��Ų���ʱ��,���˽���ʱ������ÿ��40t����һ�Σ������˽����������
#define OverTime  300		//����ͣ��ʱʱ��,���������ĳ�㾲ֹʱ�䳬��300t����ʻ��1�����
#define AccelerateTime 15	//����ʱ��
#define UpTime	51			//����ʱ��
#define DownTime 61			//�½�ʱ��
#define UpSlowTime 14		//��������
#define DownSlowTime 23		//�½�����
#define DoorTime	20		//���Ź���ʱ��,���źͿ��Ÿ���Ҫ20t
#define InOutTime	25		//��������ʱ��,ÿ���˽������ݾ���Ҫ25t
#define MaxGiveUpTime 300		//�˿������ʱ��
#define MaxInterTime 100	//��һ���˿���ĳ���ʱ��
#endif

#define MaxWaitNumInFloor 7	//ÿһ������ȴ�����
#define MaxFloor	4		//��߲�
#define MinFloor	0		//��Ͳ�
#define MaxElevator   10
#define Empty	0
#define FloorHight 3		//¥��߶�		
#define LeftElevatorX 6		//��ߵ��ݵ�x��ʼ����
#define LeftElevatorY 12	//��ߵ��ݵ�y��ʼ����
#define RightElevatorX 34	//�ұߵ��ݵ�x��ʼ����
#define RightElevatorY 12	//�ұߵ��ݵ�y��ʼ����

#ifdef Debug
//����ʱ����ʱ���һ��
#define	CloseTestTime 3		//���ݹ��Ų���ʱ��,���˽���ʱ������ÿ��40t����һ�Σ������˽����������
#define OverTime  300		//����ͣ��ʱʱ��,���������ĳ�㾲ֹʱ�䳬��300t����ʻ��1�����
#define AccelerateTime 1	//����ʱ��
#define UpTime	1			//����ʱ��
#define DownTime 1			//�½�ʱ��
#define UpSlowTime 1		//��������
#define DownSlowTime 1		//�½�����
#define DoorTime	1		//���Ź���ʱ��,���źͿ��Ÿ���Ҫ20t
#define InOutTime	1		//��������ʱ��,ÿ���˽������ݾ���Ҫ25t
#define MaxInterTime 8		//��һ���˿���ĳ���ʱ��
#define MaxGiveUpTime 4		//�˿������ʱ��
#endif // DEBUG
//------------------------------------------------------

//״̬�Ķ���
enum	ElevatorMove { Opening, Opened, Closing, Closed, Moving, SlowDown, Waiting, Accelerate };  //���ݵ�8�ֶ���
enum	ElevatorState{ GoingUp, GoingDown,Idle };												  //���ݵ�״̬
//---------------------------------------------------------------------------------------------------------------



//�˿���صĽṹ�嶨��
typedef struct ClientNode {
	int ClientID;			//�˿͵�ID
	int InFloor;			//�����¥��
	int OutFloor;			//Ŀ��¥��
	int GiveupTime;			//����ʱ��
}*Client;

typedef int Status;					
typedef Client ElemType;			

//�˿Ͷ���
typedef struct ClientQueueNode {
	Client data;
	ClientQueueNode * next;
}* ClientQueuePtr;

//�˿͵ĵȴ�����
typedef struct WaitQueueNode {
	ClientQueuePtr front;
	ClientQueuePtr rear;
	int WaitClientNum;
}WaitQueue;

//�˿�ջ
typedef struct ClientStackNode {
	ElemType *base;				//ջ��ָ��
	ElemType *top;				//ջ��ָ��
	int			stacksize;	    //��ǰ�ѷ���洢�ռ䣬��Ԫ��Ϊ��λ
}ClientStack;
//------------------------------------------------------

//������صĽṹ�嶨��
typedef struct ElevatorNode {
	int ClientNum;			//�˿�����
	ElevatorState State;	//���ݵ�״̬
	int StateCount;			//��ǰ״̬�ļ�ʱ
	int floor;				//��ǰ���ڵ�¥��
	ElevatorMove move;		//��ǰ���ݵĶ���
	int CallCar[5];			//Ŀ��¥������
	ClientStack Stack[5];	//�˿�ջ
	int ClientID[8];		//�����������洢�����ڵĳ˿ͱ��
}*Elevator;
//------------------------------------------------------


void PrintElevatorFrame(void);
void ShowTitle();
void gotoxy(short x, short y);
void ShowWaitingClient(WaitQueue Q, int flag, int x);
void DisappearWaitingClient(WaitQueue Q, int flag, int num);
void ShowElevatorBody(Elevator e, int k, int x);
void DisappearEvelatorBody(Elevator e, int k, int x);
void PrintfElevatorMove(Elevator e, int k);
void PrintfHelp();

Status InitQueue(WaitQueue &Q);
Status DestroyQueue(WaitQueue &Q);
Status EnterQueue(WaitQueue &Q, ElemType e);
Status DeleteQueue(WaitQueue &Q, ElemType &e);
Status IsQueueEmpty(WaitQueue Q);
Status DeleteQueueNode(WaitQueue &Q, ClientQueuePtr p);
void PrintQueue(WaitQueue Q);

Status InitStack(ClientStack &S);
Status DestroyStack(ClientStack &S);
Status StackEmpty(ClientStack S);
Status Push(ClientStack &S, ElemType e);
Status Pop(ClientStack &S, ElemType &e);

Status TimeUp(Elevator e);
void ElevatorChangeMove(Elevator e, int k);
void Controler(Elevator E[]);
int HigherRequires(Elevator e);
int LowerRequires(Elevator e);

void NewClient();
Status ClientGiveUp(WaitQueue &Q, Elevator E[], int floor);
Status ClientInOrOut(Elevator e, int k);

#endif