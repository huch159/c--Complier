#include "node.h"
#include <string.h>
extern struct_node head_struct;
void delete_array(array_node arg)
{
	free(arg->each_dimen);
	free(arg);		
}
void copy_array(array_node arg1,array_node arg2,int num)//num:dimension
{
	arg1->kind=arg2->kind;
	arg1->id_struct=arg2->id_struct;
	arg1->dimension=num;
	arg1->each_dimen=(int*)malloc(num*sizeof(int));
	int i;
	for(i=0;i<num;i++)
		(arg1->each_dimen)[i]=(arg2->each_dimen)[arg2->dimension-num+i];
}
int same_type_array(array_node arg1,array_node arg2)
{
	if((arg1->kind==arg2->kind)||((is_type(arg1->kind)&&is_type(arg2->kind))))
	{
		if(arg1->kind==structure&&arg1->id_struct!=arg2->id_struct)
			return 0;
		else if(arg1->dimension!=arg2->dimension)
			return 0;
		else
			return 1;
	}
	else
	{
		return 0;
	}
}
int size_of_array(array_node arg)
{
	if(arg==NULL)
		return 0;
	else
	{
		int i,size=1;
		for(i=0;i<arg->dimension;i++)
			size*=(arg->each_dimen)[i];
		if(arg->kind==structure)
			size=size*((search_structid(head_struct,arg->id_struct))->size);
		else if(arg->kind==type_int)
			size*=int_size;
		else
			size*=float_size;
		return size;
	}
}
