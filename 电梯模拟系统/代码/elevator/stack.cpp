#include "elevator.h"

Status InitStack(ClientStack &S) {
	//����һ����ջ
	S.base = (ElemType *)malloc(STACK_INIT_SIZE * sizeof(ElemType));
	if (!S.base) return OVERFLOW;
	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;
	return OK;
}//InitStack


Status DestroyStack(ClientStack &S) {
	//����ջS
	ElemType	*p;
	if (S.base) {
		for (p = S.base; p < S.top; p++)
//			DestoryClient(*p);
		free(S.base);
	}
	return OK;
}

Status StackEmpty(ClientStack S) {
	//��ջSΪ�գ��򷵻�TRUE�����򷵻�FALSE
	if (S.top == S.base)
		return TRUE;
	else
		return FALSE;
}
Status Push(ClientStack &S, ElemType e) {
	//��ջ
	if (!S.base)
		return ERROR;
	if (S.top - S.base >= S.stacksize) {
		S.base = (ElemType *)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(ElemType));
		if (!S.base)
			return OVERFLOW;
		S.top = S.base + S.stacksize;
		S.stacksize += STACKINCREMENT;
	}
	*S.top++ = e;
	return OK;
}//Push
Status Pop(ClientStack &S, ElemType &e) {   //��ջ������ջ��Ϣ����
	//��ջ
	if (S.top == S.base)
		return ERROR;
	e = *(--S.top);
	return OK;
}//Pop

