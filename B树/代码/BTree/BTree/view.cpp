#include "BTree.h"

int menu() {//�˵�  
	int choice;
	printf("\n\n\t\t\t������������������������B�����ԡ���������������������\n");
	printf("\n\n");
	printf("\t\t\t    __________________B�����ܲ˵�_________________\n");
	printf("\t\t\t   |                                              |\n");
	printf("\t\t\t   |                                              |\n");
	printf("\t\t\t   |   1.����B��            2.���B��             |\n");
	printf("\t\t\t   |                                              |\n");
	printf("\t\t\t   |   3.B�����Ĳ���      4.B������ɾ��       |\n");
	printf("\t\t\t   |                                              |\n");
	printf("\t\t\t   |   5.B�����������      6.B���ĺ������       |\n");
	printf("\t\t\t   |                                              |\n");
	printf("\t\t\t   |   7.B���Ľڵ����      8.B���ڵ���           |\n");
	printf("\t\t\t   |                                              |\n");
	printf("\t\t\t   |   9.B���Ĵݻ�          0.�˳�                |\n");
	printf("\t\t\t   |______________________________________________|\n");
	printf("\n\n");
	printf("\t\t\t������������������������������������������������������\n\n");
	printf("\t\t\t\t    ����� 3117004958 �������(4)��\n");
	printf("\n\t\t\t������������������������������������������������������\n");
	printf("\n\t\t\t��ѡ��˵��е���Ӧ����������0-9��:");
	while (scanf("%d", &choice) != 1 || choice < 0 || choice>9) {
		while (getchar() != '\n')	//���մ�������
			continue;
		printf("\n\t\t\t����������������");
		printf("\n\t\t\t��ѡ��˵��е���Ӧ����������0-9��:");
	}//����Ƿ�����  
	return choice;
}