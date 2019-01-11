#include "BTree.h"

extern int m;
//�������n��������Ϲ���m��B��
Status CreateBTree(BTree &T, int n, int rootNum)
{
	int  *num, index;
	result r;
	newRoot(T,NULL,rootNum,NULL);
	if (NULL == T)
		return OVERFLOW;
	num = (int *)malloc(sizeof(int)*n);
	if (NULL == num)
		return OVERFLOW;
	for (int i = 0; i < n; i++)
	{
		//��������
		num[i] = rand() % 9999;
	}
	//��ʼ����
	for (int i = 0; i < n; i++)
	{
		SearchBTree(T,num[i],r);
		//printf(" %d,%d ",num[i],r.tag);
		if(r.tag==0)
			InsertBTree(T, num[i],r.pt,r.i);
	}
	return OK;
}
//���ѣ���sΪ�е㣬��q���ѳ������֣�ǰ�벿�ֱ���ԭ�ڵ㣬��벿�ַ���ap
void spilt(BTree &q, int s, BTree &ap)
{
	int i, j;
	//�����ڴ�
	ap = (BTNode *)malloc(sizeof(BTNode));
	ap->ptr[0] = q->ptr[s];
	q->ptr[s] = NULL;
	//��ֵ�½ڵ�Ĺؼ������������ָ������
	//��վɽڵ��벿�ֵĹؼ��������ָ������
	for (i = s + 1, j = 1; i <= q->keynum; i++, j++)
	{
		ap->key[j] = q->key[i];
		ap->ptr[j] = q->ptr[i];
		q->key[i] = 0;
		q->ptr[i] = NULL;
	}
	//��ap�ĺ���ָ����ֵ
	for (i = 0; i <= q->keynum - s; i++)
	{
		if (ap->ptr[i] != NULL)
			ap->ptr[i]->parent = ap;
	}
	ap->keynum = q->keynum - s;
	q->keynum = s-1;   
	ap->parent = q->parent;
}
void Insert(BTree &q,int i,int x,BTree ap)
{
	//��key[i]����x,ָ������ptr[i]����ap
	//����������ǽ�ԭ����ֵ���ƣ�Ȼ����iλ������ֵ
	int j;
	for (j = q->keynum; j >= i; j--)
	{
		q->key[j + 1] = q->key[j];
		q->ptr[j + 1] = q->ptr[j];
	}
	q->key[i] = x;
	q->ptr[i] = ap;
	if (ap != NULL)
	{
		ap->parent = q;
	}
	//������ɺ�ؼ��ָ�����һ
	q->keynum++;
}
//�½����ڵ㣬��ֵΪx����������ptr[0]Ϊp,ptr[1]Ϊap
void newRoot(BTree &t,BTree p,int x,BTree ap)
{
	t = (BTree)malloc(sizeof(BTNode));
	t->ptr[0] = p;
	t->ptr[1] = ap;
	t->parent = NULL;
	t->key[1] = x;
	t->keynum = 1;
	if (p)
		p->parent = t;
	if (ap)
		ap->parent = t;
}
//��B��t��q�ڵ��key[i-1]��key[i]֮�����ؼ���k
//�������ڵ�ؼ��ָ�������B���Ľף�����˫��ָ�������нڵ���ѣ�ʹt����m��B��
void InsertBTree(BTree &t,int k,BTree q,int i)
{
	int x, s, finish = 0, needNewRoot = 0;
	BTree ap=NULL;
	//ֻ�е����ڵ�ΪNULL��ʱ��Ż����qΪNULL�����
	//�����½�һ�����ڵ�
	if (NULL == q)
		newRoot(t,NULL,k,NULL);		//�����µĸ��ڵ�
	else
	{
		x = k, ap = NULL;
		while (finish == 0 && needNewRoot == 0)
		{
			//�ؼ���������iλ�ò���k
			//����ָ��������iλ�ò���ap
			Insert(q, i, x, ap);
			//���С�ڽ���m����ɹ�
			if (q->keynum < m)
				finish = 1;
			//������Ҫ���з���
			else
			{
				s = (m + 1) / 2;
				//��sΪ�м䣬���ѳ�������
				spilt(q, s, ap);
				x = q->key[s];
				//Ѱ���м�Ԫ���ڸ��ڵ�Ĳ���λ��
				if (q->parent != NULL)
				{
					i = Search(q->parent, x);
					q = q->parent;
				}
				else
					needNewRoot = 1;
			}
			
		}
		if (needNewRoot == 1)
			newRoot(t, q, x, ap);
	}
}
int Search(BTree p, int k) 
{
	int i = 1;
	while (i <= p->keynum&&k > p->key[i])
		i++;
	return i;
}
void SearchBTree(BTree T, int k, result &r)
{
	//��m��B��T�ϲ��ҹؼ���k������(pt,i,tag)  
	//�����ҳɹ���������ֵtag=1,ָ��pt���½���е�i���ؼ��ֵ���k;����  
	//����ֵtag=0������k�Ĺؼ��ּ�¼Ӧ������ָ��pt��ָ����е�i-1���͵�i���ؼ��ּ�  
	int i = 0, found = 0;
	BTree p = T, q = NULL;
	while (p != NULL && 0 == found) 
	{
		i = Search(p, k);		//��p->key[1..keynum]�в���p->key[i-1]<k<=p->p->key[i]  
		if (i > 0 && p->key[i] == k)
			found = 1;			//�ҵ�����ؼ���
		else {
			q = p;				//�Ҳ��������亢�ӽڵ�ȥ����
			p = p->ptr[i - 1];
		}
	}
	if (1 == found)        //���ҳɹ�
	{
		r.pt = p;
		r.i = i;
		r.tag = 1;
	}
	else                  //���Ҳ��ɹ�������key�Ĳ���λ��i 
	{
		r.pt = q;
		r.i = i;
		r.tag = 0;
	}
}
//��p��Ai�������ҵ���С�� ���²���ն˽ڵ�
void Successor(BTree &p,int i)
{
	BTree q = p->ptr[i];
	while (q->ptr[0])
		q= q->ptr[0];
	//��ֵ
	p->key[i] = q->key[1];
	p = q;
}
//ɾ��p�ڵ�ĵ�i���ؼ���
void Remove(BTree &p,int i)
{
	for (int j = i; j < p->keynum; j++)
		p->key[j] = p->key[j + 1];
	p->keynum--;
}
//�Ѹ��ڵ��q����P
void ChangeRoot(BTree &q,BTree &p)
{
	BTree temp;
	temp = q;
	q= p;
	p->parent = NULL;
	//free(temp);
}
//����B��
void Restore(BTree &p,int i)
{ 
	int num = 0, j = 0;
	BTree q = p->parent, brother;
	//�ҳ�p�ڵ�λ��q�ڵ���ĸ�λ��,��ֵnum
	while (num < q->keynum)
	{
		if (q->ptr[num] == p)
			break;
		num++;
	}
	//�ж����ֵ��Ƿ��ж���Ľڵ�
	if (num > 0 && q->ptr[num - 1]->keynum > m  / 2)
	{
		brother = q->ptr[num - 1];
		j = i-1;
		//����ߵĸ��׽ڵ��Ĺؼ��ֱ�p�ڵ��κιؼ��ֶ�С�����Ժ�������Ԫ��
		while (j<=p->keynum)
			p->key[j+1] = p->key[j++];
		//�ٸ�ֵ��p�ڵ��һ���ؼ���
		p->key[1] = q->key[num ];
		p->keynum++;
		//�����ֵ����Ľڵ㸳ֵ�����ڵ㣬�����Ǹ����ֵ�p�ڵ�Ĺؼ���
		q->key[num] = brother->key[brother->keynum];
		Remove(brother,brother->keynum);
	}
	//�ж����ֵ��Ƿ��ж���Ľڵ�
	else if (num < q->keynum&&q->ptr[num + 1]->keynum>m/2)
	{
		brother = q->ptr[num + 1];
		//���ұߵĸ��ڵ��Ĺؼ��ֱ�p�ڵ��κιؼ��ֶ������Ը�ֵ����p�����һ���ڵ�
		p->key[p->keynum+1] = q->key[num + 1];
		p->keynum++;
		//�����ֵܵ���С�ؼ��ָ�ֵ�����ڵ㣬�����Ǹ����ֵ�p�ڵ�Ĺؼ���
		q->key[num + 1] = brother->key[1];
		//ɾ�����ֵܵĴ���Ĺؼ���
		Remove(brother,1);
	}
	//�����ֵܶ�û����Ľڵ㣬ֻ���򸸽ڵ��
	else
	{
		//�ϲ��ؼ���Ҫע��ϲ����Ⱥ�˳��
		//�����ֵ��Լ��������ǵĸ��ڵ�ϲ�
		if (num != 0)
		{
			brother = q->ptr[num - 1];
			//�Ѹ��ڵ�������p������ڵ�Ĺؼ��ָ�ֵ�����ֵܵĹؼ�������
			//���Һ��ӵĵ�һ������ָ��Ҳ�游�ڵ�Ĺؼ���һ����뵽brother�ڵ�
			Insert(brother,brother->keynum+1,q->key[num],p->ptr[0]);
			//brother->key[++p->keynum] = q->key[num];
			j = 1;
			//��p�еĽڵ�Ҳ�ϲ���borther�ڵ���
			//���ӽڵ�Ҳ�ϲ�(�������)
			while (j <= p->keynum)
			{
				//brother->key[++p->keynum] = p->key[j];
				Insert(brother, brother->keynum + 1, p->key[j], p->ptr[j]);
				j++;
			}
			//ɾ�����ڵ�ùؼ���
			Remove(q,num);
			for(j=num;j<=q->keynum;j++)
				q->ptr[j] = q->ptr[j+1];
			q->ptr[q->keynum + 1] = NULL;
			//�ͷ�p���ڴ�
			free(p);
		}
		//�ϲ����ڵ�����Һ��ӵ�p�ڵ���
		else {
			brother = q->ptr[num + 1];
			//�Ѹ��ڵ�Ĺؼ��ָ�ֵ��p
			Insert(p,p->keynum+1,q->key[num+1],brother->ptr[0]);
			//p->key[++p->keynum] = q->key[num + 1];
			Remove(q,num+1);
			j = 1;
			//�ϲ�p�ҽڵ�Ĺؼ���
			//�Լ��ϲ��亢�ӽڵ�
			while (j <= brother->keynum)
			{
				//p->key[++p->keynum] = brother->key[j++];
				Insert(p,p->keynum+1,brother->key[j],brother->ptr[j]);
				j++;
			}
			for(j=num+1;j<=q->keynum;j++)
				q->ptr[j] = q->ptr[j+1];
			q->ptr[q->keynum+1] = NULL;
			free(brother);
		}
		//ɾ����������ڵ�ؼ�����С��m/2��Ҫ����
		if (q->keynum < m / 2)
		{
			//������Ǹ��ڵ㣬�ٽ��е���
			if (q->parent != NULL)
				Restore(q, i);
			//������˸��ڵ���Ҫ�������ڵ�
			else
				ChangeRoot(q,p);
		}
			
	}
}
//ɾ��B��p�ڵ�ĵ�i���ؼ���
void DeleteBTree(BTree &p,int i)
{
	
	if (p->ptr[i]!=NULL)			//���������²�ķ��ն˽ڵ�
	{
		Successor(p,i);				//�ҵ����²�ķ��ն˽ڵ�
		DeleteBTree(p,1);			//ɾ��������С�Ľڵ�
	}
	else							//�������²�ķ��ն˽ڵ�
	{
		Remove(p,i);
		if (p->keynum < (m - 1) / 2)
			Restore(p,i);
	}
}
//������������ʾЧ����ӡ��B��(�������)
void ShowBTree(BTree t,int x)
{
	if (t == NULL)
		return;
	printf("     -------------------------------------------------------\n");
	for (int i = 0; i < 5*x; i++)
		printf(" ");
	//�ȴ�ӡ���ýڵ�����йؼ��ֵ�ֵ
	printf("|");
	for (int i = 1; i <= t->keynum; i++)
		printf("%d|", t->key[i]);
	printf("\n");
	//ÿ��ӡ��һ���ڵ�ͻ���
	for (int i = 0; i <= t->keynum; i++)
	{
		ShowBTree(t->ptr[i],x+1);
	}

}
//����������B��
void TravelBTreeInorder(BTree t)
{
	
	int i;
	if (t == NULL)
		return;
	for (i = 0; i <= t->keynum / 2; i++)
		TravelBTreeInorder(t->ptr[i]);
	for (i = 1; i <= t->keynum; i++)
		printf(" %d ",t->key[i]);
	for(i= t->keynum / 2+1;i<=t->keynum;i++)
		TravelBTreeInorder(t->ptr[i]);
}
//����������B��
void TravelBTreePosterorder(BTree t)
{
	int  i;
	if (t == NULL)
		return;
	for (i = 0; i <= t->keynum; i++)
		TravelBTreePosterorder(t->ptr[i]);
	for (i = 1; i <= t->keynum; i++)
		printf(" %d ",t->key[i]);
	
}
//����B���ؼ�����
int CountBTreeKeyNum(BTree t)
{
	int num=0;
	if (t == NULL)
		return 0;
	else
	{
		num += t->keynum;
		for (int i = 0; i <= t->keynum; i++)
		{
			if (t->ptr[i] == NULL)
				continue;
			num+=CountBTreeKeyNum(t->ptr[i]);
		}
	}
	return num;
}
//����B���ڵ���
void CountBTreeNodeNum(BTree t,int& num)
{
	int i = 0;
	if (t == NULL)
		return;
	for (; i <= t->keynum; i++)
	{
		if (t->ptr[i] != NULL)
			num++;
		CountBTreeNodeNum(t->ptr[i],num);
	}
}
void DestroyBTree(BTree T) 
{
	//��B�����дݻ�
	int i = 1;
	if (NULL != T)
	{
		for (; i <= T->keynum + 1; i++)
		{
			DestroyBTree(T->ptr[i - 1]);
		}
		free(T);   //�ͷŽ��
	}
}

