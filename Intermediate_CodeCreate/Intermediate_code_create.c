#include <string.h>
#include "node.h"
extern struct_node head_struct;
extern FILE* file;
static int id_num=0;
static int lable_num=0;
static new_id head_code_list=NULL;
char code_tem[52];
void id_create(char* arg)
{
	sprintf(arg,"t%d",++id_num);
}
void label_create(char* arg)
{
	sprintf(arg,"label%d",++lable_num);
}
new_id id_create_from_hashnode(hash_node arg)
{
	new_id flag=(new_id)malloc(sizeof(struct New_id));
	flag->kind=arg->kind;
	if(arg->kind==array)
	{
		flag->size=size_of_array(arg->info_array);
		//flag->kind==address;
	}
	else if(arg->kind==structure)
	{
		flag->size=search_structid(head_struct,arg->id_struct)->size;
		//flag->kind==address;
	}
	else if(arg->kind==type_int)
		flag->size=int_size;
	else
		flag->size=float_size;
	id_create(flag->id);
	return flag;
}
struct New_id id_create_from_null(tag arg,int size)
{
	struct New_id flag;
	id_create(flag.id);
	flag.kind=arg;
	flag.size=size;
	return flag;
}
void print_id(new_id arg)
{
	switch(arg->kind)
	{
		case type_INT:fprintf(file,"#%d",arg->type_int);break;
		case type_FLOAT:fprintf(file,"#%f",arg->type_float);break;
		case type_strong:fprintf(file,"*%s",arg->id);break;
		case err_type:fprintf(file,"%s","ERROR");break;
		default:fprintf(file,"%s",arg->id);
	}
}
void copy_code(new_id arg)
{
	switch(arg->kind)
	{
		case type_INT:sprintf(code_tem,"#%d",arg->type_int);break;
		case type_FLOAT:sprintf(code_tem,"#%f",arg->type_float);break;
		case type_strong:sprintf(code_tem,"*%s",arg->id);break;
		case err_type:sprintf(code_tem,"%s","ERROR");break;
		default:sprintf(code_tem,"%s",arg->id);
	}
}

