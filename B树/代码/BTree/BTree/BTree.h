#pragma once
#include<stdio.h>
#include<stdlib.h>
#include <conio.h>

#define M 10			//Ĭ��10��B��
#define KeyType int

//����״̬
typedef enum {
	ERROR, OK, OVERFLOW
}Status;

typedef struct {  //��¼�Ľṹ����  
	KeyType key;
	char data;
}Record;

typedef struct BTNode {
	int keynum;					//�ڵ㵱ǰ�Ĺؼ��ָ���
	KeyType key[M + 1];			//�ؼ������� ��key[0]δ��
	struct BTNode * parent;		//˫�׽ڵ�ָ��
	struct BTNode * ptr[M+ 1];	//���ӽڵ�ָ������
	Record * recptr[M + 1];		//��¼ָ��������0�ŵ�Ԫδ��
}BTNode,* BTree;

typedef struct {
	BTree pt;
	int i;
	int tag;
}result;

//��������
int menu();
Status CreateBTree(BTree &T, int n,  int rootNum);
void InsertBTree(BTree &t, int k, BTree q, int i);
int Search(BTree p, int k);
void SearchBTree(BTree T, int k, result &r);
void ShowBTree(BTree t, int x);
void newRoot(BTree &t, BTree p, int x, BTree ap);
int CountBTreeKeyNum(BTree t);
void CountBTreeNodeNum(BTree t, int& num);
void DestroyBTree(BTree T);
void TravelBTreePosterorder(BTree t);
void TravelBTreeInorder(BTree t);
void DeleteBTree(BTree &p, int i);