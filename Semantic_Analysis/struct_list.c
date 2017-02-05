#include <string.h>
#include "node.h"
struct_node head_struct=NULL;
extern FILE* file;
extern struct New_id tem_id;
void insert_structnode(struct_node newnode)		
{
	
	if(head_struct==NULL)
		head_struct=newnode;	
	else 
	{
		newnode->next=head_struct;
		head_struct=newnode;
	}
}
void delete_structnode()
{
	struct_node flag=head_struct->next;
	delete_struct(head_struct);
	head_struct=flag;
}
struct_node search_structnode(char* id)
{
	struct_node flag=head_struct;
	while(flag!=NULL)
	{
		if(!strcmp(flag->name,id))
			return flag;
		else
			flag=flag->next;
	}
	return NULL;
}
struct_node search_structid(struct_node arg,int num)//arg:begin search,num:id of structure
{
	if(arg==NULL)
		return NULL;
	else if(num==arg->id_struct)
		return arg;
	else
	{
		struct_node flag=search_structid(arg->kid,num);
		if(flag!=NULL)
			return flag;
		else
			return search_structid(arg->next,num);
	}
}
void delete_struct(struct_node arg)
{
	int i;
	for(i=0;i<arg->num_para;i++)
	{
		if((arg->para+i)->kind==array)
			delete_array((arg->para+i)->info_array);	
	}
	free(arg->para);
	if(arg->kid!=NULL)
	{
		struct_node flag=arg->kid;
		while(flag!=NULL)
		{
			delete_struct(flag);
			flag=flag->next;
		}
	}
	free(arg);
}
struct Para_node search_attr(struct_node arg,char *name,int arr_str)
{
	int i;
	struct Para_node flag;
	int x=0,y=0;
	for(i=0;i<arg->num_para;i++)
	{
		if((arg->para+i)->kind==type_int)
			x+=int_size;
		else if((arg->para+i)->kind==type_int)
			x+=int_size;
		else if((arg->para+i)->kind==array)
			x+=size_of_array((arg->para+i)->info_array);
		else if((arg->para+i)->kind==structure)
			x+=search_structid(head_struct,(arg->para+i)->id_struct)->size;
		if(!strcmp((arg->para+i)->name,name))
		{
			struct New_id flag_id_new=id_create_from_null(address,address_size);
			if(arr_str==2&&y==0)
			{
				struct New_id flag_id_new=id_create_from_null(address,address_size);
				print_id(&flag_id_new);
				fprintf(file," %s &",":=");
				print_id(&tem_id);
				fprintf(file,"%s\n"," ");
				tem_id=flag_id_new;
				
			}
			else if(arr_str==2&&y!=0)
			{
				struct New_id flag_id_new=id_create_from_null(address,address_size);
				print_id(&flag_id_new);
				fprintf(file," %s &",":=");
				print_id(&tem_id);
				fprintf(file," + #%d\n",y);
				tem_id=flag_id_new;
			}
			if(arr_str!=2&&y!=0)
			{
				print_id(&flag_id_new);
				fprintf(file," %s ",":=");
				print_id(&tem_id);
				fprintf(file," + #%d\n",y);
				tem_id=flag_id_new;
			}
			flag.kind=(arg->para+i)->kind;
			if(flag.kind==array)
			{
				flag.info_array=(arg->para+i)->info_array;
			}
			else if(flag.kind==structure)
			{
				flag.id_struct=(arg->para+i)->id_struct;
			}
			else
			{
				tem_id.kind=type_strong;
			}
			return flag;
		}
		y=x;
	}
	flag.kind=err_type;
	return flag;
}
void copy_struct(struct_node arg1,struct_node arg2)
{
	int i;
	strcpy(arg1->name,arg2->name);
	arg1->num_para=arg2->num_para;
	arg1->id_struct=arg2->id_struct;
	arg1->para=(para_node)malloc(arg1->num_para*sizeof(struct Para_node));
	arg1->size=arg2->size;
	for(i=0;i<arg1->num_para;i++)
	{
		(arg1->para+i)->kind=(arg2->para+i)->kind;
		strcpy((arg1->para+i)->name,(arg2->para+i)->name);
		if((arg2->para+i)->kind==structure)
			(arg1->para+i)->id_struct=(arg2->para+i)->id_struct;
		else if((arg2->para+i)->kind==array)
		{
			(arg1->para+i)->info_array=(array_node)malloc(sizeof(struct Array_node));
			copy_array((arg1->para+i)->info_array,(arg2->para+i)->info_array,(arg2->para+i)->info_array->dimension);
		}
	}
	if(arg2->kid!=NULL) 
	{
		arg1->kid=(struct_node)malloc(sizeof(struct Struct_node));
		copy_struct(arg1->kid,arg2->kid);
	}
	if(arg2->next!=NULL)
	{
		arg1->next=(struct_node)malloc(sizeof(struct Struct_node));
		copy_struct(arg1->next,arg2->next);
	}
}
