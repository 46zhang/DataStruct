#include "BTree.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int m=10;			//����ȫ�ֱ��� һ��ʼĬ��Ϊ10
void CreateBTreeByInput(BTree &T);
void InsertBTreeByInput(BTree &T);
void CountKeyNum(BTree t);
void CountNode(BTree t);
void FineKeyInBTree(BTree t);
void DeleteBTreeNode(BTree &BT);
int main()
{
	int choice, m;
	
	BTree BT = NULL;
	while (1)
	{
		choice = menu();
		switch (choice)
		{
		case 0:	exit(0);					 //�˳�
				break;
		case 1:	CreateBTreeByInput(BT);		//����B�� 
				break;
		case 2:	ShowBTree(BT, 1);			//��ʾB��
				break;
		case 3:	InsertBTreeByInput(BT);		//����B��
				break;
		case 4:	DeleteBTreeNode(BT);		//ɾ��B���Ľڵ�
				break;
		case 5:	TravelBTreeInorder(BT);		//B�����������
				break;
		case 6:	TravelBTreePosterorder(BT);	//B���ĺ������
				break;
		case 7:	FineKeyInBTree(BT);			//���ҹؼ���
				break;
		case 8:	CountNode(BT);				//�ڵ���
				break;
		case 9:	DestroyBTree(BT);			//������ 
				break;
		}
		
	}
	return 0;
}
void CreateBTreeByInput(BTree &T)
{
	int  n, rootNum;
	printf("��������Ҫ����Ĵ�����B���Ľ���(1<m<=10):");
	while (scanf("%d", &m) != 1 || m < 1 || m>10)
	{
		printf("����������������\n");
		while (getchar() != '\n')
			continue;
		printf("��������Ҫ����Ĵ�����B���Ľ���(1<m<=10):");
	}
	printf("������B���ĳ�ʼ���ؼ��ָ���(0<=����<=10000)\n����=");
	while (scanf("%d", &n) != 1 || n < 0 || n>10000)
	{
		printf("����������������\n");
		while (getchar() != '\n')
			continue;
		printf("������B���ĳ�ʼ���ؼ��ָ���(0<=����<=10000)\n����=");
	}
	printf("������B���ĵĸ��ڵ��ֵ");
	while (scanf("%d", &rootNum) != 1 )
	{
		printf("����������������\n");
		while (getchar() != '\n')
			continue;
		printf("������B���ĵĸ��ڵ��ֵ");
	}
	if (OK == CreateBTree(T, n, rootNum))
		printf("\t\t�����ɹ�\n");
	else
		printf("\t\t����ʧ��\n");
}

void InsertBTreeByInput(BTree &T)
{
	int num;
	result r;
	printf("\t�������������Ľڵ����ֵ��");
	while (scanf("%d", &num)!=1)
	{
		printf("\t����������������\n");
		while (getchar() != '\n')
			continue;
		printf("\t�������������Ľڵ����ֵ��");
	}
	if (T == NULL)
		newRoot(T, NULL, num, NULL);
	else {
		SearchBTree(T, num, r);
		if (r.tag == 0)
		{
			InsertBTree(T, num, r.pt, r.i);
		}
	}
}
void CountKeyNum(BTree t)
{
	int num = 0;
	num = CountBTreeKeyNum(t);
	printf("\t\t��B���Ĺؼ��ָ���Ϊ%d\n",num);
}
void CountNode(BTree t)
{
	int num = 1;
	CountBTreeNodeNum(t, num);
	printf("\t\t��B���Ľڵ���Ϊ%d\n",num);
}
void FineKeyInBTree(BTree t)
{
	int num;
	result r;
	printf("\t����������ҵĹؼ��֣�");
	while (scanf("%d", &num) != 1)
	{
		printf("��������\n\t����������ҵĹؼ��֣�");
		while (getchar() != '\n')
			continue;
	}
	SearchBTree(t, num, r);
	
	if (r.tag == 0)
		printf("\t�Ҳ����ùؼ���\n");
	else
		printf("\t�����ҵĹؼ���λ�ã��ؼ���Ϊ%d�Ľڵ�ĵ�%d��",(r.pt)->key[1],r.i);
}
void DeleteBTreeNode(BTree &BT)
{
	int num;
	result r;
	printf("\t\t����������Ҫɾ���Ĺؼ���");
	while (scanf("%d", &num) != 1)
	{
		printf("��������\n\t����������ҵĹؼ��֣�");
		while (getchar() != '\n')
			continue;
	}
	SearchBTree(BT, num, r);
	if (r.tag == 0)
		printf("\t�Ҳ����ùؼ���\n");
	else
		DeleteBTree(r.pt,r.i);
	//������������ڵ�Ϊ�գ������¸�ֵ���ڵ�
	if (BT->keynum == 0)
		BT = BT->ptr[0];
}