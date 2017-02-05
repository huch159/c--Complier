#include "node.h"
#include <stdio.h>
void prttab(int n)
{
	while(n--)
		printf(" ");
}
void display(pNode arg,int layer)
{
	int flag=0;
	int count=arg->num;
	while(count--)
	{
		if(!arg) break;
		if(((arg->kids+count)->kind)!=0||(((arg->kids+count))->kid)!=NULL)
		{	
			flag=1;
			break;
		}
	}
	if(flag)
	{
		int layer_inv=layer;
		prttab(layer);
		printf("%s (%d)\n",arg->type_id,arg->row);
		for(int i=0;i<arg->num;i++)
			switch((arg->kids+i)->kind) 
			{
				case gram:if(((arg->kids+i))->kid) {display(((arg->kids+i))->kid,layer+2);}break;
				case type_ID:prttab(layer+2);printf("ID:%s",((arg->kids+i))->type_id);printf("\n");break;
				case type_int:prttab(layer+2);printf("TYPE:int");printf("\n");break;
				case type_float:prttab(layer+2);printf("TYPE:float");printf("\n");break;
				case type_INT:prttab(layer+2);printf("INT:%d",((arg->kids+i))->type_int);printf("\n");break;
				case lex:prttab(layer+2);printf("%s",((arg->kids+i))->type_id);printf("\n");break;
				case type_FLOAT:prttab(layer+2);printf("FLOAT:%f",((arg->kids+i))->type_float);printf("\n");break;
				case type_RELOP:prttab(layer+2);printf("RELOP:%s",((arg->kids+i))->type_id);printf("\n");break;
			}
	}
}
void delete_tree(pNode arg)
{
	if(arg!=NULL)
	{
		int i;
		for(i=0;i<arg->num;i++)
		{
			if((arg->kids+i)->kind==gram)
				delete_tree((arg->kids+i)->kid);		
		}
		free(arg->kids);
		free(arg);
	}
}
