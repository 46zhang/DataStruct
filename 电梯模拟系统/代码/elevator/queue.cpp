#include "elevator.h"
//�ȴ����еĻ�������
Status InitQueue(WaitQueue &Q) {
	//����һ���ն���Q
	Q.front=Q.rear= new ClientQueueNode;
	if (!Q.front)
		return OVERFLOW;//����洢ʧ��
	Q.front->next = NULL;
	Q.front->data = NULL;
	Q.WaitClientNum = 0;
	return OK;
}

Status DestroyQueue(WaitQueue &Q) {
	//���ٶ���Q
	while (Q.front) {
		Q.rear = Q.front->next;
		if (Q.front->data) free(Q.front->data);
		delete Q.front;
		Q.front = Q.rear;
	}
	return OK;
}

Status EnterQueue(WaitQueue &Q, ElemType e) {
	//����Ԫ��eΪQ���µĶ�βԪ��
	ClientQueuePtr p;
	p = new ClientQueueNode;
	if (!p)
		return OVERFLOW;
	p->data = e; p->next = NULL;
	Q.rear->next = p;
	Q.rear = p;
	Q.WaitClientNum++;
	return OK;
}
Status DeleteQueue(WaitQueue &Q, ElemType &e) { //��ͷ���Ķ���
	//�����в���,��ɾ��Q�Ķ�ͷԪ��,��e������ֵ,������OK;
	//���򷵻�ERROR
	ClientQueuePtr p;
	if (Q.front == Q.rear)  //�������Ϊ�գ��򷵻ش���
		return ERROR;
	p = Q.front->next;    //�ƹ�ͷ���
	e = p->data;
	Q.front->next = p->next;
	if (Q.rear == p)
		Q.rear = Q.front;
	delete p;
	Q.WaitClientNum--;
	return OK;
}

Status IsQueueEmpty(WaitQueue Q) {
	//�ж϶����Ƿ�Ϊ��
	if (Q.front == Q.rear)
		return TRUE;
	else
		return FALSE;
}

Status DeleteQueueNode(WaitQueue &Q, ClientQueuePtr p) {
	//ɾ��������pָ��Ľ�����һ�����
	ClientQueuePtr q;
	ClientQueuePtr *temp=&Q.rear;
	if (p == NULL || p->next == NULL)
		return ERROR;
	q = p->next;
	p->next = q->next;
	if (p->next == NULL)
		*(temp) = p;
	//free(q->data);
	//free(q);
	Q.WaitClientNum--;
	return OK;
}

void PrintQueue(WaitQueue Q) {
	//�������
	ClientQueuePtr q;
	int count = 0;
	if (Q.front->next == NULL)  //�������Ϊ�գ���ת��end
		goto end;
	q = Q.front->next;
	while (q != NULL) {
		cout << setw(3) << q->data->ClientID << ' ';
		q = q->next;
		count++;
	}
end:
	while (count++ <= 4) //����ո�ȫռλ��ÿ��ռ3���ַ�λ
		cout << "    ";
}