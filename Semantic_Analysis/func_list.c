#include <string.h>
#include <stdarg.h>
#include "node.h"
func_node static head_func=NULL;
void search_func_nodefine_use(void)
{
	func_node flag=head_func;
	while(flag!=NULL)
	{
		if(flag->decl==0)
		{
			row_node flag_row=flag->use_row;
			while(flag_row!=NULL)
			{
				printf("Error type 18 at Line %d :Undefined function \"%s\"(has been declared)\n",flag_row->row,flag->name);
				flag_row=flag_row->next;
			}
		}	
		flag=flag->next;
	}
}
void insert_funcnode(func_node newnode)
{
	if(head_func==NULL)
	{
		head_func=newnode;
		head_func->next=NULL;
	}
	else
	{
		func_node flag=head_func->next;
		newnode->next=head_func;
		head_func=newnode;
	}
}
func_node search_funcnode(char* id)
{
	func_node flag=head_func;
	while(flag!=NULL)
	{
		if(!strcmp(flag->name,id))
			return flag;
		else
			flag=flag->next;
	}
	return NULL;
}
void delete_func()
{
	while(head_func!=NULL)
	{
		func_node flag=head_func;
		head_func=head_func->next;
		if(!strcmp(flag->name,"read")||!strcmp(flag->name,"write"))
		{
			free(flag);
			continue;
		}
		delete_func_para(flag);
	}
}
void delete_func_para(func_node arg)
{
	int i;
	for(i=0;i<arg->num_para;i++)
	{
		if((arg->para+i)->kind==array)
			delete_array((arg->para+i)->info_array);
	}
	while(arg->use_row!=NULL)
	{
		row_node flag=arg->use_row->next;
		free(arg->use_row);
		arg->use_row=flag;	
	}
	free(arg->para);
	if(arg->re_turn->kind==array)
		delete_array(arg->re_turn->info_array);
	free(arg->re_turn);
	free(arg);
}
/*int search_func_para(func_node arg,int num,...)
{
	va_list vl;
	va_start(vl,num);
	func_node flag=search_funcnode(arg->name);
	if(flag->num_para!=num) return 0;
	int i;
	for(i=0;i<num;i++)
	{
		if(((flag->para+i)->kind)!=va_arg(vl,tag))
			return 0;
	}
	va_end(vl);
	return 1;
}*/
/****************************
	parameter:
		arg1:func_node
		arg2:func_node
	return:
		wether parameters of two function is all same
******************************/
int is_same_func(func_node arg1,func_node arg2)
{
	if(!is_same_para(arg1->re_turn,arg2->re_turn))
		return 0;
	else if(strcmp(arg1->name,arg2->name))
		return 0;
	else if(arg1->num_para!=arg2->num_para)
		return 0;
	else
	{
		int i;
		for(i=0;i<arg2->num_para;i++)
		{
			if(!is_same_func_para((arg1->para+i),(arg2->para+i)))
				return 0;
		}
	}
	return 1;
}
int is_same_func_para(para_node arg1,para_node arg2)
{
	if((arg1->kind!=arg2->kind))
		return 0;
	else
	{
		if(arg1->kind==structure&&(arg1->id_struct!=arg2->id_struct))
			return 0;
		else if(arg1->kind==array)
		{
			if(!same_type_array(arg1->info_array,arg2->info_array))
				return 0;
		}
		else 
			return 1;
	}
}
