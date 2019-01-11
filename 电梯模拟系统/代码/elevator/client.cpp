#include"elevator.h"

extern int NextClientInterTime;							//��һ���˿ͽ���ϵͳ��ʱ��
extern int TotalClient;									//�ܹ��ж��ٸ��˿�
extern WaitQueue FloorWaitQueue[2][5];					//ÿ��¥�������ȴ����У�0����,1����
extern int CallUp[5], CallDown[5];			//¥����������°�ť,�Լ�Ŀ��¥��
extern int MaxClientNum;
void NewClient()
{
	Client c;
	c = (Client)malloc(sizeof(ClientNode));
	srand((unsigned)time(NULL));
	//��������¥�㣬Ŀǰ¥�㣬����ʱ��
	c->ClientID = TotalClient++;
	c->GiveupTime = rand()%MaxGiveUpTime;
	//����ʱ����50t-300t֮��
	while(c->GiveupTime<=50)
		c->GiveupTime= rand() % MaxGiveUpTime;
	c->InFloor = rand()*11 % (MaxFloor+1);
	c->OutFloor = rand()*17 % (MaxFloor+1);
	//���Ŀǰ¥��������¥��һ���������¸�ֵ
	while (c->OutFloor == c->InFloor)
		c->OutFloor= rand() % MaxFloor;
	NextClientInterTime = rand() % MaxInterTime;
	//��һ �˿ͳ��ֵ�ʱ����5-120s֮��
	while(NextClientInterTime<5)
		NextClientInterTime = rand() % MaxInterTime;
	//����ȴ�����
	//���ж��������ϻ������£�Ȼ������ԭ���Ķ�����ʾ������˿͵�����
	//��������ʾ�µĳ˿Ͷ���
	if (c->InFloor > c->OutFloor)
	{
		DisappearWaitingClient(FloorWaitQueue[1][c->InFloor], 0, 0);
		DisappearWaitingClient(FloorWaitQueue[0][c->InFloor], 1, FloorWaitQueue[1][c->InFloor].WaitClientNum);
		//����ȴ�����
		EnterQueue(FloorWaitQueue[1][c->InFloor], c);
		//���¸ò�ĵ��ݰ�ť
		if(CallDown[c->InFloor] !=-1)
			CallDown[c->InFloor] = 1;
		ShowWaitingClient(FloorWaitQueue[1][c->InFloor], 0,0);
		ShowWaitingClient(FloorWaitQueue[0][c->InFloor], 1,FloorWaitQueue[1][c->InFloor].WaitClientNum);
	}
	else
	{
		DisappearWaitingClient(FloorWaitQueue[1][c->InFloor], 0, 0);
		DisappearWaitingClient(FloorWaitQueue[0][c->InFloor], 1, FloorWaitQueue[1][c->InFloor].WaitClientNum);
		EnterQueue(FloorWaitQueue[0][c->InFloor], c);
		//���¸ò�ĵ��ݰ�ť
		if (CallUp[c->InFloor] != -1)
			CallUp[c->InFloor] = 1;
		//��ʾ���ϸ����µĳ˿�
		ShowWaitingClient(FloorWaitQueue[1][c->InFloor], 0,0);
		ShowWaitingClient(FloorWaitQueue[0][c->InFloor], 1,FloorWaitQueue[1][c->InFloor].WaitClientNum);
	}
}
Status ClientOut(Elevator &e, int k)
{
	Client p;
	int i, j;
	//˵���г˿͵�Ŀ��¥��Ϊ���,���Ҹò�˿�ջ��Ϊ��,�ó˿ͳ�ջ,ÿ��ȥһ���˿���Ҫ�ȴ�25��t
	if (StackEmpty(e->Stack[e->floor]) != TRUE)
	{
		Pop(e->Stack[e->floor], p);
		for (i = 0; i < MaxClientNum; i++)
		{
			if (e->ClientID[i] == p->ClientID)
				break;
		}
		e->ClientNum--;
		//���ǵ��˿�ID���������
		for (j = i; j < 7; j++)
			e->ClientID[j] = e->ClientID[j + 1];
		gotoxy(10 + 30 * k, 6);
		printf("                         ");
		gotoxy(10 + 30 * k, 6);
		printf(" %d�˿ͳ����� %d", p->ClientID, k);
		free(p);
		return TRUE;
	}
	else
		return FALSE;
}
Status ClientIn(Elevator &e, int k)
{
	Client p;
	if (e->ClientNum >= 8)
		return FALSE;
	switch (e->State)
	{
		//����õ������£�ֻ�������µĳ˿�
	case GoingDown:
		if (FloorWaitQueue[1][e->floor].WaitClientNum != 0)
		{	//���ӳɹ�
			if (OK == DeleteQueue(FloorWaitQueue[1][e->floor], p))
			{
				if (OK == Push(e->Stack[p->OutFloor], p))
				{
					e->CallCar[p->OutFloor] = 1;
					e->ClientID[e->ClientNum++] = p->ClientID;
					gotoxy(10 + 30 * k, 6);
					printf("                         ");
					gotoxy(10 + 30 * k, 6);
					printf(" %d�˿ͳɹ��������%d", p->ClientID, k);
				}
				else
					printf(" %d�˿ͽ������%dʧ��", p->ClientID, k);
			}

		}
		else
			return FALSE;
		break;
	case GoingUp:
		//����õ�������ֻ�������µĳ˿�
		if (FloorWaitQueue[0][e->floor].WaitClientNum != 0)
		{
			if (OK == DeleteQueue(FloorWaitQueue[0][e->floor], p))
			{
				//ѹ��ջ��
				if (OK == Push(e->Stack[p->OutFloor], p))
				{
					e->CallCar[p->OutFloor] = 1;
					e->ClientID[e->ClientNum++] = p->ClientID;
					gotoxy(10 + 30 * k, 6);
					printf("                         ");
					gotoxy(10 + 30 * k, 6);
					printf(" %d�˿ͳɹ��������%d", p->ClientID, k);
				}
				else
					printf(" %d�˿ͽ������%dʧ��", p->ClientID, k);
			}
			else
				return FALSE;
		}
		else
			return FALSE;
		break;
		//����Ǵ��ڵȴ�״̬��Ҫ��������
	case Idle:
		break;
	}
}
//�˿ͽ������ݴ��������ȳ����
Status ClientInOrOut(Elevator e, int k)
{
	if (ClientOut(e, k) == FALSE)
	{
		if (ClientIn(e, k) == FALSE)
			return FALSE;
		//���˽�����ݣ���Ҫ������ʾ�ȴ�����
		DisappearWaitingClient(FloorWaitQueue[1][e->floor], 0, 0);
		DisappearWaitingClient(FloorWaitQueue[0][e->floor], 1, FloorWaitQueue[1][e->floor].WaitClientNum);
		ShowWaitingClient(FloorWaitQueue[1][e->floor], 0, 0);
		ShowWaitingClient(FloorWaitQueue[0][e->floor], 1, FloorWaitQueue[1][e->floor].WaitClientNum);
	}
	return TRUE;
}
//�������еĳ˿��Ƿ�Ҫ������˵���
//floor��ʾ��ǰ¥�㣬flag��ʾ���»�������
Status ClientGiveUp(WaitQueue &Q,Elevator E[],int floor)
{
	Client node;
	ClientQueuePtr p=Q.front;
	if (Q.WaitClientNum <= 0)
		return FALSE;
	else if (NULL != Q.front->next)
		node = Q.front->next->data;
	else
		return FALSE;
	//�������ʱ�䵽�˲��Ҹò�û�е���
	if (node != NULL && node->GiveupTime <= 0 && E[0]->floor != floor && E[1]->floor != floor)
	{
		gotoxy(10 + 30, 6);
		printf("                         ");
		gotoxy(10 + 30, 6);
		printf(" %d�ų˿ͷ�����˵���\n", node->ClientID);
		//�ȴ��ĳ˿��뿪���ݣ�Ӧ��������������ʾ�ڵȴ��ĳ˿�
		DisappearWaitingClient(FloorWaitQueue[1][floor], 0, 0);
		DisappearWaitingClient(FloorWaitQueue[0][floor], 1, FloorWaitQueue[1][floor].WaitClientNum);
		DeleteQueueNode(Q,p);
		ShowWaitingClient(FloorWaitQueue[1][floor], 0, 0);
		ShowWaitingClient(FloorWaitQueue[0][floor], 1, FloorWaitQueue[1][floor].WaitClientNum);
	}
	else
		node->GiveupTime--;
	/*ClientQueuePtr p=Q.front;
	//�ҵ���һ���ȴ��ĳ˿ͣ��ж����Ƿ�Ҫ�����ȴ�
	if (Q.WaitClientNum <= 0)
		return FALSE;
	if (p->next != NULL)
		node =p->next->data;
	else
		return FALSE;
	//�������ʱ�䵽�˲��Ҹò�û�е���
	if (node->GiveupTime <= 0 && E[0]->floor != floor && E[1]->floor != floor)
	{
		gotoxy(10 + 30 , 6);
		printf("                         ");
		gotoxy(10 + 30 , 6);
		printf(" %d�ų˿ͷ�����˵���\n", node->ClientID);
		//�˿ͳ���
		DeleteQueueNode(Q, p);//�������ȴ�����ɾ��
	}
	else
		node->GiveupTime--;*/
	return TRUE;
}
