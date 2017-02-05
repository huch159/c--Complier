#include "node.h"
#include <string.h>
int is_type(tag kind)
{
	if(kind==type_int||kind==type_float||kind==type_FLOAT||kind==type_INT)
		return 1;
	else
		return 0;
}
/*struct Para_node ID_type(char* name)
{
	hash_node flag=search_hashtable(name);
	struct Para_node para;
	if(flag==NULL)
	{
		para.kind=err_type;
	}
	else
	{	
		para.kind==flag->kind;
		if(para.kind==array)
			para.info_array=flag->info_array;
		if(para.kind==structure)
			para.id_struct=flag->id_struct;
	}	
	para.left_value=1;
	return para;	
}
*/
void type_parameter(tag arg)
{
	switch(arg)
	{
		case type_INT:
		case type_int:printf("int");break;
		case type_FLOAT:
		case type_float:printf("float");break;
		case array:printf("array");break;
		case structure:printf("struct");break;
		case type_ID:printf("ID");break;
		case err_type:printf("err_type");break;
		default:printf("...");break;
	}
}
/*********************
	prarmeter:
		arg1:func_node
		arg2:Args or NULL
***********************/
void print_func_para_fault(func_node arg1,pNode arg2)
{
	printf("Error type 9 at Line %d :Function \"%s(",arg2->row,arg1->name);
	int j;
	for(j=0;j<arg1->num_para-1;j++)
	{
		type_parameter((arg1->para+j)->kind);
		printf(",");
	}
	if(arg1->num_para!=0)
		type_parameter((arg1->para+j)->kind);
	printf(")\" is not applicable for arguments \"(");
	if(arg2!=NULL)
	{
		pNode flag_help=arg2;
		while(flag_help->num!=1)
		{
			type_parameter((flag_help->kids->kid->para).kind);
			printf(",");
			flag_help=(flag_help->kids+2)->kid;
		}
		type_parameter((flag_help->kids->kid->para).kind);
	}
	printf(")\"\n");
}
int is_same_para(para_node arg1,para_node arg2)
{
	if((arg1->kind!=arg2->kind)&&(!is_type(arg1->kind)||!is_type(arg2->kind)))
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
void copy_para_withoutname(para_node arg1,para_node arg2)
{
	arg1->kind=arg2->kind;
	arg1->left_value=arg2->left_value;
	if(arg1->kind==structure)
		arg1->id_struct=arg2->id_struct;
	else if(arg1->kind==array)
	{	
		//arg1->info_array=(array_node)malloc(sizeof(struct Array_node));
		arg1->info_array=arg2->info_array;
	}
}
