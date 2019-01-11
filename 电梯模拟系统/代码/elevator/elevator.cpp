#include"elevator.h"

extern  int MaxClientNum ;						//���˿�����
extern  int MaxRunTime;							//�������ʱ��
extern  int NextClientInterTime;				//��һ���˿ͽ���ϵͳ��ʱ��
extern  WaitQueue FloorWaitQueue[2][5];			//ÿ��¥�������ȴ����У�0����,1����
extern  int CallUp[5], CallDown[5];				//¥����������°�ť

//�жϸö�����ʱ�������Ƿ���
Status TimeUp(Elevator e)
{
	if (e->State == Idle && e->floor == 1)
		return FALSE;
	else
		if (e->StateCount<=0)
			return TRUE;
}

//û���˽������
Status NobodyIn(Elevator e)
{
	if (e->State == GoingUp && FloorWaitQueue[0][e->floor].WaitClientNum == 0)
		return TRUE;
	else if (e->State == GoingDown && FloorWaitQueue[1][e->floor].WaitClientNum == 0)
		return TRUE;
	else
		return FALSE;
}
//û���˳�����
Status NobodyOut(Elevator e)
{
	//û�� �˿������¥��ĵ���
	if (!e->CallCar[e->floor])
		return TRUE;
	//�ò�û�г˿ͻ����߹�
	else if (e->State == GoingUp && StackEmpty(e->Stack[e->floor]) == TRUE )
		return TRUE;
	//�ò�û�г˿ͳ��˿�
	else if (e->State == GoingDown && StackEmpty(e->Stack[e->floor]) == TRUE )
		return TRUE;
	else
		return FALSE;
}
//û�˽�������
Status NobodyInOrOut(Elevator e)
{
	//û�� �˿������¥��ĵ���
	if (NobodyIn(e) == TRUE && NobodyOut(e) == TRUE)
		return TRUE;
	else
		return FALSE;
}

//�Ƿ�Ҫ����һ��ͣ��
Status StopNextFloor(Elevator e)
{
	//���������Ҫ����һ��ͣ��
	//��һ���г˿ͣ���һ��ΪĿ�Ĳ㣬��һ��Ϊ�յ�
	if (e->State == GoingUp)
	{
		if (e->floor == MaxFloor)
			return TRUE;
		else if (CallUp[e->floor] == 1)
			return TRUE;
		else if (e->CallCar[e->floor] == 1)
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		if (e->floor  == MinFloor)
			return TRUE;
		else if (CallDown[e->floor ] == 1)
			return TRUE;
		else if (e->CallCar[e->floor ] == 1)
			return TRUE;
		else
			return FALSE;
	}
}
//�ı���ݵ���Ϊ
void ElevatorChangeMove(Elevator e,int k)
{
	switch (e->move)
	{
	case Opening:
		e->move = Opened;
		e->StateCount = CloseTestTime;
		break;
	case Opened:
		//���û���˽������ߵ��������ﵽ���ޣ������
		if (NobodyInOrOut(e) == TRUE || e->ClientNum == MaxClientNum)
		{
			e->move = Closing;
			e->StateCount = DoorTime;
		}
		else
			e->StateCount = CloseTestTime;
		break;
	case Closing:
		if (NobodyInOrOut(e) == TRUE)
		{
			e->move = Closed;
		}
		//��������ڼ����˽��룬����
		else if(e->ClientNum<MaxClientNum)
		{	
			e->move = Opening;
			e->StateCount = DoorTime;
		}
		break;
	case Closed:
		//������ɺ��ж���Ҫ�����ƶ����������ƶ�����ֹͣ
		if (e->State == GoingUp)
		{
			//�����Ӧ�¼�
			if (-1 == CallUp[e->floor])
				CallUp[e->floor] = 0;
			int j;
			//�ж��Ƿ���Ҫ��������
			for ( j = e->floor; j <= MaxFloor; j++)
			{
				//����ϲ���Ŀ��¥�㣬����Ҫ����
				if (e->CallCar[j] == 1)
				{
					e->move = Accelerate;
					e->StateCount = AccelerateTime;
					break;
				}
			}//˵��û�����ϵ�Ŀ��¥��
			if (j > MaxFloor)
				e->move = Waiting, e->State = Idle;
		}
		else if (e->State == GoingDown)
		{
			//�����Ӧ�¼�
			if (-1 == CallDown[e->floor])
				CallDown[e->floor] = 0;
			int j;
			//���и��׵�Ŀ��¥�㣬��������
			for ( j = e->floor; j >=0; j--)
			{
				if (1 == e->CallCar[j])
				{
					e->move = Accelerate;
					e->StateCount = AccelerateTime;
					break;
				}
			}
			if (j < 0)
				e->move = Waiting, e->State = Idle;
		}
		else
			e->move = Waiting;
		break;
	case Accelerate:
		e->move = Moving;
		//����ʱ����½�ʱ�䲻һ��,Ҫ���ֿ�
		if (e->State == GoingUp)
			e->StateCount = UpTime;
		else
			e->StateCount = DownTime;
		break;
	case Moving:	//����ƶ�
		//��������������ʾ����
		DisappearEvelatorBody(e,k,e->floor);
		if (e->State == GoingUp)
		{
			e->floor++;
			if (TRUE == StopNextFloor(e))
			{
				e->move = SlowDown;
				e->StateCount = UpSlowTime;
			}
			//�����������
			else
				e->StateCount = UpTime;
		}
		else if(GoingDown==e->State)
		{
			e->floor--;
			if (TRUE == StopNextFloor(e))
			{
				e->move = SlowDown;
				e->StateCount = DownSlowTime;
			}
			//�����½�
			else
				e->StateCount = DownTime;
		}
		ShowElevatorBody(e,k,e->floor);
		break;
	case SlowDown:
		//����ò㣬��յ����ڵ�Ŀ��¥���־λ
		e->CallCar[e->floor] = 0;
		//�ж��Ƿ���Ҫת�䷽��
		if (e->State == GoingUp)
		{
			int need = 1;
			//��û����Ŀ��㣬����ת��
			for (int j = e->floor; j < MaxFloor; j++)
			{
				if (1 == e->CallCar[j])
				{
					need = 0;
					break;
				}
			}
			//����ı䷽��,�жϸ߲��Ƿ����������û�У��ı䷽��
			if (1==need&& -1==HigherRequires(e) )
				e->State = GoingDown;;

		}
		else if (e->State = GoingDown)
		{
			int need = 1;
			//û�ﵽ���Ŀ����������
			for (int j = e->floor; j > 0; j--)
			{
				if (1 == e->CallCar[j])
				{
					need = 0;
					break;
				}
			}
			//���������ײ㻹û��Ӧ�ı�״̬
			if (1 == need && -1 == LowerRequires(e))
				e->State = GoingUp;
		}
		
		e->move = Opening;
		e->StateCount = DoorTime;
		
		break;
	case Waiting: //�ȴ�״̬��Control����������һ�ж����ж�
		break;
	}
}

//�����ϻ������µ����󣬷������һ��ĵ�������
int HasUpOrDownRequires(Elevator e)
{
	int i, j;
	for (j = e->floor-1, i = e->floor+1; j >= 0 || i <= MaxFloor; j--, i++)
	{
		if (j>=0&&(CallDown[j]==1 || CallUp[j]==1))
			return j;
		if (i <= MaxFloor && (CallDown[i] == 1 || CallUp[i] == 1))
			return i;
	}
}
//�߲��Ƿ��������з��ظ�¥�㣬û�з���-1
int HigherRequires(Elevator e)
{
	int floor;
	for (floor = e->floor; floor <= MaxFloor; floor++)
	{
		if (CallUp[floor])
			return floor;
	}

	return -1;
}
//�ײ��Ƿ�������,�з��ظ�¥�㣬û�з���-1
int LowerRequires(Elevator e)
{
	int floor;
	for (floor = e->floor; floor >= 0; floor--)
	{
		if (CallDown[floor])
			return floor;
	}
	return -1;
}
//���������ƶ�
Status ElevatorMoveDown(Elevator &e)
{
	int floor = LowerRequires(e);
	if (floor != -1)
	{
		//���������Ǹò���ݣ�ֱ�Ӵ򿪵�����
		if (e->floor == floor)
			e->move = Opening;
		else
		{
			e->CallCar[floor] = 1;
			e->move = Accelerate;
			//��Ӧ�ò������
			CallDown[floor] = -1;
		}
		return TRUE;
	}
	return FALSE;
}
//���������ƶ�
Status ElevatorMoveUp(Elevator &e)
{
	int floor = HigherRequires(e);
	//��¥������
	if (floor != -1)
	{
		//�����Ӧ�ĵ���Ϊ���㣬��ֱ�ӿ���
		if (floor == e->floor)
			e->move = Opening;
		else
		{
			e->CallCar[floor] = 1;
			e->move = Accelerate;
			//��Ӧ��־
			CallUp[floor] = -1;
		}
		return TRUE;
	}
	else
		return FALSE;
	

}
//���ƺ����������жϵ��ݵ���һ���ƶ�����
//�ò����˼��Ϊ����0�ŵ���ȥ��Ӧ�����һ̨����
//����1�ŵ���ȥ��Ӧ�����һ̨����
//ͨ����Ӧ�¼�������Ӧ��ĵ��ݰ�ť����Ϊ-1��������̨���ݾ���ͬһ��Դ
void Controler(Elevator E[])
{
	for (int i = 0; i <= 1; i++)
	{//���i�ŵ���λ�ڵȴ�״̬
		if (E[i]->move == Waiting)
		{
			//������������µ��ݵĻ���
			int  floor;
			//����ڱ����г˿ͣ�ֱ�ӿ���
			if (CallUp[E[i]->floor] == 1)
				E[i]->move = Opening, CallUp[E[i]->floor] = -1, E[i]->State = GoingUp;
			else if (CallDown[E[0]->floor] == 1)
				E[i]->move = Opening, CallDown[E[i]->floor] = -1, E[i]->State = GoingDown;
			//�����ҵ����������¥��
			else
			{
				floor = HasUpOrDownRequires(E[i]);
				//û�г˿���Ӧ
				if (floor<0 || floor>MaxFloor)
					return;
				else if (1==CallUp[floor])
				{
					//�����˶�
					if (floor >= E[i]->floor)
					{
						E[i]->move = Accelerate;
						E[i]->CallCar[floor] = 1;
						//����¥����Ӧ��־
						CallUp[floor] = -1;
						E[i]->State = GoingUp;
					}
					else
					{
						E[i]->move = Accelerate;
						E[i]->CallCar[floor] = 1;
						//����¥����Ӧ��־
						CallUp[floor] = -1;
						E[i]->State = GoingDown;
					}
				}
				else if (CallDown[floor] == 1)
				{
					//���������˶�
					if (floor >= E[i]->floor)
					{
						E[i]->move = Accelerate;
						E[i]->CallCar[floor] = 1;
						//����¥����Ӧ��־
						CallDown[floor] = -1;
						E[i]->State = GoingUp;
					}
					else
					{
						E[i]->move = Accelerate;
						E[i]->CallCar[floor] = 1;
						//����¥����Ӧ��־
						CallDown[floor] = -1;
						E[i]->State = GoingDown;
					}
				}

			}
		}
	}
	
}